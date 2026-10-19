/*
 * dump.c - Graphics dump of the game (assets/original): every sprite, tile, fixed screen and level of the game as editable PNG sheets + index.json.
 *
 * It is made from YOUR copy of the game, the first time it is needed: the program plays the game by itself (title -> map -> shop -> each of the 12 missions) with
 * scripted joystick/key input, and decodes the graphics tables the game leaves in RAM (formats in the "memory" entries of index.json and in README.txt of the dump).
 * The hashes in index.json are those used by gfxpack.c to recognize each graphic by content, so the layout must not change.
 */
#include <stdarg.h>
#include "scene.h"
#include "level.h"
#include "gfxpack.h"
#include "input.h"
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#endif

#define NMISSIONS 12
#define MARGIN 4
#define GROUND_T 0x5649Au
#define OBJECT_T 0x57C9Au
#define NOBJ 210
#define NGROUND 6
#define MAP_PAL "\x00\x00\x07\x77\x00\x12\x00\x14\x00\x25\x01\x36\x02\x46\x03\x56\x04\x67\x07\x40\x06\x30\x05\x20\x03\x53\x02\x42\x01\x31\x00\x20"
#define SHOP_PAL "\x00\x00\x07\x62\x05\x55\x04\x43\x02\x33\x01\x52\x05\x20\x00\x21\x07\x76\x01\x22\x05\x32\x03\x20\x01\x31\x00\x11\x00\x47\x05\x42"

#define SAMPLE_PHASE 0                                         /* screens are sampled when vbl % 4 == this */
static void (*progress_cb)(const char *) = NULL;
static void say(const char *fmt, ...) {
    char b[200]; va_list ap; va_start(ap, fmt); vsnprintf(b, sizeof b, fmt, ap); va_end(ap);
    host_log("graphics dump: %s", b); if (progress_cb) progress_cb(b);
}

/* ------------------------------------------------------------------------------------------------ files and JSON */
static char out_root[700];
static void make_dirs(const char *file) {                      /* creates the folders of `file` (path with / or \) */
    char p[800]; snprintf(p, sizeof p, "%s", file);
    for (char *c = p + 1; *c; c++) if (*c == '/' || *c == '\\') { char t = *c; *c = 0;
#ifdef _WIN32
        _mkdir(p);
#endif
        *c = t; }
}
static void out_path(char *dst, size_t n, const char *rel) { snprintf(dst, n, "%s/%s", out_root, rel); }

typedef struct { char *s; size_t n, cap; } SB;
static void sb_add(SB *b, const char *fmt, ...) {
    va_list ap, ap2; va_start(ap, fmt); va_copy(ap2, ap); int k = vsnprintf(NULL, 0, fmt, ap); va_end(ap); if (k < 0) { va_end(ap2); return; }
    if (b->n + (size_t)k + 1 > b->cap) { b->cap = (b->n + (size_t)k + 1) * 2; b->s = (char *)realloc(b->s, b->cap); }
    vsnprintf(b->s + b->n, (size_t)k + 1, fmt, ap2); va_end(ap2); b->n += (size_t)k;
}
static void sb_str(SB *b, const char *s) {                    /* JSON string */
    sb_add(b, "\""); for (; *s; s++) { if (*s == '"' || *s == '\\') sb_add(b, "\\%c", *s); else if ((unsigned char)*s < 32) sb_add(b, " "); else sb_add(b, "%c", *s); } sb_add(b, "\"");
}
static SB sheets;                                              /* body of the "sheets" object */
static SB reflevels;                                           /* names of the reference levels */
static int nsheets = 0, nreflev = 0;

typedef struct { int i; int has; char hash[16]; int fw, fh; int cx, cy, cw, ch; char key[40]; char mem[80]; } Cell;
static void sb_cells(SB *b, const Cell *c, int n) {
    sb_add(b, "[");
    for (int i = 0; i < n; i++) {
        sb_add(b, "%s{\"i\":%d", i ? "," : "", c[i].i);
        if (c[i].has) { sb_add(b, ",\"hash\":\"%s\",\"frame\":[%d,%d],\"crop\":[%d,%d,%d,%d]", c[i].hash, c[i].fw, c[i].fh, c[i].cx, c[i].cy, c[i].cw, c[i].ch); if (c[i].key[0]) { sb_add(b, ",\"key\":"); sb_str(b, c[i].key); } if (c[i].mem[0]) { sb_add(b, ",\"mem\":"); sb_str(b, c[i].mem); } }
        sb_add(b, "}");
    }
    sb_add(b, "]");
}
static void sb_ints(SB *b, const int *v, int n) { sb_add(b, "["); for (int i = 0; i < n; i++) sb_add(b, "%s%d", i ? "," : "", v[i]); sb_add(b, "]"); }

/* ------------------------------------------------------------------------------------------------ palettes */
typedef uint8_t Rgb16[16][3];
static Rgb16 pals[NMISSIONS];
static void pal_cols(const uint8_t *pal32, Rgb16 out) {
    for (int i = 0; i < 16; i++) { int v = (pal32[2 * i] << 8) | pal32[2 * i + 1]; int c[3] = { (v >> 8) & 15, (v >> 4) & 15, v & 15 }; for (int k = 0; k < 3; k++) out[i][k] = (uint8_t)(((c[k] & 7) << 1 | ((c[k] >> 3) & 1)) * 17); }
}

/* ------------------------------------------------------------------------------------------------ decoding of the graphics tables */
typedef struct { int w, h; uint8_t *idx, *mask; } Fr;
static Fr fr_new(int w, int h) { Fr f; f.w = w; f.h = h; f.idx = (uint8_t *)calloc((size_t)w * h, 1); f.mask = (uint8_t *)calloc((size_t)w * h, 1); return f; }
static void grp(const uint32_t *ws, int np, int force, int masked, uint8_t *idx, uint8_t *m) {
    for (int b = 0; b < 16; b++) {
        int v = 0; for (int p = 0; p < np; p++) v |= (int)((ws[p] >> (15 - b)) & 1) << p;
        int op = masked ? (v != 0) : 1; if (force && op) v |= force;
        idx[b] = (uint8_t)(op ? v : 0); m[b] = (uint8_t)op;
    }
}
/* rows of `ng` groups of 16 px with `np` sequential planes; pat[g] = 'm' masked, 'r' raw */
static Fr rows_fr(uint32_t a, int rows, int ng, int np, int force, const char *pat, int rowstride) {
    Fr f = fr_new(ng * 16, rows);
    for (int r = 0; r < rows; r++) for (int g = 0; g < ng; g++) {
        uint32_t ws[4]; for (int p = 0; p < np; p++) ws[p] = mr16(a + (uint32_t)r * (uint32_t)rowstride + (uint32_t)(g * np + p) * 2);
        grp(ws, np, force, pat[g] == 'm', f.idx + r * f.w + g * 16, f.mask + r * f.w + g * 16);
    }
    return f;
}
static Fr dec_enemy(int k) { return rows_fr(0x5E60A + (uint32_t)k * 0x84, 22, 1, 3, 8, "m", 6); }
static Fr dec_shots(int k) { return rows_fr(0x4694A + (uint32_t)k * 0x36, 9, 1, 3, 8, "m", 6); }
static Fr dec_shots_low(int k) { return rows_fr(0x4694A + (uint32_t)k * 0x36, 9, 1, 3, 0, "m", 6); }
static Fr dec_fx(int k) { return rows_fr(0x49A3A + (uint32_t)k * 0x80, 16, 1, 4, 0, "m", 8); }
static Fr dec_boom(int k) { return rows_fr(0x388DC + (uint32_t)k * 0x108, 22, 2, 3, 0, "mm", 12); }
static Fr dec_veh_a(int k) { return rows_fr(0x3D3FC + (uint32_t)k * 0xB40, 90, 4, 4, 0, "mrrm", 32); }
static Fr dec_veh_b(int k) { return rows_fr(0x3F07C + (uint32_t)k * 0xB40, 90, 4, 4, 0, "mrrm", 32); }
static Fr dec_veh_c(int k) { return rows_fr(0x3EA7C + (uint32_t)k * 0x300, 48, 2, 4, 0, "mm", 16); }
static Fr dec_veh_d(int k) { return rows_fr(0x406FC + (uint32_t)k * 0x5E8, 63, 3, 4, 0, "mmm", 24); }
static Fr dec_player(int k) {                                  /* per row 4 long words (planes 0-3): high word = left 16 px, low word = right 16 px */
    Fr f = fr_new(32, 25); uint32_t a = 0x2F91C + (uint32_t)k * 0x190;
    for (int r = 0; r < 25; r++) {
        uint32_t hi[4], lo[4]; for (int p = 0; p < 4; p++) { uint32_t L = mr32(a + (uint32_t)r * 16 + (uint32_t)p * 4); hi[p] = L >> 16; lo[p] = L & 0xFFFF; }
        grp(hi, 4, 0, 1, f.idx + r * 32, f.mask + r * 32); grp(lo, 4, 0, 1, f.idx + r * 32 + 16, f.mask + r * 32 + 16);
    }
    return f;
}
static Fr dec_turret(int k) {                                  /* 8 interleaved words per row: g0p0 g1p0 g0p1 g1p1 ... */
    Fr f = fr_new(32, 32); uint32_t a = 0x45736 + (uint32_t)k * 0x200;
    for (int r = 0; r < 32; r++) {
        uint32_t w[8]; for (int i = 0; i < 8; i++) w[i] = mr16(a + (uint32_t)r * 16 + (uint32_t)i * 2);
        uint32_t g0[4] = { w[0], w[2], w[4], w[6] }, g1[4] = { w[1], w[3], w[5], w[7] };
        grp(g0, 4, 0, 1, f.idx + r * 32, f.mask + r * 32); grp(g1, 4, 0, 1, f.idx + r * 32 + 16, f.mask + r * 32 + 16);
    }
    return f;
}

