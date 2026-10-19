/* state.c - machine snapshots (DOWS format) with the RAM compressed by repetitions. */
#include "host.h"

State *state_new(void) { State *s = (State *)calloc(1, sizeof(State)); s->ram = (uint8_t *)malloc(RAMSZ); s->song = -1; return s; }
void state_free(State *s) { if (!s) return; free(s->ram); free(s); }
State *state_clone(const State *s) { State *c = state_new(); uint8_t *r = c->ram; *c = *s; c->ram = r; memcpy(r, s->ram, RAMSZ); return c; }

/* RLE: control 0..127 = (c+1) literals; 128..255 = (c-125) repetitions (3..130) of the next byte */
static size_t rle_pack(const uint8_t *in, size_t n, uint8_t *out) {
    size_t i = 0, o = 0;
    while (i < n) {
        size_t run = 1; while (i + run < n && in[i + run] == in[i] && run < 130) run++;
        if (run >= 3) { out[o++] = (uint8_t)(run + 125); out[o++] = in[i]; i += run; continue; }
        size_t j = i, lit = 0;
        while (j < n && lit < 128) { size_t r2 = 1; while (j + r2 < n && in[j + r2] == in[j] && r2 < 3) r2++; if (r2 >= 3) break; j++; lit++; }
        if (!lit) { lit = 1; j = i + 1; }
        out[o++] = (uint8_t)(lit - 1); memcpy(out + o, in + i, lit); o += lit; i += lit;
    }
    return o;
}
static int rle_unpack(const uint8_t *in, size_t n, uint8_t *out, size_t outn) {
    size_t i = 0, o = 0;
    while (i < n && o < outn) {
        uint8_t c = in[i++];
        if (c < 128) { size_t l = (size_t)c + 1; if (i + l > n || o + l > outn) return 0; memcpy(out + o, in + i, l); o += l; i += l; }
        else { size_t r = (size_t)c - 125; if (i >= n || o + r > outn) return 0; memset(out + o, in[i++], r); o += r; }
    }
    return o == outn;
}

#define PUT32(p, v) do { (p)[0] = (uint8_t)((v) >> 24); (p)[1] = (uint8_t)((v) >> 16); (p)[2] = (uint8_t)((v) >> 8); (p)[3] = (uint8_t)(v); } while (0)
#define GET32(p) (((uint32_t)(p)[0] << 24) | ((uint32_t)(p)[1] << 16) | ((uint32_t)(p)[2] << 8) | (p)[3])

int state_write_file(const State *s, const char *path) {
    size_t cap = RAMSZ + RAMSZ / 100 + 4096 + 0x8000 + 1024; uint8_t *b = (uint8_t *)malloc(cap), *p = b;
    memcpy(p, "DOWS1\0\0\0", 8); p += 8;
    uint32_t hv[] = { s->pc, s->vbl, s->sr, s->usp, s->isp, s->a7, s->heap, s->scrbase };
    for (int i = 0; i < 8; i++) { PUT32(p, hv[i]); p += 4; }
    for (int i = 0; i < 8; i++) { PUT32(p, s->d[i]); p += 4; }
    for (int i = 0; i < 7; i++) { PUT32(p, s->a[i]); p += 4; }
    PUT32(p, (uint32_t)s->song); p += 4;
    PUT32(p, (uint32_t)s->nrx); p += 4; memcpy(p, s->rx, 256); p += 256;
    PUT32(p, (uint32_t)s->nzones); p += 4;
    for (int i = 0; i < MAXZONES; i++) { PUT32(p, (uint32_t)s->zones[i].line); p += 4; memcpy(p, s->zones[i].pal, 32); p += 32; }
    memcpy(p, s->ym, 16); p += 16; PUT32(p, (uint32_t)s->ymsel); p += 4;
    memcpy(p, s->curdir, 260); p += 260;
    memcpy(p, s->io, 0x8000); p += 0x8000;
    uint8_t *tmp = (uint8_t *)malloc(RAMSZ + RAMSZ / 64 + 1024);
    size_t cl = rle_pack(s->ram, RAMSZ, tmp);
    PUT32(p, (uint32_t)cl); p += 4; memcpy(p, tmp, cl); p += cl; free(tmp);
    int ok = write_file(path, b, (size_t)(p - b)); free(b); return ok;
}

int state_read_file(State *s, const char *path) {
    size_t n = 0; uint8_t *b = read_file(path, &n); if (!b) return 0;
    const uint8_t *p = b; int ok = 0;
    if (n < 8 || memcmp(p, "DOWS1", 5) != 0) goto done;
    p += 8;
    uint32_t hv[8]; for (int i = 0; i < 8; i++) { hv[i] = GET32(p); p += 4; }
    s->pc = hv[0]; s->vbl = hv[1]; s->sr = hv[2]; s->usp = hv[3]; s->isp = hv[4]; s->a7 = hv[5]; s->heap = hv[6]; s->scrbase = hv[7];
    for (int i = 0; i < 8; i++) { s->d[i] = GET32(p); p += 4; }
    for (int i = 0; i < 7; i++) { s->a[i] = GET32(p); p += 4; }
    s->song = (int32_t)GET32(p); p += 4;
    s->nrx = (int)GET32(p); p += 4; memcpy(s->rx, p, 256); p += 256;
    s->nzones = (int)GET32(p); p += 4;
    for (int i = 0; i < MAXZONES; i++) { s->zones[i].line = (int)GET32(p); p += 4; memcpy(s->zones[i].pal, p, 32); p += 32; }
    memcpy(s->ym, p, 16); p += 16; s->ymsel = (int)GET32(p); p += 4;
    memcpy(s->curdir, p, 260); p += 260;
    memcpy(s->io, p, 0x8000); p += 0x8000;
    { uint32_t cl = GET32(p); p += 4; if ((size_t)(p - b) + cl > n) goto done; ok = rle_unpack(p, cl, s->ram, RAMSZ); }
done:
    free(b); return ok;
}
