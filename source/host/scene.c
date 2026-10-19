/*
 * scene.c - Scene capture per logic frame: background + HUD + units that reference a sprite atlas.
 *
 * A unit is considered drawn if the game wrote to the drawing buffer during its real execution. The image comes from the atlas by key
 * (type, frame); if it is missing it is decoded from the graphics tables in RAM (sprites.c) and VALIDATED against what the game really drew.
 */
#include "scene.h"

uint32_t scene_last_vbl = (uint32_t)-9999; int scene_nframes_since = 0;
static SceneCb cb;

/* ------------------------------------------------------------------------------------------------ definition of the drawing units */
typedef struct UnitDef {
    int id; int func; uint32_t start, end; int reg;      /* reg: 0 = a0, 4 = a4, 6 = a6, 16 = a6 + d6 */
} UnitDef;
static const UnitDef UD[U_NUNITS] = {
    { U_ENEMY,   1, 0x5BA8, 0,      6 },
    { U_PLAYER,  1, 0x597A, 0,      4 },
    { U_BULLET,  0, 0x6848, 0x68B4, 0 },
    { U_BULLET2, 0, 0x68DC, 0x6946, 0 },
    { U_OBJ5E32, 0, 0x5E3A, 0x5F10, 6 },
    { U_OBJ5F1A, 0, 0x5F22, 0x5FF4, 6 },
    { U_FX,      0, 0xB6F2, 0xB6F8, 6 },
    { U_VEH6,    1, 0x61FA, 0,      6 },
    { U_VEH4,    1, 0x62C0, 0,      6 },
    { U_VEH,     1, 0x60F0, 0,      6 },
    { U_TURRET,  0, 0x6E54, 0x6F18, 16 },
    { U_BOOM,    0, 0x6D1A, 0x6D5A, 4 },
};
#define FRAME_START 0x8EEA
#define FRAME_END 0x8EE2
#define FRAME_START_H 0x4752
#define FRAME_END_H 0x474A
#define BG_SETUP 0x893C
#define BG_H 0x488C
#define RING 0x28000u
#define RING_LEN (256 * 96)
#define VX0 64
#define VX1 256

static uint64_t mkkey(int unit, int k0, int k1) { return ((uint64_t)unit << 40) | ((uint64_t)(k0 & 0xFFFF) << 8) | (uint64_t)(k1 & 0xFF); }
static int key_unit(uint64_t k) { return (int)(k >> 40); }
static int key_k0(uint64_t k) { return (int)((k >> 8) & 0xFFFF); }
static int key_k1(uint64_t k) { return (int)(k & 0xFF); }

/* ------------------------------------------------------------------------------------------------ frames and atlas */
Frame *frame_new(void) {
    Frame *f = (Frame *)calloc(1, sizeof(Frame)); f->ref = 1;
    f->hud = (uint8_t *)malloc(200 * 320); return f;
}
Frame *frame_ref(Frame *f) { if (f) __atomic_fetch_add(&f->ref, 1, __ATOMIC_ACQ_REL); return f; }          /* the frames pass from the emulation thread to the presenter's: atomic references */
extern void render_free_cache(Frame *f);
void frame_unref(Frame *f) {
    if (!f || __atomic_sub_fetch(&f->ref, 1, __ATOMIC_ACQ_REL) > 0) return;
    for (int i = 0; i < f->nunits; i++) sprite_unref(f->units[i].spr);
    render_free_cache(f);
    free(f->hud); free(f->ring_idx); free(f->res_mask); free(f->res_idx); free(f);
}

typedef struct { uint64_t key; Sprite *spr; uint8_t used, bad, carry, has_geom; int gax, gay, gw, gh; } AEnt;
struct Atlas { AEnt t[4096]; int n; };
Atlas *g_atlas;
static AEnt *afind(Atlas *a, uint64_t key, int create) {
    unsigned h = (unsigned)((key * 0x9E3779B97F4A7C15ull) >> 52) & 4095;
    for (int i = 0; i < 4096; i++) {
        AEnt *e = &a->t[(h + (unsigned)i) & 4095];
        if (e->used && e->key == key) return e;
        if (!e->used) { if (!create) return NULL; e->used = 1; e->key = key; a->n++; return e; }
    }
    return NULL;
}
Sprite *atlas_get(Atlas *a, uint64_t key) { AEnt *e = afind(a, key, 0); return e ? e->spr : NULL; }
void atlas_put(Atlas *a, uint64_t key, Sprite *s) { AEnt *e = afind(a, key, 1); if (!e) return; sprite_unref(e->spr); e->spr = sprite_ref(s); if (cb.on_sprite) cb.on_sprite(key, s); }
void atlas_drop(Atlas *a, uint64_t key) { AEnt *e = afind(a, key, 0); if (e && e->spr) { sprite_unref(e->spr); e->spr = NULL; if (cb.on_drop) cb.on_drop(key); } }

