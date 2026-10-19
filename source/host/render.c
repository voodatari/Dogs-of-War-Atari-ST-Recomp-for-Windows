/*
 * render.c - Composition of a screen frame at any instant between two logic frames.
 *
 * Background with fractional scroll (exact box filter), residual layer glued to the scroll, and interpolated sprites with sub-pixel position
 * (bilinear spread with their mask as alpha). The output is RGB of (200*S) x (320*S).
 */
#include "render.h"

int (*pack_sprite_cb)(Sprite *sp, int S, PackSpr *out) = NULL;
void (*pack_hud_cb)(uint8_t *base, const uint8_t *idx, const Zone *zones, int nz, int S) = NULL;
int (*pack_texture_cb)(const uint8_t *tile32, int S, const uint8_t **rgb) = NULL;
int (*pack_level_cb)(uint32_t id, const uint8_t *pal32, int S, const uint8_t **rgb, int *rows_px, int *H) = NULL;
int pack_gen = 0;

/* Palette words are 0x0RGB (4 bits per channel, the STE way). During a fade (see tos.c) bit 15 of a word means "dim this colour" and bits 12-14 of words 0, 1 and 2 hold the 8-bit factor. */
void pal_to_rgb(const uint8_t *pal, uint8_t rgb[16][3]) {
    int f8 = 255; for (int i = 0; i < 16; i++) if (pal[2 * i] & 0x80) { f8 = (((pal[0] >> 4) & 7) << 5) | (((pal[2] >> 4) & 7) << 2) | ((pal[4] >> 4) & 3); break; }
    for (int i = 0; i < 16; i++) {
        int v = (pal[2 * i] << 8) | pal[2 * i + 1];
        int r = v >> 8, g = v >> 4, b = v;
        int c[3] = { (((r & 7) << 1) | ((r >> 3) & 1)) * 17, (((g & 7) << 1) | ((g >> 3) & 1)) * 17, (((b & 7) << 1) | ((b >> 3) & 1)) * 17 };
        for (int k = 0; k < 3; k++) rgb[i][k] = (uint8_t)((pal[2 * i] & 0x80) ? (c[k] * f8 + 127) / 255 : c[k]);
    }
}

/* ------------------------------------------------------------------------------------------------ per-frame caches */
typedef struct FCache {
    int S, gen;
    uint8_t *hud;                                  /* HUD (and everything else) in RGB at scale S */
    uint8_t *ring[MAXZONES]; uint8_t ring_pal[MAXZONES][32];     /* ring in RGB per palette: (256, 192*S, 3) */
    float *strip[MAXZONES]; int strip_h[MAXZONES]; uint8_t strip_pal[MAXZONES][32];  /* horizontal texture per palette: (rows, 9*32*S, 3) */
    int nres; int *ry, *rx; uint8_t *rcol;         /* residue: pixels and color */
    int res_ok;
} FCache;

void render_free_cache(Frame *f) {
    FCache *c = (FCache *)f->cache; if (!c) return;
    free(c->hud); for (int i = 0; i < MAXZONES; i++) { free(c->ring[i]); free(c->strip[i]); } free(c->ry); free(c->rx); free(c->rcol); free(c); f->cache = NULL;
}
static FCache *fcache(Frame *f, int S) {
    FCache *c = (FCache *)f->cache;
    if (c && (c->S != S || c->gen != pack_gen)) { render_free_cache(f); c = NULL; }
    if (!c) { c = (FCache *)calloc(1, sizeof(FCache)); c->S = S; c->gen = pack_gen; f->cache = c; }
    return c;
}

