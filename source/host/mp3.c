/*
 * mp3.c - Optional MP3 music: decodes with Windows Media Foundation (mp3, wav...) to 16-bit stereo 44100 Hz PCM, trims the digital silence
 * at the ends and reads it in a gapless ring. The files are looked up in the `music` folder next to the program.
 */
#include "audio.h"
#ifdef _WIN32
#define COBJMACROS
#include <windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>
#include <mferror.h>
#endif

#define NSONG 16
typedef struct Music {
    char dir[500];
    int state[NSONG];                    /* 0 = not requested, 1 = decoding, 2 = ready, 3 = unreadable */
    int16_t *pcm[NSONG]; int frames[NSONG];
    int lp_start[NSONG], lp_back[NSONG];  /* loop points (frames of the decoded audio) or -1 */
    int prefetching; int gen;            /* generation of the folder: when the music pack changes, whatever was being decoded from the previous one is discarded */
#ifdef _WIN32
    CRITICAL_SECTION cs;
#endif
} Music;

static const char *EXTS[] = { ".mp3", ".ogg", ".wav", ".wma", ".m4a", NULL };
static void song_name(int n, char *out, size_t k) {                          /* names of the files in the "music" folder */
    static const char *const NAMES[8] = { "mission", "map", NULL, "bonus", NULL, "highscores", "endlevel", "fail" };
    if (n >= 0 && n < 8 && NAMES[n]) snprintf(out, k, "%s", NAMES[n]); else snprintf(out, k, "song%d", n);
}
static int find_path(Music *m, int n, char *out, size_t k) {
    char nm[40], dir[500]; song_name(n, nm, sizeof nm);
#ifdef _WIN32
    EnterCriticalSection(&m->cs); snprintf(dir, sizeof dir, "%s", m->dir); LeaveCriticalSection(&m->cs);
#else
    snprintf(dir, sizeof dir, "%s", m->dir);
#endif
    for (int i = 0; EXTS[i]; i++) { char p[600]; snprintf(p, sizeof p, "%s/%s%s", dir, nm, EXTS[i]); if (file_exists(p)) { snprintf(out, k, "%s", p); return 1; } }
    return 0;
}

Music *music_new(const char *folder) {
    Music *m = (Music *)calloc(1, sizeof(Music)); snprintf(m->dir, sizeof m->dir, "%s", folder);
#ifdef _WIN32
    InitializeCriticalSection(&m->cs);
#endif
    for (int i = 0; i < NSONG; i++) m->lp_start[i] = m->lp_back[i] = -1;
    return m;
}