/* ------------------------------------------------------------------------------------------------ recorder state */
typedef struct Inst {
    int used, unit; uint32_t ident, ret; uint32_t w[16];
    int px, py, fx; uint64_t key; int replay, edge, diff; int r0, r1; uint8_t *before;
} Inst;
#define MAXOPEN 16
static Inst open_[MAXOPEN];
static Frame *cur = NULL;
static int hs_un = 0, hs_prev = -1, hs_prev_valid = 0;
static uint32_t lvl_id = 0;

void scene_init(const SceneCb *c) {
    cb = *c; g_atlas = (Atlas *)calloc(1, sizeof(Atlas));
}

static void inst_free(Inst *in) { free(in->before); in->before = NULL; in->used = 0; }

void scene_clear_frames(void) {
    for (int i = 0; i < MAXOPEN; i++) if (open_[i].used) inst_free(&open_[i]);
    if (cur) { frame_unref(cur); cur = NULL; }
}

void scene_new_segment(void) {
    /* the atlas is KEPT: each hit is validated against what the game really draws (carry); a sprite that no longer matches is discarded and extracted again */
    for (int i = 0; i < 4096; i++) if (g_atlas->t[i].used && g_atlas->t[i].spr) g_atlas->t[i].carry = 1;
    lvl_id = 0; { extern void level_new_segment(void); level_new_segment(); }
    scene_nframes_since = 0;
    if (cb.on_reset) cb.on_reset();
}

/* ------------------------------------------------------------------------------------------------ background and phases */
static void on_frame_start(void) {
    if (cur) { extern void scene_close(void); scene_close(); }
    if (tos.vbl - scene_last_vbl > 60) scene_new_segment();
    scene_last_vbl = tos.vbl;
    cur = frame_new(); cur->vbl = tos.vbl; cur->draw = mr32(0x28e); cur->scroll = -1;
    if (scene_nframes_since == 0) cur->reset = 1;
}
static void on_frame_end(void) { if (cur && cur->bg_ok) { extern void scene_close(void); scene_close(); } }
static void on_bg(void) {
    if (!cur) return;
    cur->scroll = (int)mr8(0x281); cur->mode_h = 0; cur->bg_ok = 1; hs_prev_valid = 0;
    if (!cur->ring_idx) cur->ring_idx = (uint8_t *)malloc(256 * 192);
    planes_to_idx(ram + RING, 256, 12, cur->ring_idx);
    cur->a280 = (int)mr16(0x280); cur->sec = (int)mr16(0x21A);
}
static void on_bg_h(void) {
    if (!cur) return;
    int p = (int)(mr16(0x24A) & 0x1E);
    if (!hs_prev_valid) hs_un = p; else hs_un += ((p - hs_prev + 16 + 32 * 8) % 32) - 16;
    hs_prev = p; hs_prev_valid = 1;
    uint8_t t[32 * 64]; planes_to_idx(ram + 0x28000, 32, 4, t);
    for (int y = 0; y < 32; y++) memcpy(cur->tile + y * 32, t + y * 64, 32);
    cur->mode_h = 1; cur->hs = hs_un; cur->scroll = 0; cur->bg_ok = 1;
}

/* ------------------------------------------------------------------------------------------------ units */
static const UnitDef *unit_by_start(uint32_t a) { for (int i = 0; i < U_NUNITS; i++) if (UD[i].start == a) return &UD[i]; return NULL; }

static void read_words(uint32_t p, uint32_t *w) { for (int i = 0; i < 16; i++) w[i] = (p + 31 < RAMSZ && p < 0x3FFFE0) ? mr16(p + (uint32_t)i * 2) : 0; }

