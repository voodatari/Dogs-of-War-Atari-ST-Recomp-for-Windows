/*
 * png.c - Minimal PNG reader and writer (no dependencies): reads 8-bit PNGs (gray, RGB, palette, gray+alpha, RGBA; non-interlaced) into RGBA and writes RGB/RGBA
 * with "stored" deflate blocks (no compression: fast; the captures and the dumps are small).
 */
#include "host.h"

/* ------------------------------------------------------------------------------------------------ crc / adler */
static uint32_t crc_tab[256]; static int crc_ok = 0;
static void crc_init(void) { for (uint32_t n = 0; n < 256; n++) { uint32_t c = n; for (int k = 0; k < 8; k++) c = (c & 1) ? 0xEDB88320u ^ (c >> 1) : c >> 1; crc_tab[n] = c; } crc_ok = 1; }
static uint32_t crc32_(uint32_t c, const uint8_t *p, size_t n) { if (!crc_ok) crc_init(); c = ~c; for (size_t i = 0; i < n; i++) c = crc_tab[(c ^ p[i]) & 255] ^ (c >> 8); return ~c; }
static uint32_t adler(const uint8_t *p, size_t n) { uint32_t a = 1, b = 0; while (n) { size_t k = n > 5552 ? 5552 : n; n -= k; while (k--) { a += *p++; b += a; } a %= 65521; b %= 65521; } return (b << 16) | a; }
static void be32(uint8_t *p, uint32_t v) { p[0] = (uint8_t)(v >> 24); p[1] = (uint8_t)(v >> 16); p[2] = (uint8_t)(v >> 8); p[3] = (uint8_t)v; }

/* ------------------------------------------------------------------------------------------------ inflate */
typedef struct { const uint8_t *p; size_t n, pos; uint32_t bit, nbit; } BS;
static int getbits(BS *s, int n) { while ((int)s->nbit < n) { if (s->pos >= s->n) return -1; s->bit |= (uint32_t)s->p[s->pos++] << s->nbit; s->nbit += 8; } int v = (int)(s->bit & ((1u << n) - 1)); s->bit >>= n; s->nbit -= (uint32_t)n; return v; }
typedef struct { uint16_t count[16], symbol[288]; } Huff;
static void huff_build(Huff *h, const uint8_t *len, int n) {
    memset(h->count, 0, sizeof h->count); for (int i = 0; i < n; i++) h->count[len[i]]++; h->count[0] = 0;
    uint16_t offs[16]; offs[1] = 0; for (int i = 1; i < 15; i++) offs[i + 1] = (uint16_t)(offs[i] + h->count[i]);
    for (int i = 0; i < n; i++) if (len[i]) h->symbol[offs[len[i]]++] = (uint16_t)i;
}
static int huff_dec(BS *s, const Huff *h) {
    int code = 0, first = 0, index = 0;
    for (int len = 1; len <= 15; len++) { int b = getbits(s, 1); if (b < 0) return -1; code |= b; int cnt = h->count[len]; if (code - cnt < first) return h->symbol[index + (code - first)]; index += cnt; first += cnt; first <<= 1; code <<= 1; }
    return -1;
}
static const uint16_t LB[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const uint16_t LE[] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const uint16_t DB[] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
static const uint16_t DE[] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };

