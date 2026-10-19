/*
 * gfxpack.c - Replacement graphics packs.
 *
 * The original dump (assets/original/) has, per graphic type, a PNG SHEET with all its frames in a grid and an index.json with the hash of the game's drawing
 * that each cell represents. A pack is a sibling folder (assets/<name>/) with sheets of the same name and size: every changed cell replaces that
 * graphic on screen (in full color, with soft alpha); what is not touched is drawn with the original. Identity by CONTENT: 6-byte blake2b hash of the drawing of color indices.
 */
#include "gfxpack.h"
#include "render.h"
#include "level.h"
#ifdef _WIN32
#include <windows.h>
#endif

/* ------------------------------------------------------------------------------------------------ blake2b (6-byte digest, no key) */
typedef struct { uint64_t h[8], t[2]; uint8_t buf[128]; size_t n; } B2;
static const uint64_t B2IV[8] = { 0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL, 0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL, 0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL, 0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL };
static const uint8_t B2S[12][16] = {
    { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }, { 11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 },
    { 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 }, { 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 }, { 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 },
    { 12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 }, { 13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 }, { 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 },
    { 10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13, 0 }, { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, { 14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 } };
#define ROR64(x, n) (((x) >> (n)) | ((x) << (64 - (n))))
static void b2_compress(B2 *s, const uint8_t *blk, int last) {
    uint64_t m[16], v[16]; for (int i = 0; i < 16; i++) { m[i] = 0; for (int k = 7; k >= 0; k--) m[i] = (m[i] << 8) | blk[i * 8 + k]; }
    for (int i = 0; i < 8; i++) { v[i] = s->h[i]; v[i + 8] = B2IV[i]; }
    v[12] ^= s->t[0]; v[13] ^= s->t[1]; if (last) v[14] = ~v[14];
#define G(a, b, c, d, x, y) do { v[a] = v[a] + v[b] + (x); v[d] = ROR64(v[d] ^ v[a], 32); v[c] = v[c] + v[d]; v[b] = ROR64(v[b] ^ v[c], 24); v[a] = v[a] + v[b] + (y); v[d] = ROR64(v[d] ^ v[a], 16); v[c] = v[c] + v[d]; v[b] = ROR64(v[b] ^ v[c], 63); } while (0)
    for (int r = 0; r < 12; r++) {
        const uint8_t *z = B2S[r];
        G(0, 4, 8, 12, m[z[0]], m[z[1]]); G(1, 5, 9, 13, m[z[2]], m[z[3]]); G(2, 6, 10, 14, m[z[4]], m[z[5]]); G(3, 7, 11, 15, m[z[6]], m[z[7]]);
        G(0, 5, 10, 15, m[z[8]], m[z[9]]); G(1, 6, 11, 12, m[z[10]], m[z[11]]); G(2, 7, 8, 13, m[z[12]], m[z[13]]); G(3, 4, 9, 14, m[z[14]], m[z[15]]);
    }
#undef G
    for (int i = 0; i < 8; i++) s->h[i] ^= v[i] ^ v[i + 8];
}
static void b2_init(B2 *s, int outlen) { memset(s, 0, sizeof *s); for (int i = 0; i < 8; i++) s->h[i] = B2IV[i]; s->h[0] ^= 0x01010000ULL ^ (uint64_t)outlen; }
static void b2_update(B2 *s, const void *p, size_t n) {
    const uint8_t *d = (const uint8_t *)p;
    while (n) {
        if (s->n == 128) { s->t[0] += 128; if (s->t[0] < 128) s->t[1]++; b2_compress(s, s->buf, 0); s->n = 0; }
        size_t k = 128 - s->n; if (k > n) k = n; memcpy(s->buf + s->n, d, k); s->n += k; d += k; n -= k;
    }
}
static void b2_hex(B2 *s, int outlen, char *hex) {
    s->t[0] += s->n; if (s->t[0] < s->n) s->t[1]++; memset(s->buf + s->n, 0, 128 - s->n); b2_compress(s, s->buf, 1);
    for (int i = 0; i < outlen; i++) sprintf(hex + 2 * i, "%02x", (unsigned)((s->h[i / 8] >> (8 * (i % 8))) & 255));
    hex[2 * outlen] = 0;
}

/* hash of the drawing of a sprite (int16 shape + indices + mask) and of a tile (int32 shape + indices) */
static void sprite_hash(const Sprite *sp, char *hex) {
    B2 s; b2_init(&s, 6); int16_t shp[2] = { (int16_t)sp->h, (int16_t)sp->w }; b2_update(&s, shp, 4);
    b2_update(&s, sp->img, (size_t)sp->w * sp->h); b2_update(&s, sp->mask, (size_t)sp->w * sp->h); b2_hex(&s, 6, hex);
}
static void array_hash(const uint8_t *a, int h, int w, char *hex) {
    B2 s; b2_init(&s, 6); int32_t shp[2] = { h, w }; b2_update(&s, shp, 8); b2_update(&s, a, (size_t)h * w); b2_hex(&s, 6, hex);
}

/* ------------------------------------------------------------------------------------------------ JSON minimo (DOM) */
typedef struct J { int t; double n; char *s; int cnt; char **keys; struct J **v; } J;     /* t: 0 null 1 bool 2 num 3 str 4 arr 5 obj */
static const char *jskip(const char *p) { while (*p == ' ' || *p == 9 || *p == 10 || *p == 13) p++; return p; }
static J *jparse(const char **pp);
static char *jstr(const char **pp) {
    const char *p = *pp + 1; size_t cap = 32, n = 0; char *o = (char *)malloc(cap);
    while (*p && *p != '"') {
        int c = (unsigned char)*p++;
        if (c == 0x5C) { c = (unsigned char)*p++; if (c == 'n') c = 10; else if (c == 't') c = 9; else if (c == 'r') c = 13; else if (c == 'u') { unsigned u = 0; sscanf(p, "%4x", &u); p += 4; c = u < 128 ? (int)u : '?'; } }
        if (n + 2 > cap) { cap *= 2; o = (char *)realloc(o, cap); } o[n++] = (char)c;
    }
    o[n] = 0; *pp = *p ? p + 1 : p; return o;
}
static J *jnew(int t) { J *j = (J *)calloc(1, sizeof(J)); j->t = t; return j; }
static J *jparse(const char **pp) {
    const char *p = jskip(*pp); J *j = NULL;
    if (*p == '{') {
        j = jnew(5); p = jskip(p + 1); int cap = 8; j->keys = (char **)malloc(sizeof(char *) * cap); j->v = (J **)malloc(sizeof(J *) * cap);
        while (*p && *p != '}') {
            if (*p == ',') { p = jskip(p + 1); continue; }
            if (*p != '"') break; char *k = jstr(&p); p = jskip(p); if (*p == ':') p++; J *v = jparse(&p); p = jskip(p);
            if (j->cnt == cap) { cap *= 2; j->keys = (char **)realloc(j->keys, sizeof(char *) * cap); j->v = (J **)realloc(j->v, sizeof(J *) * cap); } j->keys[j->cnt] = k; j->v[j->cnt++] = v;
        }
        if (*p == '}') p++;
    } else if (*p == '[') {
        j = jnew(4); p = jskip(p + 1); int cap = 8; j->v = (J **)malloc(sizeof(J *) * cap);
        while (*p && *p != ']') {
            if (*p == ',') { p = jskip(p + 1); continue; }
            J *v = jparse(&p); p = jskip(p); if (j->cnt == cap) { cap *= 2; j->v = (J **)realloc(j->v, sizeof(J *) * cap); } j->v[j->cnt++] = v;
        }
        if (*p == ']') p++;
    } else if (*p == '"') { j = jnew(3); j->s = jstr(&p); }
    else if (!strncmp(p, "true", 4)) { j = jnew(1); j->n = 1; p += 4; }
    else if (!strncmp(p, "false", 5)) { j = jnew(1); p += 5; }
    else if (!strncmp(p, "null", 4)) { j = jnew(0); p += 4; }
    else { char *e; j = jnew(2); j->n = strtod(p, &e); p = e == p ? p + 1 : e; }
    *pp = p; return j;
}
static void jfree(J *j) { if (!j) return; for (int i = 0; i < j->cnt; i++) { jfree(j->v[i]); if (j->keys) free(j->keys[i]); } free(j->keys); free(j->v); free(j->s); free(j); }
static J *jget(const J *o, const char *k) { if (!o || o->t != 5) return NULL; for (int i = 0; i < o->cnt; i++) if (!strcmp(o->keys[i], k)) return o->v[i]; return NULL; }
static double jnum(const J *o, const char *k, double d) { J *v = jget(o, k); return v && (v->t == 2 || v->t == 1) ? v->n : d; }
static const char *jstrv(const J *o, const char *k, const char *d) { J *v = jget(o, k); return v && v->t == 3 ? v->s : d; }

/* ------------------------------------------------------------------------------------------------ rescaling (RGBA, premultiplied) */
typedef struct { int xmin, n; float *w; } Wts;
static double f_box(double x) { return (x > -0.5 && x <= 0.5) ? 1.0 : 0.0; }
static double f_bil(double x) { x = fabs(x); return x < 1.0 ? 1.0 - x : 0.0; }
static Wts *make_wts(int in, int out, int kind) {                     /* kind: 0 box, 1 bilinear (like PIL) */
    Wts *r = (Wts *)calloc((size_t)out, sizeof(Wts)); double scale = (double)in / out, fs = scale < 1.0 ? 1.0 : scale, support = (kind ? 1.0 : 0.5) * fs;
    for (int x = 0; x < out; x++) {
        double c = (x + 0.5) * scale; int xmin = (int)(c - support + 0.5), xmax = (int)(c + support + 0.5); if (xmin < 0) xmin = 0; if (xmax > in) xmax = in; int n = xmax - xmin; if (n < 1) { n = 1; if (xmin >= in) xmin = in - 1; }
        r[x].xmin = xmin; r[x].n = n; r[x].w = (float *)malloc(sizeof(float) * (size_t)n); double tot = 0;
        for (int i = 0; i < n; i++) { double w = (kind ? f_bil : f_box)((i + xmin - c + 0.5) / fs); r[x].w[i] = (float)w; tot += w; }
        if (tot != 0) for (int i = 0; i < n; i++) r[x].w[i] = (float)(r[x].w[i] / tot); else r[x].w[0] = 1;
    }
    return r;
}
static void free_wts(Wts *w, int n) { for (int i = 0; i < n; i++) free(w[i].w); free(w); }
/* a: RGBA (ah x aw) with aw = k*w, ah = k*h -> RGBA (h*S x w*S) */
static uint8_t *to_scale(const uint8_t *a, int aw, int ah, int h, int w, int S) {
    int k = ah / h; int ow = w * S, oh = h * S; uint8_t *out = (uint8_t *)malloc((size_t)ow * oh * 4);
    if (k == S) { memcpy(out, a, (size_t)ow * oh * 4); return out; }
    if (S % k == 0) {                                                    /* replication */
        int r = S / k; for (int y = 0; y < oh; y++) for (int x = 0; x < ow; x++) memcpy(out + ((size_t)y * ow + x) * 4, a + ((size_t)(y / r) * aw + x / r) * 4, 4); return out;
    }
    int kind = k > S ? 0 : 1; Wts *wx = make_wts(aw, ow, kind), *wy = make_wts(ah, oh, kind);
    float *pm = (float *)malloc((size_t)aw * ah * 4 * sizeof(float));
    for (int i = 0; i < aw * ah; i++) { float al = a[i * 4 + 3] / 255.0f; for (int c = 0; c < 3; c++) pm[i * 4 + c] = a[i * 4 + c] * al; pm[i * 4 + 3] = a[i * 4 + 3]; }
    float *tmp = (float *)malloc((size_t)ow * ah * 4 * sizeof(float));      /* horizontal */
    for (int y = 0; y < ah; y++) for (int x = 0; x < ow; x++) { float s[4] = { 0, 0, 0, 0 }; for (int i = 0; i < wx[x].n; i++) { const float *p = pm + ((size_t)y * aw + wx[x].xmin + i) * 4; for (int c = 0; c < 4; c++) s[c] += p[c] * wx[x].w[i]; } memcpy(tmp + ((size_t)y * ow + x) * 4, s, 16); }
    for (int y = 0; y < oh; y++) for (int x = 0; x < ow; x++) {
        float s[4] = { 0, 0, 0, 0 }; for (int i = 0; i < wy[y].n; i++) { const float *p = tmp + ((size_t)(wy[y].xmin + i) * ow + x) * 4; for (int c = 0; c < 4; c++) s[c] += p[c] * wy[y].w[i]; }
        uint8_t *o = out + ((size_t)y * ow + x) * 4; float al = s[3];
        for (int c = 0; c < 3; c++) { float v = al > 0.5f ? s[c] * 255.0f / (al > 1.0f ? al : 1.0f) : 0.0f; v += 0.5f; o[c] = (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v); }
        { float v = al + 0.5f; o[3] = (uint8_t)(v < 0 ? 0 : v > 255 ? 255 : v); }
    }
    free(pm); free(tmp); free_wts(wx, ow); free_wts(wy, oh); return out;
}

/* ------------------------------------------------------------------------------------------------ state */
typedef struct Rep {
    char kind; char hash[16];
    uint8_t *cell; int cw, ch, k, cropx, cropy, margin;          /* sheet cell (cw x ch game px, k times larger) */
    uint8_t *scaled; int sw, sh, dx, dy, sS;                     /* a escala S */
    uint8_t *tex; int tS;                                        /* texture of the horizontal stretch (RGB 32S x 32S) */
} Rep;
typedef struct Layer { int x, y, w, h, order, ingame, area, psum; uint8_t *idx, *mask, *rgba; char name[96]; } Layer;      /* rgba == NULL: occlusion-only layer (its sheet was not changed) */

static char root_[600]; static char pack_[96] = "original"; static int S_ = 3;
static J *index_ = NULL; static Rep *reps = NULL; static int nreps = 0, capreps = 0; static int *tab = NULL; static int tabn = 0;
static Layer *layers = NULL; static int nlayers = 0; static int has_tiles = 0;
static char packs[64][96]; static int npacks = 0;

/* The threads that build levels (level.c) read the replaced tiles while the main thread may reread the pack: a lock protects the tables and what is freed
 * while any builder is still active is parked in a graveyard (freed when the last one finishes). */
#ifdef _WIN32
static CRITICAL_SECTION gk; static int gk_init = 0;
static void GLOCK(void) { if (!gk_init) { InitializeCriticalSection(&gk); gk_init = 1; } EnterCriticalSection(&gk); }
static void GUNLOCK(void) { LeaveCriticalSection(&gk); }
#else
static void GLOCK(void) {} static void GUNLOCK(void) {}
#endif
static int builders = 0; static void **grave = NULL; static int ngrave = 0;
static void defer_free(void *p) { if (!p) return; if (!builders) { free(p); return; } grave = (void **)realloc(grave, sizeof(void *) * (size_t)(ngrave + 1)); grave[ngrave++] = p; }
void gfxpack_builders(int delta) { GLOCK(); builders += delta; if (builders <= 0) { builders = 0; for (int i = 0; i < ngrave; i++) free(grave[i]); free(grave); grave = NULL; ngrave = 0; } GUNLOCK(); }

static unsigned hstr(const char *s) { unsigned h = 2166136261u; while (*s) h = (h ^ (unsigned char)*s++) * 16777619u; return h; }
static Rep *find_rep(char kind, const char *hash) {
    if (!tab) return NULL; unsigned h = hstr(hash) ^ (unsigned)kind * 7919u;
    for (int i = 0; i < tabn; i++) { int e = tab[(h + (unsigned)i) % (unsigned)tabn]; if (e < 0) return NULL; if (reps[e].kind == kind && !strcmp(reps[e].hash, hash)) return &reps[e]; }
    return NULL;
}
static void tab_put(int e) { unsigned h = hstr(reps[e].hash) ^ (unsigned)reps[e].kind * 7919u; for (int i = 0; i < tabn; i++) { int *s = &tab[(h + (unsigned)i) % (unsigned)tabn]; if (*s < 0) { *s = e; return; } } }
static void add_rep(const Rep *r) {
    if (nreps == capreps) { capreps = capreps ? capreps * 2 : 256; reps = (Rep *)realloc(reps, sizeof(Rep) * (size_t)capreps); }
    reps[nreps++] = *r;
    if (!tab || nreps * 2 > tabn) { free(tab); tabn = nreps * 4 + 64; tab = (int *)malloc(sizeof(int) * (size_t)tabn); for (int i = 0; i < tabn; i++) tab[i] = -1; for (int e = 0; e < nreps; e++) tab_put(e); }
    else tab_put(nreps - 1);
}
static void clear_reps(void) {
    for (int i = 0; i < nreps; i++) { defer_free(reps[i].cell); defer_free(reps[i].scaled); defer_free(reps[i].tex); }
    nreps = 0; free(tab); tab = NULL; tabn = 0;
    for (int i = 0; i < nlayers; i++) { free(layers[i].idx); free(layers[i].mask); free(layers[i].rgba); } free(layers); layers = NULL; nlayers = 0; has_tiles = 0;
}

static int same_cell(const uint8_t *a, int aw, const uint8_t *b, int bw, int w, int h) {
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) {
        const uint8_t *p = a + ((size_t)y * aw + x) * 4, *q = b + ((size_t)y * bw + x) * 4;
        if (p[3] != q[3]) return 0; if (p[3] && memcmp(p, q, 4)) return 0;
    }
    return 1;
}
extern void pal_to_rgb(const uint8_t *pal, uint8_t rgb[16][3]);
static int pal_sum(const uint8_t *pal) {
    uint8_t rgb[16][3]; pal_to_rgb(pal, rgb); int t = 0; for (int i = 0; i < 16; i++) for (int k = 0; k < 3; k++) t += rgb[i][k];
    return t;
}

