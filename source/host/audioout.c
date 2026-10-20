/*
 * audioout.c - Audio output: Windows device (winmm) with 1024-sample buffers refilled by a callback and a circular
 * buffer fed with chunks of 16-bit STEREO PCM. The emulator runs ahead of what is shown: the same delay is kept in the
 * buffer so sound and image come out in sync; if the buffer drifts from the target it is slightly resampled (up to
 * +-0.4 %) and if it overfills the oldest data is discarded.
 */
#include "audio.h"
#ifdef _WIN32
#define COBJMACROS
#include <windows.h>
#include <mmsystem.h>
#include <initguid.h>
#include <objbase.h>
#include <mmdeviceapi.h>
#include <audioclient.h>
#endif

#define RING_BYTES (1 << 22)
#define NBUF       4
#define CHUNK      1024

struct AudioOut {
    int fs;
    double target_ms;
    int started;
    /* bytes */
    uint8_t *ring;
    size_t head, len;
    int underruns, drops;
    double last_push_t; /* when the emulation thread last gave audio (to explain a gap) */
    long pushed;
#ifdef _WIN32
    CRITICAL_SECTION cs;
    HWAVEOUT hwo;
    WAVEHDR hdr[NBUF];
    int16_t *bufs[NBUF];
    int open;
    volatile int closing;
    /* 0 = winmm (1024-sample buffers), 1 = event-driven shared WASAPI (low latency) */
    int mode;
    HANDLE wa_thread, wa_ready, wa_stopev;
    volatile int wa_stop, wa_ok;
    double wa_ms;
#endif
};

/* ---- the circular buffer ---- */
/* How many bytes of 16-bit stereo audio ms milliseconds are. */
static size_t bytes_for(AudioOut *a, double ms) {
    return (size_t)((int)(a->fs * ms / 1000.0)) * 4;
}
/* Throws away the oldest n bytes of the buffer. */
static void ring_drop(AudioOut *a, size_t n) {
    if (n > a->len)
        n = a->len;
    a->head = (a->head + n) % RING_BYTES;
    a->len -= n;
}
/* Copies the oldest n bytes out of the buffer (without removing them). */
static void ring_read(AudioOut *a, uint8_t *dst, size_t n) {
    size_t h = a->head;
    for (size_t i = 0; i < n; i++)
        dst[i] = a->ring[(h + i) % RING_BYTES];
}

/* Sets the delay the buffer should keep (ms): the same as the image's, so sound and picture come out in sync. */
void audio_set_target(AudioOut *a, double ms) {
    a->target_ms = ms;
}
/* How much audio is waiting in the buffer, in ms. */
double audio_level_ms(AudioOut *a) {
#ifdef _WIN32
    EnterCriticalSection(&a->cs);
#endif
    double v = (double)a->len / 4.0 / a->fs * 1000.0;
#ifdef _WIN32
    LeaveCriticalSection(&a->cs);
#endif
    return v;
}
/* Counters since the start: buffer underruns (gaps), drops (data discarded because the buffer overfilled) and samples
 * played. */
void audio_stats(AudioOut *a, int *u, int *d, long *p) {
    *u = a->underruns;
    *d = a->drops;
    *p = a->pushed;
}

/* Adds `frames` of 16-bit stereo PCM to the buffer (called by the emulation thread once per VBL). If it overfills,
 * the oldest data is discarded. */
void audio_push(AudioOut *a, const int16_t *stereo, int frames) {
    if (!a)
        return;
#ifdef _WIN32
    EnterCriticalSection(&a->cs);
#endif
    size_t n = (size_t)frames * 4;
    const uint8_t *s = (const uint8_t *)stereo;
    if (a->len + n > RING_BYTES)
        ring_drop(a, a->len + n - RING_BYTES);
    size_t tail = (a->head + a->len) % RING_BYTES;
    for (size_t i = 0; i < n; i++)
        a->ring[(tail + i) % RING_BYTES] = s[i];
    a->len += n;
    a->pushed += (long)n;
    a->last_push_t = now_s();
    size_t target = bytes_for(a, a->target_ms);
    /* too full: trims the oldest data down to the target */
    if (a->len > target * 3) {
        size_t over = (a->len - target) & ~(size_t)3;
        if (a->drops < 40)
            host_log_file("audio: buffer trimmed when pushing (it held %.0f ms, target %.0f ms: %.0f ms discarded)",
                          (double)a->len / 4.0 / a->fs * 1000.0, a->target_ms, (double)over / 4.0 / a->fs * 1000.0);
        ring_drop(a, over);
        a->drops++;
    }
#ifdef _WIN32
    LeaveCriticalSection(&a->cs);
#endif
}