static int unit_active(int u, const uint32_t *w) {
    switch (u) {
    case U_ENEMY: return w[2] != 0; case U_PLAYER: return w[0] != 0; case U_BULLET: case U_BULLET2: return w[4] != 0;
    case U_OBJ5E32: return w[2] != 0; case U_OBJ5F1A: return w[0] != 0; case U_FX: return w[4] != 0;
    case U_VEH6: case U_VEH4: case U_VEH: return w[0] != 0; case U_TURRET: return w[2] != 0; case U_BOOM: return w[2] != 0;
    }
    return 0;
}
static void unit_pos(int u, const uint32_t *w, int *px, int *py, int *fx) {
    *fx = 0;
    switch (u) {
    case U_ENEMY: *px = (int)(w[0] & ~1u); *py = (int)w[1]; break;
    case U_PLAYER: *px = (int)w[1]; *py = (int)w[2]; break;
    case U_BULLET: case U_BULLET2: *px = (int)w[0]; *py = (int)w[1]; break;
    case U_OBJ5E32: *px = (int)w[0]; *py = (int)w[1]; break;
    case U_OBJ5F1A: *px = (int)w[1]; *py = (int)w[2]; break;
    case U_FX: *px = (int)w[0]; *py = (int)w[1]; break;
    case U_VEH6: case U_VEH4: case U_VEH: *px = (int)w[1]; *py = (int)w[2]; break;
    case U_TURRET: *px = (int)(w[0] & ~15u); *py = w[1] >= 0x8000 ? (int)w[1] - 65536 : (int)w[1]; *fx = (int)(w[0] & 15); break;
    case U_BOOM: *px = (int)(((w[0] - 0x64) & ~15u) + 0x64); *py = (int)w[1]; *fx = (int)((w[0] - 0x64) & 15); break;
    default: *px = *py = 0;
    }
}
static uint64_t unit_key(int u, const uint32_t *w) {
    switch (u) {
    case U_ENEMY: return mkkey(u, (int)w[4], 0);
    case U_PLAYER: return mkkey(u, (int)w[3], 0);
    case U_BULLET: case U_BULLET2: return mkkey(u, (int)((w[0] - 0x24) & 7), 0);
    case U_OBJ5E32: return mkkey(u, (int)w[5], 0);
    case U_OBJ5F1A: return mkkey(u, (int)w[3], 0);
    case U_FX: { uint32_t d = w[7] + (mr16(0x272) & 1); return mkkey(u, d <= 11 ? (int)d : 10, 0); }
    case U_VEH6: case U_VEH4: return mkkey(u, (int)w[4], 0);
    case U_VEH: return mkkey(u, (int)w[4], w[8] >= 2);
    case U_TURRET: return mkkey(u, (int)w[5], 0);
    case U_BOOM: return mkkey(u, (int)w[3], 0);
    }
    return 0;
}

static void clamp_rows(Inst *in) {
    int r0 = in->r0, r1 = in->r1;
    r0 = r0 < 0 ? 0 : r0 > 199 ? 199 : r0; r1 = r1 < r0 + 1 ? r0 + 1 : r1 > 200 ? 200 : r1; in->r0 = r0; in->r1 = r1;
}