static int inflate_raw(const uint8_t *in, size_t n, uint8_t *out, size_t outn) {
    BS s = { in, n, 0, 0, 0 }; size_t o = 0; int last;
    do {
        last = getbits(&s, 1); int type = getbits(&s, 2); if (last < 0 || type < 0) return 0;
        if (type == 0) {
            s.bit = 0; s.nbit = 0; if (s.pos + 4 > s.n) return 0; unsigned len = in[s.pos] | (in[s.pos + 1] << 8); s.pos += 4;
            if (s.pos + len > s.n || o + len > outn) return 0; memcpy(out + o, in + s.pos, len); s.pos += len; o += len;
        } else if (type == 1 || type == 2) {
            Huff lh, dh; uint8_t lens[320];
            if (type == 1) { int i = 0; for (; i < 144; i++) lens[i] = 8; for (; i < 256; i++) lens[i] = 9; for (; i < 280; i++) lens[i] = 7; for (; i < 288; i++) lens[i] = 8; huff_build(&lh, lens, 288); for (i = 0; i < 30; i++) lens[i] = 5; huff_build(&dh, lens, 30); }
            else {
                int nlen = getbits(&s, 5) + 257, ndist = getbits(&s, 5) + 1, ncode = getbits(&s, 4) + 4; static const uint8_t ord[19] = { 16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15 };
                uint8_t cl[19] = { 0 }; for (int i = 0; i < ncode; i++) { int v = getbits(&s, 3); if (v < 0) return 0; cl[ord[i]] = (uint8_t)v; }
                Huff ch; huff_build(&ch, cl, 19); int idx = 0;
                while (idx < nlen + ndist) {
                    int sym = huff_dec(&s, &ch); if (sym < 0) return 0;
                    if (sym < 16) lens[idx++] = (uint8_t)sym;
                    else { int rep, val = 0; if (sym == 16) { if (!idx) return 0; val = lens[idx - 1]; rep = 3 + getbits(&s, 2); } else if (sym == 17) rep = 3 + getbits(&s, 3); else rep = 11 + getbits(&s, 7); if (idx + rep > nlen + ndist) return 0; while (rep--) lens[idx++] = (uint8_t)val; }
                }
                huff_build(&lh, lens, nlen); huff_build(&dh, lens + nlen, ndist);
            }
            for (;;) {
                int sym = huff_dec(&s, &lh); if (sym < 0) return 0;
                if (sym < 256) { if (o >= outn) return 0; out[o++] = (uint8_t)sym; }
                else if (sym == 256) break;
                else {
                    sym -= 257; if (sym >= 29) return 0; int len = LB[sym] + getbits(&s, LE[sym]); int ds = huff_dec(&s, &dh); if (ds < 0 || ds >= 30) return 0;
                    int dist = DB[ds] + getbits(&s, DE[ds]); if ((size_t)dist > o || o + (size_t)len > outn) return 0; while (len--) { out[o] = out[o - (size_t)dist]; o++; }
                }
            }
        } else return 0;
    } while (!last);
    return (int)(o == outn || o > 0);
}