/* Fills the device's buffer from the circular buffer. The delay is steered towards the target: if the buffer is too
 * full or too empty the audio is resampled very slightly (up to 0.4 %) instead of skipping; if it runs dry the last
 * sample is held and a gap is counted. */
static void fill(AudioOut *a, int16_t *dst, int frames) {
    size_t need = (size_t)frames * 4;
    uint8_t *out = (uint8_t *)dst;
#ifdef _WIN32
    EnterCriticalSection(&a->cs);
#endif
    size_t target = bytes_for(a, a->target_ms);
    if (!a->started) {
        if (a->len >= target)
            a->started = 1;
        else {
            memset(out, 0, need);
            goto done;
        }
    }
    if (a->len < need) {
        size_t n = a->len;
        ring_read(a, out, n);
        memset(out + n, 0, need - n);
        ring_drop(a, n);
        a->started = 0;
        a->underruns++;
        if (a->underruns <= 40)
            host_log_file("audio: gap no. %d (the buffer ran empty: %d samples missing; the emulation thread last gave "
                          "audio %.0f ms ago)",
                          a->underruns, (int)((need - n) / 4), (now_s() - a->last_push_t) * 1000.0);
        goto done;
    }
    {
        double err_ms = ((double)a->len - (double)target) / 4.0 / a->fs * 1000.0, ratio = 1.0;
        if (err_ms > 150.0) {
            size_t cut = (a->len - target - bytes_for(a, 60.0)) & ~(size_t)3;
            if ((long)cut > 0) {
                if (a->drops < 40)
                    host_log_file(
                        "audio: buffer trimmed while playing (it held %.0f ms, target %.0f ms: %.0f ms discarded)",
                        (double)a->len / 4.0 / a->fs * 1000.0, a->target_ms, (double)cut / 4.0 / a->fs * 1000.0);
                ring_drop(a, cut);
                a->drops++;
                err_ms = 60.0;
            }
        }
        if (fabs(err_ms) > 30.0) {
            double e = err_ms - (err_ms > 0 ? 30.0 : -30.0);
            ratio = 1.0 + fmax(-0.004, fmin(0.004, e / 20000.0));
        }
        size_t take = ratio != 1.0 ? (size_t)((long)(need / 4 * ratio)) * 4 : need;
        if (take > (a->len & ~(size_t)3))
            take = a->len & ~(size_t)3;
        if (take == need) {
            ring_read(a, out, need);
            ring_drop(a, need);
        } else {
            static uint8_t tmp[CHUNK * 8 * 4];
            if (take > sizeof tmp)
                take = sizeof tmp;
            ring_read(a, tmp, take);
            ring_drop(a, take);
            const int16_t *x = (const int16_t *)tmp;
            int nsrc = (int)(take / 4), nout = frames;
            double stp = (double)(nsrc - 1) / (double)(nout > 1 ? nout - 1 : 1);
            for (int i = 0; i < nout; i++) {
                double pos = i * stp;
                int i0 = (int)pos;
                double fr = pos - i0;
                int i1 = i0 + 1 < nsrc ? i0 + 1 : nsrc - 1;
                for (int c = 0; c < 2; c++)
                    dst[2 * i + c] = (int16_t)(x[2 * i0 + c] * (1.0 - fr) + x[2 * i1 + c] * fr);
            }
        }
    }
done:
#ifdef _WIN32
    LeaveCriticalSection(&a->cs);
#endif
    return;
}

#ifdef _WIN32
/* ---- winmm output (buffers refilled by a callback) ---- */
/* Called by winmm when a buffer has been played: refills it and queues it again. */
static void CALLBACK wave_cb(HWAVEOUT hwo, UINT msg, DWORD_PTR inst, DWORD_PTR p1, DWORD_PTR p2) {
    (void)hwo;
    (void)p2;
    if (msg != WOM_DONE)
        return;
    AudioOut *a = (AudioOut *)inst;
    if (a->closing)
        return;
    WAVEHDR *h = (WAVEHDR *)p1;
    fill(a, (int16_t *)h->lpData, CHUNK);
    waveOutWrite(a->hwo, h, sizeof(WAVEHDR));
}
#endif