static uint8_t *build_hud(const Frame *f, int S) {
    uint8_t *o = (uint8_t *)malloc((size_t)SCR_H * S * SCR_W * S * 3);
    int W = SCR_W * S;
    for (int zi = 0; zi < f->nzones; zi++) {
        uint8_t pal[16][3]; pal_to_rgb(f->zones[zi].pal, pal);
        int line = f->zones[zi].line, end = zi + 1 < f->nzones ? f->zones[zi + 1].line : SCR_H;
        for (int y = line; y < end && y < SCR_H; y++) {
            uint8_t *row = o + ((size_t)y * S) * W * 3;
            for (int x = 0; x < SCR_W; x++) { const uint8_t *c = pal[f->hud[y * SCR_W + x]]; for (int s = 0; s < S; s++) { uint8_t *p = row + ((size_t)x * S + s) * 3; p[0] = c[0]; p[1] = c[1]; p[2] = c[2]; } }
            for (int s = 1; s < S; s++) memcpy(row + (size_t)s * W * 3, row, (size_t)W * 3);
        }
    }
    if (pack_hud_cb) pack_hud_cb(o, f->hud, f->zones, f->nzones, S);
    return o;
}

static const uint8_t *ring_rgb(Frame *f, FCache *c, const uint8_t *pal32, int S) {
    int slot = -1;
    for (int i = 0; i < MAXZONES; i++) { if (c->ring[i] && memcmp(c->ring_pal[i], pal32, 32) == 0) return c->ring[i]; if (!c->ring[i] && slot < 0) slot = i; }
    if (slot < 0) { slot = 0; free(c->ring[0]); c->ring[0] = NULL; }
    uint8_t pal[16][3]; pal_to_rgb(pal32, pal);
    uint8_t *r = (uint8_t *)malloc((size_t)256 * 192 * S * 3);
    for (int y = 0; y < 256; y++) for (int x = 0; x < 192; x++) { const uint8_t *col = pal[f->ring_idx[y * 192 + x]]; for (int s = 0; s < S; s++) { uint8_t *p = r + (((size_t)y * 192 + x) * S + s) * 3; p[0] = col[0]; p[1] = col[1]; p[2] = col[2]; } }
    c->ring[slot] = r; memcpy(c->ring_pal[slot], pal32, 32); return r;
}

/* strip of repeated texture: (rows, 9*32*S, 3) in float */
static const float *tile_strip(Frame *f, FCache *c, const uint8_t *pal32, int S, int *rows) {
    int slot = -1;
    for (int i = 0; i < MAXZONES; i++) { if (c->strip[i] && memcmp(c->strip_pal[i], pal32, 32) == 0) { *rows = c->strip_h[i]; return c->strip[i]; } if (!c->strip[i] && slot < 0) slot = i; }
    if (slot < 0) { slot = 0; free(c->strip[0]); c->strip[0] = NULL; }
    const uint8_t *prgb = NULL; int nrows = 32; int per = 32 * S;
    if (pack_texture_cb && pack_texture_cb(f->tile, S, &prgb)) nrows = 32 * S;
    float *u = (float *)malloc((size_t)nrows * per * 9 * 3 * sizeof(float));
    uint8_t pal[16][3]; pal_to_rgb(pal32, pal);
    for (int y = 0; y < nrows; y++) for (int x = 0; x < per; x++) {
        const uint8_t *col; if (prgb) col = prgb + ((size_t)y * per + x) * 3; else col = pal[f->tile[y * 32 + x / S]];
        for (int r = 0; r < 9; r++) { float *p = u + (((size_t)y * 9 + r) * per + x) * 3; p[0] = col[0]; p[1] = col[1]; p[2] = col[2]; }
    }
    c->strip[slot] = u; c->strip_h[slot] = nrows; memcpy(c->strip_pal[slot], pal32, 32); *rows = nrows; return u;
}

