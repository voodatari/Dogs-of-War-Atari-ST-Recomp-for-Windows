/*
 * present.c - Presenter: emulator message queue, enemy-motion filters, VBL clock, scroll stabilization
 *             and choice of the frame to show.
 */
#include "present.h"
#ifdef _WIN32
#include <windows.h>
#endif

PFlags pflags = { 1, 1, 1, 1, 1, 1, 1, 1, 1 };

#define FIT_N 25
#define ML 24
#define MS 4
#define KL 0.05
#define KS 0.5
#define ATHR 0.5
#define AREL 0.5
#define TKSTOP 0.7
#define ECD 0.85
#define ECMAX 4.0
#define TCAP 5.0
#define MAXF 3.0
#define MAXFR 40
#define MAXE 128

/* ------------------------------------------------------------------------------------------------ message queue */
#define QCAP 512
static Msg *q_[QCAP]; static int q_h = 0, q_n = 0;
#ifdef _WIN32
static CRITICAL_SECTION q_cs; static int q_init = 0; static HANDLE q_ev = NULL;           /* q_ev: new-message notice (the presenter waits here instead of spinning) */
#define QLOCK() do { if (!q_init) { InitializeCriticalSection(&q_cs); q_ev = CreateEvent(NULL, FALSE, FALSE, NULL); q_init = 1; } EnterCriticalSection(&q_cs); } while (0)
#define QUNLOCK() LeaveCriticalSection(&q_cs)
#else
#define QLOCK()
#define QUNLOCK()
#endif

Msg *msg_new(int type) { Msg *m = (Msg *)calloc(1, sizeof(Msg)); m->type = type; return m; }
void msg_free(Msg *m) { if (!m) return; if (m->frame) frame_unref(m->frame); free(m->idx); free(m); }
void msgq_push(Msg *m) {
    for (;;) {
        QLOCK();
        if (q_n < QCAP - 1 || m->type == MSG_MENU) {
            if (q_n >= QCAP) { QUNLOCK(); msg_free(m); return; }
            q_[(q_h + q_n) % QCAP] = m; q_n++; QUNLOCK();
#ifdef _WIN32
            SetEvent(q_ev);
#endif
            return;
        }
        QUNLOCK(); sleep_ms(1);
    }
}
Msg *msgq_pop(void) { QLOCK(); Msg *m = NULL; if (q_n) { m = q_[q_h]; q_h = (q_h + 1) % QCAP; q_n--; } QUNLOCK(); return m; }
void msgq_clear(void) { Msg *m; while ((m = msgq_pop())) msg_free(m); }
int msgq_count(void) { QLOCK(); int n = q_n; QUNLOCK(); return n; }

/* ------------------------------------------------------------------------------------------------ presenter state */
typedef struct Dq { double x[FIT_N], y[FIT_N]; int n; } Dq;
static void dq_push(Dq *d, int cap, double x, double y) {
    if (d->n >= cap) { memmove(d->x, d->x + 1, sizeof(double) * (size_t)(cap - 1)); memmove(d->y, d->y + 1, sizeof(double) * (size_t)(cap - 1)); d->n = cap - 1; }
    d->x[d->n] = x; d->y[d->n] = y; d->n++;
}
typedef struct EState {
    int used; uint32_t ident;
    Dq h;                                             /* history of smoothed positions (world) */
    Dq r;                                             /* last 6 raw positions (world) */
    double off[2]; int ref, stop; int n; int chg[2];
    double efx, efy;
} EState;

struct Presenter {
    int S;
    Frame *fr[MAXFR]; int nfr;
    int clk_set; double clk_t, clk_v, clk_r;
    double tv_last; int umode_h, have_umode; double uy; int uprev, have_uprev;
    EState es[MAXE];
    UState ust;
    /* fixed screen */
    uint32_t menu_vbl; uint8_t *menu_idx; Zone menu_zones[MAXZONES]; int menu_nz; int have_menu;
    uint32_t last_vbl;
    char scr[16];
    char text[256]; double text_t; int has_text;
    double stat[6]; int have_stat; char err[256]; int has_err;
    double ahead;
    /* sfix */
    int sf_set; int sf_mode; double sf_t; int sf_last;
    uint8_t *img;                                     /* salida */
    uint8_t *mimg; uint32_t mimg_vbl; int mimg_gen; int mimg_valid;
};

