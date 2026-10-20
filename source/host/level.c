/*
 * level.c - Map of the vertical-scroll sections.
 *
 * Format (deduced from the game code $8B2E/$8B6A/$8BDE/$8D4C): table of sections at $73BAE (word n -> the map starts at
 * $73BAE + $40 + word); byte stream, 3 blocks per row of 32 lines (columns of 64 px), from BOTTOM to TOP, up to the
 * byte $FF: byte g = floor block (g & $7F, 64x32 px, $5649A, 1024 bytes each); if g & $80 entries follow up to a $00: t
 * in $F1..$F4 = event (1 byte); any other t = masked 16x16 object tile ($57C9A, 128 bytes) + 1 position byte (table
 * $8D80).
 */
#include "level.h"
#include "scene.h"
#include "gfxpack.h"
#include "render.h"
#ifdef _WIN32
#include <windows.h>
#endif

#define GROUND_T 0x5649Au
#define OBJECT_T 0x57C9Au
#define TABLE_T  0x73BAEu
#define POSTAB_T 0x8D80u

/* ---- decoding ---- */
static uint8_t *ground_tile(int i) {
    uint8_t *t = (uint8_t *)malloc(32 * 64);
    planes_to_idx(ram + GROUND_T + (uint32_t)i * 1024u, 32, 4, t);
    return t;
}
/* Decodes object tile i (16 x 16, masked, colour 0 transparent). */
static uint8_t *obj_tile(int i) {
    uint8_t *t = (uint8_t *)calloc(16 * 16, 1);
    for (int r = 0; r < 16; r++) {
        uint32_t w[4];
        for (int p = 0; p < 4; p++)
            w[p] = mr16(OBJECT_T + (uint32_t)i * 128u + (uint32_t)r * 8u + (uint32_t)p * 2u);
        for (int c = 0; c < 16; c++) {
            int bit = 15 - c, v = 0;
            for (int p = 0; p < 4; p++)
                v |= (int)((w[p] >> bit) & 1) << p;
            t[r * 16 + c] = (uint8_t)v;
        }
    }
    return t;
}
/* The object tile number of a type byte of the level map (the byte $7E is skipped). */
static int obj_index(int t) {
    return t <= 0x7D ? t : t - 1;
}

/* Reads the map of section `sec` from the game's RAM into a Level: for each row of 32 lines, the ground block and the
 * object tiles of each of the 3 blocks (columns of 64 pixels). The map is stored from the bottom up. */
static Level *parse_level(int sec) {
    uint32_t p = TABLE_T + 0x40 + mr16(TABLE_T + 2u * (uint32_t)sec);
    int cap = 64, nb = 0;
    LevBlk *bl = (LevBlk *)calloc((size_t)cap * 3, sizeof(LevBlk));
    int row = 0, col = 0;
    while (row < 4000 && p < RAMSZ - 8) {
        uint32_t g = mr8(p++);
        if (g == 0xFF)
            break;
        if (row * 3 + col >= cap * 3) {
            int nc = cap * 2;
            bl = (LevBlk *)realloc(bl, sizeof(LevBlk) * (size_t)nc * 3);
            memset(bl + (size_t)cap * 3, 0, sizeof(LevBlk) * (size_t)cap * 3);
            cap = nc;
        }
        LevBlk *b = &bl[row * 3 + col];
        b->ground = (int)(g & 0x7F);
        if (g & 0x80) {
            int oc = 0;
            while (p < RAMSZ && mr8(p) != 0) {
                uint32_t t = mr8(p++);
                /* evento */
                if (t >= 0xF1 && t <= 0xF4)
                    continue;
                if (b->nobj == oc) {
                    oc = oc ? oc * 2 : 4;
                    b->objt = (int *)realloc(b->objt, sizeof(int) * (size_t)oc);
                    b->objc = (int *)realloc(b->objc, sizeof(int) * (size_t)oc);
                }
                b->objt[b->nobj] = (int)t;
                b->objc[b->nobj] = (int)mr8(p);
                b->nobj++;
                p++;
            }
            /* terminador $00 */
            p++;
        }
        if (++col == 3) {
            col = 0;
            row++;
        }
    }
    if (row == 0) {
        free(bl);
        return NULL;
    }
    Level *L = (Level *)calloc(1, sizeof(Level));
    L->sec = sec;
    L->nrows = row;
    L->blk = (LevBlk *)calloc((size_t)(row + 8) * 3, sizeof(LevBlk));
    /* 8 rows of uniform floor below the map (prefix the game fills in when the section starts) */
    int g0 = bl[0].ground;
    for (int i = 0; i < 24; i++)
        L->blk[i].ground = g0;
    memcpy(L->blk + 24, bl, sizeof(LevBlk) * (size_t)row * 3);
    free(bl);
    L->H = 32 * (row + 8);
    for (int c = 0; c < 64; c++) {
        int off = (int16_t)mr16(POSTAB_T + 4u * (uint32_t)c);
        if (off >= 0) {
            L->pos_ok[c] = 1;
            L->pos_x[c] = ((off % 96) / 8 * 16) % 64;
            L->pos_y[c] = off / 96;
        }
    }
    for (int i = 0; i < (row + 8) * 3; i++) {
        LevBlk *b = &L->blk[i];
        if (!L->gtile[b->ground & 127])
            L->gtile[b->ground & 127] = ground_tile(b->ground & 127);
        for (int k = 0; k < b->nobj; k++) {
            int oi = obj_index(b->objt[k]) & 255;
            if (!L->otile[oi])
                L->otile[oi] = obj_tile(oi);
        }
    }
    return L;
}