typedef struct { const char *name; Fr (*dec)(int); int n, cols; const char *units; const char *table, *stride; int rows; const char *format, *note; } SpriteDef;
static const SpriteDef SPRITES[] = {
    { "player", dec_player, 92, 8, "player", "$2F91C", "$190", 25, "per row 4 long words = bitplanes 0-3; high word = left 16 px, low word = right 16 px; colour 0 = transparent", "two soldiers (blond and dark-haired): walking, aiming, firing and death animations" },
    { "enemy", dec_enemy, 39, 8, "enemy", "$5E60A", "$84", 22, "per row 3 words = bitplanes 0-2 (16 px); plane 3 is set wherever any bit is set (colours 9-15); 0 = transparent", "the game also keeps 7 copies shifted by 2 px steps (stride $2838 per step) derived from these frames" },
    { "shots", dec_shots, 29, 8, "obj5e32", "$4694A", "$36", 9, "per row 3 words = bitplanes 0-2 (16 px); plane 3 forced; 0 = transparent", "projectiles, grenades, rockets, sparks" },
    { "shots_b", dec_shots_low, 29, 8, "obj5f1a", "$4694A", "$36", 9, "same table as \"shots\" but routine $5F1A clears plane 3 (colours 1-7 instead of 9-15)", "the same drawings in the low colour bank" },
    { "turrets", dec_turret, 6, 6, "turret", "$45736", "$200", 32, "per row 2 groups of 16 px; the 8 words are interleaved (g0p0 g1p0 g0p1 g1p1 g0p2 g1p2 g0p3 g1p3); colour 0 = transparent", "fixed mines and gun emplacements (object list $18E6, frame = type - $F0): bunker, dome, camouflaged bush, turret, spiked mine, wreck" },
    { "fx", dec_fx, 12, 6, "fx", "$49A3A", "$80", 16, "per row 4 words = bitplanes 0-3 (16 px); 0 = transparent", "smoke / dust / sparks (effects)" },
    { "boom", dec_boom, 11, 4, "boom", "$388DC", "$108", 22, "per row 2 groups of 3 words = bitplanes 0-2 (32 px); plane 3 = 0; 0 = transparent (dithered)", "explosion" },
    { "vehicle_a", dec_veh_a, 2, 2, "veh", "$3D3FC", "$B40", 90, "per row 4 groups of 16 px x 4 planes: masked, raw, raw, masked", "vehicle (types 0-1): intact, destroyed" },
    { "vehicle_b", dec_veh_b, 2, 2, "veh", "$3F07C", "$B40", 90, "same as vehicle_a", "vehicle (types 2+): intact, destroyed" },
    { "vehicle_c", dec_veh_c, 2, 2, "veh6", "$3EA7C", "$300", 48, "per row 2 groups of 16 px x 4 planes, masked", "small vehicle (type 6)" },
    { "vehicle_d", dec_veh_d, 2, 2, "veh4", "$406FC", "$5E8", 63, "per row 3 groups of 16 px x 4 planes, masked", "large vehicle (type 4)" },
};
#define NSPRITES ((int)(sizeof SPRITES / sizeof SPRITES[0]))

/* ------------------------------------------------------------------------------------------------ sheets */
static void fr_hash(const Fr *f, int sprite, char *hex, int *cx, int *cy, int *cw, int *ch) {
    int x0 = f->w, y0 = f->h, x1 = -1, y1 = -1;
    for (int y = 0; y < f->h; y++) for (int x = 0; x < f->w; x++) if (f->mask[y * f->w + x]) { if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y; }
    *cx = x0; *cy = y0; *cw = x1 - x0 + 1; *ch = y1 - y0 + 1;
    if (sprite) {
        Sprite s; memset(&s, 0, sizeof s); s.w = *cw; s.h = *ch; s.img = (uint8_t *)malloc((size_t)s.w * s.h); s.mask = (uint8_t *)malloc((size_t)s.w * s.h);
        for (int y = 0; y < s.h; y++) for (int x = 0; x < s.w; x++) { s.img[y * s.w + x] = f->idx[(y0 + y) * f->w + x0 + x]; s.mask[y * s.w + x] = f->mask[(y0 + y) * f->w + x0 + x]; }
        gfx_sprite_hash(&s, hex); free(s.img); free(s.mask);
    } else gfx_array_hash(f->idx, f->h, f->w, hex);
}
/* frames (all of the same size; NULL entries = nothing) -> sheet RGBA (malloc) + cells. */
static uint8_t *make_sheet(Fr *frames, int n, int cols, int cw, int ch, int margin, const Rgb16 C, int sprite, Cell *cells, int *W, int *H) {
    int rows = (n + cols - 1) / cols; *W = cols * cw; *H = rows * ch; uint8_t *sh = (uint8_t *)calloc((size_t)*W * *H * 4, 1);
    for (int i = 0; i < n; i++) {
        memset(&cells[i], 0, sizeof cells[i]); cells[i].i = i; const Fr *f = &frames[i]; if (!f->idx) continue;
        int x = (i % cols) * cw, y = (i / cols) * ch, any = 0;
        for (int yy = 0; yy < f->h; yy++) for (int xx = 0; xx < f->w; xx++) if (f->mask[yy * f->w + xx]) {
            any = 1; uint8_t *d = sh + ((size_t)(y + margin + yy) * *W + x + margin + xx) * 4; const uint8_t *c = C[f->idx[yy * f->w + xx]]; d[0] = c[0]; d[1] = c[1]; d[2] = c[2]; d[3] = 255;
        }
        if (any) { cells[i].has = 1; cells[i].fw = f->w; cells[i].fh = f->h; fr_hash(f, sprite, cells[i].hash, &cells[i].cx, &cells[i].cy, &cells[i].cw, &cells[i].ch); }
    }
    return sh;
}
static void save_rgba(const char *rel, const uint8_t *px, int w, int h) { char p[800]; out_path(p, sizeof p, rel); make_dirs(p); png_write(p, px, w, h, 4); }