static void unit_start(const UnitDef *u) {
    if (!cur) return;
    uint32_t p;
    if (u->reg == 16) p = (g_cpu.a[6] + (g_cpu.d[6] & 0xFFFF)) & 0xFFFFFF;
    else if (u->reg == 6) p = g_cpu.a[6]; else if (u->reg == 4) p = g_cpu.a[4]; else p = g_cpu.a[0];
    uint32_t w[16]; read_words(p, w);
    if (!unit_active(u->id, w)) return;
    Inst *in = NULL; for (int i = 0; i < MAXOPEN; i++) if (!open_[i].used) { in = &open_[i]; break; }
    if (!in) return;
    memset(in, 0, sizeof *in); in->used = 1; in->unit = u->id; in->ident = p; memcpy(in->w, w, sizeof w);
    unit_pos(u->id, w, &in->px, &in->py, &in->fx); in->key = unit_key(u->id, w);
    if (u->func) in->ret = mr32(g_cpu.a[7]);
    AEnt *ae = afind(g_atlas, in->key, 0); Sprite *ent = ae ? ae->spr : NULL;
    in->replay = ent == NULL; in->edge = 0; in->diff = ae && ae->bad;
    if (in->diff) {                                       /* unreliable key: measure by difference, without atlas */
        in->replay = 0;
        if (ae->has_geom) { in->r0 = in->py + ae->gay - 16; in->r1 = in->py + ae->gay + ae->gh + 16; if (in->r0 < 0) in->r0 = 0; if (in->r1 > 200) in->r1 = 200; }
        else { in->r0 = 0; in->r1 = 200; }
    } else if (ent) {                                     /* near an edge of the game area the game's clipping is not a simple rectangle */
        int x0 = in->px + ent->ax, y0 = in->py + ent->ay;
        int crossing = x0 < VX0 || x0 + ent->w > VX1 || y0 < 0 || y0 + ent->h > 200;
        if (u->id == U_ENEMY) crossing = crossing || in->px < 0x64 || in->px > 0x114 || in->py < 0x64 || in->py > 0x116;      /* thresholds of routine $5BA8 on the position of the structure */
        in->edge = crossing && !ae->carry;                 /* it crosses the edge: the WHOLE sprite is used and the renderer clips per pixel */
        in->replay = 0;
        int r0 = y0 - 1; r0 = r0 < 0 ? 0 : r0 > 199 ? 199 : r0; in->r0 = r0; in->r1 = y0 + ent->h + 1;
        if (in->r1 < r0 + 1) in->r1 = r0 + 1; if (in->r1 > 200) in->r1 = 200;
    }
    if (in->replay) { in->r0 = 0; in->r1 = 200; }
    clamp_rows(in);
    in->before = (uint8_t *)malloc((size_t)(in->r1 - in->r0) * 160);
    if (cur->draw + (uint32_t)in->r1 * 160 <= RAMSZ) memcpy(in->before, ram + cur->draw + (uint32_t)in->r0 * 160, (size_t)(in->r1 - in->r0) * 160);
    else memset(in->before, 0, (size_t)(in->r1 - in->r0) * 160);
}

/* sprite measured by the difference between before and after (exact in what is visible) */
static Sprite *from_diff(const uint8_t *ib, const uint8_t *ia, int R, int W, int r0, int px, int py, int xo) {
    int x0 = W, y0 = R, x1 = -1, y1 = -1;
    for (int y = 0; y < R; y++) for (int x = 0; x < W; x++) if (ia[y * W + x] != ib[y * W + x]) { if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y; }
    if (x1 < 0) return NULL;
    Sprite *s = sprite_new(x1 - x0 + 1, y1 - y0 + 1); s->partial = 1;
    for (int y = 0; y < s->h; y++) for (int x = 0; x < s->w; x++) { int i = (y0 + y) * W + x0 + x; s->img[y * s->w + x] = ia[i]; s->mask[y * s->w + x] = ia[i] != ib[i]; }
    s->ax = xo + x0 - px; s->ay = r0 + y0 - py; return s;
}

static void push_unit(Inst *in, Sprite *spr, int edge) {
    if (cur->nunits >= MAX_UNITS) return;
    UnitRec *u = &cur->units[cur->nunits]; memset(u, 0, sizeof *u);
    u->unit = in->unit; u->key = in->key; u->ident = in->ident; u->px = in->px; u->fine = in->fx; u->py = in->py;
    u->kf0 = key_k0(in->key); u->kf1 = key_k1(in->key);
    u->order = cur->nunits; u->edge = edge; u->spr = sprite_ref(spr); cur->nunits++;
}