static void bg_horizontal(uint8_t *out, Frame *B, FCache *c, const Frame *near_, double pos_px, int S) {
    int ncol = 192 * S, W = SCR_W * S, per = 32 * S, rowlen = per * 9;
    double j = pos_px * S; long j0 = (long)floor(j + 1e-9); double phi = j - (double)j0; if (phi < 1.0 / 512) phi = 0.0;
    long off = ((j0 % per) + per) % per; int X0 = 64 * S;
    for (int zi = 0; zi < near_->nzones; zi++) {
        int line = near_->zones[zi].line, end = zi + 1 < near_->nzones ? near_->zones[zi + 1].line : SCR_H;
        int nrows; const float *U = tile_strip(B, c, near_->zones[zi].pal, S, &nrows);
        /* one mixed texture row (only the distinct ones), then they are copied to the output rows */
        uint8_t *rowbuf = (uint8_t *)malloc((size_t)nrows * ncol * 3);
        for (int ty = 0; ty < nrows; ty++) {
            const float *src = U + ((size_t)ty * rowlen + (size_t)off) * 3; uint8_t *dst = rowbuf + (size_t)ty * ncol * 3;
            if (phi == 0.0) { for (int x = 0; x < ncol * 3; x++) dst[x] = (uint8_t)src[x]; }
            else for (int x = 0; x < ncol; x++) for (int k = 0; k < 3; k++) { float v = src[x * 3 + k] * (1.0f - (float)phi) + src[(x + 1) * 3 + k] * (float)phi; dst[x * 3 + k] = (uint8_t)(v + 0.5f); }
        }
        for (int y = line * S; y < end * S; y++) {
            int ty = (nrows == 32) ? (y / S) % 32 : y % nrows;
            memcpy(out + ((size_t)y * W + X0) * 3, rowbuf + (size_t)ty * ncol * 3, (size_t)ncol * 3);
        }
        free(rowbuf);
    }
}

/* ------------------------------------------------------------------------------------------------ sprites */
typedef struct Item { int order; const UnitRec *src; double ox, oy, px, py; } Item;
static int item_cmp(const void *a, const void *b) { const Item *x = (const Item *)a, *y = (const Item *)b; return x->order < y->order ? -1 : x->order > y->order ? 1 : (x < y ? -1 : 1); }

static UEntry *ust_find(UState *st, int unit, uint32_t ident, int create) {
    UEntry *free_ = NULL;
    for (int i = 0; i < 256; i++) { UEntry *e = &st->e[i]; if (e->used && e->unit == unit && e->ident == ident) return e; if (!e->used && !free_) free_ = e; }
    if (create && free_) { memset(free_, 0, sizeof *free_); free_->used = 1; free_->unit = unit; free_->ident = ident; return free_; }
    return NULL;
}

static int exiting(const UnitRec *u, UState *st) {
    UEntry *en = ust_find(st, u->unit, u->ident, 0); if (!en) return 0;
    double vx = en->ub[0] - en->ua[0]; const Sprite *sp = u->spr; double x0 = u->pos[0] + sp->ax, x1 = x0 + sp->w;
    return (x0 < 64 + 20 && vx < -0.5) || (x1 > 256 - 20 && vx > 0.5);
}

void render_clear_palmax(void) { }