#ifdef _WIN32
static int mf_started = 0;
static int16_t *decode_file(const char *path, int *frames, int *head, int *rate) {
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (!mf_started) { if (FAILED(MFStartup(MF_VERSION, MFSTARTUP_FULL))) return NULL; mf_started = 1; }
    wchar_t wp[700]; MultiByteToWideChar(CP_ACP, 0, path, -1, wp, 700);
    IMFSourceReader *rd = NULL; if (FAILED(MFCreateSourceReaderFromURL(wp, NULL, &rd))) return NULL;
    *rate = 44100; { IMFMediaType *nmt = NULL; if (SUCCEEDED(IMFSourceReader_GetNativeMediaType(rd, MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, &nmt))) { UINT32 r = 0; if (SUCCEEDED(IMFMediaType_GetUINT32(nmt, &MF_MT_AUDIO_SAMPLES_PER_SECOND, &r)) && r >= 8000 && r <= 192000) *rate = (int)r; IMFMediaType_Release(nmt); } }     /* sample rate of the file itself */
    IMFMediaType *mt = NULL; MFCreateMediaType(&mt);
    IMFMediaType_SetGUID(mt, &MF_MT_MAJOR_TYPE, &MFMediaType_Audio); IMFMediaType_SetGUID(mt, &MF_MT_SUBTYPE, &MFAudioFormat_PCM);
    IMFMediaType_SetUINT32(mt, &MF_MT_AUDIO_NUM_CHANNELS, 2); IMFMediaType_SetUINT32(mt, &MF_MT_AUDIO_SAMPLES_PER_SECOND, 44100); IMFMediaType_SetUINT32(mt, &MF_MT_AUDIO_BITS_PER_SAMPLE, 16);
    IMFMediaType_SetUINT32(mt, &MF_MT_AUDIO_BLOCK_ALIGNMENT, 4); IMFMediaType_SetUINT32(mt, &MF_MT_AUDIO_AVG_BYTES_PER_SECOND, 176400);
    HRESULT hr = IMFSourceReader_SetCurrentMediaType(rd, MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, mt); IMFMediaType_Release(mt);
    if (FAILED(hr)) { IMFSourceReader_Release(rd); return NULL; }
    size_t cap = 1 << 22, n = 0; uint8_t *buf = (uint8_t *)malloc(cap);
    for (;;) {
        DWORD flags = 0; IMFSample *sm = NULL; LONGLONG ts = 0;
        hr = IMFSourceReader_ReadSample(rd, MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, NULL, &flags, &ts, &sm);
        if (FAILED(hr) || (flags & MF_SOURCE_READERF_ERROR)) { if (sm) IMFSample_Release(sm); break; }
        if (sm) {
            IMFMediaBuffer *mb = NULL; if (SUCCEEDED(IMFSample_ConvertToContiguousBuffer(sm, &mb))) {
                BYTE *d = NULL; DWORD ml = 0, cl = 0; if (SUCCEEDED(IMFMediaBuffer_Lock(mb, &d, &ml, &cl))) { if (n + cl > cap) { while (n + cl > cap) cap *= 2; buf = (uint8_t *)realloc(buf, cap); } memcpy(buf + n, d, cl); n += cl; IMFMediaBuffer_Unlock(mb); }
                IMFMediaBuffer_Release(mb);
            }
            IMFSample_Release(sm);
        }
        if (flags & MF_SOURCE_READERF_ENDOFSTREAM) break;
    }
    IMFSourceReader_Release(rd);
    int fr = (int)(n / 4); int16_t *pcm = (int16_t *)buf;
    int a = 0, b = fr - 1;                                                    /* trims the digital silence at the ends (|x| <= 24) */
    while (a < fr && abs(pcm[a * 2]) <= 24 && abs(pcm[a * 2 + 1]) <= 24) a++;
    while (b > a && abs(pcm[b * 2]) <= 24 && abs(pcm[b * 2 + 1]) <= 24) b--;
    if (a >= fr) { a = 0; b = fr - 1; }
    int len = b - a + 1; if (len < 1000) { free(buf); return NULL; }
    int16_t *out = (int16_t *)malloc((size_t)len * 4); memcpy(out, pcm + (size_t)a * 2, (size_t)len * 4); free(buf); *frames = len; *head = a; return out;
}

/* <song>.txt next to the audio file:   Loop point start = "N"   and   Loop point back = "M"   (samples of the file at ITS OWN sample rate, 44100 Hz, 48000 Hz..., counted from the start of the file).
   When the playback reaches `start` it goes back to `back`. An empty file, or one without both numbers, leaves the song as it was (whole file in a loop). */