static void unit_finish(Inst *in) {
    int r0 = in->r0, r1 = in->r1; uint8_t *before = in->before; in->before = NULL;
    in->used = 0;
    if (!cur) { free(before); return; }
    int R = r1 - r0; const uint8_t *after = (cur->draw + (uint32_t)r1 * 160 <= RAMSZ) ? ram + cur->draw + (uint32_t)r0 * 160 : NULL;
    if (!after || memcmp(after, before, (size_t)R * 160) == 0) { free(before); return; }           /* the game drew nothing */
    uint64_t k = in->key; AEnt *ae = afind(g_atlas, k, 0); Sprite *ent = ae ? ae->spr : NULL;
    if (in->edge && ent) { push_unit(in, ent, 1); free(before); return; }
    /* columns involved (groups of 16 px): those of the expected sprite +-1; with unknown geometry, all of them */
    int g0 = 0, g1 = 20; int have = 0, pax = 0, pw = 0;
    if (ent && !in->replay) { have = 1; pax = ent->ax; pw = ent->w; }
    else if (ae && ae->has_geom && !in->replay) { have = 1; pax = ae->gax; pw = ae->gw; }
    if (have) { int x0p = in->px + pax; g0 = x0p / 16 - 1; if (x0p < 0) g0 = -((-x0p + 15) / 16) - 1; if (g0 < 0) g0 = 0; g1 = (x0p + pw) / 16 + 2; if (g1 > 20) g1 = 20; if (g1 <= g0) { g0 = 0; g1 = 20; } }
    int NG = g1 - g0, W = NG * 16;
    uint8_t *rb = (uint8_t *)malloc((size_t)R * NG * 8), *ra = (uint8_t *)malloc((size_t)R * NG * 8);
    for (int y = 0; y < R; y++) { memcpy(rb + (size_t)y * NG * 8, before + (size_t)y * 160 + g0 * 8, (size_t)NG * 8); memcpy(ra + (size_t)y * NG * 8, after + (size_t)y * 160 + g0 * 8, (size_t)NG * 8); }
    uint8_t *ib = (uint8_t *)malloc((size_t)R * W), *ia = (uint8_t *)malloc((size_t)R * W), *ex = (uint8_t *)malloc((size_t)R * W);
    planes_to_idx(rb, R, NG, ib); planes_to_idx(ra, R, NG, ia); free(rb); free(ra);
    int xo = g0 * 16;
    Sprite *cand = NULL; int cand_owned = 0, ok = 0;
    if (in->diff) cand = NULL;
    else if (in->replay) { cand = rom_sprite(in->unit, key_k0(k), key_k1(k)); cand_owned = 1; }
    else cand = ent;
    if (cand && in->replay) {                                                /* new sprite decoded from the table: if it crosses an edge it cannot be validated against the game's clipped drawing */
        int x0 = in->px + cand->ax, y0 = in->py + cand->ay;
        int crossing = x0 < VX0 || x0 + cand->w > VX1 || y0 < 0 || y0 + cand->h > 200;
        if (in->unit == U_ENEMY) crossing = crossing || in->px < 0x64 || in->px > 0x114 || in->py < 0x64 || in->py > 0x116;
        if (crossing) {                                                      /* the game clips with its own rules (rows and groups of 16 px): the window the game painted is taken from the complete drawing and validated; the atlas entry is saved when it is seen in the interior */
            Sprite *d = NULL; int x0 = W, y0 = R, x1 = -1, y1 = -1;
            for (int y = 0; y < R; y++) for (int x = 0; x < W; x++) if (ia[y * W + x] != ib[y * W + x]) { if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y; }
            if (x1 >= 0) {
                int wa = ((xo + x0) / 16) * 16, wb = ((xo + x1) / 16 + 1) * 16, ya = r0 + y0, yb = r0 + y1;       /* window on screen */
                int sx = in->px + cand->ax, sy = in->py + cand->ay;
                int cx0 = cand->w, cy0 = cand->h, cx1 = -1, cy1 = -1;
                for (int y = 0; y < cand->h; y++) for (int x = 0; x < cand->w; x++) if (cand->mask[y * cand->w + x]) {
                    int ax_ = sx + x, ay_ = sy + y; if (ax_ < wa || ax_ >= wb || ay_ < ya || ay_ > yb) continue;
                    if (x < cx0) cx0 = x; if (x > cx1) cx1 = x; if (y < cy0) cy0 = y; if (y > cy1) cy1 = y; }
                if (cx1 >= 0) {
                    d = sprite_new(cx1 - cx0 + 1, cy1 - cy0 + 1); d->partial = 1; d->ax = cand->ax + cx0; d->ay = cand->ay + cy0;
                    for (int y = 0; y < d->h; y++) for (int x = 0; x < d->w; x++) {
                        int ax_ = sx + cx0 + x, ay_ = sy + cy0 + y, q = (cy0 + y) * cand->w + cx0 + x;
                        if (cand->mask[q] && ax_ >= wa && ax_ < wb && ay_ >= ya && ay_ <= yb) { d->img[y * d->w + x] = cand->img[q]; d->mask[y * d->w + x] = 1; } }
                    memcpy(ex, ib, (size_t)R * W);                           /* validation of the window */
                    int px0 = in->px + d->ax - xo, py0 = in->py + d->ay - r0;
                    for (int y = 0; y < d->h; y++) for (int x = 0; x < d->w; x++) { int xx = px0 + x, yy = py0 + y; if (xx >= 0 && xx < W && yy >= 0 && yy < R && d->mask[y * d->w + x]) ex[yy * W + xx] = d->img[y * d->w + x]; }
                    if (memcmp(ex, ia, (size_t)R * W) != 0) { sprite_unref(d); d = NULL; }
                }
            }
            if (!d) d = from_diff(ib, ia, R, W, r0, in->px, in->py, xo);
            sprite_unref(cand); cand = NULL; cand_owned = 0;
            if (d) push_unit(in, d, 0);
            sprite_unref(d); free(ib); free(ia); free(ex); free(before); return;
        }
    }
    if (cand) {                                                              /* VALIDATION: what the game really painted must match what the sprite predicts */
        memcpy(ex, ib, (size_t)R * W);
        int sx = in->px + cand->ax - xo, sy = in->py + cand->ay - r0;
        int xs = sx < 0 ? 0 : sx, ys = sy < 0 ? 0 : sy, xe = sx + cand->w > W ? W : sx + cand->w, ye = sy + cand->h > R ? R : sy + cand->h;
        for (int y = ys; y < ye; y++) for (int x = xs; x < xe; x++) { int q = (y - sy) * cand->w + (x - sx); if (cand->mask[q]) ex[y * W + x] = cand->img[q]; }
        ok = memcmp(ex, ia, (size_t)R * W) == 0;
    }
    if (ok) {
        if (!in->replay) { if (ae) ae->carry = 0; }
        if (in->replay) { if (!ae) ae = afind(g_atlas, k, 1); if (ae && !ae->spr) atlas_put(g_atlas, k, cand); }
        push_unit(in, cand, 0);
        if (cand_owned) sprite_unref(cand);
    } else {
        if (cand_owned && cand) sprite_unref(cand);
        if (cand && !in->diff && !in->replay && ae && ae->carry) { ae->carry = 0; atlas_drop(g_atlas, k); }              /* inherited sprite that is no longer valid: it is discarded and extracted again */
        else if (cand && !in->diff && !in->replay) { if (!ae) ae = afind(g_atlas, k, 1); ae->bad = 1; atlas_drop(g_atlas, k); }   /* failed prediction: the key does not identify the drawing */
        Sprite *d = from_diff(ib, ia, R, W, r0, in->px, in->py, xo);
        if (d) {
            if (!ae) ae = afind(g_atlas, k, 1);
            if (ae) { ae->has_geom = 1; ae->gax = d->ax; ae->gay = d->ay; ae->gw = d->w; ae->gh = d->h; }
            push_unit(in, d, 0); sprite_unref(d);
        }
    }
    free(ib); free(ia); free(ex); free(before);
}