/* ------------------------------------------------------------------------------------------------ lectura */
uint8_t *png_read_rgba(const char *path, int *w, int *h) {
    size_t n; uint8_t *d = read_file(path, &n); if (!d) return NULL;
    uint8_t *res = NULL, *idat = NULL; size_t idat_n = 0, idat_cap = 0;
    if (n < 33 || memcmp(d, "\x89PNG\r\n\x1a\n", 8)) goto done;
    uint32_t W = 0, Hh = 0; int bd = 0, ct = 0, il = 0; uint8_t pal[768]; uint8_t trns[256]; int npal = 0, ntrns = 0; memset(trns, 255, sizeof trns);
    size_t pos = 8;
    while (pos + 12 <= n) {
        uint32_t len = ((uint32_t)d[pos] << 24) | ((uint32_t)d[pos + 1] << 16) | ((uint32_t)d[pos + 2] << 8) | d[pos + 3]; const uint8_t *tp = d + pos + 4, *cd = d + pos + 8;
        if (pos + 12 + len > n) break;
        if (!memcmp(tp, "IHDR", 4)) { W = ((uint32_t)cd[0] << 24) | ((uint32_t)cd[1] << 16) | ((uint32_t)cd[2] << 8) | cd[3]; Hh = ((uint32_t)cd[4] << 24) | ((uint32_t)cd[5] << 16) | ((uint32_t)cd[6] << 8) | cd[7]; bd = cd[8]; ct = cd[9]; il = cd[12]; }
        else if (!memcmp(tp, "PLTE", 4)) { npal = (int)(len / 3); if (npal > 256) npal = 256; memcpy(pal, cd, (size_t)npal * 3); }
        else if (!memcmp(tp, "tRNS", 4)) { ntrns = (int)len > 256 ? 256 : (int)len; if (ct == 3) memcpy(trns, cd, (size_t)ntrns); }
        else if (!memcmp(tp, "IDAT", 4)) { if (idat_n + len > idat_cap) { idat_cap = (idat_n + len) * 2; idat = (uint8_t *)realloc(idat, idat_cap); } memcpy(idat + idat_n, cd, len); idat_n += len; }
        else if (!memcmp(tp, "IEND", 4)) break;
        pos += 12 + len;
    }
    if (!W || !Hh || bd != 8 || il != 0 || W > 16384 || Hh > 16384) goto done;
    int ch = ct == 0 ? 1 : ct == 2 ? 3 : ct == 3 ? 1 : ct == 4 ? 2 : ct == 6 ? 4 : 0; if (!ch || idat_n < 6) goto done;
    size_t stride = (size_t)W * ch, rawn = (stride + 1) * Hh; uint8_t *raw = (uint8_t *)malloc(rawn);
    if (!inflate_raw(idat + 2, idat_n - 2, raw, rawn)) { free(raw); goto done; }
    for (uint32_t y = 0; y < Hh; y++) {                                      /* filtros */
        uint8_t *row = raw + y * (stride + 1) + 1, *prev = y ? raw + (y - 1) * (stride + 1) + 1 : NULL; int f = raw[y * (stride + 1)];
        for (size_t x = 0; x < stride; x++) {
            int a = x >= (size_t)ch ? row[x - ch] : 0, b = prev ? prev[x] : 0, c = (prev && x >= (size_t)ch) ? prev[x - ch] : 0, v = row[x];
            switch (f) { case 1: v += a; break; case 2: v += b; break; case 3: v += (a + b) / 2; break;
                case 4: { int pp = a + b - c, pa = abs(pp - a), pb = abs(pp - b), pc = abs(pp - c); v += (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c); break; } }
            row[x] = (uint8_t)v;
        }
    }
    res = (uint8_t *)malloc((size_t)W * Hh * 4);
    for (uint32_t y = 0; y < Hh; y++) for (uint32_t x = 0; x < W; x++) {
        const uint8_t *s = raw + y * (stride + 1) + 1 + (size_t)x * ch; uint8_t *o = res + ((size_t)y * W + x) * 4;
        switch (ct) { case 0: o[0] = o[1] = o[2] = s[0]; o[3] = 255; break; case 2: o[0] = s[0]; o[1] = s[1]; o[2] = s[2]; o[3] = 255; break;
            case 3: o[0] = pal[s[0] * 3]; o[1] = pal[s[0] * 3 + 1]; o[2] = pal[s[0] * 3 + 2]; o[3] = trns[s[0]]; break;
            case 4: o[0] = o[1] = o[2] = s[0]; o[3] = s[1]; break; default: memcpy(o, s, 4); }
    }
    free(raw); *w = (int)W; *h = (int)Hh;
done:
    free(idat); free(d); return res;
}