static void list_packs(void) {
    npacks = 0; if (!index_ || (int)jnum(index_, "format", 0) != 2) return; J *sh = jget(index_, "sheets"); if (!sh) return;
#ifdef _WIN32
    char pat[700]; snprintf(pat, sizeof pat, "%s/*", root_); WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat, &fd); if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.' || !strncmp(fd.cFileName, "original", 8)) continue;
        int any = 0; for (int i = 0; i < sh->cnt && !any; i++) { char p[900]; snprintf(p, sizeof p, "%s/%s/%s.png", root_, fd.cFileName, sh->keys[i]); any = file_exists(p); }
        if (any && npacks < 64) snprintf(packs[npacks++], 96, "%s", fd.cFileName);
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    for (int i = 1; i < npacks; i++) { char t[96]; memcpy(t, packs[i], 96); int j = i - 1; while (j >= 0 && strcmp(packs[j], t) > 0) { memcpy(packs[j + 1], packs[j], 96); j--; } memcpy(packs[j + 1], t, 96); }
#endif
}

static void add_layer(const J *sd, const char *name, const uint8_t *cell, int cw, int ch, int k, const uint8_t *orig, int ow) {
    char p[900]; snprintf(p, sizeof p, "%s/original/%s", root_, jstrv(sd, "idx", "")); int w, h; uint8_t *idx = png_read_indexed(p, &w, &h);
    if (!idx) { host_log("Layer without indices (%s)", name); return; }
    (void)ow; Layer L; memset(&L, 0, sizeof L); J *pos = jget(sd, "pos"); L.x = pos && pos->cnt > 1 ? (int)pos->v[0]->n : 0; L.y = pos && pos->cnt > 1 ? (int)pos->v[1]->n : 0; L.w = w; L.h = h; L.order = (int)jnum(sd, "order", 0); L.ingame = (int)jnum(sd, "ingame", 0);
    snprintf(L.name, sizeof L.name, "%s", name); L.psum = (int)jnum(sd, "palette_sum", 0); L.idx = idx; L.mask = (uint8_t *)malloc((size_t)w * h);
    for (int i = 0; i < w * h; i++) { L.mask[i] = orig[i * 4 + 3] > 0; L.area += L.mask[i]; }
    L.rgba = to_scale(cell, cw * k, ch * k, h, w, S_);
    layers = (Layer *)realloc(layers, sizeof(Layer) * (size_t)(nlayers + 1)); layers[nlayers++] = L;
}