static void draw_sprite(uint8_t *out, int S, const Sprite *sp, const uint8_t pal[16][3], double ox, double oy, int subpx, const PackSpr *ps) {
    int W = SCR_W * S, H = SCR_H * S, ww, hh; double xf, yf;
    const uint8_t *prgba = NULL;
    if (ps) { prgba = ps->rgba; ww = ps->w * S; hh = ps->h * S; ox += ps->dx; oy += ps->dy; } else { ww = sp->w * S; hh = sp->h * S; }
    xf = ox * S; yf = oy * S;
    if (!subpx) { xf = (double)lrint(xf); yf = (double)lrint(yf); }
    long x0 = (long)floor(xf), y0 = (long)floor(yf); double fx = xf - (double)x0, fy = yf - (double)y0;
    /* premultiplied at scale S: c[y][x] = (rgb*a, a) */
    float *c = (float *)malloc((size_t)hh * ww * 4 * sizeof(float));
    for (int y = 0; y < hh; y++) for (int x = 0; x < ww; x++) {
        float *p = c + ((size_t)y * ww + x) * 4;
        if (prgba) { const uint8_t *q = prgba + ((size_t)y * ww + x) * 4; float a = q[3] / 255.0f; p[0] = q[0] * a; p[1] = q[1] * a; p[2] = q[2] * a; p[3] = a; }
        else { int q = (y / S) * sp->w + x / S; float a = sp->mask[q] ? 1.0f : 0.0f; const uint8_t *col = pal[sp->img[q]]; p[0] = col[0] * a; p[1] = col[1] * a; p[2] = col[2] * a; p[3] = a; }
    }
    if (fx < 0.04 && fy < 0.04) {
        long xs = x0 > 64 * S ? x0 : 64 * S, ys = y0 > 0 ? y0 : 0, xe = x0 + ww < 256 * S ? x0 + ww : 256 * S, ye = y0 + hh < H ? y0 + hh : H;
        for (long y = ys; y < ye; y++) for (long x = xs; x < xe; x++) {
            const float *p = c + ((size_t)(y - y0) * ww + (x - x0)) * 4; uint8_t *o = out + ((size_t)y * W + x) * 3;
            if (prgba) { float a = p[3]; for (int k = 0; k < 3; k++) { float v = o[k] * (1.0f - a) + p[k] + 0.5f; o[k] = (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v); } }
            else if (p[3] > 0.5f) { const uint8_t *col = pal[sp->img[((y - y0) / S) * sp->w + (x - x0) / S]]; o[0] = col[0]; o[1] = col[1]; o[2] = col[2]; }
        }
        free(c); return;
    }
    float w00 = (float)((1 - fx) * (1 - fy)), w10 = (float)(fx * (1 - fy)), w01 = (float)((1 - fx) * fy), w11 = (float)(fx * fy);
    long xs = x0 > 64 * S ? x0 : 64 * S, ys = y0 > 0 ? y0 : 0, xe = x0 + ww + 1 < 256 * S ? x0 + ww + 1 : 256 * S, ye = y0 + hh + 1 < H ? y0 + hh + 1 : H;
    for (long y = ys; y < ye; y++) for (long x = xs; x < xe; x++) {
        long sx = x - x0, sy = y - y0; float t[4] = { 0, 0, 0, 0 };
        for (int k = 0; k < 4; k++) {
            float v = 0;
            if (sy < hh && sx < ww) v += c[((size_t)sy * ww + sx) * 4 + k] * w00;
            if (sy < hh && sx >= 1) v += c[((size_t)sy * ww + sx - 1) * 4 + k] * w10;
            if (sy >= 1 && sx < ww) v += c[((size_t)(sy - 1) * ww + sx) * 4 + k] * w01;
            if (sy >= 1 && sx >= 1) v += c[((size_t)(sy - 1) * ww + sx - 1) * 4 + k] * w11;
            t[k] = v;
        }
        if (t[3] <= 0.0f && t[0] <= 0.0f && t[1] <= 0.0f && t[2] <= 0.0f) continue;
        uint8_t *o = out + ((size_t)y * W + x) * 3;
        for (int k = 0; k < 3; k++) { float v = o[k] * (1.0f - t[3]) + t[k] + 0.5f; o[k] = (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v); }
    }
    free(c);
}