Presenter *present_new(int S) {
    Presenter *p = (Presenter *)calloc(1, sizeof(Presenter)); p->S = S; p->tv_last = -1e18; strcpy(p->scr, "otro");
    p->img = (uint8_t *)calloc((size_t)SCR_W * SCR_H * S * S * 3, 1); p->mimg = (uint8_t *)calloc((size_t)SCR_W * SCR_H * S * S * 3, 1);
    p->menu_idx = (uint8_t *)calloc(64000, 1);
    return p;
}
void present_set_scale(Presenter *p, int S) { if (S == p->S) return; free(p->img); free(p->mimg); p->S = S; p->img = (uint8_t *)calloc((size_t)SCR_W * SCR_H * S * S * 3, 1); p->mimg = (uint8_t *)calloc((size_t)SCR_W * SCR_H * S * S * 3, 1); p->mimg_valid = 0; }
int present_scale(Presenter *p) { return p->S; }
void present_set_ahead(Presenter *p, double a) { p->ahead = a; }
const char *present_screen_kind(Presenter *p) { return p->scr; }
int present_last_vbl(Presenter *p) { return (int)p->last_vbl; }
int present_umode_h(Presenter *p) { return p->umode_h; }
const char *present_error(Presenter *p) { return p->has_err ? p->err : NULL; }
void present_stat(Presenter *p, double *o, int *have) { memcpy(o, p->stat, sizeof p->stat); *have = p->have_stat; }
void present_menu_cur(Presenter *p, const uint8_t **idx, const Zone **z, int *nz) { *idx = p->menu_idx; *z = p->menu_zones; *nz = p->menu_nz; }
void present_pack_changed(Presenter *p) { p->mimg_valid = 0; }
int present_take_text(Presenter *p, char *out, int n, double *t) { if (!p->has_text) return 0; snprintf(out, (size_t)n, "%s", p->text); *t = p->text_t; p->has_text = 0; return 1; }
Frame *present_latest(Presenter *p) { return p->nfr ? frame_ref(p->fr[p->nfr - 1]) : NULL; }

double present_tv(Presenter *p, double at) { return p->clk_v + (at - p->clk_t) * p->clk_r; }

static void clear_filters(Presenter *p) { memset(p->es, 0, sizeof p->es); memset(&p->ust, 0, sizeof p->ust); }
static void clear_frames(Presenter *p) { for (int i = 0; i < p->nfr; i++) frame_unref(p->fr[i]); p->nfr = 0; }

static EState *es_find(Presenter *p, uint32_t ident) { for (int i = 0; i < MAXE; i++) if (p->es[i].used && p->es[i].ident == ident) return &p->es[i]; return NULL; }
static EState *es_new(Presenter *p, uint32_t ident) {
    for (int i = 0; i < MAXE; i++) if (!p->es[i].used) { EState *e = &p->es[i]; memset(e, 0, sizeof *e); e->used = 1; e->ident = ident; return e; }
    return NULL;
}