/* Layer that is not replaced (the game draws it): it only serves so the layers below do not repaint the pixels it covers (e.g. the intro soldier over the new jungle: the soldier's pixels
   that have the same color index as the original background at that spot used to be repainted with the new background). */
static void add_occluder(const J *sd, const char *name) {
    char p[900]; snprintf(p, sizeof p, "%s/original/%s", root_, jstrv(sd, "idx", "")); int w, h; uint8_t *idx = png_read_indexed(p, &w, &h); if (!idx) return;
    snprintf(p, sizeof p, "%s/original/%s.png", root_, name); int ow, oh; uint8_t *orig = png_read_rgba(p, &ow, &oh); if (!orig || ow != w || oh != h) { free(idx); free(orig); return; }
    Layer L; memset(&L, 0, sizeof L); J *pos = jget(sd, "pos"); L.x = pos && pos->cnt > 1 ? (int)pos->v[0]->n : 0; L.y = pos && pos->cnt > 1 ? (int)pos->v[1]->n : 0; L.w = w; L.h = h; L.order = (int)jnum(sd, "order", 0);
    snprintf(L.name, sizeof L.name, "%s", name); L.idx = idx; L.mask = (uint8_t *)malloc((size_t)w * h);
    for (int i = 0; i < w * h; i++) { L.mask[i] = orig[i * 4 + 3] > 0; L.area += L.mask[i]; }
    free(orig); layers = (Layer *)realloc(layers, sizeof(Layer) * (size_t)(nlayers + 1)); layers[nlayers++] = L;
}