static uint64_t fnv64(uint64_t h, const void *p, size_t n) { const uint8_t *d = (const uint8_t *)p; for (size_t i = 0; i < n; i++) h = (h ^ d[i]) * 1099511628211ULL; return h; }

static uint8_t *mram[NMISSIONS];                               /* RAM of each mission once it is loaded */
static void use_mission(int m) { memcpy(ram, mram[m], RAMSZ); }
static int used_gmax[NMISSIONS];

typedef struct { int g[NMISSIONS]; int n; uint64_t sig; } Group;
static int make_groups(const uint64_t *sig, Group *gr) {       /* groups of missions with the same signature, in order of their first mission */
    int ng = 0;
    for (int m = 0; m < NMISSIONS; m++) {
        int k = 0; for (; k < ng; k++) if (gr[k].sig == sig[m]) break;
        if (k == ng) { gr[ng].sig = sig[m]; gr[ng].n = 0; ng++; }
        gr[k].g[gr[k].n++] = m;
    }
    return ng;
}

static void dump_sprites(void) {
    for (int s = 0; s < NSPRITES; s++) {
        const SpriteDef *d = &SPRITES[s]; Fr *per[NMISSIONS]; uint64_t sig[NMISSIONS];
        for (int m = 0; m < NMISSIONS; m++) {
            use_mission(m); per[m] = (Fr *)calloc((size_t)d->n, sizeof(Fr)); uint64_t h = 1469598103934665603ULL;
            for (int k = 0; k < d->n; k++) { per[m][k] = d->dec(k); h = fnv64(h, per[m][k].idx, (size_t)per[m][k].w * per[m][k].h); h = fnv64(h, per[m][k].mask, (size_t)per[m][k].w * per[m][k].h); }
            sig[m] = h;
        }
        Group gr[NMISSIONS]; int ng = make_groups(sig, gr);
        for (int gi = 0; gi < ng; gi++) {
            int m0 = gr[gi].g[0]; Fr *frames = per[m0]; int cw = frames[0].w + 2 * MARGIN, ch = frames[0].h + 2 * MARGIN; Cell cells[128]; int W, H;
            uint8_t *sh = make_sheet(frames, d->n, d->cols, cw, ch, MARGIN, pals[m0], 1, cells, &W, &H);
            char name[80]; if (ng == 1) snprintf(name, sizeof name, "sprites/%s", d->name); else snprintf(name, sizeof name, "sprites/%s_%d", d->name, gi + 1);
            for (int i = 0; i < d->n; i++) if (cells[i].has) { snprintf(cells[i].key, sizeof cells[i].key, "%d", i); snprintf(cells[i].mem, sizeof cells[i].mem, "%s+%d*%s", d->table, i, d->stride); }
            char rel[100]; snprintf(rel, sizeof rel, "%s.png", name); save_rgba(rel, sh, W, H); free(sh);
            int ms[NMISSIONS]; for (int j = 0; j < gr[gi].n; j++) ms[j] = gr[gi].g[j] + 1;
            if (nsheets++) sb_add(&sheets, ",");
            sb_str(&sheets, name); sb_add(&sheets, ":{\"kind\":\"sprite\",\"width\":%d,\"height\":%d,\"cell\":[%d,%d],\"cols\":%d,\"margin\":%d,\"units\":[\"%s\"],\"missions\":", W, H, cw, ch, d->cols, MARGIN, d->units);
            sb_ints(&sheets, ms, gr[gi].n); sb_add(&sheets, ",\"palette_mission\":%d,\"memory\":{\"table\":\"%s\",\"stride\":\"%s\",\"rows\":%d,\"format\":", m0 + 1, d->table, d->stride, d->rows);
            sb_str(&sheets, d->format); sb_add(&sheets, ",\"note\":"); sb_str(&sheets, d->note); sb_add(&sheets, "},\"cells\":"); sb_cells(&sheets, cells, d->n); sb_add(&sheets, "}");
            say("%s", name);
        }
        for (int m = 0; m < NMISSIONS; m++) { for (int k = 0; k < d->n; k++) { free(per[m][k].idx); free(per[m][k].mask); } free(per[m]); }
    }
}

/* the 2x2 px bullets are not in a table: routines $6848 and $68DC clear/set them with bit masks (colour 8 and colour 0) */
static void dump_bullets(void) {
    Fr fr[4]; Cell cells[4]; const char *keys[4] = { "bullet:0", "bullet:7", "bullet2:0", "bullet2:7" }; const char *units[4] = { "bullet", "bullet", "bullet2", "bullet2" };
    for (int i = 0; i < 4; i++) {
        fr[i] = fr_new(2, 2); int w = (i & 1) ? 1 : 2, col = i < 2 ? 8 : 0;
        for (int y = 0; y < 2; y++) for (int x = 0; x < w; x++) { fr[i].idx[y * 2 + x] = (uint8_t)col; fr[i].mask[y * 2 + x] = 1; }
    }
    int W, H; uint8_t *sh = make_sheet(fr, 4, 8, 10, 10, MARGIN, pals[0], 1, cells, &W, &H);
    for (int i = 0; i < 4; i++) if (cells[i].has) { snprintf(cells[i].key, sizeof cells[i].key, "%s", keys[i]); }
    save_rgba("sprites/bullets.png", sh, W, H); free(sh);
    if (nsheets++) sb_add(&sheets, ",");
    sb_add(&sheets, "\"sprites/bullets\":{\"kind\":\"sprite\",\"width\":%d,\"height\":%d,\"cell\":[10,10],\"cols\":8,\"margin\":%d,\"units\":[\"bullet\",\"bullet2\"],\"missions\":[1,2,3,4,5,6,7,8,9,10,11,12],\"palette_mission\":1,\"memory\":{\"note\":\"no table: routines $6848 and $68DC clear/set 2x2 px with bit masks (colour 8 and colour 0)\"},\"cells\":", W, H, MARGIN);
    /* the cells carry the unit too */
    sb_add(&sheets, "["); for (int i = 0; i < 4; i++) { sb_add(&sheets, "%s{\"i\":%d", i ? "," : "", i); if (cells[i].has) { sb_add(&sheets, ",\"hash\":\"%s\",\"frame\":[%d,%d],\"crop\":[%d,%d,%d,%d],\"key\":\"%s\",\"unit\":\"%s\"", cells[i].hash, cells[i].fw, cells[i].fh, cells[i].cx, cells[i].cy, cells[i].cw, cells[i].ch, keys[i], units[i]); } sb_add(&sheets, "}"); }
    sb_add(&sheets, "]}"); say("sprites/bullets");
}