/* indices of a palette PNG (for the .idx.png layers) */
uint8_t *png_read_indexed(const char *path, int *w, int *h) {
    size_t n; uint8_t *d = read_file(path, &n); if (!d) return NULL;
    uint8_t *res = NULL, *idat = NULL; size_t idat_n = 0, idat_cap = 0;
    if (n < 33 || memcmp(d, "\x89PNG\r\n\x1a\n", 8)) goto done;
    uint32_t W = 0, Hh = 0; int bd = 0, ct = 0; size_t pos = 8;
    while (pos + 12 <= n) {
        uint32_t len = ((uint32_t)d[pos] << 24) | ((uint32_t)d[pos + 1] << 16) | ((uint32_t)d[pos + 2] << 8) | d[pos + 3]; const uint8_t *tp = d + pos + 4, *cd = d + pos + 8;
        if (pos + 12 + len > n) break;
        if (!memcmp(tp, "IHDR", 4)) { W = ((uint32_t)cd[0] << 24) | ((uint32_t)cd[1] << 16) | ((uint32_t)cd[2] << 8) | cd[3]; Hh = ((uint32_t)cd[4] << 24) | ((uint32_t)cd[5] << 16) | ((uint32_t)cd[6] << 8) | cd[7]; bd = cd[8]; ct = cd[9]; }
        else if (!memcmp(tp, "IDAT", 4)) { if (idat_n + len > idat_cap) { idat_cap = (idat_n + len) * 2; idat = (uint8_t *)realloc(idat, idat_cap); } memcpy(idat + idat_n, cd, len); idat_n += len; }
        else if (!memcmp(tp, "IEND", 4)) break;
        pos += 12 + len;
    }
    if (!W || !Hh || ct != 3 || bd != 8) goto done;
    size_t stride = W, rawn = (stride + 1) * Hh; uint8_t *raw = (uint8_t *)malloc(rawn);
    if (!inflate_raw(idat + 2, idat_n - 2, raw, rawn)) { free(raw); goto done; }
    for (uint32_t y = 0; y < Hh; y++) {
        uint8_t *row = raw + y * (stride + 1) + 1, *prev = y ? raw + (y - 1) * (stride + 1) + 1 : NULL; int f = raw[y * (stride + 1)];
        for (size_t x = 0; x < stride; x++) {
            int a = x ? row[x - 1] : 0, b = prev ? prev[x] : 0, c = (prev && x) ? prev[x - 1] : 0, v = row[x];
            switch (f) { case 1: v += a; break; case 2: v += b; break; case 3: v += (a + b) / 2; break;
                case 4: { int pp = a + b - c, pa = abs(pp - a), pb = abs(pp - b), pc = abs(pp - c); v += (pa <= pb && pa <= pc) ? a : (pb <= pc ? b : c); break; } }
            row[x] = (uint8_t)v;
        }
    }
    res = (uint8_t *)malloc((size_t)W * Hh); for (uint32_t y = 0; y < Hh; y++) memcpy(res + (size_t)y * W, raw + y * (stride + 1) + 1, W);
    free(raw); *w = (int)W; *h = (int)Hh;
done:
    free(idat); free(d); return res;
}