static void rescan_locked(void);
static void rescan(void) { GLOCK(); rescan_locked(); GUNLOCK(); }
static void rescan_locked(void) {
    clear_reps(); pack_gen++;
    if (!index_ || !strcmp(pack_, "original") || !pack_[0]) return;
    J *sh = jget(index_, "sheets"); if (!sh) return;
    char base[700]; snprintf(base, sizeof base, "%s/%s", root_, pack_);
    for (int si = 0; si < sh->cnt; si++) {
        const char *name = sh->keys[si]; J *sd = sh->v[si]; char path[900]; snprintf(path, sizeof path, "%s/%s.png", base, name);
        if (!file_exists(path)) continue;
        int aw, ah; uint8_t *a = png_read_rgba(path, &aw, &ah); if (!a) { host_log("Could not read the sheet %s", path); continue; }
        int W = (int)jnum(sd, "width", 0), H = (int)jnum(sd, "height", 0);
        if (W <= 0 || aw % W || ah / (aw / W) != H) { host_log("Sheet ignored (%s: %dx%d, it must be %dx%d or an integer multiple)", name, aw, ah, W, H); free(a); continue; }
        int k = aw / W; uint8_t *orig = NULL; int ow = 0, oh = 0; char op[900]; snprintf(op, sizeof op, "%s/original/%s.png", root_, name); if (file_exists(op)) orig = png_read_rgba(op, &ow, &oh);
        J *cl = jget(sd, "cell"); int cw = cl && cl->cnt > 1 ? (int)cl->v[0]->n : 0, ch = cl && cl->cnt > 1 ? (int)cl->v[1]->n : 0, cols = (int)jnum(sd, "cols", 1), margin = (int)jnum(sd, "margin", 0);
        const char *ks = jstrv(sd, "kind", ""); char kind = !strcmp(ks, "sprite") ? 's' : !strcmp(ks, "ground") ? 'g' : !strcmp(ks, "object") ? 'o' : 'c'; int n = 0;
        J *cells = jget(sd, "cells");
        for (int ci = 0; cells && ci < cells->cnt; ci++) {
            J *c = cells->v[ci]; const char *hs = jstrv(c, "hash", ""); if (!hs[0]) continue;
            int i = (int)jnum(c, "i", 0), x = (i % cols) * cw, y = (i / cols) * ch;
            uint8_t *cell = (uint8_t *)malloc((size_t)cw * k * ch * k * 4); int any = 0;
            for (int r = 0; r < ch * k; r++) { memcpy(cell + (size_t)r * cw * k * 4, a + ((size_t)(y * k + r) * aw + x * k) * 4, (size_t)cw * k * 4); }
            for (int q = 0; q < cw * k * ch * k && !any; q++) any = cell[q * 4 + 3] != 0;
            if (!any) { free(cell); continue; }
            if (orig && k == 1 && x + cw <= ow && y + ch <= oh && same_cell(cell, cw, orig + ((size_t)y * ow + x) * 4, ow, cw, ch)) { free(cell); continue; }
            if (find_rep(kind, hs)) { free(cell); continue; }             /* the first sheet that changes it wins (the table is rebuilt at the end: linear search here) */
            Rep r; memset(&r, 0, sizeof r); r.kind = kind; snprintf(r.hash, sizeof r.hash, "%s", hs); r.cell = cell; r.cw = cw; r.ch = ch; r.k = k; r.margin = margin;
            J *cr = jget(c, "crop"); if (cr && cr->cnt > 1) { r.cropx = (int)cr->v[0]->n; r.cropy = (int)cr->v[1]->n; }
            add_rep(&r); n++;
            if (kind == 'c' && orig) add_layer(sd, name, cell, cw, ch, k, orig, ow);
        }
        if (n && (kind == 'g' || kind == 'o')) has_tiles = 1;
        free(a); free(orig);
    }
    if (nlayers) for (int si = 0; si < sh->cnt; si++) {                       /* screen layers the pack does not change: only as occluders of the ones it does change */
        const char *name = sh->keys[si]; J *sd = sh->v[si]; if (strcmp(jstrv(sd, "kind", ""), "screen")) continue;
        int have = 0; for (int i = 0; i < nlayers; i++) if (!strcmp(layers[i].name, name)) have = 1;
        if (!have) add_occluder(sd, name);
    }
    for (int i = 1; i < nlayers; i++) { Layer t = layers[i]; int j = i - 1; while (j >= 0 && layers[j].order > t.order) { layers[j + 1] = layers[j]; j--; } layers[j + 1] = t; }
}