static void read_loop_points(const char *audio_path, int head, int frames, int rate, int *ls, int *lb) {
    char tp[640]; snprintf(tp, sizeof tp, "%s", audio_path); char *dot = strrchr(tp, '.'); if (!dot) return; strcpy(dot, ".txt");
    size_t n = 0; char *t = (char *)read_file(tp, &n); *ls = *lb = -1; if (!t) return;
    char *txt = (char *)malloc(n + 1); memcpy(txt, t, n); txt[n] = 0; free(t);
    long long st = -1, bk = -1;
    for (char *line = strtok(txt, "\r\n"); line; line = strtok(NULL, "\r\n")) {
        char low[200]; size_t k = 0; for (; line[k] && k < sizeof low - 1; k++) low[k] = (char)tolower((unsigned char)line[k]); low[k] = 0;
        long long *dst = strstr(low, "loop point start") ? &st : strstr(low, "loop point back") ? &bk : NULL; if (!dst) continue;
        const char *eq = strchr(line, '='); if (!eq) continue; eq++; while (*eq == ' ' || *eq == '\t' || *eq == '"') eq++;
        if (*eq >= '0' && *eq <= '9') *dst = _strtoi64(eq, NULL, 10);
    }
    free(txt);
    if (st < 0 || bk < 0) return;
    if (rate != 44100) { st = (st * 44100 + rate / 2) / rate; bk = (bk * 44100 + rate / 2) / rate; }                  /* the audio is decoded at 44100 Hz */
    st -= head; bk -= head;                                                    /* the digital silence trimmed at the start is not part of the decoded buffer */
    if (bk < 0 || st <= bk || st > frames) { host_log("Music: loop points of %s are outside the song (start %lld, back %lld, length %d): ignored", tp, st + head, bk + head, frames + head); return; }
    *ls = (int)st; *lb = (int)bk;
}
static void store_song(Music *m, int n, int16_t *pcm, int fr, const char *path, int head, int rate) {      /* called with the lock held */
    m->pcm[n] = pcm; m->frames[n] = fr; m->state[n] = 2; read_loop_points(path, head, fr, rate, &m->lp_start[n], &m->lp_back[n]);
}
static DWORD WINAPI load_thread(LPVOID arg) {
    Music *m = ((void **)arg)[0]; int n = (int)(intptr_t)((void **)arg)[1]; free(arg);
    char p[600]; EnterCriticalSection(&m->cs); int gen = m->gen; LeaveCriticalSection(&m->cs);
    if (!find_path(m, n, p, sizeof p)) { EnterCriticalSection(&m->cs); if (gen == m->gen) m->state[n] = 3; LeaveCriticalSection(&m->cs); return 0; }
    int fr = 0, head = 0, rate = 44100; int16_t *pcm = decode_file(p, &fr, &head, &rate);
    EnterCriticalSection(&m->cs);
    if (gen != m->gen) free(pcm);                                              /* the music pack was changed while decoding */
    else if (pcm) store_song(m, n, pcm, fr, p, head, rate); else { host_log("MP3: cannot read %s", p); m->state[n] = 3; }
    LeaveCriticalSection(&m->cs);
    return 0;
}
static DWORD WINAPI prefetch_thread(LPVOID arg) {
    Music *m = (Music *)arg; static const int order[] = { 0, 1, 6, 3, 7, 5, 2, 4, 8, 9, 10, 11 };
    EnterCriticalSection(&m->cs); int gen = m->gen; LeaveCriticalSection(&m->cs);
    for (int i = 0; i < 12; i++) {
        int n = order[i]; char p[600]; EnterCriticalSection(&m->cs); if (gen != m->gen) { LeaveCriticalSection(&m->cs); break; } int skip = m->state[n] != 0 || !find_path(m, n, p, sizeof p); if (!skip) m->state[n] = 1; LeaveCriticalSection(&m->cs);
        if (skip) continue;
        int fr = 0, head = 0, rate = 44100; int16_t *pcm = decode_file(p, &fr, &head, &rate);
        EnterCriticalSection(&m->cs);
        if (gen != m->gen) free(pcm);
        else if (pcm) store_song(m, n, pcm, fr, p, head, rate); else { host_log("MP3: cannot read %s", p); m->state[n] = 3; }
        LeaveCriticalSection(&m->cs);
    }
    EnterCriticalSection(&m->cs); if (gen == m->gen) m->prefetching = 0; LeaveCriticalSection(&m->cs); return 0;
}
static void request(Music *m, int n) {
    char p[600]; EnterCriticalSection(&m->cs);
    int go = m->state[n] == 0 && find_path(m, n, p, sizeof p); if (go) m->state[n] = 1; LeaveCriticalSection(&m->cs);
    if (!go) return;
    void **a = (void **)malloc(2 * sizeof(void *)); a[0] = m; a[1] = (void *)(intptr_t)n; CloseHandle(CreateThread(NULL, 0, load_thread, a, 0, NULL));
}
void music_prefetch(Music *m) { EnterCriticalSection(&m->cs); int go = !m->prefetching; m->prefetching = 1; LeaveCriticalSection(&m->cs); if (go) CloseHandle(CreateThread(NULL, 0, prefetch_thread, m, 0, NULL)); }