/* ------------------------------------------------------------------------------------------------ writing (deflate: LZ77 + fixed Huffman codes) */
static void chunk(FILE *f, const char *type, const uint8_t *d, uint32_t n) {
    uint8_t h[8]; be32(h, n); memcpy(h + 4, type, 4); fwrite(h, 1, 8, f);
    uint32_t c = crc32_(0, (const uint8_t *)type, 4); if (n) { fwrite(d, 1, n, f); c = crc32_(c, d, n); }
    uint8_t t[4]; be32(t, c); fwrite(t, 1, 4, f);
}
typedef struct { uint8_t *p; size_t n; uint32_t acc; int nb; } BitW;
static void bw_bits(BitW *b, uint32_t v, int n) { b->acc |= v << b->nb; b->nb += n; while (b->nb >= 8) { b->p[b->n++] = (uint8_t)b->acc; b->acc >>= 8; b->nb -= 8; } }
static void bw_code(BitW *b, uint32_t code, int len) { uint32_t r = 0; for (int i = 0; i < len; i++) r |= ((code >> i) & 1u) << (len - 1 - i); bw_bits(b, r, len); }
static void bw_lit(BitW *b, int v) { if (v < 144) bw_code(b, 0x30u + (uint32_t)v, 8); else if (v < 256) bw_code(b, 0x190u + (uint32_t)(v - 144), 9); else if (v < 280) bw_code(b, (uint32_t)(v - 256), 7); else bw_code(b, 0xC0u + (uint32_t)(v - 280), 8); }
static const uint16_t LBASE[29] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31, 35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258 };
static const uint8_t  LEXT[29] = { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0 };
static const uint16_t DBASE[30] = { 1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129, 193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097, 6145, 8193, 12289, 16385, 24577 };
static const uint8_t  DEXT[30] = { 0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13 };
/* zlib stream of `in`; returns the malloc'd stream and its size in *outn */
static uint8_t *zlib_pack(const uint8_t *in, size_t n, size_t *outn) {
    BitW b; b.p = (uint8_t *)malloc(n + n / 2 + 64); b.n = 0; b.acc = 0; b.nb = 0; b.p[b.n++] = 0x78; b.p[b.n++] = 0x01;
    bw_bits(&b, 1, 1); bw_bits(&b, 1, 2);                                                   /* last block, fixed Huffman */
    int32_t *head = (int32_t *)malloc(65536 * sizeof(int32_t)), *prev = (int32_t *)malloc((n + 1) * sizeof(int32_t)); for (int i = 0; i < 65536; i++) head[i] = -1;
    size_t i = 0;
    while (i < n) {
        int bl = 0, bd = 0;
        if (i + 3 <= n) {
            uint32_t h = (((uint32_t)in[i] << 8) ^ ((uint32_t)in[i + 1] << 4) ^ in[i + 2] ^ ((uint32_t)in[i + 2] << 11)) & 0xFFFF; int32_t c = head[h]; int depth = 0;
            while (c >= 0 && i - (size_t)c <= 32768 && depth++ < 24) {
                int l = 0; size_t mx = n - i < 258 ? n - i : 258; while ((size_t)l < mx && in[(size_t)c + (size_t)l] == in[i + (size_t)l]) l++;
                if (l > bl) { bl = l; bd = (int)(i - (size_t)c); if (l == 258) break; }
                c = prev[c];
            }
        }
        size_t adv = 1;
        if (bl >= 3) {
            int lc = 28; while (LBASE[lc] > bl) lc--; bw_lit(&b, 257 + lc); if (LEXT[lc]) bw_bits(&b, (uint32_t)(bl - LBASE[lc]), LEXT[lc]);
            int dc = 29; while (DBASE[dc] > bd) dc--; bw_code(&b, (uint32_t)dc, 5); if (DEXT[dc]) bw_bits(&b, (uint32_t)(bd - DBASE[dc]), DEXT[dc]);
            adv = (size_t)bl;
        } else bw_lit(&b, in[i]);
        for (size_t k = 0; k < adv; k++) if (i + k + 3 <= n) { uint32_t h = (((uint32_t)in[i + k] << 8) ^ ((uint32_t)in[i + k + 1] << 4) ^ in[i + k + 2] ^ ((uint32_t)in[i + k + 2] << 11)) & 0xFFFF; prev[i + k] = head[h]; head[h] = (int32_t)(i + k); }
        i += adv;
    }
    bw_lit(&b, 256); if (b.nb) bw_bits(&b, 0, 8 - b.nb);
    uint8_t t[4]; be32(t, adler(in, n)); memcpy(b.p + b.n, t, 4); b.n += 4; free(head); free(prev); *outn = b.n; return b.p;
}
/* ct: 2 = RGB, 6 = RGBA, 3 = palette (8 bits, `plte` = 768 bytes, px = indices) */
static int png_write_ct(const char *path, const uint8_t *px, int w, int h, int bpp, int ct, const uint8_t *plte) {
    FILE *f = fopen(path, "wb"); if (!f) return 0;
    size_t stride = (size_t)w * (size_t)bpp, rawn = (stride + 1) * (size_t)h; uint8_t *raw = (uint8_t *)malloc(rawn);
    for (int y = 0; y < h; y++) { raw[(size_t)y * (stride + 1)] = 0; memcpy(raw + (size_t)y * (stride + 1) + 1, px + (size_t)y * stride, stride); }
    size_t zn; uint8_t *z = zlib_pack(raw, rawn, &zn);
    fwrite("\x89PNG\r\n\x1a\n", 1, 8, f); uint8_t ih[13]; be32(ih, (uint32_t)w); be32(ih + 4, (uint32_t)h); ih[8] = 8; ih[9] = (uint8_t)ct; ih[10] = 0; ih[11] = 0; ih[12] = 0;
    chunk(f, "IHDR", ih, 13); if (ct == 3) chunk(f, "PLTE", plte, 768); chunk(f, "IDAT", z, (uint32_t)zn); chunk(f, "IEND", NULL, 0);
    fclose(f); free(raw); free(z); return 1;
}
int png_write(const char *path, const uint8_t *px, int w, int h, int channels /* 3 or 4 */) { return png_write_ct(path, px, w, h, channels, channels == 4 ? 6 : 2, NULL); }
int png_write_indexed(const char *path, const uint8_t *idx, int w, int h, const uint8_t *plte768) { return png_write_ct(path, idx, w, h, 1, 3, plte768); }