static uint8_t *obj_fr(int i, Fr *f) {                         /* 16x16 object tile: 4 planes of 16 px per line (8 bytes), colour 0 transparent */
    *f = fr_new(16, 16);
    for (int r = 0; r < 16; r++) {
        uint32_t w[4]; for (int p = 0; p < 4; p++) w[p] = mr16(OBJECT_T + (uint32_t)i * 128u + (uint32_t)r * 8u + (uint32_t)p * 2u);
        for (int c = 0; c < 16; c++) { int v = 0; for (int p = 0; p < 4; p++) v |= (int)((w[p] >> (15 - c)) & 1) << p; f->idx[r * 16 + c] = (uint8_t)v; f->mask[r * 16 + c] = v > 0; }
    }
    return f->idx;
}
static void dump_tiles(void) {
    uint64_t sig[NMISSIONS]; for (int m = 0; m < NMISSIONS; m++) sig[m] = fnv64(1469598103934665603ULL, mram[m] + GROUND_T, OBJECT_T + NOBJ * 128 - GROUND_T);
    Group gr[NMISSIONS]; int ngr = make_groups(sig, gr);
    for (int gi = 0; gi < ngr; gi++) {
        int m0 = gr[gi].g[0]; use_mission(m0);
        int ng = NGROUND; for (int j = 0; j < gr[gi].n; j++) if (used_gmax[gr[gi].g[j]] + 1 > ng) ng = used_gmax[gr[gi].g[j]] + 1;
        int ms[NMISSIONS]; for (int j = 0; j < gr[gi].n; j++) ms[j] = gr[gi].g[j] + 1;
        Fr gf[128]; Cell cells[256]; int W, H; char name[60], rel[80];
        for (int i = 0; i < ng; i++) { gf[i] = fr_new(64, 32); planes_to_idx(ram + GROUND_T + (uint32_t)i * 1024u, 32, 4, gf[i].idx); memset(gf[i].mask, 1, 64 * 32); }
        uint8_t *sh = make_sheet(gf, ng, 6, 64, 32, 0, pals[m0], 0, cells, &W, &H);
        for (int i = 0; i < ng; i++) if (cells[i].has) { snprintf(cells[i].key, sizeof cells[i].key, "%d", i); snprintf(cells[i].mem, sizeof cells[i].mem, "$5649A+%d*$400", i); }
        snprintf(name, sizeof name, "tiles/ground_%d", gi + 1); snprintf(rel, sizeof rel, "%s.png", name); save_rgba(rel, sh, W, H); free(sh);
        if (nsheets++) sb_add(&sheets, ",");
        sb_str(&sheets, name); sb_add(&sheets, ":{\"kind\":\"ground\",\"width\":%d,\"height\":%d,\"cell\":[64,32],\"cols\":6,\"margin\":0,\"missions\":", W, H); sb_ints(&sheets, ms, gr[gi].n);
        sb_add(&sheets, ",\"palette_mission\":%d,\"cells\":", m0 + 1); sb_cells(&sheets, cells, ng);
        sb_add(&sheets, ",\"memory\":{\"table\":\"$5649A\",\"stride\":\"$400\",\"rows\":32,\"format\":\"32 lines x 4 groups of 16 px x 4 planes (64 px wide); in horizontal sections the left half (32 px) is repeated\",\"note\":\"6 ground blocks; the RAM that follows is the object tiles\"}}");
        for (int i = 0; i < ng; i++) { free(gf[i].idx); free(gf[i].mask); }
        Fr of[NOBJ]; for (int i = 0; i < NOBJ; i++) obj_fr(i, &of[i]);
        sh = make_sheet(of, NOBJ, 16, 16, 16, 0, pals[m0], 0, cells, &W, &H);
        for (int i = 0; i < NOBJ; i++) if (cells[i].has) { snprintf(cells[i].key, sizeof cells[i].key, "%d", i); snprintf(cells[i].mem, sizeof cells[i].mem, "$57C9A+%d*$80", i); }
        snprintf(name, sizeof name, "tiles/objects_%d", gi + 1); snprintf(rel, sizeof rel, "%s.png", name); save_rgba(rel, sh, W, H); free(sh);
        sb_add(&sheets, ",");
        sb_str(&sheets, name); sb_add(&sheets, ":{\"kind\":\"object\",\"width\":%d,\"height\":%d,\"cell\":[16,16],\"cols\":16,\"margin\":0,\"missions\":", W, H); sb_ints(&sheets, ms, gr[gi].n);
        sb_add(&sheets, ",\"palette_mission\":%d,\"cells\":", m0 + 1); sb_cells(&sheets, cells, NOBJ);
        sb_add(&sheets, ",\"memory\":{\"table\":\"$57C9A\",\"stride\":\"$80\",\"rows\":16,\"format\":\"16 lines x 4 planes of 16 px (8 bytes per line); colour 0 = transparent\",\"note\":\"210 tiles (indices 0-209); in the level map the code t is the index if t<=$7D and t-1 above that\"}}");
        for (int i = 0; i < NOBJ; i++) { free(of[i].idx); free(of[i].mask); }
        say("tiles %d", gi + 1);
    }
}

static void dump_font(void) {                                  /* the 8x8 character set at $8700 (reference only) */
    use_mission(0); int n = 64, cols = 16, W = cols * 8, H = ((n + cols - 1) / cols) * 8; uint8_t *sh = (uint8_t *)calloc((size_t)W * H * 4, 1);
    for (int c = 0; c < n; c++) for (int y = 0; y < 8; y++) { int g = mr8(0x8700 + (uint32_t)c * 8 + (uint32_t)y);
        for (int x = 0; x < 8; x++) { uint8_t *d = sh + ((size_t)((c / cols) * 8 + y) * W + (c % cols) * 8 + x) * 4; d[0] = d[1] = d[2] = 255; d[3] = ((g >> (7 - x)) & 1) ? 255 : 0; } }
    save_rgba("reference/font_8x8.png", sh, W, H); free(sh);
}

/* ------------------------------------------------------------------------------------------------ scripted play */
static uint8_t *scr;                                           /* current screen (indices) */
static Zone scz[MAXZONES]; static int nscz;
static void step(int n) {
    for (int i = 0; i < n; i++) { if (tos_run_slice(0x7FFFFFFF) < 0) return; tos_vbl(); ym_events_clear(); }
}
static void tap(int st, int n, int wait) { tos_joystick(1, st); step(n); tos_joystick(1, 0); step(wait); }
static void tapkey(int sc) { tos_key(sc); step(4); tos_key(sc | 0x80); step(6); }
static void cursor(int *x, int *y) { *x = (int)mr16(0x1EC); *y = (int)mr16(0x1EE); }
static void goto_xy(int tx, int ty, int speed, int tol) {      /* moves the game's cursor ($1EC/$1EE) to (tx, ty) */
    for (int it = 0; it < 14; it++) {
        int x, y; cursor(&x, &y); int dx = tx - x, dy = ty - y; if (abs(dx) <= tol && abs(dy) <= tol) return;
        int st = 0, n = 0, c = 0; int cand[2];
        if (abs(dx) > tol) { st |= dx > 0 ? J_RIGHT : J_LEFT; cand[c++] = abs(dx); }
        if (abs(dy) > tol) { st |= dy > 0 ? J_DOWN : J_UP; cand[c++] = abs(dy); }
        n = c == 2 ? (cand[0] < cand[1] ? cand[0] : cand[1]) : cand[0]; n /= speed; if (n < 1) n = 1; if (n > 60) n = 60;
        tos_joystick(1, st); step(n); tos_joystick(1, 0); step(6);
    }
}
static void shop_cell(int col, int row) { goto_xy(32 + 64 * col, 12 + 24 * row, 4, 3); }

typedef struct { int tag, vbl; uint8_t *idx; Zone z[MAXZONES]; int nz; } Snap;
static Snap *snaps; static int nsnaps, capsnaps; static int snap_tag; static int sampling;
static void sample(void) {
    if (nsnaps == capsnaps) { capsnaps = capsnaps ? capsnaps * 2 : 256; snaps = (Snap *)realloc(snaps, sizeof(Snap) * (size_t)capsnaps); }
    Snap *s = &snaps[nsnaps++]; s->tag = snap_tag; s->vbl = (int)tos.vbl; s->idx = (uint8_t *)malloc(64000); tos_screen_idx(s->idx, s->z, &s->nz);
}
static void step_s(int n) { for (int i = 0; i < n; i++) { step(1); if (sampling && (int)(tos.vbl % 4) == SAMPLE_PHASE) sample(); } }
static void tap_s(int st, int n, int wait) { tos_joystick(1, st); step_s(n); tos_joystick(1, 0); step_s(wait); }
static void tapkey_s(int sc) { tos_key(sc); step_s(4); tos_key(sc | 0x80); step_s(6); }