/* ------------------------------------------------------------------------------------------------ API */
static int cb_sprite(Sprite *sp, int S, PackSpr *out) {
    if (!nreps) return 0;
    if (!sp->hashed) { sprite_hash(sp, sp->phash); sp->hashed = 1; }
    Rep *r = find_rep('s', sp->phash); if (!r) return 0;
    if (r->sS != S || !r->scaled) { free(r->scaled); r->scaled = to_scale(r->cell, r->cw * r->k, r->ch * r->k, r->ch, r->cw, S); r->sS = S; r->sw = r->cw; r->sh = r->ch; r->dx = -(r->margin + r->cropx); r->dy = -(r->margin + r->cropy); }
    out->rgba = r->scaled; out->w = r->sw; out->h = r->sh; out->dx = r->dx; out->dy = r->dy; return 1;
}
static int cb_texture(const uint8_t *tile32, int S, const uint8_t **rgb) {
    if (!nreps) return 0;
    uint8_t full[32 * 64]; for (int y = 0; y < 32; y++) { memcpy(full + y * 64, tile32 + y * 32, 32); memcpy(full + y * 64 + 32, tile32 + y * 32, 32); }
    char hx[16]; array_hash(full, 32, 64, hx); Rep *r = find_rep('g', hx); if (!r) return 0;
    if (!r->tex || r->tS != S) {
        free(r->tex); uint8_t *a = to_scale(r->cell, r->cw * r->k, r->ch * r->k, 32, 64, S); int per = 32 * S; r->tex = (uint8_t *)malloc((size_t)per * per * 3);
        for (int y = 0; y < per; y++) for (int x = 0; x < per; x++) memcpy(r->tex + ((size_t)y * per + x) * 3, a + ((size_t)y * 64 * S + x) * 4, 3);
        free(a); r->tS = S;
    }
    *rgb = r->tex; return 1;
}
static void cb_hud(uint8_t *base, const uint8_t *idx, const Zone *zones, int nz, int S) { gfxpack_apply_layers(base, idx, zones, nz, S, 1); }