/* ---- level registry ---- */
#define NLEV 24
static Level *levs[NLEV];
static int nlev = 0;
#ifdef _WIN32
static CRITICAL_SECTION level_lock;
static int lk_init = 0;
static void LEVEL_LOCK(void) {
    if (!lk_init) {
        InitializeCriticalSection(&level_lock);
        lk_init = 1;
    }
    EnterCriticalSection(&level_lock);
}
static void LEVEL_UNLOCK(void) {
    LeaveCriticalSection(&level_lock);
}
#else
static void LEVEL_LOCK(void) {}
static void LEVEL_UNLOCK(void) {}
#endif

Level *level_find(uint32_t id) {
    Level *r = NULL;
    LEVEL_LOCK();
    for (int i = 0; i < nlev; i++)
        if (levs[i]->id == id)
            r = levs[i];
    LEVEL_UNLOCK();
    return r;
}
/* ---- levels in memory ---- */
/* Frees a decoded level. */
static void level_free(Level *L) {
    for (int i = 0; i < (L->nrows + 8) * 3; i++) {
        free(L->blk[i].objt);
        free(L->blk[i].objc);
    }
    for (int i = 0; i < 128; i++)
        free(L->gtile[i]);
    for (int i = 0; i < 256; i++)
        free(L->otile[i]);
    free(L->blk);
    free(L);
}

/* FNV-1a hash of some bytes (the key of a section: its map and tiles). */
static uint32_t fnv1a(uint32_t h, const uint8_t *p, size_t n) {
    for (size_t i = 0; i < n; i++)
        h = (h ^ p[i]) * 16777619u;
    return h;
}
static uint32_t last_key = 0, last_id = 0;
/* A new mission: forget which level was seen last. */
void level_new_segment(void) {
    last_key = 0;
    last_id = 0;
}

/* First time a vertical-scroll section is seen (or after a mission change): its map is decoded with the current tiles
 */
void scene_check_level(Frame *f) {
    if (f->mode_h || !f->bg_ok || f->sec < 0 || f->sec > 0x7FFF)
        return;
    uint32_t start = TABLE_T + 0x40 + mr16(TABLE_T + 2u * (uint32_t)f->sec), key = 2166136261u;
    key = fnv1a(key, (const uint8_t *)&f->sec, sizeof f->sec);
    key = fnv1a(key, ram + TABLE_T, 0x40);
    if (start + 64 < RAMSZ)
        key = fnv1a(key, ram + start, 64);
    if (key == last_key && last_id) {
        f->lvl = last_id;
        return;
    }
    Level *L = parse_level(f->sec);
    if (!L)
        return;
    uint32_t id = 2166136261u;
    id = fnv1a(id, (const uint8_t *)&f->sec, sizeof f->sec);
    for (int i = 0; i < 128; i++)
        if (L->gtile[i])
            id = fnv1a(id, L->gtile[i], 32 * 64);
    for (int i = 0; i < 256; i++)
        if (L->otile[i])
            id = fnv1a(id, L->otile[i], 16 * 16);
    if (!id)
        id = 1;
    L->id = id;
    last_key = key;
    last_id = id;
    f->lvl = id;
    LEVEL_LOCK();
    /* it was already there (same section and same tiles) */
    for (int i = 0; i < nlev; i++)
        if (levs[i]->id == id) {
            LEVEL_UNLOCK();
            level_free(L);
            return;
        }
    if (nlev == NLEV) {
        Level *o = levs[0];
        memmove(levs, levs + 1, sizeof(Level *) * (NLEV - 1));
        nlev--;
        (void)o; /* the old level is left (it may be in use): bounded leak */
    }
    levs[nlev++] = L;
    LEVEL_UNLOCK();
}