static void ingest(Presenter *p, Frame *f) {
    int mode_h = f->mode_h;
    if (!p->have_umode || mode_h != p->umode_h) { p->umode_h = mode_h; p->have_umode = 1; p->uy = 0.0; p->have_uprev = 0; clear_filters(p); }
    double Ux, Uy;
    if (mode_h) { Ux = -(double)f->hs; Uy = 0.0; }
    else {
        if (p->have_uprev) p->uy += (double)((((f->scroll - p->uprev + 128) % 256) + 256) % 256 - 128);
        p->uprev = f->scroll; p->have_uprev = 1; Ux = 0.0; Uy = p->uy;
    }
    f->Ux = (float)Ux; f->Uy = (float)Uy;
    const Frame *prevf = p->nfr ? p->fr[p->nfr - 1] : NULL;
    uint8_t seen[MAXE]; memset(seen, 0, sizeof seen);
    for (int i = 0; i < f->nunits; i++) {
        UnitRec *u = &f->units[i];
        float px = (float)(u->px + (f->mode_h ? u->fine : 0)), py = (float)u->py;       /* fine part of x only in the horizontal stretch (there x changes with the scroll and the 16 px jumps are avoided); in the vertical one x does not change and the game already draws in place */
        u->raw[0] = px; u->raw[1] = py; u->pos[0] = px; u->pos[1] = py; u->fpos[0] = px; u->fpos[1] = py; u->gpos[0] = px; u->gpos[1] = py; u->cw[0] = 1e30f; u->cw[1] = 1e30f;
        if (u->unit != U_ENEMY) continue;
        uint32_t ident = u->ident; double x = px - Ux, y = py - Uy;
        EState *e = es_find(p, ident);
        if (e && e->h.n && (fabs(e->h.x[e->h.n - 1] - x) > 24 || fabs(e->h.y[e->h.n - 1] - y) > 24)) {       /* jump = new enemy in the same slot */
            double sx = e->efx, sy = e->efy; memset(e, 0, sizeof *e); e->used = 1; e->ident = ident; e->efx = sx; e->efy = sy;
        }
        if (!e) { e = es_new(p, ident); if (!e) continue; }
        seen[e - p->es] = 1;
        double dec = e->stop > 0 ? 0.4 : ECD;
        e->off[0] *= dec; e->off[1] *= dec; e->ref = e->ref > 0 ? e->ref - 1 : 0; e->stop = e->stop > 0 ? e->stop - 1 : 0;
        dq_push(&e->r, 6, x, y);
        if (e->r.n >= 6) {
            double dx[5], dy[5]; for (int k = 1; k < 6; k++) { dx[k - 1] = e->r.x[k] - e->r.x[k - 1]; dy[k - 1] = e->r.y[k] - e->r.y[k - 1]; }
            double psx = (dx[0] + dx[1] + dx[2]) * 2.0 / 3.0, psy = (dy[0] + dy[1] + dy[2]) * 2.0 / 3.0;
            double ssx = dx[3] + dx[4], ssy = dy[3] + dy[4];
            double pm = sqrt(psx * psx + psy * psy), sm = sqrt(ssx * ssx + ssy * ssy);
            if (pm >= 2.0 && sm <= 0.5) {                          /* REAL STOP: the compensation is cancelled and the follower converges quickly */
                e->stop = 4; e->off[0] *= 0.2; e->off[1] *= 0.2; e->ref = 2;
            } else if (pflags.hold && !e->ref && pm >= 2.0 && sm < 0.75 * pm && fabs(dx[4]) + fabs(dy[4]) < 0.75 * (pm / 2.0) * 1.4) {
                double ox = e->off[0] + psx - ssx, oy = e->off[1] + psy - ssy;           /* COMPENSATION FOR THE LOST FRAME */
                e->off[0] = ox < -ECMAX ? -ECMAX : ox > ECMAX ? ECMAX : ox; e->off[1] = oy < -ECMAX ? -ECMAX : oy > ECMAX ? ECMAX : oy; e->ref = 2;
            }
        }
        double cx = x + e->off[0], cy = y + e->off[1];
        u->cw[0] = (float)cx; u->cw[1] = (float)cy;
        double fx = cx, fy = cy;                                    /* average with the position of the previous frame (the game moves the slow ones in steps of 2 px) */
        if (prevf) for (int j = 0; j < prevf->nunits; j++) if (prevf->units[j].unit == U_ENEMY && prevf->units[j].ident == ident && prevf->units[j].cw[0] < 1e29f) {
            double qx = prevf->units[j].cw[0], qy = prevf->units[j].cw[1];
            if (fabs(qx - cx) <= 24 && fabs(qy - cy) <= 24) { fx = (cx + qx) / 2.0; fy = (cy + qy) / 2.0; }
            break;
        }
        dq_push(&e->h, FIT_N, fx, fy);
        if (e->h.n > 1) {                                           /* velocity follower on both axes */
            double out[2]; e->n++;
            for (int c = 0; c < 2; c++) {
                const double *h = c == 0 ? e->h.x : e->h.y; int hn = e->h.n; double last = h[hn - 1];
                int mS = MS < hn - 1 ? MS : hn - 1; int t1 = ML < hn - 1 ? ML : hn - 1, t2 = e->n - e->chg[c]; int mL = t1 < t2 ? t1 : t2; if (mL < mS) mL = mS;
                double vL = (last - h[hn - 1 - mL]) / mL, vS = (last - h[hn - 1 - mS]) / mS, v, kk;
                if (e->stop > 0) { v = vS; kk = TKSTOP; e->chg[c] = e->n; }
                else if (fabs(vS - vL) > fmax(ATHR, AREL * fabs(vL))) { v = vS; kk = KS; e->chg[c] = e->n; }
                else { v = vL; kk = KL; }
                double pf = c == 0 ? e->efx : e->efy, pred = pf + v, v_ = pred + kk * (last - pred);
                if (fabs(v_ - last) > TCAP) v_ = last + (v_ > last ? TCAP : -TCAP);
                out[c] = v_;
            }
            e->efx = out[0]; e->efy = out[1];
        } else { e->efx = fx; e->efy = fy; }
        if (e->stop == 4) { double e0x = e->efx, e0y = e->efy; e->efx = cx + 0.25 * (e0x - cx); e->efy = cy + 0.25 * (e0y - cy); }   /* real stop: almost all of the excess is trimmed at once */
        if (pflags.yfilter) { fx = e->efx; fy = e->efy; }
        u->pos[0] = (float)(cx + Ux); u->pos[1] = (float)(cy + Uy); u->fpos[0] = (float)(fx + Ux); u->fpos[1] = (float)(fy + Uy);
    }
    for (int i = 0; i < MAXE; i++) if (p->es[i].used && !seen[i]) p->es[i].used = 0;
    if (p->nfr >= MAXFR) { frame_unref(p->fr[0]); memmove(p->fr, p->fr + 1, sizeof(Frame *) * (MAXFR - 1)); p->nfr--; }
    p->fr[p->nfr++] = frame_ref(f); p->last_vbl = f->vbl;
}