/* ------------------------------------------------------------------------------------------------ frame closing */
static void compose_idx(const Frame *f, uint8_t *out) {
    memcpy(out, f->hud, 200 * 320);
    if (f->mode_h) {
        for (int y = 0; y < 200; y++) for (int x = 0; x < 192; x++) { int c = ((x + f->hs) % 32 + 32) % 32; out[y * 320 + 64 + x] = f->tile[(y % 32) * 32 + c]; }
    } else {
        int first = 255 - f->scroll;
        for (int y = 0; y < 200; y++) memcpy(out + y * 320 + 64, f->ring_idx + (size_t)(((first + y) % 256 + 256) % 256) * 192, 192);
    }
    for (int i = 0; i < f->nunits; i++) {
        const UnitRec *u = &f->units[i]; const Sprite *sp = u->spr;
        int x0 = u->px + sp->ax;
        int y0 = u->py + sp->ay;
        for (int y = 0; y < sp->h; y++) { int yy = y0 + y; if (yy < 0 || yy >= 200) continue;
            for (int x = 0; x < sp->w; x++) { int xx = x0 + x; if (xx < VX0 || xx >= VX1) continue; if (sp->mask[y * sp->w + x]) out[yy * 320 + xx] = sp->img[y * sp->w + x]; } }
    }
}