/* replaced tiles of the level (level.c): RGBA at scale S or NULL; the pointer is valid while the pack is not reread or the builder is active */
int gfxpack_has_tiles(void) { return has_tiles; }
static const uint8_t *tile_rgba(char kind, const uint8_t *tile, int h, int w, int S) {
    char hx[16]; array_hash(tile, h, w, hx); const uint8_t *res = NULL; GLOCK();
    Rep *r = find_rep(kind, hx);
    if (r) {
        if (r->sS != S || !r->scaled) { defer_free(r->scaled); r->scaled = to_scale(r->cell, r->cw * r->k, r->ch * r->k, h, w, S); r->sS = S; }
        res = r->scaled;
    }
    GUNLOCK(); return res;
}
const uint8_t *gfxpack_ground_rgba(const uint8_t *tile32x64, int S) { return nreps ? tile_rgba('g', tile32x64, 32, 64, S) : NULL; }
const uint8_t *gfxpack_object_rgba(const uint8_t *tile16x16, int S) { return nreps ? tile_rgba('o', tile16x16, 16, 16, S) : NULL; }

/* What the game draws ON TOP of the recognized screen (muzzle flash, mouth, cursors, text...): pixels whose index differs from that of the full-screen layer that fits best. Holes of up to
   30 pixels and the 3x3 closing also protect the pixels of those drawings that happen to match the background index (the dotted halo of the muzzle flash). M[] = 1 where it is not repainted. Returns 0 if there is nothing. */