static void shot(const char *name) {                          /* development: DOW_DUMP_SHOTS=folder saves the current screen */
    const char *d = getenv("DOW_DUMP_SHOTS"); if (!d) return;
    uint8_t *b = (uint8_t *)malloc(64000); Zone z[MAXZONES]; int nz; tos_screen_idx(b, z, &nz); uint8_t *rgb = (uint8_t *)malloc(64000 * 4);
    for (int y = 0; y < 200; y++) { const uint8_t *pal = z[0].pal; for (int i = 0; i < nz; i++) if (z[i].line <= y) pal = z[i].pal;
        for (int x = 0; x < 320; x++) { int v = b[y * 320 + x]; int w = (pal[2 * v] << 8) | pal[2 * v + 1]; uint8_t *o = rgb + ((size_t)y * 320 + x) * 4; o[0] = (uint8_t)((((w >> 8) & 7) << 1 | ((w >> 11) & 1)) * 17); o[1] = (uint8_t)((((w >> 4) & 7) << 1 | ((w >> 7) & 1)) * 17); o[2] = (uint8_t)(((w & 7) << 1 | ((w >> 3) & 1)) * 17); o[3] = 255; } }
    char p[800]; snprintf(p, sizeof p, "%s/%s.png", d, name); png_write(p, rgb, 320, 200, 4); free(b); free(rgb);
}
enum { T_INTRO, T_TITLE, T_PLAYERS, T_MAP, T_SHOP };
static int box[40][5], nbox;
static void read_boxes(void) {
    nbox = 0; uint32_t a = 0xBF98;
    while (nbox < 40) { int xmin = (int16_t)mr16(a); if (xmin < 0) break; for (int k = 0; k < 5; k++) box[nbox][k] = (int16_t)mr16(a + (uint32_t)k * 2); nbox++; a += 10; }
}
static int nonblack(const uint8_t *idx, const Zone *z, int nz) {
    int cnt = 0;
    for (int y = 0; y < 200; y++) { const uint8_t *pal = z[0].pal; for (int i = 0; i < nz; i++) if (z[i].line <= y) pal = z[i].pal;
        for (int x = 0; x < 320; x++) { int v = idx[y * 320 + x]; if (pal[2 * v] || pal[2 * v + 1]) cnt++; } }
    return cnt;
}
static void wait_map(void) {
    uint8_t *b = (uint8_t *)malloc(64000); Zone z[MAXZONES]; int nz;
    for (int k = 0; k < 3000; k++) { step_s(1); tos_screen_idx(b, z, &nz); if (nonblack(b, z, nz) > 20000) break; }
    free(b);
}
static State *title_state;

/* title -> 1 player -> map -> mission box `mission` (box value) -> shop -> EXIT -> in mission. The title screen must be showing. */
static void start_mission(int mission) {
    char nm[40]; step(60); shot("a_title"); tap(J_FIRE, 6, 120); shot("b_players"); tapkey(0x02); wait_map(); step(200); shot("c_map");
    read_boxes(); int bi = -1; for (int i = 0; i < nbox; i++) if (box[i][4] == mission) { bi = i; break; }
    if (bi < 0) return;
    goto_xy((box[bi][0] + box[bi][1]) / 2, (box[bi][2] + box[bi][3]) / 2, 1, 1); step(30); shot("d_cursor");
    tap(J_FIRE, 8, 200); shot("e_shop");
    shop_cell(1, 0); step(20); tap(J_FIRE, 8, 30); shop_cell(2, 4); step(20); tap(J_FIRE, 8, 30);              /* a rifle and its ammunition (the game does not let you go without weapons) */
    shop_cell(4, 5); step(20); shot("f_exit"); tap(J_FIRE, 8, 60); shot("g_after1");
    { uint8_t *b0 = (uint8_t *)malloc(64000), *b1 = (uint8_t *)malloc(64000); Zone z[MAXZONES]; int nz; tos_screen_idx(b0, z, &nz);      /* fire again until the screen changes: the mission is loading */
      for (int k = 0; k < 8; k++) { tap(J_FIRE, 8, 120); tos_screen_idx(b1, z, &nz); int d = 0; for (int i = 0; i < 64000; i++) d += b0[i] != b1[i]; if (d > 5000) break; }
      free(b0); free(b1); }
    shot("h_after2"); step(300); shot("i_mission"); (void)nm;
}

/* ------------------------------------------------------------------------------------------------ fixed screens */
static void cols_zone(const Zone *z, int nz, int y, Rgb16 out) { const uint8_t *pal = z[0].pal; for (int i = 0; i < nz; i++) if (z[i].line <= y) pal = z[i].pal; pal_cols(pal, out); }
static void to_rgb(const uint8_t *idx, const Zone *z, int nz, int w, int h, int x0, uint8_t *rgba, int alpha) {       /* idx: 200x320, region h x w at column x0 */
    for (int y = 0; y < h; y++) { Rgb16 c; cols_zone(z, nz, y, c); for (int x = 0; x < w; x++) { uint8_t *d = rgba + ((size_t)y * w + x) * 4; const uint8_t *p = c[idx[y * 320 + x0 + x]]; d[0] = p[0]; d[1] = p[1]; d[2] = p[2]; d[3] = (uint8_t)alpha; } }
}
static int pal_sum(const Zone *z, int nz, int line) { Rgb16 c; cols_zone(z, nz, line, c); int s = 0; for (int i = 0; i < 16; i++) s += c[i][0] + c[i][1] + c[i][2]; return s; }
static uint8_t GREY[768];

static void dil(const uint8_t *m, uint8_t *d, int diag) {      /* dilation of a 200x320 mask */
    memcpy(d, m, 64000);
    for (int y = 0; y < 200; y++) for (int x = 0; x < 320; x++) if (m[y * 320 + x]) {
        for (int dy = -1; dy <= 1; dy++) for (int dx = -1; dx <= 1; dx++) { if (!diag && dy && dx) continue; int yy = y + dy, xx = x + dx; if (yy >= 0 && yy < 200 && xx >= 0 && xx < 320) d[yy * 320 + xx] = 1; }
    }
}
static void fill_holes(uint8_t *m, int maxarea) {              /* fills the holes (4-connected) that do not touch the border and have at most maxarea pixels */
    static uint8_t seen[64000]; static int q[64000]; memset(seen, 0, sizeof seen);
    for (int s = 0; s < 64000; s++) {
        if (m[s] || seen[s]) continue;
        int h = 0, t = 0, touch = 0; q[t++] = s; seen[s] = 1;
        while (h < t) {
            int c = q[h++], cy = c / 320, cx = c % 320; if (cy == 0 || cy == 199 || cx == 0 || cx == 319) touch = 1;
            const int nb[4][2] = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
            for (int k = 0; k < 4; k++) { int ny = cy + nb[k][0], nx = cx + nb[k][1]; if (ny >= 0 && ny < 200 && nx >= 0 && nx < 320 && !m[ny * 320 + nx] && !seen[ny * 320 + nx]) { seen[ny * 320 + nx] = 1; q[t++] = ny * 320 + nx; } }
        }
        if (!touch && t <= maxarea) for (int i = 0; i < t; i++) m[q[i]] = 1;
    }
}
static void fill_sil(const uint8_t *m, uint8_t *out) {          /* silhouette without holes: 3x3 closing + fill of the enclosed gaps */
    static uint8_t d[64000], e[64000], o[64000], reach[64000], n2[64000];
    dil(m, d, 0); memcpy(e, d, 64000);
    for (int y = 0; y < 200; y++) for (int x = 0; x < 320; x++) { int v = d[y * 320 + x]; if (y > 0) v &= d[(y - 1) * 320 + x]; if (y < 199) v &= d[(y + 1) * 320 + x]; if (x > 0) v &= d[y * 320 + x - 1]; if (x < 319) v &= d[y * 320 + x + 1]; e[y * 320 + x] = (uint8_t)v; }
    for (int i = 0; i < 64000; i++) o[i] = !e[i];
    memset(reach, 0, 64000); for (int x = 0; x < 320; x++) { reach[x] = o[x]; reach[199 * 320 + x] = o[199 * 320 + x]; } for (int y = 0; y < 200; y++) { reach[y * 320] = o[y * 320]; reach[y * 320 + 319] = o[y * 320 + 319]; }
    for (;;) { dil(reach, n2, 0); int ch = 0; for (int i = 0; i < 64000; i++) { n2[i] &= o[i]; if (n2[i] != reach[i]) ch = 1; } memcpy(reach, n2, 64000); if (!ch) break; }
    for (int i = 0; i < 64000; i++) out[i] = e[i] | (o[i] & !reach[i]) | m[i];
}
static void refine_mask(const uint8_t *m, const uint8_t *rgb /*200x320x4*/, uint8_t *out) {
    static uint8_t a[64000], b[64000], c[64000], d[64000];
    dil(m, a, 0); for (int i = 0; i < 64000; i++) a[i] = !a[i]; dil(a, b, 0); for (int i = 0; i < 64000; i++) b[i] = !b[i];
    fill_holes(b, 30); dil(b, c, 1);
    for (int i = 0; i < 64000; i++) { const uint8_t *p = rgb + (size_t)i * 4; int mx = p[0] > p[1] ? p[0] : p[1]; if (p[2] > mx) mx = p[2]; d[i] = b[i] | ((mx <= 51) && c[i]); }
    memcpy(out, d, 64000);
}
static void layer(const char *name, const uint8_t *rgba, const uint8_t *idx, int w, int h, int px, int py, int order, int ingame, const Zone *z, int nz, const char *desc) {
    char rel[100], p[800]; snprintf(rel, sizeof rel, "screens/%s.png", name); save_rgba(rel, rgba, w, h);
    snprintf(rel, sizeof rel, "screens/%s.idx.png", name); out_path(p, sizeof p, rel); make_dirs(p); png_write_indexed(p, idx, w, h, GREY);
    int line = py + h / 2; if (line > 199) line = 199;
    if (nsheets++) sb_add(&sheets, ",");
    sb_add(&sheets, "\"screens/%s\":{\"kind\":\"screen\",\"width\":%d,\"height\":%d,\"cell\":[%d,%d],\"cols\":1,\"margin\":0,\"pos\":[%d,%d],\"order\":%d,\"ingame\":%s,\"idx\":\"screens/%s.idx.png\",\"palette_sum\":%d,\"description\":", name, w, h, w, h, px, py, order, ingame ? "true" : "false", name, pal_sum(z, nz, line));
    sb_str(&sheets, desc); sb_add(&sheets, ",\"cells\":[{\"i\":0,\"hash\":\"%s\",\"frame\":[%d,%d],\"crop\":[0,0,%d,%d]}],\"missions\":[],\"memory\":{\"note\":\"fixed picture: 320x200 screen in 4 bitplanes (ST format) in the screen RAM\"}}", name, w, h, w, h);
    say("screens/%s", name);
}
static void full_idx(const uint8_t *idx, int x0, int w, uint8_t *out) { for (int y = 0; y < 200; y++) memcpy(out + (size_t)y * w, idx + y * 320 + x0, (size_t)w); }
static int zones_have(const Snap *s, const char *pal) { for (int i = 0; i < s->nz; i++) if (!memcmp(s->z[i].pal, pal, 32)) return 1; return 0; }

