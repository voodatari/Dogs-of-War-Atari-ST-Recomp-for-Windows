/*
 * ym.c - Atari ST YM2149 synthesizer: 3 tone channels, noise, mixer and envelope from time-stamped register writes,
 * plus a separate channel for the 4-bit samples timer A plays (writes tagged with register + 16).
 */
#include "audio.h"

#define YM_CLOCK 2000000.0
static const float DAC16[16] = { 0.0f, 0.00999f, 0.01445f, 0.02105f, 0.03070f, 0.04555f, 0.06450f, 0.10736f, 0.12663f, 0.20499f, 0.29221f, 0.37284f, 0.49253f, 0.63544f, 0.80575f, 1.0f };
static float DAC32[32];
#define NOISE_N ((1 << 17) - 1)
static uint8_t *NOISE = NULL;
const float *ym_dac16(void) { return DAC16; }

struct YM {
    int fs; uint8_t regs[16]; double tphase[3], nphase; long nstart; double ephase; int has_eshape; int first[32], second[32], hold; long epos;
    double carry; double dc_in, dc_last; int sreg[2]; float sgain; float cgain[3];
};

static void ym_tables(void) {
    if (NOISE) return;
    DAC32[0] = 0; for (int i = 1; i < 32; i++) DAC32[i] = DAC16[(i + 1) / 2 > 15 ? 15 : (i + 1) / 2];
    NOISE = (uint8_t *)malloc(NOISE_N); uint32_t r = 1;
    for (int i = 0; i < NOISE_N; i++) { NOISE[i] = (uint8_t)(r & 1); uint32_t bit = (r ^ (r >> 3)) & 1; r = (r >> 1) | (bit << 16); }
}

YM *ym_new(int fs) { ym_tables(); YM *y = (YM *)calloc(1, sizeof(YM)); y->fs = fs; y->sgain = 1.0f; y->cgain[0] = y->cgain[1] = y->cgain[2] = 1.0f; return y; }
void ym_set_sample_gain(YM *y, double g) { y->sgain = (float)g; }
void ym_set_channel_gains(YM *y, const float *g) { for (int c = 0; c < 3; c++) y->cgain[c] = g[c]; }
void ym_set_state(YM *y, const uint8_t *regs16) { memcpy(y->regs, regs16, 16); }
void ym_get_phase(YM *y, double *t) { t[0] = y->tphase[0]; t[1] = y->tphase[1]; t[2] = y->tphase[2]; t[3] = y->nphase; }
int ym_samples_for(YM *y, double seconds) { double x = seconds * y->fs + y->carry; int n = (int)x; y->carry = x - n; return n; }

static void shape_table(int shape, int *first, int *second, int *hold) {
    int cont = (shape >> 3) & 1, att = (shape >> 2) & 1, alt = (shape >> 1) & 1, hd = shape & 1;
    for (int i = 0; i < 32; i++) first[i] = att ? i : 31 - i;
    if (!cont) { for (int i = 0; i < 32; i++) second[i] = 0; *hold = 1; return; }
    if (hd) { int fin = alt ? 31 - first[31] : first[31]; for (int i = 0; i < 32; i++) second[i] = fin; *hold = 1; return; }
    for (int i = 0; i < 32; i++) second[i] = alt ? 31 - first[i] : first[i];
    *hold = 0;
}

#define MAXN 8192
static void timeline(int *out, int init, const int *pos, const int *val, int ne, int n) {
    int cur = init, k = 0, p = 0;
    for (int e = 0; e < ne; e++) { int ip = pos[e]; if (ip < 0) ip = 0; if (ip > n) ip = n; for (; p < ip; p++) out[p] = cur; cur = val[e]; k++; }
    (void)k; for (; p < n; p++) out[p] = cur;
}