static int dyn_mask(const uint8_t *idx, uint8_t *M) {
    const Layer *E = NULL; int best = 0;
    for (int i = 0; i < nlayers; i++) {
        const Layer *L = &layers[i]; if (L->x || L->y || L->w != SCR_W || L->h != SCR_H) continue;
        int c = 0; for (int k = 0; k < SCR_W * SCR_H; k++) c += idx[k] == L->idx[k];
        if (c > best) { best = c; E = L; }
    }
    if (!E || best < 0.9 * SCR_W * SCR_H || best == SCR_W * SCR_H) return 0;
    int x0 = SCR_W, x1 = -1, y0 = SCR_H, y1 = -1;
    for (int y = 0; y < SCR_H; y++) for (int x = 0; x < SCR_W; x++) if (idx[y * SCR_W + x] != E->idx[y * SCR_W + x]) { if (x < x0) x0 = x; if (x > x1) x1 = x; if (y < y0) y0 = y; if (y > y1) y1 = y; }
    x0 -= 3; y0 -= 3; x1 += 3; y1 += 3; if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0; if (x1 >= SCR_W) x1 = SCR_W - 1; if (y1 >= SCR_H) y1 = SCR_H - 1;
    int w = x1 - x0 + 1, h = y1 - y0 + 1; size_t n = (size_t)w * h;
    uint8_t *a = (uint8_t *)calloc(n, 1), *b = (uint8_t *)calloc(n, 1); int *q = (int *)malloc(n * sizeof(int));
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) a[y * w + x] = idx[(y0 + y) * SCR_W + x0 + x] != E->idx[(y0 + y) * SCR_W + x0 + x];
#define NB(arr, x, y) (((x) > 0 && arr[(y) * w + (x) - 1]) || ((x) < w - 1 && arr[(y) * w + (x) + 1]) || ((y) > 0 && arr[((y) - 1) * w + (x)]) || ((y) < h - 1 && arr[((y) + 1) * w + (x)]))
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) b[y * w + x] = a[y * w + x] || NB(a, x, y);             /* dilation */
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) a[y * w + x] = b[y * w + x] && !((x > 0 && !b[y * w + x - 1]) || (x < w - 1 && !b[y * w + x + 1]) || (y > 0 && !b[(y - 1) * w + x]) || (y < h - 1 && !b[(y + 1) * w + x]));   /* erosion */
    memset(b, 0, n);
    for (int s = 0; s < (int)n; s++) {                                                                         /* small holes that do not touch the window edge */
        if (a[s] || b[s]) continue; int qn = 0, touch = 0; q[qn++] = s; b[s] = 1;
        for (int qi = 0; qi < qn; qi++) { int c = q[qi], cx = c % w, cy = c / w; if (cx == 0 || cy == 0 || cx == w - 1 || cy == h - 1) touch = 1;
            if (cx > 0 && !a[c - 1] && !b[c - 1]) { b[c - 1] = 1; q[qn++] = c - 1; } if (cx < w - 1 && !a[c + 1] && !b[c + 1]) { b[c + 1] = 1; q[qn++] = c + 1; }
            if (cy > 0 && !a[c - w] && !b[c - w]) { b[c - w] = 1; q[qn++] = c - w; } if (cy < h - 1 && !a[c + w] && !b[c + w]) { b[c + w] = 1; q[qn++] = c + w; } }
        if (!touch && qn <= 30) for (int qi = 0; qi < qn; qi++) a[q[qi]] = 1;
    }
    memset(M, 0, (size_t)SCR_W * SCR_H); int any = 0;
    for (int y = 0; y < h; y++) for (int x = 0; x < w; x++) if (a[y * w + x]) { M[(y0 + y) * SCR_W + x0 + x] = 1; any = 1; }
    free(a); free(b); free(q); return any;
}

void gfxpack_apply_layers(uint8_t *out, const uint8_t *idx, const Zone *zones, int nz, int S, int ingame_only) {
    if (S != S_ && nlayers) { int k = S_; S_ = S; (void)k; rescan(); }
    static uint8_t DM[SCR_W * SCR_H], COV[SCR_W * SCR_H]; int have_dm = !ingame_only && nlayers && dyn_mask(idx, DM);
    for (int li = 0; li < nlayers; li++) {
        Layer *L = &layers[li]; if (!L->rgba || (ingame_only && !L->ingame)) continue;
        /* ON-TOP layers present on screen (>= 60 % of their pixels): their whole silhouette is left out, both to recognize this screen (the title covers half the jungle) and to repaint
           (the soldier's mouth, which changes, must not be repainted with the new background even if some pixel matches the jungle index) */
        int nocc = 0;
        for (int oj = 0; oj < nlayers && !ingame_only; oj++) {
            const Layer *O = &layers[oj]; if (O->order <= L->order) continue;
            int oc = 0; for (int y = 0; y < O->h; y++) { const uint8_t *m = O->mask + y * O->w, *c = O->idx + y * O->w, *sc = idx + (O->y + y) * SCR_W + O->x; for (int x = 0; x < O->w; x++) oc += m[x] & (sc[x] == c[x]); }
            if (oc >= 0.6 * O->area) {
                if (!nocc) memset(COV, 0, sizeof COV);
                nocc++; for (int y = 0; y < O->h; y++) { const uint8_t *m = O->mask + y * O->w; uint8_t *cv = COV + (O->y + y) * SCR_W + O->x; for (int x = 0; x < O->w; x++) cv[x] |= m[x]; }
            }
        }
        int cnt = 0, aeff = 0;
        for (int y = 0; y < L->h; y++) {
            const uint8_t *m = L->mask + y * L->w, *c = L->idx + y * L->w, *sc = idx + (L->y + y) * SCR_W + L->x, *cv = COV + (L->y + y) * SCR_W + L->x;
            for (int x = 0; x < L->w; x++) { int v = m[x] & (!nocc || !cv[x]); aeff += v; cnt += v & (sc[x] == c[x]); }
        }
        if (aeff < 0.25 * L->area || cnt < 0.6 * aeff) continue;
        float f = 1.0f;
        if (L->psum > 0) { int line = L->y + L->h / 2; if (line > 199) line = 199; const Zone *z = &zones[0]; for (int i = 0; i < nz; i++) if (zones[i].line <= line) z = &zones[i]; float v = (float)pal_sum(z->pal) / L->psum; f = v < 1.0f ? v : 1.0f; }
        int f256 = (int)(f * 256.0f + 0.5f); if (f256 > 256) f256 = 256;
        int W = SCR_W * S;
        for (int gy = 0; gy < L->h; gy++) {                                     /* it is decided at the game's resolution (64000 pixels) and STRIPS of the enlarged layer are copied */
            const uint8_t *m = L->mask + gy * L->w, *c = L->idx + gy * L->w, *sc = idx + (L->y + gy) * SCR_W + L->x, *cv = COV + (L->y + gy) * SCR_W + L->x, *dm = DM + (L->y + gy) * SCR_W + L->x;
            int gx = 0;
            while (gx < L->w) {
                #define PAINT(g) (m[g] && sc[g] == c[g] && !(nocc && cv[g]) && !(have_dm && dm[g]))
                while (gx < L->w && !PAINT(gx)) gx++;
                if (gx >= L->w) break;
                int g0 = gx; while (gx < L->w && PAINT(gx)) gx++;
                #undef PAINT
                int npx = (gx - g0) * S;
                for (int sy = 0; sy < S; sy++) {
                    uint8_t *o = out + (((size_t)(L->y + gy) * S + sy) * W + (size_t)(L->x + g0) * S) * 3; const uint8_t *q = L->rgba + (((size_t)gy * S + sy) * L->w * S + (size_t)g0 * S) * 4;
                    for (int k = 0; k < npx; k++, o += 3, q += 4) {
                        int a = q[3]; if (!a) continue;
                        if (a == 255 && f256 == 256) { o[0] = q[0]; o[1] = q[1]; o[2] = q[2]; }
                        else for (int ch = 0; ch < 3; ch++) { int v = (o[ch] * (255 - a) + ((q[ch] * f256) >> 8) * a + 127) / 255; o[ch] = (uint8_t)(v > 255 ? 255 : v); }
                    }
                }
            }
        }
    }
}
void present_static_hook(const uint8_t *idx, const Zone *z, int nz, int S, uint8_t *out) { if (nlayers) gfxpack_apply_layers(out, idx, z, nz, S, 0); }