/* per-pixel mode (the most frequent index) of the snapshots in `sel`; conf = fraction of frames that agree */
static void mode_of(const uint8_t *const *frames, int n, uint8_t *idx, float *conf) {
    static uint16_t cnt[64000][16]; memset(cnt, 0, sizeof cnt);
    for (int f = 0; f < n; f++) for (int i = 0; i < 64000; i++) cnt[i][frames[f][i] & 15]++;
    for (int i = 0; i < 64000; i++) { int bi = 0; for (int k = 1; k < 16; k++) if (cnt[i][k] > cnt[i][bi]) bi = k; idx[i] = (uint8_t)bi; if (conf) conf[i] = (float)cnt[i][bi] / (float)(n ? n : 1); }
}
static int same_zones(const Snap *a, const Snap *b) { if (a->nz != b->nz) return 0; for (int i = 0; i < a->nz; i++) if (a->z[i].line != b->z[i].line || memcmp(a->z[i].pal, b->z[i].pal, 32)) return 0; return 1; }

static void dump_screens(const Snap *hudsn, int nhud) {
    static uint8_t rgba[64000 * 4], idx[64000]; uint8_t *ji = NULL, *si = NULL, *ti = NULL; const Snap *jz = NULL, *sz = NULL, *tz = NULL; const uint8_t *prev = NULL;
    for (int i = 0; i < nsnaps; i++) {
        const Snap *s = &snaps[i]; if (s->tag != T_INTRO) continue;
        int seen[16] = { 0 }, nu = 0; for (int k = 0; k < 64000; k++) if (!seen[s->idx[k] & 15]) { seen[s->idx[k] & 15] = 1; nu++; }
        if (nu < 12) { prev = s->idx; continue; }
        if (!ji) { ji = s->idx; jz = s; prev = s->idx; continue; }
        if (!si) { int d = 0; for (int k = 0; k < 64000; k++) d += s->idx[k] != ji[k]; if (d >= 2500 && d <= 60000) { si = s->idx; sz = s; } else { ji = s->idx; jz = s; } prev = s->idx; continue; }
        if (!ti) { int d = 0; for (int k = 0; k < 64000; k++) d += s->idx[k] != prev[k]; if (d > 8000) { ti = s->idx; tz = s; } }
        prev = s->idx;
    }
    if (!ji || !si || !ti) say("warning: the whole intro was not found");
    else {
        to_rgb(ji, jz->z, jz->nz, 320, 200, 0, rgba, 255); layer("intro_jungle", rgba, ji, 320, 200, 0, 0, 0, 0, jz->z, jz->nz, "intro background: the jungle (without the soldier)");
        static uint8_t m[64000], m2[64000];
        for (int k = 0; k < 64000; k++) m[k] = si[k] != ji[k]; fill_sil(m, m2); to_rgb(si, sz->z, sz->nz, 320, 200, 0, rgba, 255); refine_mask(m2, rgba, m);
        for (int k = 0; k < 64000; k++) rgba[k * 4 + 3] = m[k] ? 255 : 0;
        layer("intro_soldier", rgba, si, 320, 200, 0, 0, 1, 0, sz->z, sz->nz, "the intro soldier (resting pose) over the jungle; the mouth and muzzle flash change and are drawn as the game draws them");
        for (int k = 0; k < 64000; k++) m2[k] = ti[k] != si[k]; to_rgb(ti, tz->z, tz->nz, 320, 200, 0, rgba, 255); refine_mask(m2, rgba, m);
        for (int k = 0; k < 64000; k++) rgba[k * 4 + 3] = m[k] ? 255 : 0;
        layer("title_logo", rgba, ti, 320, 200, 0, 0, 2, 0, tz->z, tz->nz, "DOGS OF WAR logo, credits and Elite logo overlaid on the title");
    }
    for (int pass = 0; pass < 2; pass++) {                      /* map and shop */
        int tag = pass ? T_SHOP : T_MAP; const char *pal = pass ? SHOP_PAL : MAP_PAL; const uint8_t *fr[2000]; int n = 0; const Snap *first = NULL;
        for (int i = 0; i < nsnaps && n < 2000; i++) if (snaps[i].tag == tag && zones_have(&snaps[i], pal)) { if (!first) first = &snaps[i]; fr[n++] = snaps[i].idx; }
        if (!n) continue;
        int best = 0, bestn = 0; for (int i = 0; i < nsnaps; i++) if (snaps[i].tag == tag && zones_have(&snaps[i], pal)) { int c = 0; for (int j = 0; j < nsnaps; j++) if (snaps[j].tag == tag && zones_have(&snaps[j], pal) && same_zones(&snaps[i], &snaps[j])) c++; if (c > bestn) { bestn = c; best = i; } }
        const Snap *zs = &snaps[best]; mode_of(fr, n, idx, NULL);
        static uint8_t alpha[64000]; memset(alpha, 255, sizeof alpha);
        if (pass) {                                              /* text box (description, cost, credit): it changes with the cursor */
            for (int y = 144; y < 182; y++) for (int x = 4; x < 316; x++) { idx[y * 320 + x] = 0; alpha[y * 320 + x] = 0; }
            for (int y = 184; y < 197; y++) { for (int x = 78; x < 138; x++) { idx[y * 320 + x] = 0; alpha[y * 320 + x] = 0; } for (int x = 220; x < 304; x++) { idx[y * 320 + x] = 0; alpha[y * 320 + x] = 0; } }
        }
        to_rgb(idx, zs->z, zs->nz, 320, 200, 0, rgba, 255); for (int k = 0; k < 64000; k++) rgba[k * 4 + 3] = alpha[k];
        layer(pass ? "shop" : "map", rgba, idx, 320, 200, 0, 0, 0, 0, zs->z, zs->nz, pass ? "the weapon shop (without the text box or the cursor)" : "world map (without cursor or mission dots)");
    }
    if (nhud > 8) {                                              /* mission side panels */
        const uint8_t *fr[2000]; int n = 0; for (int i = 0; i < nhud && n < 2000; i++) fr[n++] = hudsn[i].idx;
        static float conf[64000]; mode_of(fr, n, idx, conf); static uint8_t dyn[64000], d2[64000], sub[200 * 64], sa[64000 * 4];
        for (int k = 0; k < 64000; k++) dyn[k] = conf[k] < 0.97f;
        for (int p = 0; p < 2; p++) {
            int x0 = p ? 256 : 0; static uint8_t al[200 * 64]; memset(al, 255, sizeof al); memset(d2, 0, 64000);
            for (int y = 0; y < 200; y++) for (int x = 0; x < 64; x++) if (dyn[y * 320 + x0 + x]) for (int dy = -2; dy <= 2; dy++) for (int dx = -2; dx <= 2; dx++) { int yy = ((y + dy) % 200 + 200) % 200, xx = ((x + dx) % 64 + 64) % 64; d2[yy * 320 + x0 + xx] = 1; }      /* the margin wraps around inside the panel */
            for (int y = 0; y < 200; y++) for (int x = 0; x < 64; x++) { sub[y * 64 + x] = idx[y * 320 + x0 + x]; if (d2[y * 320 + x0 + x]) al[y * 64 + x] = 0; }
            static const int rc[4][4] = { { 47, 61, 4, 60 }, { 78, 124, 8, 58 }, { 126, 140, 10, 56 }, { 176, 196, 0, 64 } };      /* score, weapon icon, ammo, lives / M.I.A. */
            for (int r = 0; r < 4; r++) for (int y = rc[r][0]; y < rc[r][1]; y++) for (int x = rc[r][2]; x < rc[r][3]; x++) al[y * 64 + x] = 0;
            for (int y = 0; y < 200; y++) { Rgb16 c; cols_zone(hudsn[0].z, hudsn[0].nz, y, c); for (int x = 0; x < 64; x++) { uint8_t *d = sa + ((size_t)y * 64 + x) * 4; const uint8_t *q = c[sub[y * 64 + x]]; d[0] = q[0]; d[1] = q[1]; d[2] = q[2]; d[3] = al[y * 64 + x]; } }
            layer(p ? "hud_right" : "hud_left", sa, sub, 64, 200, x0, 0, 0, 1, hudsn[0].z, hudsn[0].nz, "mission side panel (the changing counters are left transparent and drawn by the game)");
        }
    }
}