void present_drain(Presenter *p) {
    Msg *m; int n = 0;
    while (n < 200 && (m = msgq_pop())) {
        n++;
        switch (m->type) {
        case MSG_RATE:
            if (p->clk_set) {
                double now = now_s(), old_tv = present_tv(p, now), new_tv = m->vbl0 + (now - m->t0) * m->rate;
                if (!(0.0 < old_tv - new_tv && old_tv - new_tv < 40.0)) p->tv_last = -1e18;      /* the time is only kept on a small re-anchor backwards */
            }
            p->clk_set = 1; p->clk_t = m->t0; p->clk_v = m->vbl0; p->clk_r = m->rate; break;
        case MSG_SCR: snprintf(p->scr, sizeof p->scr, "%s", m->text); break;
        case MSG_TEXT: snprintf(p->text, sizeof p->text, "%s", m->text); p->text_t = now_s(); p->has_text = 1; break;
        case MSG_RESET: p->tv_last = -1e18; clear_frames(p); clear_filters(p); p->have_uprev = 0; p->uy = 0.0; break;
        case MSG_FRAME: ingest(p, m->frame); break;
        case MSG_MENU:
            p->menu_vbl = m->vbl; p->last_vbl = m->vbl; if (m->idx) memcpy(p->menu_idx, m->idx, 64000); memcpy(p->menu_zones, m->zones, sizeof m->zones); p->menu_nz = m->nz; p->have_menu = 1; p->mimg_valid = 0; break;
        case MSG_STAT: memcpy(p->stat, m->stat, sizeof p->stat); p->have_stat = 1; break;
        case MSG_ERROR: snprintf(p->err, sizeof p->err, "%s", m->text); p->has_err = 1; break;
        default: break;
        }
        msg_free(m);
    }
}