int gfxpack_init(const char *assets_root, int S, const char *want) {
    snprintf(root_, sizeof root_, "%s", assets_root); S_ = S; char p[800]; snprintf(p, sizeof p, "%s/original/index.json", root_);
    if (getenv("DOW_ASSETS") && getenv("DOW_ASSETS")[0] == '0') return 0;
    size_t n; uint8_t *d = read_file(p, &n); if (!d) return 0;
    uint8_t *z = (uint8_t *)realloc(d, n + 1); z[n] = 0; const char *cp = (const char *)z; if (n >= 3 && z[0] == 0xEF && z[1] == 0xBB && z[2] == 0xBF) cp += 3;
    index_ = jparse(&cp); free(z);
    if (!index_ || (int)jnum(index_, "format", 0) != 2) { host_log("assets/original/index.json is from another version of the program: ignored (the new dump is needed)"); jfree(index_); index_ = NULL; return 0; }
    list_packs(); const char *w = want && want[0] ? want : (npacks ? packs[0] : "original"); int ok = !strcmp(w, "original");
    for (int i = 0; i < npacks; i++) if (!strcmp(packs[i], w)) ok = 1;
    if (!ok) { host_log("Graphics pack \"%s\" not found in %s (using %s)", w, root_, npacks ? packs[0] : "the original"); w = npacks ? packs[0] : "original"; }
    snprintf(pack_, sizeof pack_, "%s", w); rescan();
    if (strcmp(pack_, "original")) host_log("Graphics pack: %s (%d replaced elements)", pack_, nreps);
    return 1;
}
static int cb_level(uint32_t id, const uint8_t *pal32, int S, const uint8_t **rgb, int *rows_px, int *H) {
    if (!has_tiles) return 0;
    const uint8_t *p = level_rgb(id, pal32, S, rows_px); if (!p) return 0;
    Level *L = level_find(id); if (!L) return 0; *H = L->H; *rgb = p; return 1;
}
void gfxpack_attach(void) { if (!index_) return; pack_sprite_cb = cb_sprite; pack_hud_cb = cb_hud; pack_texture_cb = cb_texture; pack_level_cb = cb_level; }
const char *gfxpack_name(void) { return pack_; }
int gfxpack_replaced(void) { return nreps; }
int gfxpack_list_count(void) { return npacks; }
const char *gfxpack_list_name(int i) { return packs[i]; }
void gfxpack_select(const char *name) { snprintf(pack_, sizeof pack_, "%s", name); rescan(); }
void gfxpack_reload(void) { list_packs(); rescan(); }
int gfxpack_has_layers(void) { return nlayers > 0; }

/* content hashes for the graphics dump (dump.c) */
void gfx_sprite_hash(const Sprite *sp, char *hex) { sprite_hash(sp, hex); }
void gfx_array_hash(const uint8_t *a, int h, int w, char *hex) { array_hash(a, h, w, hex); }