/* ------------------------------------------------------------------------------------------------ README of the dump */
static const char README_TXT[] =
"DOGS OF WAR - GRAPHICS DUMP (assets/original)\n==============================================\n\n"
"Every graphic of the game, taken straight from the data the game itself decompresses into RAM, as editable PNG sheets in canonical order\n"
"(the order of the game's own tables, not the order in which things happen to show up on screen). Generated from your own copy of the game.\n\n"
"LAYOUT\n"
"  sprites/   player.png (92 frames), enemy_1/2.png (39 frames; two variants, see index.json for which missions use which), shots.png / shots_b.png (29 projectile/\n"
"             grenade/spark frames; _b is the same shapes in the other colour bank), fx.png (12), boom.png (11 explosion frames), vehicle_a/b/c/d.png, bullets.png\n"
"  tiles/     ground_N.png (64x32 ground blocks) and objects_N.png (16x16 objects with transparency) for each of the 3 tile sets (the missions of each set are in index.json).\n"
"             The ground blocks are also the repeating textures of the horizontal-scroll sections (the left 32 columns).\n"
"  screens/   fixed pictures, as LAYERS: intro_jungle, intro_soldier, title_logo, map, shop, hud_left, hud_right (transparent areas are left to the game: scores, ammo,\n"
"             weapon icon, lives, text, cursors...). The *.idx.png files are the same pixels as palette indices; the game uses them to know which pixels are still \"the layer\".\n"
"  levels/    (reference only) every vertical-scroll level assembled with its tiles, to help you choose colours. Not used by the game.\n"
"  reference/ font_8x8.png: the game's character set (reference only: text is drawn by the game straight into the screen planes and is not replaced).\n"
"  index.json for every sheet: size, cell size, columns, margin, and for each cell the frame number (key), the content hash, the frame size and the memory address\n"
"             of its data (\"mem\": table + frame * stride) and the data format (\"memory\").\n\n"
"HOW TO REPLACE GRAPHICS\n"
"  1. Make a folder next to this one, e.g.  assets/vga/  and copy into it (keeping the same relative path and file name) only the sheets you want to change.\n"
"  2. Edit them with any pixel-art editor. RGBA: use as many colours as you like (a 256-colour VGA look) and soft alpha. Keep the sheet size (or an integer multiple\n"
"     of it: 2x, 3x... is accepted). Do not move cells: each cell is looked up by its position.\n"
"  3. Sprite cells have a transparent MARGIN of 4 px around the frame: you can paint there (soft shadows, glows, outlines); it is drawn around the sprite.\n"
"  4. Start the game. If assets/ contains a pack folder it is used automatically (or choose it in the options menu: \"Graphics pack\"). F9 reloads the pack while playing.\n"
"  5. Cells you did not change (or left empty) are drawn with the original graphics. Only the cells that differ from this dump are used.\n\n"
"HOW IT WORKS\n"
"  The game's data is 4-bitplane (16 colour) ST graphics, so a 256-colour picture cannot be written back into the game's memory. Instead the program recognises each\n"
"  graphic the game draws by its CONTENT (the same drawing of colour indices is the same graphic in any mission) and draws your PNG instead, at the same position,\n"
"  with the same animation and the 60 fps interpolation. Screens are layers: a layer replaces the screen pixels whose colour index is still the one of the original layer.\n"
"  Palette fades dim the replacement in proportion.\n\n"
"LIMITS\n"
"  - Text, score digits, weapon icons in the side panel and the lives icons are drawn by the game itself (they stay as they are).\n"
"  - Sprites cut by the screen edge in a rare case, and a few leftover pixels of routines that are not hooked (some fixed mines/turrets), are drawn as the game draws them.\n"
"  - The palette of every dumped sprite/tile is the one of the first mission that uses it; the game's palette changes per mission, so a replacement is drawn with your colours as they are.\n";

/* ------------------------------------------------------------------------------------------------ main entry */
static int mission_value(int m) { return m; }                  /* value of the mission in the map boxes ($BF98 records) */

