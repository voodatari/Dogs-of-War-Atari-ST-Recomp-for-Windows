/*
 * sprites.c - decoding of the sprite tables in the game's RAM (verified against the drawing routines).
 *
 * Each table stores frames in ST planes (colors 0..15) with color 0 transparent (except the "raw" groups of the
 * vehicles). rom_sprite() returns the cropped opaque drawing of the frame and the position of its corner relative to
 * that of the game structure (ax, ay).
 */
#include "scene.h"

const char *unit_name[U_NUNITS] = {"enemy", "player", "bullet", "bullet2", "obj5e32", "obj5f1a",
                                   "fx",    "veh6",   "veh4",   "veh",     "turret",  "boom"};

/* A new sprite of w x h pixels (reference count 1) with its indices and mask. */
Sprite *sprite_new(int w, int h) {
    Sprite *s = (Sprite *)calloc(1, sizeof(Sprite));
    s->w = w;
    s->h = h;
    s->ref = 1;
    s->img = (uint8_t *)calloc((size_t)w * h, 1);
    s->mask = (uint8_t *)calloc((size_t)w * h, 1);
    return s;
}
/* Takes a reference to a sprite. */
Sprite *sprite_ref(Sprite *s) {
    if (s)
        __atomic_fetch_add(&s->ref, 1, __ATOMIC_ACQ_REL);
    return s;
}
/* Drops a reference; the last one frees the sprite. */
void sprite_unref(Sprite *s) {
    if (s && __atomic_sub_fetch(&s->ref, 1, __ATOMIC_ACQ_REL) <= 0) {
        free(s->img);
        free(s->mask);
        free(s);
    }
}

/* a group of 16 px: `np` plane words (ws[0] = plane 0). masked: opaque where some plane is set; force is added to the
 * opaque ones */
static void group(const uint32_t *ws, int np, int force, int masked, uint8_t *idx, uint8_t *m) {
    for (int b = 0; b < 16; b++) {
        int v = 0;
        for (int p = 0; p < np; p++)
            v |= (int)((ws[p] >> (15 - b)) & 1) << p;
        int op = masked ? (v != 0) : 1;
        if (force && op)
            v |= force;
        idx[b] = (uint8_t)(op ? v : 0);
        m[b] = (uint8_t)op;
    }
}

/* crops (idx, m: rows x cols) to its opaque content; ox, oy = position of the corner of the complete frame relative to
 * that of the structure */
static Sprite *crop(const uint8_t *idx, const uint8_t *m, int rows, int cols, int ox, int oy) {
    int x0 = cols, y0 = rows, x1 = -1, y1 = -1;
    for (int y = 0; y < rows; y++)
        for (int x = 0; x < cols; x++)
            if (m[y * cols + x]) {
                if (x < x0)
                    x0 = x;
                if (x > x1)
                    x1 = x;
                if (y < y0)
                    y0 = y;
                if (y > y1)
                    y1 = y;
            }
    if (x1 < 0)
        return NULL;
    Sprite *s = sprite_new(x1 - x0 + 1, y1 - y0 + 1);
    for (int y = 0; y < s->h; y++)
        for (int x = 0; x < s->w; x++) {
            s->img[y * s->w + x] = idx[(y0 + y) * cols + x0 + x];
            s->mask[y * s->w + x] = m[(y0 + y) * cols + x0 + x];
        }
    s->ax = ox + x0;
    s->ay = oy + y0;
    return s;
}

#define MAXC 64
/* generic decoder: rows of `ng` groups with a masking pattern, np planes per group (sequential words) */
static Sprite *dec_rows(uint32_t a, int rows, int ng, int np, int force, const char *pat, int rowstride, int ox,
                        int oy) {
    uint8_t idx[128 * MAXC], m[128 * MAXC];
    int cols = ng * 16;
    if (a + (uint32_t)rows * (uint32_t)rowstride > RAMSZ)
        return NULL;
    for (int r = 0; r < rows; r++)
        for (int g = 0; g < ng; g++) {
            uint32_t ws[4];
            for (int p = 0; p < np; p++)
                ws[p] = mr16(a + (uint32_t)r * (uint32_t)rowstride + (uint32_t)(g * np + p) * 2);
            group(ws, np, force, pat[g] == 'm', idx + r * cols + g * 16, m + r * cols + g * 16);
        }
    return crop(idx, m, rows, cols, ox, oy);
}