/* ------------------------------------------------------------------------------------------------ frame */
void render(const Frame *Ac, const Frame *Bc, float f, const RenderOpts *o, uint8_t *out) {
    Frame *A = (Frame *)Ac, *B = (Frame *)Bc; int S = o->S, interp = o->interpolate;
    int W = SCR_W * S, H = SCR_H * S;
    Frame *near_ = (f >= 0.5f && interp) ? B : A;
    FCache *nc = fcache(near_, S);
    if (!nc->hud) nc->hud = build_hud(near_, S);
    memcpy(out, nc->hud, (size_t)W * H * 3);
    if (!interp) { B = A; f = 0.0f; }
    int hmode = B->mode_h;
    if (hmode != A->mode_h) { A = (f >= 0.5f) ? B : A; B = A; f = 0.0f; hmode = A->mode_h; }
    FCache *bc = fcache(B, S);
    int dsh = 0, d = 0;
    if (hmode) {
        dsh = B->hs - A->hs;
        bg_horizontal(out, B, bc, near_, A->hs + (double)f * dsh, S);
    } else {
        int fa = 255 - A->scroll, fb = 255 - B->scroll; d = (((fb - fa + 128) % 256) + 256) % 256 - 128;
        double first = fa + (double)f * d; int n = 200 * S, X0 = 64 * S, ncol = 192 * S;
        const uint8_t *LR[MAXZONES] = { 0 }; int lrows[MAXZONES] = { 0 }, lvl_ok = 0, lvl_h = 0;      /* background of the whole level with tiles of the pack (if the pack changes them) */
        if (pack_level_cb && A->lvl && A->lvl == B->lvl) {
            lvl_ok = 1;
            for (int zi = 0; zi < near_->nzones; zi++) if (!pack_level_cb(A->lvl, near_->zones[zi].pal, S, &LR[zi], &lrows[zi], &lvl_h)) { lvl_ok = 0; break; }
            if (lvl_ok) first = (double)(lvl_h - 1 - A->a280) + (double)f * d;                        /* line of the level (from the top) that falls on screen row 0 */
        }
        for (int zi = 0; zi < near_->nzones; zi++) {
            int line = near_->zones[zi].line, end = zi + 1 < near_->nzones ? near_->zones[zi + 1].line : SCR_H;
            if (lvl_ok) {
                const uint8_t *Rl = LR[zi]; int Rh = lrows[zi];
                for (int y = line * S; y < end * S && y < n; y++) {
                    double Y = first * S + y; double kf = floor(Y + 1e-9); double t = Y - kf; if (t < 0.004) t = 0;
                    long r0 = (long)kf; int wq = (int)lrint(t * 256); long rr0 = r0 < 0 ? 0 : r0 > Rh - 2 ? Rh - 2 : r0;
                    uint8_t *dst = out + ((size_t)y * W + X0) * 3; const uint8_t *a = Rl + (size_t)rr0 * ncol * 3;
                    if (wq > 0) { long r1 = r0 + 1; r1 = r1 < 0 ? 0 : r1 > Rh - 1 ? Rh - 1 : r1; const uint8_t *b = Rl + (size_t)r1 * ncol * 3; for (int x = 0; x < ncol * 3; x++) dst[x] = (uint8_t)((a[x] * (256 - wq) + b[x] * wq + 128) >> 8); }
                    else memcpy(dst, a, (size_t)ncol * 3);
                }
                continue;
            }
            const uint8_t *R = ring_rgb(B, bc, near_->zones[zi].pal, S);
            for (int y = line * S; y < end * S && y < n; y++) {
                double pos = first + (double)y / S; double kf = floor(pos + 1e-9); double t = pos - kf;
                double wv = (t + 1.0 / S - 1.0) * S; if (wv < 0) wv = 0; if (wv > 1) wv = 1; if (wv < 0.004) wv = 0;
                int r0 = (int)(((long)kf % 256 + 256) % 256); int wq = (int)lrint(wv * 256);
                uint8_t *dst = out + ((size_t)y * W + X0) * 3; const uint8_t *a = R + (size_t)r0 * ncol * 3;
                if (wq > 0) { const uint8_t *b = R + (size_t)((r0 + 1) % 256) * ncol * 3; for (int x = 0; x < ncol * 3; x++) dst[x] = (uint8_t)((a[x] * (256 - wq) + b[x] * wq + 128) >> 8); }
                else memcpy(dst, a, (size_t)ncol * 3);
            }
        }
    }
    /* residue (glued to the scroll): from frame B shifted (1-f)*d lines */
    const Frame *Rf = NULL;
    if (o->use_res) { if (B->res_mask) Rf = B; else if (A->res_mask && !interp) Rf = A; }
    if (Rf) {
        Frame *R = (Frame *)Rf; FCache *rc = fcache(R, S);
        int shift = interp ? (int)lrint((1 - f) * d * S) : 0, shx = interp ? (int)lrint((1 - f) * dsh * S) : 0;
        if (!rc->res_ok) {
            int n = 0; for (int i = 0; i < 64000; i++) if (R->res_mask[i]) n++;
            rc->nres = n; rc->ry = (int *)malloc(sizeof(int) * (size_t)(n + 1)); rc->rx = (int *)malloc(sizeof(int) * (size_t)(n + 1)); rc->rcol = (uint8_t *)malloc((size_t)(n + 1) * 3);
            uint8_t pal[16][3]; pal_to_rgb(R->zones[R->nzones - 1].pal, pal); int q = 0;
            for (int y = 0; y < 200; y++) for (int x = 0; x < 320; x++) if (R->res_mask[y * 320 + x]) { rc->ry[q] = y; rc->rx[q] = x; memcpy(rc->rcol + q * 3, pal[R->res_idx[y * 320 + x]], 3); q++; }
            rc->res_ok = 1;
        }
        for (int q = 0; q < rc->nres; q++) for (int sy = 0; sy < S; sy++) for (int sx = 0; sx < S; sx++) {
            int yy = rc->ry[q] * S + sy + shift, xx = rc->rx[q] * S + sx + shx;
            if (yy >= 0 && yy < H && xx >= 64 * S && xx < 256 * S) memcpy(out + ((size_t)yy * W + xx) * 3, rc->rcol + q * 3, 3);       /* the shifted residue does not leave the game area (it does not overwrite the panels) */
        }
    }
    /* sprites */
    Item items[2 * MAX_UNITS]; int ni = 0; UState *st = o->st; static uint8_t seen[256];
    if (st) memset(seen, 0, sizeof seen);
    typedef struct { const UnitRec *ua, *ub; } Pair; Pair pairs[2 * MAX_UNITS]; int np = 0;
    for (int i = 0; i < B->nunits; i++) {
        const UnitRec *ub = &B->units[i], *ua = NULL;
        for (int j = 0; j < A->nunits; j++) if (A->units[j].unit == ub->unit && A->units[j].ident == ub->ident) { ua = &A->units[j]; break; }
        if (ua && (fabsf(ub->pos[0] - ua->pos[0]) > 24 || fabsf(ub->pos[1] - ua->pos[1]) > 24)) ua = NULL;
        pairs[np].ua = ua; pairs[np].ub = ub; np++;
    }
    for (int j = 0; j < A->nunits; j++) {
        const UnitRec *ua = &A->units[j], *ub = NULL;
        for (int i = 0; i < B->nunits; i++) if (B->units[i].unit == ua->unit && B->units[i].ident == ua->ident) { ub = &B->units[i]; break; }
        if (ub == NULL || fabsf(ub->pos[0] - ua->pos[0]) > 24 || fabsf(ub->pos[1] - ua->pos[1]) > 24) { pairs[np].ua = ua; pairs[np].ub = NULL; np++; }
    }
    for (int p = 0; p < np; p++) {
        const UnitRec *ua = pairs[p].ua, *ub = pairs[p].ub, *src = NULL; double px = 0, py = 0;
        if (ua && ub && interp) {
            src = f < 0.5f ? ua : ub;
            px = ua->pos[0] + (double)f * (ub->pos[0] - ua->pos[0]); py = ua->pos[1] + (double)f * (ub->pos[1] - ua->pos[1]);
            if (st && ub->unit == U_ENEMY) {
                UEntry *en = ust_find(st, ub->unit, ub->ident, 1); if (en) {
                    int idx = (int)(en - st->e); seen[idx] = 1;
                    double e0 = 0, e1 = 0; uint32_t p0 = A->vbl, p1 = B->vbl;
                    if (en->pair0 || en->pair1) {                         /* entry that already exists */
                        e0 = en->e[0] * o->st_decay; e1 = en->e[1] * o->st_decay;
                        if (en->pair0 != p0 || en->pair1 != p1) {
                            double tvn = (double)A->vbl + (double)f * (double)(B->vbl - A->vbl); double fo = (tvn - (double)en->pair0) / (double)((en->pair1 - en->pair0) > 1 ? (en->pair1 - en->pair0) : 1);
                            double pox = en->ua[0] + fo * (en->ub[0] - en->ua[0]), poy = en->ua[1] + fo * (en->ub[1] - en->ua[1]);
                            e0 = pox + e0 - px; e1 = poy + e1 - py;
                            if (fabs(e0) > 8 || fabs(e1) > 8) { e0 = 0; e1 = 0; }
                        }
                    }
                    en->pair0 = p0; en->pair1 = p1; en->ua[0] = ua->pos[0]; en->ua[1] = ua->pos[1]; en->ub[0] = ub->pos[0]; en->ub[1] = ub->pos[1]; en->e[0] = (float)e0; en->e[1] = (float)e1;
                    px += e0; py += e1;
                }
            }
        } else if (ub && (f >= 0.5f || !interp)) { src = ub; px = ub->pos[0]; py = ub->pos[1]; }
        else if (ua && !ub && interp && st && f < 1.0f && ua->unit == U_ENEMY && exiting(ua, st)) {
            UEntry *en = ust_find(st, ua->unit, ua->ident, 0); src = ua;
            px = ua->pos[0] + (double)f * (en->ub[0] - en->ua[0]); py = ua->pos[1] + (double)f * (en->ub[1] - en->ua[1]);
        } else if (ua && !ub && f < 0.5f) { src = ua; px = ua->pos[0]; py = ua->pos[1]; }
        else continue;
        items[ni].order = src->order; items[ni].src = src; items[ni].px = px; items[ni].py = py;
        items[ni].ox = px + src->spr->ax; items[ni].oy = py + src->spr->ay; ni++;
    }
    if (st) for (int i = 0; i < 256; i++) if (st->e[i].used && !seen[i]) st->e[i].used = 0;
    qsort(items, (size_t)ni, sizeof(Item), item_cmp);
    for (int i = 0; i < ni; i++) {
        const UnitRec *src = items[i].src; Sprite *sp = src->spr;
        int ymid = (int)(items[i].oy + sp->h / 2.0); if (ymid < 0) ymid = 0; if (ymid > 199) ymid = 199;
        int zi = 0; for (int z = 0; z < near_->nzones; z++) if (near_->zones[z].line <= ymid) zi = z;
        uint8_t pal[16][3]; pal_to_rgb(near_->zones[zi].pal, pal);
        PackSpr ps; int has = 0;
        if (pack_sprite_cb && !sp->partial) has = pack_sprite_cb(sp, S, &ps);
        draw_sprite(out, S, sp, pal, items[i].ox, items[i].oy, o->subpx, has ? &ps : NULL);
    }
}

void render_static(const uint8_t *idx, const Zone *zones, int nz, int S, uint8_t *out) {
    int W = SCR_W * S;
    for (int zi = 0; zi < nz; zi++) {
        uint8_t pal[16][3]; pal_to_rgb(zones[zi].pal, pal);
        int line = zones[zi].line, end = zi + 1 < nz ? zones[zi + 1].line : SCR_H;
        for (int y = line; y < end && y < SCR_H; y++) {
            uint8_t *row = out + ((size_t)y * S) * W * 3;
            for (int x = 0; x < SCR_W; x++) { const uint8_t *c = pal[idx[y * SCR_W + x]]; for (int s = 0; s < S; s++) { uint8_t *p = row + ((size_t)x * S + s) * 3; p[0] = c[0]; p[1] = c[1]; p[2] = c[2]; } }
            for (int s = 1; s < S; s++) memcpy(row + (size_t)s * W * 3, row, (size_t)W * 3);
        }
    }
}