int dump_original(const char *game_dir, const char *out_dir, void (*progress)(const char *)) {
    progress_cb = progress; snprintf(out_root, sizeof out_root, "%s", out_dir); scr = (uint8_t *)malloc(64000);
    for (int i = 0; i < 256; i++) GREY[i * 3] = GREY[i * 3 + 1] = GREY[i * 3 + 2] = i < 16 ? (uint8_t)(i * 16) : 0;
    State *boot = boot_state_make(game_dir); if (!boot) { say("the original game files cannot be read"); return 0; }
    tos_init(game_dir); tos_load_state(boot);
    /* 1. intro, up to the title (screens are sampled every 4 VBL) */
    say("intro"); sampling = 1; snap_tag = T_INTRO; step_s(1000); sampling = 0;
    title_state = state_new(); tos_save_state(title_state);
    /* 2. title -> map -> shop (screens), always from the title */
    say("map and shop"); sampling = 1; snap_tag = T_TITLE; step_s(60); tap_s(J_FIRE, 6, 120); snap_tag = T_PLAYERS; tapkey_s(0x02); snap_tag = T_MAP; wait_map(); step_s(200);
    read_boxes();
    if (nbox < 12) { say("the mission map was not reached"); return 0; }
    for (int k = 0; k < 2; k++) { int mis = k ? 5 : 2; int bi = -1; for (int i = 0; i < nbox; i++) if (box[i][4] == mis) { bi = i; break; } if (bi < 0) continue; goto_xy((box[bi][0] + box[bi][1]) / 2, (box[bi][2] + box[bi][3]) / 2, 1, 1); step_s(60); }
    snap_tag = T_SHOP; tap_s(J_FIRE, 8, 0); step_s(200);
    { static const int cr[6][2] = { { 1, 0 }, { 2, 4 }, { 3, 2 }, { 0, 1 }, { 2, 2 }, { 4, 3 } }; for (int i = 0; i < 6; i++) { goto_xy(32 + 64 * cr[i][0], 12 + 24 * cr[i][1], 4, 3); step_s(20); tap_s(J_FIRE, 8, 30); } }
    goto_xy(32 + 64 * 4, 12 + 24 * 5, 4, 3); step_s(20); sampling = 0;
    /* 3. the 12 missions: RAM and palette once the mission is loaded; levels; the side panels from mission 2 played with immunity */
    Snap *hud = NULL; int nhud = 0;
    int nmis = getenv("DOW_DUMP_MISSIONS") ? atoi(getenv("DOW_DUMP_MISSIONS")) : NMISSIONS;
    for (int m = 0; m < nmis; m++) {
        say("mission %d of %d", m + 1, NMISSIONS);
        tos_load_state(title_state); start_mission(mission_value(m));
        step(100);
        mram[m] = (uint8_t *)malloc(RAMSZ); memcpy(mram[m], ram, RAMSZ);
        { uint8_t p32[32]; int line = -1; for (int i = 0; i < tos.nzones; i++) if (tos.zones[i].line <= 100 && tos.zones[i].line > line) { line = tos.zones[i].line; memcpy(p32, tos.zones[i].pal, 32); } if (line < 0) memcpy(p32, tos.zones[0].pal, 32); pal_cols(p32, pals[m]);
          for (int sec = 0; sec < 4; sec++) { int rows, gm; uint8_t *rgb = level_dump_rgb(sec, p32, &rows, &gm); if (!rgb) continue; if (gm > used_gmax[m]) used_gmax[m] = gm;
              uint8_t *a = (uint8_t *)malloc((size_t)192 * rows * 4); for (int i = 0; i < 192 * rows; i++) { memcpy(a + (size_t)i * 4, rgb + (size_t)i * 3, 3); a[i * 4 + 3] = 255; }
              char rel[100]; snprintf(rel, sizeof rel, "levels/mission%02d_section%d", m + 1, sec + 1); { char fn[120]; snprintf(fn, sizeof fn, "%s.png", rel); save_rgba(fn, a, 192, rows); }
              if (nreflev++) sb_add(&reflevels, ","); sb_str(&reflevels, rel); free(a); free(rgb); } }
        if (getenv("DOW_DUMP_PLAY") && m != 1) {                /* development: plays the mission to see which code is still not translated (it would show up as an invalid-address error in error.log) */
            int np = atoi(getenv("DOW_DUMP_PLAY")); tos_set_patch(0, 1);
            for (int k = 0; k < np; k++) { int f = k / 2; if (k % 2 == 0) tos_joystick(1, J_UP | (((f / 30) % 6 == 1) ? J_LEFT : 0) | (((f / 30) % 6 == 3) ? J_RIGHT : 0) | ((f % 10) < 5 ? J_FIRE : 0)); step(1); }
            tos_set_patch(0, 0); tos_joystick(1, 0);
        }
        if (m == 1) {                                          /* side panels: the screen every 12 VBL for a long stretch of play with immunity */
            tos_set_patch(0, 1); hud = (Snap *)calloc(600, sizeof(Snap));
            for (int k = 0; k < 6400; k++) {
                int f = k / 2; if (k % 2 == 0) tos_joystick(1, J_UP | (((f / 30) % 6 == 1) ? J_LEFT : 0) | (((f / 30) % 6 == 3) ? J_RIGHT : 0) | ((f % 10) < 5 ? J_FIRE : 0));
                step(1); if (k % 12 == 0 && nhud < 600) { Snap *s = &hud[nhud++]; s->idx = (uint8_t *)malloc(64000); tos_screen_idx(s->idx, s->z, &s->nz); }
            }
            tos_set_patch(0, 0); tos_joystick(1, 0);
        }
    }
    if (nmis < NMISSIONS) { say("partial run (development)"); return 1; }
    /* 4. write everything */
    say("writing the sheets");
    dump_sprites(); dump_bullets(); dump_tiles(); dump_font(); dump_screens(hud, nhud);
    { char p[800]; out_path(p, sizeof p, "README.txt"); make_dirs(p); write_file(p, README_TXT, strlen(README_TXT)); }
    SB idx = { 0 }; sb_add(&idx, "{\"format\":2,\"margin\":%d,\"sheets\":{%s},\"reference_levels\":[%s],\"reference\":{\"reference/font_8x8\":{\"cell\":[8,8],\"cols\":16,\"first_char\":32,\"memory\":{\"table\":\"$8700\",\"stride\":\"8\",\"format\":\"8 bytes per character, 1 bit per pixel, ASCII from $20\"},\"note\":\"reference only: the game draws text straight into the screen planes; it is not replaced\"}}}\n", MARGIN, sheets.s ? sheets.s : "", reflevels.s ? reflevels.s : "");
    { char p[800], t[800]; out_path(p, sizeof p, "index.json"); snprintf(t, sizeof t, "%s.tmp", p); write_file(t, idx.s, idx.n);
#ifdef _WIN32
      MoveFileExA(t, p, MOVEFILE_REPLACE_EXISTING);
#endif
    }
    say("done: %s", out_dir); return 1;
}

/* ------------------------------------------------------------------------------------------------ in a thread with a big stack (like boot.c) */
typedef struct { char game[700], out[700]; } DumpJob;
static volatile LONG dj_done = 0, dj_ok = 0; static char dj_status[200]; static CRITICAL_SECTION dj_lk; static int dj_lk_init = 0;
static void dj_say(const char *s) { EnterCriticalSection(&dj_lk); snprintf(dj_status, sizeof dj_status, "%s", s); LeaveCriticalSection(&dj_lk); }
void core_forget_host(void); void core_reset(void);
static DWORD WINAPI dump_thread(LPVOID p) {
    DumpJob *j = (DumpJob *)p; int ok = dump_original(j->game, j->out, dj_say); free(j);
    core_reset(); core_forget_host();                                      /* the core's fibers belong to this thread: the emulation thread creates its own */
    dj_ok = ok; dj_done = 1; return 0;
}
int dump_begin(const char *game_dir, const char *out_dir) {
    if (!dj_lk_init) { InitializeCriticalSection(&dj_lk); dj_lk_init = 1; }
    DumpJob *j = (DumpJob *)calloc(1, sizeof *j); snprintf(j->game, sizeof j->game, "%s", game_dir); snprintf(j->out, sizeof j->out, "%s", out_dir);
    dj_done = 0; dj_ok = 0; HANDLE h = CreateThread(NULL, 64u << 20, dump_thread, j, 0, NULL); if (!h) { free(j); return 0; } CloseHandle(h); return 1;
}
int dump_finished(int *ok) { if (!dj_done) return 0; if (ok) *ok = (int)dj_ok; return 1; }
void dump_status(char *out, size_t n) { EnterCriticalSection(&dj_lk); snprintf(out, n, "%s", dj_status); LeaveCriticalSection(&dj_lk); }