void music_set_dir(Music *m, const char *folder) {                           /* switches the music pack: what was already decoded is freed (only from the thread that plays) */
    EnterCriticalSection(&m->cs);
    snprintf(m->dir, sizeof m->dir, "%s", folder); m->gen++; m->prefetching = 0;
    for (int i = 0; i < NSONG; i++) { free(m->pcm[i]); m->pcm[i] = NULL; m->frames[i] = 0; m->state[i] = 0; m->lp_start[i] = m->lp_back[i] = -1; }
    LeaveCriticalSection(&m->cs);
}

const int16_t *music_get(Music *m, int n, int *frames) {
    if (n < 0 || n >= NSONG) return NULL;
    EnterCriticalSection(&m->cs); int st = m->state[n]; const int16_t *p = st == 2 ? m->pcm[n] : NULL; if (p && frames) *frames = m->frames[n]; LeaveCriticalSection(&m->cs);
    if (st == 0) request(m, n);
    return p;
}
int music_loop_points(Music *m, int n, int *start, int *back) {
    if (n < 0 || n >= NSONG) return 0;
    EnterCriticalSection(&m->cs); int ok = m->state[n] == 2 && m->lp_start[n] >= 0; if (ok) { *start = m->lp_start[n]; *back = m->lp_back[n]; } LeaveCriticalSection(&m->cs); return ok;
}
int music_pending(Music *m, int n) {
    if (n < 0 || n >= NSONG) return 0;
    EnterCriticalSection(&m->cs); int st = m->state[n]; LeaveCriticalSection(&m->cs);
    if (st == 2 || st == 3) return 0;
    if (st == 1) return 1;
    char p[600]; return find_path(m, n, p, sizeof p);
}
#else
const int16_t *music_get(Music *m, int n, int *frames) { (void)m; (void)n; (void)frames; return NULL; }
int music_pending(Music *m, int n) { (void)m; (void)n; return 0; }
int music_loop_points(Music *m, int n, int *start, int *back) { (void)m; (void)n; (void)start; (void)back; return 0; }
void music_prefetch(Music *m) { (void)m; }
void music_set_dir(Music *m, const char *folder) { (void)m; (void)folder; }
#endif

int music_loops(int n) { return !(n == 2 || n == 3 || n == 4 || n == 6 || n == 7); }          /* songs 2, 3, 4, 6 and 7 end by themselves (with the driver's fade-out); the rest repeat */
int looper_take_once(Looper *l, const int16_t *a, int frames, int n, int16_t *out) {
    int p = l->pos; for (int i = 0; i < n; i++) { if (p < frames) { out[2 * i] = a[2 * p]; out[2 * i + 1] = a[2 * p + 1]; p++; } else { out[2 * i] = 0; out[2 * i + 1] = 0; } }
    l->pos = p; return p >= frames;
}
void looper_take(Looper *l, const int16_t *a, int frames, int n, int16_t *out) {
    int p = l->pos % frames;
    for (int i = 0; i < n; i++) { out[2 * i] = a[2 * p]; out[2 * i + 1] = a[2 * p + 1]; if (++p >= frames) p = 0; }
    l->pos = p;
}
void looper_take_lp(Looper *l, const int16_t *a, int frames, int start, int back, int n, int16_t *out) {
    int p = l->pos; if (p < 0 || p >= frames) p = 0;
    for (int i = 0; i < n; i++) { if (p >= start) p = back + (p - start) % (start - back); out[2 * i] = a[2 * p]; out[2 * i + 1] = a[2 * p + 1]; p++; }
    l->pos = p;
}