/* 25 rows x 4 long words (planes 0-3): high word = left 16 px, low word = right 16 px */
static Sprite *dec_player(int k) {
    uint8_t idx[25 * 32], m[25 * 32];
    uint32_t a = 0x2F91C + (uint32_t)k * 0x190;
    for (int r = 0; r < 25; r++) {
        uint32_t L[4], hi[4], lo[4];
        for (int p = 0; p < 4; p++) {
            L[p] = mr32(a + (uint32_t)r * 16 + (uint32_t)p * 4);
            hi[p] = L[p] >> 16;
            lo[p] = L[p] & 0xFFFF;
        }
        group(hi, 4, 0, 1, idx + r * 32, m + r * 32);
        group(lo, 4, 0, 1, idx + r * 32 + 16, m + r * 32 + 16);
    }
    return crop(idx, m, 25, 32, 0, 0);
}
/* 22 rows x 2 groups of 3 planes; plane 3 at 0 */
static Sprite *dec_boom(int k) {
    return dec_rows(0x388DC + (uint32_t)k * 0x108, 22, 2, 3, 0, "mm", 12, -36, -100);
}
/* 32 rows, 8 interleaved words (g0p0 g1p0 g0p1 g1p1 ...) */
static Sprite *dec_turret(int k) {
    uint8_t idx[32 * 32], m[32 * 32];
    uint32_t a = 0x45736 + (uint32_t)k * 0x200;
    for (int r = 0; r < 32; r++) {
        uint32_t w[8];
        for (int i = 0; i < 8; i++)
            w[i] = mr16(a + (uint32_t)r * 16 + (uint32_t)i * 2);
        uint32_t g0[4] = {w[0], w[2], w[4], w[6]}, g1[4] = {w[1], w[3], w[5], w[7]};
        group(g0, 4, 0, 1, idx + r * 32, m + r * 32);
        group(g1, 4, 0, 1, idx + r * 32 + 16, m + r * 32 + 16);
    }
    return crop(idx, m, 32, 32, 0, 0);
}

/* 2x2 px (1x2 in phase 7): color 8 (bullet) or 0 (bullet2) */
static Sprite *bullet_sprite(int unit, int ph) {
    int w = ph == 7 ? 1 : 2;
    Sprite *s = sprite_new(w, 2);
    for (int i = 0; i < w * 2; i++) {
        s->img[i] = unit == U_BULLET ? 8 : 0;
        s->mask[i] = 1;
    }
    s->ax = -0x24;
    s->ay = -0x64;
    return s;
}

/* The sprite of frame (key0, key1) of a unit type, decoded from the game's tables in RAM: the cropped opaque drawing
 * and the position of its corner relative to the game structure. NULL if the frame cannot be decoded (the scene
 * capture then measures it from the screen). */
Sprite *rom_sprite(int unit, int key0, int key1) {
    switch (unit) {
        case U_ENEMY:
            if (key0 < 0 || key0 > 38)
                return NULL;
            return dec_rows(0x5E60A + (uint32_t)key0 * 0x84, 22, 1, 3, 8, "m", 6, -36, -100);
        case U_PLAYER:
            if (key0 < 0 || key0 > 91)
                return NULL;
            return dec_player(key0);
        case U_OBJ5E32:
            if (key0 < 0 || key0 > 28)
                return NULL;
            return dec_rows(0x4694A + (uint32_t)key0 * 0x36, 9, 1, 3, 8, "m", 6, -36, -100);
        case U_OBJ5F1A:
            if (key0 < 0 || key0 > 28)
                return NULL;
            return dec_rows(0x4694A + (uint32_t)key0 * 0x36, 9, 1, 3, 0, "m", 6, -36, -100);
        case U_FX:
            if (key0 < 0 || key0 > 11)
                return NULL;
            return dec_rows(0x49A3A + (uint32_t)key0 * 0x80, 16, 1, 4, 0, "m", 8, -36, -100);
        case U_BOOM:
            if (key0 < 0 || key0 > 10)
                return NULL;
            return dec_boom(key0);
        case U_VEH6:
            if (key0 < 0 || key0 > 1)
                return NULL;
            return dec_rows(0x3EA7C + (uint32_t)key0 * 0x300, 48, 2, 4, 0, "mm", 16, -36, -100);
        case U_VEH4:
            if (key0 < 0 || key0 > 1)
                return NULL;
            return dec_rows(0x406FC + (uint32_t)key0 * 0x5E8, 63, 3, 4, 0, "mmm", 24, -36, -100);
        case U_VEH:
            if (key0 < 0 || key0 > 1)
                return NULL;
            return dec_rows((key1 ? 0x3F07C : 0x3D3FC) + (uint32_t)key0 * 0xB40, 90, 4, 4, 0, "mrrm", 32, -36, -100);
        case U_TURRET:
            if (key0 < 0xF0 || key0 > 0xF5)
                return NULL;
            return dec_turret(key0 - 0xF0);
        case U_BULLET:
        case U_BULLET2:
            if (key0 < 0 || key0 > 7)
                return NULL;
            return bullet_sprite(unit, key0);
    }
    return NULL;
}

/* Converts interleaved ST bitplane words (rows x groups of 16 pixels) into one colour index (0..15) per pixel. */
void planes_to_idx(const uint8_t *raw, int rows, int groups, uint8_t *out) {
    for (int y = 0; y < rows; y++)
        for (int g = 0; g < groups; g++) {
            const uint8_t *p = raw + ((size_t)y * groups + g) * 8;
            uint32_t w0 = (p[0] << 8) | p[1], w1 = (p[2] << 8) | p[3], w2 = (p[4] << 8) | p[5], w3 = (p[6] << 8) | p[7];
            uint8_t *o = out + (size_t)y * groups * 16 + g * 16;
            for (int b = 0; b < 16; b++) {
                int s = 15 - b;
                o[b] = (uint8_t)(((w0 >> s) & 1) | (((w1 >> s) & 1) << 1) | (((w2 >> s) & 1) << 2) |
                                 (((w3 >> s) & 1) << 3));
            }
        }
}