/* ---- whole level in RGB (with tiles of the pack) ---- */
typedef struct LRGB {
    uint32_t id;
    uint8_t pal[32];
    int S, gen;
    uint8_t *rgb;
    int rows_px;
    volatile int ready;
    uint32_t stamp;
} LRGB;
#define NLR 16
static LRGB lr[NLR];
static int nlr = 0;
static uint32_t lr_stamp = 0;

typedef struct BuildJob {
    LRGB *e;
    Level *L;
} BuildJob;

/* ---- building the background RGB ---- */
/* Builds the RGB picture of a whole level for a palette at scale S, with the pack's replacement tiles (it runs in a
 * thread; the level is tall: some thousand lines). */
static void build(LRGB *e, Level *L) {
    int S = e->S;
    uint8_t pal[16][3];
    pal_to_rgb(e->pal, pal);
    int tw = 64 * S, th = 32 * S, nrows = L->nrows + 8, W = 192 * S, Hh = 32 * S * nrows;
    uint8_t *out = (uint8_t *)calloc((size_t)Hh * W * 3, 1);
    uint8_t *gcol[128] = {0};
    uint8_t *ocol[256] = {0};
    uint8_t *oal[256] = {0};
    for (int r = 0; r < nrows; r++)
        for (int c = 0; c < 3; c++) {
            LevBlk *b = &L->blk[r * 3 + c];
            int gi = b->ground & 127;
            if (!gcol[gi]) {
                gcol[gi] = (uint8_t *)malloc((size_t)tw * th * 3);
                const uint8_t *rep = gfxpack_ground_rgba(L->gtile[gi], S);
                if (rep)
                    for (int i = 0; i < tw * th; i++)
                        memcpy(gcol[gi] + i * 3, rep + i * 4, 3);
                else
                    for (int y = 0; y < th; y++)
                        for (int x = 0; x < tw; x++)
                            memcpy(gcol[gi] + ((size_t)y * tw + x) * 3, pal[L->gtile[gi][(y / S) * 64 + x / S]], 3);
            }
            uint8_t *tile = (uint8_t *)malloc((size_t)tw * th * 3);
            memcpy(tile, gcol[gi], (size_t)tw * th * 3);
            for (int k = 0; k < b->nobj; k++) {
                int code = b->objc[k] & 63;
                if (!L->pos_ok[code])
                    continue;
                int oi = obj_index(b->objt[k]) & 255;
                if (!L->otile[oi])
                    continue;
                int px = L->pos_x[code], py = L->pos_y[code], h = 32 - py < 16 ? 32 - py : 16,
                    w = 64 - px < 16 ? 64 - px : 16;
                if (h <= 0 || w <= 0)
                    continue;
                if (!ocol[oi]) {
                    int ow = 16 * S;
                    ocol[oi] = (uint8_t *)malloc((size_t)ow * ow * 3);
                    oal[oi] = (uint8_t *)malloc((size_t)ow * ow);
                    const uint8_t *rep = gfxpack_object_rgba(L->otile[oi], S);
                    for (int y = 0; y < ow; y++)
                        for (int x = 0; x < ow; x++) {
                            size_t q = (size_t)y * ow + x;
                            if (rep) {
                                memcpy(ocol[oi] + q * 3, rep + q * 4, 3);
                                oal[oi][q] = rep[q * 4 + 3];
                            } else {
                                int v = L->otile[oi][(y / S) * 16 + x / S];
                                memcpy(ocol[oi] + q * 3, pal[v], 3);
                                oal[oi][q] = v > 0 ? 255 : 0;
                            }
                        }
                }
                int ow = 16 * S;
                for (int y = 0; y < h * S; y++)
                    for (int x = 0; x < w * S; x++) {
                        size_t q = (size_t)y * ow + x;
                        uint8_t *d = tile + ((size_t)(py * S + y) * tw + (px * S + x)) * 3;
                        int a = oal[oi][q];
                        if (a == 255)
                            memcpy(d, ocol[oi] + q * 3, 3);
                        else if (a)
                            for (int ch = 0; ch < 3; ch++) {
                                float v = d[ch] * (1.0f - a / 255.0f) + ocol[oi][q * 3 + ch] * (a / 255.0f) + 0.5f;
                                d[ch] = (uint8_t)(v > 255 ? 255 : v);
                            }
                    }
            }
            int y0 = Hh - th * (r + 1), x0 = tw * c;
            for (int y = 0; y < th; y++)
                memcpy(out + ((size_t)(y0 + y) * W + x0) * 3, tile + (size_t)y * tw * 3, (size_t)tw * 3);
            free(tile);
        }
    for (int i = 0; i < 128; i++)
        free(gcol[i]);
    for (int i = 0; i < 256; i++) {
        free(ocol[i]);
        free(oal[i]);
    }
    e->rows_px = Hh;
    e->rgb = out;
    e->ready = 1;
}