#ifdef _WIN32
/* Opens the winmm device (44.1 kHz, 16 bits, stereo) with 4 buffers of 1024 samples. Returns 0 on failure. */
static int wm_open(AudioOut *a) {
    WAVEFORMATEX f;
    memset(&f, 0, sizeof f);
    f.wFormatTag = WAVE_FORMAT_PCM;
    f.nChannels = 2;
    f.nSamplesPerSec = (DWORD)a->fs;
    f.wBitsPerSample = 16;
    f.nBlockAlign = 4;
    f.nAvgBytesPerSec = (DWORD)a->fs * 4;
    a->closing = 0;
    if (waveOutOpen(&a->hwo, WAVE_MAPPER, &f, (DWORD_PTR)wave_cb, (DWORD_PTR)a, CALLBACK_FUNCTION) != MMSYSERR_NOERROR)
        return 0;
    for (int i = 0; i < NBUF; i++) {
        if (!a->bufs[i])
            a->bufs[i] = (int16_t *)calloc(CHUNK * 2, sizeof(int16_t));
        memset(a->bufs[i], 0, CHUNK * 4);
        memset(&a->hdr[i], 0, sizeof(WAVEHDR));
        a->hdr[i].lpData = (LPSTR)a->bufs[i];
        a->hdr[i].dwBufferLength = CHUNK * 4;
        waveOutPrepareHeader(a->hwo, &a->hdr[i], sizeof(WAVEHDR));
        waveOutWrite(a->hwo, &a->hdr[i], sizeof(WAVEHDR));
    }
    a->open = 1;
    return 1;
}
/* Closes the winmm device. */
static void wm_close(AudioOut *a) {
    if (!a->open)
        return;
    a->closing = 1;
    waveOutReset(a->hwo);
    for (int i = 0; i < NBUF; i++)
        waveOutUnprepareHeader(a->hwo, &a->hdr[i], sizeof(WAVEHDR));
    waveOutClose(a->hwo);
    a->open = 0;
}

/* ---- event-driven shared WASAPI: the Windows engine asks for ~10 ms chunks (versus the ~93 ms of four winmm buffers).
 * Automatic format conversion (44.1 kHz, 16 bits, stereo). */
static DWORD WINAPI wasapi_thread(LPVOID arg) {
    AudioOut *a = (AudioOut *)arg;
    int com = SUCCEEDED(CoInitializeEx(NULL, COINIT_MULTITHREADED));
    IMMDeviceEnumerator *en = NULL;
    IMMDevice *dev = NULL;
    IAudioClient *ac = NULL;
    IAudioRenderClient *rc = NULL;
    UINT32 bufn = 0;
    HANDLE ev = NULL;
    int ok = 0;
    typedef HANDLE(WINAPI * PFN_AvSet)(LPCWSTR, LPDWORD);
    HMODULE av = LoadLibraryA("avrt.dll");
    PFN_AvSet avset = av ? (PFN_AvSet)GetProcAddress(av, "AvSetMmThreadCharacteristicsW") : NULL;
    DWORD task = 0;
    if (SUCCEEDED(
            CoCreateInstance(&CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, &IID_IMMDeviceEnumerator, (void **)&en)) &&
        SUCCEEDED(IMMDeviceEnumerator_GetDefaultAudioEndpoint(en, eRender, eConsole, &dev)) &&
        SUCCEEDED(IMMDevice_Activate(dev, &IID_IAudioClient, CLSCTX_ALL, NULL, (void **)&ac))) {
        WAVEFORMATEX f;
        memset(&f, 0, sizeof f);
        f.wFormatTag = WAVE_FORMAT_PCM;
        f.nChannels = 2;
        f.nSamplesPerSec = (DWORD)a->fs;
        f.wBitsPerSample = 16;
        f.nBlockAlign = 4;
        f.nAvgBytesPerSec = (DWORD)a->fs * 4;
        ev = CreateEventA(NULL, FALSE, FALSE, NULL);
        if (SUCCEEDED(IAudioClient_Initialize(ac, AUDCLNT_SHAREMODE_SHARED,
                                              AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM |
                                                  AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY,
                                              0, 0, &f, NULL)) &&
            SUCCEEDED(IAudioClient_SetEventHandle(ac, ev)) && SUCCEEDED(IAudioClient_GetBufferSize(ac, &bufn)) &&
            SUCCEEDED(IAudioClient_GetService(ac, &IID_IAudioRenderClient, (void **)&rc))) {
            BYTE *p;
            if (SUCCEEDED(IAudioRenderClient_GetBuffer(rc, bufn, &p))) {
                IAudioRenderClient_ReleaseBuffer(rc, bufn, AUDCLNT_BUFFERFLAGS_SILENT);
            }
            /* device queue + Windows mixer delay (approx.) */
            a->wa_ms = (double)bufn * 1000.0 / a->fs + 7.0;
            ok = SUCCEEDED(IAudioClient_Start(ac));
        }
    }
    a->wa_ok = ok;
    SetEvent(a->wa_ready);
    if (ok) {
        if (avset)
            avset(L"Pro Audio", &task);
        while (!a->wa_stop) {
            if (WaitForSingleObject(ev, 200) != WAIT_OBJECT_0)
                continue;
            UINT32 pad = 0;
            if (FAILED(IAudioClient_GetCurrentPadding(ac, &pad)) || pad >= bufn)
                continue;
            UINT32 n = bufn - pad;
            BYTE *p = NULL;
            if (SUCCEEDED(IAudioRenderClient_GetBuffer(rc, n, &p))) {
                fill(a, (int16_t *)p, (int)n);
                IAudioRenderClient_ReleaseBuffer(rc, n, 0);
            }
        }
        IAudioClient_Stop(ac);
    }
    if (rc)
        IAudioRenderClient_Release(rc);
    if (ac)
        IAudioClient_Release(ac);
    if (dev)
        IMMDevice_Release(dev);
    if (en)
        IMMDeviceEnumerator_Release(en);
    if (ev)
        CloseHandle(ev);
    if (com)
        CoUninitialize();
    return 0;
}
/* Starts the WASAPI thread; returns 0 if the shared-mode device could not be initialised. */
static int wa_open(AudioOut *a) {
    a->wa_stop = 0;
    a->wa_ok = 0;
    a->wa_ready = CreateEventA(NULL, TRUE, FALSE, NULL);
    a->wa_thread = CreateThread(NULL, 0, wasapi_thread, a, 0, NULL);
    WaitForSingleObject(a->wa_ready, 3000);
    CloseHandle(a->wa_ready);
    a->wa_ready = NULL;
    if (!a->wa_ok) {
        a->wa_stop = 1;
        WaitForSingleObject(a->wa_thread, 2000);
        CloseHandle(a->wa_thread);
        a->wa_thread = NULL;
        return 0;
    }
    return 1;
}
/* Stops the WASAPI thread. */
static void wa_close(AudioOut *a) {
    if (!a->wa_thread)
        return;
    a->wa_stop = 1;
    WaitForSingleObject(a->wa_thread, 2000);
    CloseHandle(a->wa_thread);
    a->wa_thread = NULL;
}
#endif