void scene_close(void) {
    Frame *f = cur; if (!f) return; cur = NULL;
    uint8_t buf[200 * 320];
    f->nzones = tos.nzones; if (!f->nzones) { f->zones[0].line = 0; tos_pal16(f->zones[0].pal); f->nzones = 1; } else memcpy(f->zones, tos.zones, sizeof(Zone) * (size_t)tos.nzones);
    if (f->draw + 32000 <= RAMSZ) planes_to_idx(ram + f->draw, 200, 20, f->hud); else memset(f->hud, 0, 200 * 320);
    if (!f->mode_h && !f->ring_idx) f->ring_idx = (uint8_t *)calloc(256 * 192, 1);
    /* residue: pixels of the game area that the reconstruction does not explain (sprites of routines not hooked yet) */
    compose_idx(f, buf);
    uint8_t *d = (uint8_t *)calloc(200 * 320, 1); int any = 0, cnt = 0;
    for (int y = 0; y < 200; y++) for (int x = VX0; x < VX1; x++) if (buf[y * 320 + x] != f->hud[y * 320 + x]) { d[y * 320 + x] = 1; any = 1; cnt++; }
    for (int i = 0; i < f->nunits; i++) if (f->units[i].edge) {                          /* edge sprites: the game clips by words and we clip by pixel; it is not residue */
        const UnitRec *u = &f->units[i]; int x0 = u->px + u->spr->ax, y0 = u->py + u->spr->ay;
        int ya = y0 - 1 < 0 ? 0 : y0 - 1, yb = y0 + u->spr->h + 1 > 200 ? 200 : y0 + u->spr->h + 1, xa = x0 - 16 < 0 ? 0 : x0 - 16, xb = x0 + u->spr->w + 16 > 320 ? 320 : x0 + u->spr->w + 16;
        for (int y = ya; y < yb; y++) for (int x = xa; x < xb; x++) if (d[y * 320 + x]) { d[y * 320 + x] = 0; cnt--; }
    }
    any = cnt > 0;
    if (any) { f->res_mask = d; f->res_idx = (uint8_t *)malloc(200 * 320); memcpy(f->res_idx, f->hud, 200 * 320); f->res_px = cnt; } else free(d);
    extern void scene_check_level(Frame *f);
    scene_check_level(f);
    scene_nframes_since++;
    if (cb.on_frame) cb.on_frame(f);
    frame_unref(f);
}

/* ------------------------------------------------------------------------------------------------ hook dispatch */
static int is_func_ret(uint32_t a) { (void)a; return 1; }
void scene_hook(uint32_t a) {
    if (a == FRAME_START || a == FRAME_START_H) { on_frame_start(); return; }
    if (a == FRAME_END || a == FRAME_END_H) { on_frame_end(); return; }
    if (a == BG_SETUP) { on_bg(); return; }
    if (a == BG_H) { on_bg_h(); return; }
    const UnitDef *u = unit_by_start(a);
    if (u) unit_start(u);
    for (int i = 0; i < U_NUNITS; i++) if (UD[i].end == a && UD[i].end) { for (int k = 0; k < MAXOPEN; k++) if (open_[k].used && open_[k].unit == UD[i].id) unit_finish(&open_[k]); }
    if (is_func_ret(a)) for (int k = 0; k < MAXOPEN; k++) if (open_[k].used && UD[open_[k].unit].func && open_[k].ret == a) unit_finish(&open_[k]);
}

/* atlas dump (tests): one line per entry with the size, the position and a sum of the content */
void scene_dump_atlas(void) {
    for (int i = 0; i < 4096; i++) {
        AEnt *e = &g_atlas->t[i]; if (!e->used || !e->spr) continue; const Sprite *s = e->spr;
        uint64_t h = 0; for (int q = 0; q < s->w * s->h; q++) h += (uint64_t)(s->mask[q] ? s->img[q] : 0) * (uint64_t)((((unsigned)q * 40503u) & 0xFFFFu) | 1u);
        printf("ATLAS %s:%d:%d %d %d %d %d %016llx\n", unit_name[key_unit(e->key)], key_k0(e->key), key_k1(e->key), s->w, s->h, s->ax, s->ay, (unsigned long long)h);
    }
}

extern int hook_registered(uint32_t addr);
__attribute__((weak)) void scene_check_level(Frame *f) { (void)f; }
__attribute__((weak)) void render_free_cache(Frame *f) { (void)f; }
void scene_install_hooks(void) {
    for (int i = 0; i < game_hook_count; i++) { uint32_t a = game_hook_addr(i); if (a && !hook_registered(a)) hook_register(a, (void (*)(uint32_t))scene_hook); }
}