#ifdef _WIN32
/* Thread entry of a background build. */
static DWORD WINAPI build_thread(LPVOID p) {
    BuildJob *j = (BuildJob *)p;
    build(j->e, j->L);
    free(j);
    gfxpack_builders(-1);
    return 0;
}
#endif

/* The RGB background of level `id` for a palette at scale S, or NULL if it is not ready (the build is started in a
 * thread and later calls find it). *rows_px = its height in pixels. */
const uint8_t *level_rgb(uint32_t id, const uint8_t *pal32, int S, int *rows_px) {
    LRGB *hit = NULL;
    extern int pack_gen;
    for (int i = 0; i < nlr; i++)
        if (lr[i].id == id && lr[i].S == S && lr[i].gen == pack_gen && !memcmp(lr[i].pal, pal32, 32)) {
            hit = &lr[i];
            break;
        }
    if (hit) {
        hit->stamp = ++lr_stamp;
        if (!hit->ready)
            return NULL;
        *rows_px = hit->rows_px;
        return hit->rgb;
    }
    Level *L = level_find(id);
    if (!L)
        return NULL;
    int slot = -1;
    if (nlr < NLR)
        slot = nlr++;
    /* the oldest ready one is reused (never one under construction) */
    else {
        uint32_t best = 0xFFFFFFFFu;
        for (int i = 0; i < NLR; i++)
            if (lr[i].ready && lr[i].stamp < best) {
                best = lr[i].stamp;
                slot = i;
            }
        if (slot < 0)
            return NULL;
        free(lr[slot].rgb);
    }
    LRGB *e = &lr[slot];
    memset(e, 0, sizeof *e);
    e->id = id;
    memcpy(e->pal, pal32, 32);
    e->S = S;
    e->gen = pack_gen;
    e->stamp = ++lr_stamp;
    BuildJob *j = (BuildJob *)malloc(sizeof *j);
    j->e = e;
    j->L = L;
    gfxpack_builders(1);
#ifdef _WIN32
    HANDLE h = CreateThread(NULL, 0, build_thread, j, 0, NULL);
    if (h)
        CloseHandle(h);
    else {
        build(e, L);
        free(j);
        gfxpack_builders(-1);
    }
#else
    build(e, L);
    free(j);
    gfxpack_builders(-1);
#endif
    return NULL;
}

/* Graphics dump: the whole vertical section `sec` of the game now in RAM as RGB at scale 1 (192 px wide, rows_px high),
   drawn with the original graphics and the 32-byte ST palette `pal32`. *gmax = highest floor block it uses. NULL if the
   section does not exist. */
/* For the graphics dump: the RGB picture of a level section with the first mission's palette; *gmax = the highest
 * ground block used. */
uint8_t *level_dump_rgb(int sec, const uint8_t *pal32, int *rows_px, int *gmax) {
    Level *L = parse_level(sec);
    if (!L)
        return NULL;
    int g = 0;
    for (int i = 0; i < (L->nrows + 8) * 3; i++)
        if (L->blk[i].ground > g)
            g = L->blk[i].ground;
    LRGB e;
    memset(&e, 0, sizeof e);
    memcpy(e.pal, pal32, 32);
    e.S = 1;
    build(&e, L);
    *rows_px = e.rows_px;
    *gmax = g;
    level_free(L);
    return e.rgb;
}
