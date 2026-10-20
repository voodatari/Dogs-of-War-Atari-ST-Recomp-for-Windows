/*
 * mixer.c - Own sample mixer: the game plays one 4-bit sample per chip channel (A: $E6EC, B: $E756) with timer A at
 * 4915.2 Hz; here they are mixed into up to 8 voices (multichannel) and/or distributed between the two speakers
 * according to the horizontal position of their source (panning).
 */
#include "audio.h"

#define SAMPLE_RATE (2457600.0 / (10 * 50))
#define MAX_VOICES  8
#define MAX_SAME    2

typedef struct Voice {
    const uint8_t *d;
    int len;
    double pos;
    int off;
    int id, ch;
    float pan;
    int kill; /* -1 = none */
} Voice;
static Voice voices[32];
static int nv = 0;
static struct {
    double ph;
    int id, ch;
    float pan;
} starts[64];
static int ns = 0;
static int opt_multi = 0, opt_pan = 0;
static uint8_t *cache[9];
static int cache_len[9];
static double hp[4] = {0, 0, 0, 0};

/* Stops every voice. */
void mixer_reset(void) {
    nv = 0;
    ns = 0;
}
/* Frees the decoded samples (after a state was loaded they may differ). */
void mixer_drop_cache(void) {
    for (int i = 0; i < 9; i++) {
        free(cache[i]);
        cache[i] = NULL;
        cache_len[i] = 0;
    }
}
/* Switches multichannel effects and stereo panning. */
void mixer_options(int multi, int pan) {
    opt_multi = multi;
    opt_pan = pan;
    ns = 0;
}
/* Is the mixer in use (instead of the chip alone)? */
int mixer_active(void) {
    return opt_multi || opt_pan;
}

/* Starts sample n (0..8) on voice slot ch: the game asked for it with the routine of channel A or B. ph = when inside
 * the VBL; pan = 0 (left) .. 1 (right). At most MAX_SAME copies of the same effect play at once. */
void mixer_start(int n, int ch, double ph, float pan) {
    if (n < 0 || n > 8 || !(opt_multi || opt_pan))
        return;
    if (!cache[n]) {
        uint32_t a = mr32(0xE86A + 8u * (uint32_t)n), b = mr32(0xE86A + 8u * (uint32_t)n + 4);
        if (!(0 < a && a < b && b <= 0x400000 && b - a < 0x8000))
            return;
        cache[n] = (uint8_t *)malloc(b - a);
        for (uint32_t i = 0; i < b - a; i++)
            cache[n][i] = ram[a + i] & 15;
        cache_len[n] = (int)(b - a);
    }
    if (ns < 64) {
        starts[ns].ph = ph;
        starts[ns].id = n;
        starts[ns].ch = ch;
        starts[ns].pan = opt_pan ? pan : 0.5f;
        ns++;
    }
}

static double mix_gain = 1.0;
/* Volume of the sampled effects. */
void mixer_set_gain(double g) {
    mix_gain = g;
}
/* Mixes the voices into n stereo samples: every voice is a 4-bit sample played at 4915.2 Hz, resampled to the output
 * rate and placed between the two speakers by its panning. */
void mixer_render(int n, float *out /* n x 2 */) {
    const float *DAC = ym_dac16();
    double step = SAMPLE_RATE / (double)AUDIO_FS;
    /* the ones that start in this chunk, sorted by instant */
    for (int i = 1; i < ns; i++) {
        typeof(starts[0]) t = starts[i];
        int j = i - 1;
        while (j >= 0 && starts[j].ph > t.ph) {
            starts[j + 1] = starts[j];
            j--;
        }
        starts[j + 1] = t;
    }
    for (int s = 0; s < ns; s++) {
        int id = starts[s].id, ch = starts[s].ch;
        if (opt_multi) {
            int same = 0, first = -1;
            for (int v = 0; v < nv; v++)
                if (voices[v].id == id && voices[v].kill < 0) {
                    same++;
                    if (first < 0)
                        first = v;
                }
            if (same >= MAX_SAME)
                voices[first].kill = 441;
        } else
            for (int v = 0; v < nv; v++)
                if (voices[v].ch == ch && voices[v].kill < 0)
                    voices[v].kill = 44;
        if (nv < 32) {
            double ph = starts[s].ph < 0 ? 0 : starts[s].ph > 0.999 ? 0.999 : starts[s].ph;
            voices[nv].d = cache[id];
            voices[nv].len = cache_len[id];
            voices[nv].pos = 0;
            voices[nv].off = (int)(ph * n);
            voices[nv].id = id;
            voices[nv].ch = ch;
            voices[nv].pan = starts[s].pan;
            voices[nv].kill = -1;
            nv++;
        }
        int live = 0, first = -1;
        for (int v = 0; v < nv; v++)
            if (voices[v].kill < 0) {
                live++;
                if (first < 0)
                    first = v;
            }
        if (live > MAX_VOICES)
            voices[first].kill = 441;
    }
    ns = 0;
    memset(out, 0, sizeof(float) * (size_t)n * 2);
    for (int v = 0; v < nv;) {
        Voice *vo = &voices[v];
        int k = n - vo->off;
        if (k > 0) {
            int cnt = 0;
            for (int i = 0; i < k; i++) {
                double idx = vo->pos + i * step;
                if (idx >= vo->len)
                    break;
                cnt++;
                float lv = DAC[vo->d[(int)idx]];
                if (vo->kill >= 0) {
                    float fl = vo->kill <= 44 ? 44.0f : 441.0f;
                    float g = (vo->kill - i) / fl;
                    g = g < 0 ? 0 : g > 1 ? 1 : g;
                    lv *= g;
                }
                out[(size_t)(vo->off + i) * 2] += lv * (1.0f - vo->pan);
                out[(size_t)(vo->off + i) * 2 + 1] += lv * vo->pan;
            }
            if (vo->kill >= 0)
                vo->kill -= cnt;
            vo->pos += k * step;
            vo->off = 0;
        }
        if (vo->pos >= vo->len || (vo->kill >= 0 && vo->kill <= 0)) {
            voices[v] = voices[nv - 1];
            nv--;
        } else
            v++;
    }
    /* DC blocking (the 4-bit levels are all positive) per speaker */
    const double R = 0.995;
    for (int c = 0; c < 2; c++) {
        double px = hp[2 * c], py = hp[2 * c + 1];
        for (int i = 0; i < n; i++) {
            double x = out[(size_t)i * 2 + c];
            py = x - px + R * py;
            px = x;
            out[(size_t)i * 2 + c] = (float)py;
        }
        hp[2 * c] = px;
        hp[2 * c + 1] = py;
    }
    float sc = (float)(1.0 / 3.0 * 1.4 * (opt_pan ? 2.0 : 1.0) * mix_gain);
    for (int i = 0; i < n * 2; i++)
        out[i] *= sc;
}