void ym_render(YM *y, int n, const YmEvent *ev, int nev, float *out) {
    int fs = y->fs;
    if (n <= 0) {
        for (int i = 0; i < nev; i++) { int r = ev[i].reg; if (r >= 16) { if ((r & 15) == 8 || (r & 15) == 9) y->sreg[(r & 15) - 8] = ev[i].val; } else y->regs[r] = ev[i].val; }
        return;
    }
    if (n > MAXN) n = MAXN;
    static int ep[16][256], evv[16][256], ecnt[16]; static int sp[2][256], sv[2][256], scnt[2];
    memset(ecnt, 0, sizeof ecnt); memset(scnt, 0, sizeof scnt);
    for (int i = 0; i < nev; i++) {
        int r = ev[i].reg, p = (int)(ev[i].ph * n);
        if (r >= 16) { int q = r & 15; if ((q == 8 || q == 9) && scnt[q - 8] < 256) { sp[q - 8][scnt[q - 8]] = p; sv[q - 8][scnt[q - 8]++] = ev[i].val; } }
        else if (ecnt[r] < 256) { ep[r][ecnt[r]] = p; evv[r][ecnt[r]++] = ev[i].val; }
    }
    static int tl[14][MAXN];
    for (int r = 0; r < 14; r++) timeline(tl[r], y->regs[r], ep[r], evv[r], ecnt[r], n);
    static float tone[3][MAXN], noise[MAXN], env_lvl[MAXN];
    for (int ch = 0; ch < 3; ch++) {
        double ph = y->tphase[ch];
        for (int i = 0; i < n; i++) {
            int period = tl[2 * ch][i] + ((tl[2 * ch + 1][i] & 15) << 8); if (period < 1) period = 1;
            double f = YM_CLOCK / (16.0 * period); ph += f / fs;
            float sq = (float)((long)floor(ph * 2.0) & 1); if (f > fs * 0.45) sq = 0.5f; tone[ch][i] = sq;
        }
        y->tphase[ch] = fmod(ph, 1.0);
    }
    { double ph = y->nphase; long base = y->nstart; long last = 0;
      for (int i = 0; i < n; i++) { int np = tl[6][i] & 31; if (np < 1) np = 1; ph += YM_CLOCK / (16.0 * np) / fs; last = (long)floor(ph); noise[i] = NOISE[((base + last) % NOISE_N + NOISE_N) % NOISE_N]; }
      y->nstart = ((base + last) % NOISE_N + NOISE_N) % NOISE_N; y->nphase = fmod(ph, 1.0); }
    int use_env = 0; for (int c = 0; c < 3 && !use_env; c++) for (int i = 0; i < n; i++) if (tl[8 + c][i] & 16) { use_env = 1; break; }
    if (use_env) {
        double step = 0; int restart = ecnt[13];
        if (!y->has_eshape || restart) { if (restart) y->regs[13] = (uint8_t)evv[13][restart - 1]; shape_table(y->regs[13] & 15, y->first, y->second, &y->hold); y->has_eshape = 1; y->ephase = 0; y->epos = 0; }
        long pos = 0;
        for (int i = 0; i < n; i++) {
            int per = tl[11][i] + (tl[12][i] << 8); if (per < 1) per = 1; step += YM_CLOCK / (8.0 * per) / fs;
            pos = y->epos + (long)floor(step + y->ephase);
            int seq = pos < 32 ? y->first[pos < 31 ? pos : 31] : (y->hold ? y->second[31] : y->second[(pos - 32) % 32]);
            env_lvl[i] = DAC32[seq];
        }
        y->epos = y->hold ? (pos < 64 ? pos : 64) : pos % 64;
    }
    int s8[MAXN], s9[MAXN]; static int a8[MAXN], a9[MAXN]; (void)s8; (void)s9;
    timeline(a8, y->sreg[0], sp[0], sv[0], scnt[0], n); timeline(a9, y->sreg[1], sp[1], sv[1], scnt[1], n);
    for (int i = 0; i < n; i++) out[i] = (DAC16[a8[i] & 15] + DAC16[a9[i] & 15]) * y->sgain;
    if (scnt[0]) y->sreg[0] = sv[0][scnt[0] - 1]; if (scnt[1]) y->sreg[1] = sv[1][scnt[1] - 1];
    for (int ch = 0; ch < 3; ch++)
        for (int i = 0; i < n; i++) {
            int v = tl[8 + ch][i]; float lvl = DAC16[v & 15]; if (use_env && (v & 16)) lvl = env_lvl[i];
            int mix = tl[7][i]; float t = ((mix >> ch) & 1) ? 1.0f : tone[ch][i], nz = ((mix >> (3 + ch)) & 1) ? 1.0f : noise[i];
            out[i] += y->cgain[ch] * lvl * (t * nz);
        }
    for (int r = 0; r < 14; r++) if (ecnt[r]) y->regs[r] = (uint8_t)evv[r][ecnt[r] - 1];
    for (int i = 0; i < n; i++) out[i] *= (float)(1.0 / 3.0);
    /* DC blocking: y[n] = x[n] - x[n-1] + R*y[n-1] */
    const double R = 0.995; double px = y->dc_in, py = y->dc_last;
    for (int i = 0; i < n; i++) { double x = out[i]; py = x - px + R * py; px = x; double v = py * 1.4; out[i] = (float)(v < -1.0 ? -1.0 : v > 1.0 ? 1.0 : v); }
    y->dc_in = px; y->dc_last = py;
}