int present_wait_menu(Presenter *p, double timeout) {
    uint32_t v0 = p->have_menu ? p->menu_vbl : 0xFFFFFFFFu; double t_end = now_s() + timeout;
    for (;;) {
        present_drain(p); if (p->have_menu && p->menu_vbl != v0) return 1;
        double left = t_end - now_s(); if (left <= 0) return 0;
#ifdef _WIN32
        wait_event_ms(q_ev, left * 1000.0);
#else
        sleep_ms(left * 1000.0);
#endif
    }
}

/* ------------------------------------------------------------------------------------------------ scroll stabilization */
static double sfix(Presenter *p, const Frame *A, const Frame *B, double f, int mode_h /*1=h 0=v*/) {
    double t = mode_h ? (A->Ux + f * (B->Ux - A->Ux)) : (A->Uy + f * (B->Uy - A->Uy));
    if (!p->sf_set || p->sf_mode != mode_h || fabs(t - p->sf_t) > 8.0) { p->sf_set = 1; p->sf_mode = mode_h; p->sf_t = t; p->sf_last = 0; return 0.0; }
    double sfv = p->sf_t, vref = mode_h ? 0.83 : 0.42, d = t - sfv, nw;
    if (d * p->sf_last < 0 && fabs(d) < 1.6) nw = sfv;                                 /* the target goes back a little: overshoot of the extrapolation */
    else { double lim = 1.3 * vref; nw = sfv + (d < -lim ? -lim : d > lim ? lim : d); }
    double mv = nw - sfv; p->sf_t = nw; if (fabs(mv) > 1e-6) p->sf_last = mv > 0 ? 1 : -1;
    return nw - t;
}

static void shift_area(uint8_t *out, int S, double sh, int mode_h) {
    int n = (int)lrint(sh * S); if (n == 0) return;
    int W = SCR_W * S, H = SCR_H * S, x0 = 64 * S, x1 = 256 * S, w = x1 - x0;
    if (mode_h) {
        for (int y = 0; y < H; y++) {
            uint8_t *row = out + ((size_t)y * W + x0) * 3;
            if (n > 0) { memmove(row + (size_t)n * 3, row, (size_t)(w - n) * 3); for (int x = 0; x < n; x++) memcpy(row + (size_t)x * 3, row + (size_t)n * 3, 3); }
            else { int m = -n; memmove(row, row + (size_t)m * 3, (size_t)(w - m) * 3); for (int x = w - m; x < w; x++) memcpy(row + (size_t)x * 3, row + (size_t)(w - m - 1) * 3, 3); }
        }
    } else {
        size_t rl = (size_t)w * 3;
        if (n > 0) { for (int y = H - 1; y >= n; y--) memcpy(out + ((size_t)y * W + x0) * 3, out + ((size_t)(y - n) * W + x0) * 3, rl); for (int y = 0; y < n; y++) memcpy(out + ((size_t)y * W + x0) * 3, out + ((size_t)n * W + x0) * 3, rl); }
        else { int m = -n; for (int y = 0; y < H - m; y++) memcpy(out + ((size_t)y * W + x0) * 3, out + ((size_t)(y + m) * W + x0) * 3, rl); for (int y = H - m; y < H; y++) memcpy(out + ((size_t)y * W + x0) * 3, out + ((size_t)(H - m - 1) * W + x0) * 3, rl); }
    }
}