/* ---- choosing the output ---- */
/* Switches between the winmm output and the low-latency WASAPI one. Returns 1 if the requested mode is active. */
int audio_set_lowlat(AudioOut *a, int on) {
#ifdef _WIN32
    if (!a)
        return 0;
    if (on && a->mode == 0) {
        wm_close(a);
        EnterCriticalSection(&a->cs);
        a->started = 0;
        LeaveCriticalSection(&a->cs);
        if (wa_open(a)) {
            a->mode = 1;
            return 1;
        }
        host_log("Low-latency audio: could not open WASAPI; normal audio stays");
        wm_open(a);
        return 0;
    }
    if (!on && a->mode == 1) {
        wa_close(a);
        a->mode = 0;
        EnterCriticalSection(&a->cs);
        a->started = 0;
        LeaveCriticalSection(&a->cs);
        wm_open(a);
    }
    return a->mode == 1 || !on;
#else
    (void)a;
    (void)on;
    return 0;
#endif
}
/* The latency of the device itself (what is queued in it), in ms: about 80 ms with winmm, one chunk with WASAPI. */
double audio_device_ms(AudioOut *a) {
#ifdef _WIN32
    /* winmm: ~3.5 of its 4 buffers of 1024 samples (23 ms) always queued */
    if (!a)
        return 0;
    return a->mode == 1 ? a->wa_ms : 82.0;
#else
    (void)a;
    return 0;
#endif
}

/* Opens the audio output at fs Hz with a buffer target of target_ms. NULL if there is no device. */
AudioOut *audio_open(int fs, double target_ms) {
#ifdef _WIN32
    AudioOut *a = (AudioOut *)calloc(1, sizeof(AudioOut));
    a->fs = fs;
    a->target_ms = target_ms;
    a->ring = (uint8_t *)malloc(RING_BYTES);
    InitializeCriticalSection(&a->cs);
    if (!wm_open(a)) {
        host_log("Sound disabled: could not open the audio device");
        free(a->ring);
        free(a);
        return NULL;
    }
    return a;
#else
    (void)fs;
    (void)target_ms;
    return NULL;
#endif
}

/* Closes the output and frees it. */
void audio_close(AudioOut *a) {
#ifdef _WIN32
    if (!a)
        return;
    if (a->mode == 1)
        wa_close(a);
    else
        wm_close(a);
#endif
}