/* ------------------------------------------------------------------------------------------------ frame a mostrar */
const uint8_t *present_frame(Presenter *p, double at, int *is_menu) {
    int S = p->S; if (is_menu) *is_menu = 0;
    present_drain(p);
    if (!p->clk_set) { memset(p->img, 0, (size_t)SCR_W * SCR_H * S * S * 3); return p->img; }
    double tv = present_tv(p, at) + p->ahead;
    if (tv < p->tv_last) tv = p->tv_last;                                   /* clock re-anchored backwards after a stop: the shown time is kept (no rewinding) */
    for (int k = p->nfr > 3 ? p->nfr - 3 : 0; k < p->nfr; k++) { Frame *f = p->fr[k]; for (int i = 0; i < f->nunits; i++) { UnitRec *u = &f->units[i]; if (pflags.xfilter) { u->pos[0] = u->fpos[0]; u->pos[1] = u->fpos[1]; } else { u->pos[0] = u->raw[0]; u->pos[1] = u->raw[1]; } } }
    RenderOpts o; memset(&o, 0, sizeof o); o.S = S; o.interpolate = 1; o.use_res = pflags.res; o.subpx = pflags.subpx; o.st = pflags.cont ? &p->ust : NULL; o.st_decay = 0.8f;
    if (p->nfr && (int)p->last_vbl - (int)p->fr[p->nfr - 1]->vbl < 8 && !(p->have_menu && p->menu_vbl > p->fr[p->nfr - 1]->vbl)) {         /* game mode */
        if (!pflags.extrap && pflags.interp) tv -= 2.0 + p->ahead;
        int i = p->nfr - 1; while (i > 0 && (double)p->fr[i]->vbl > tv) i--;
        if (!pflags.interp) {                                               /* without interpolation: the last published logic frame is shown as it is (without waiting for the next one or extrapolating) */
            Frame *F = p->fr[i]; o.interpolate = 0; p->tv_last = (double)F->vbl; p->sf_set = 0; render(F, F, 0.0f, &o, p->img); return p->img;
        }
        Frame *A, *B;
        if (i + 1 < p->nfr) { A = p->fr[i]; B = p->fr[i + 1]; }
        else if (i >= 1) { A = p->fr[i - 1]; B = p->fr[i]; }
        else { o.interpolate = 0; render(p->fr[i], p->fr[i], 0.0f, &o, p->img); return p->img; }
        double span = (double)((int)B->vbl - (int)A->vbl); if (span < 1) span = 1;
        double f = (tv - (double)A->vbl) / span; if (f < 0) f = 0; if (f > MAXF) f = MAXF;
        p->tv_last = (double)A->vbl + f * span;
        int mode_ = (A->mode_h && B->mode_h) ? 1 : ((!A->mode_h && !B->mode_h) ? 0 : -1);
        double sh = 0.0;
        if (mode_ == 1 && pflags.sfix) sh = sfix(p, A, B, f, 1);
        if (mode_ < 0) p->sf_set = 0;
        render(A, B, (float)f, &o, p->img);
        if (sh != 0.0) shift_area(p->img, S, sh, mode_ == 1);
        return p->img;
    }
    /* fixed screen (menus, map, shop, title...) */
    render_clear_palmax();
    if (is_menu) *is_menu = 1;
    if (!p->have_menu) { memset(p->mimg, 0, (size_t)SCR_W * SCR_H * S * S * 3); return p->mimg; }
    if (!p->mimg_valid || p->mimg_vbl != p->menu_vbl) {
        extern void present_static_hook(const uint8_t *idx, const Zone *z, int nz, int S, uint8_t *out);
        double tq0 = now_s();
        render_static(p->menu_idx, p->menu_zones, p->menu_nz, S, p->mimg);
        double tq1 = now_s();
        present_static_hook(p->menu_idx, p->menu_zones, p->menu_nz, S, p->mimg);
        { static double sa = 0, sb = 0, mx = 0; static int sn = 0; double tq2 = now_s(); sa += tq1 - tq0; sb += tq2 - tq1; if (tq2 - tq0 > mx) mx = tq2 - tq0; sn++;
          if (getenv("DOW_PROFLAYERS") && sn % 20 == 0) host_log_file("static: %d rebuilds, render_static %.2f ms, layers %.2f ms on average, maximum %.2f ms", sn, sa / sn * 1000, sb / sn * 1000, mx * 1000); }
        p->mimg_valid = 1; p->mimg_vbl = p->menu_vbl;
    }
    return p->mimg;
}
__attribute__((weak)) void present_static_hook(const uint8_t *idx, const Zone *z, int nz, int S, uint8_t *out) { (void)idx; (void)z; (void)nz; (void)S; (void)out; }
