/* util.c - host utilities (Windows): time, log, paths and files. */
#include "host.h"
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#endif

/* Seconds on a high-resolution monotonic clock. */
double now_s(void) {
#ifdef _WIN32
    static LARGE_INTEGER f;
    LARGE_INTEGER c;
    if (!f.QuadPart)
        QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&c);
    return (double)c.QuadPart / (double)f.QuadPart;
#else
    return 0.0;
#endif
}

#ifdef _WIN32
/* Precise wait: a high-resolution timer (Windows 10 1803+) per thread; if it does not exist Sleep is used (1 ms
 * resolution with timeBeginPeriod, or 15 ms if the system limits it). */
static __thread HANDLE hr_timer = NULL;
static __thread int hr_tried = 0;
/* The high-resolution waitable timer of the calling thread (made on first use; NULL if the system has none). */
static HANDLE get_timer(void) {
    if (!hr_tried) {
        hr_tried = 1;
        hr_timer = CreateWaitableTimerExW(NULL, NULL, 0x00000002 /* CREATE_WAITABLE_TIMER_HIGH_RESOLUTION */,
                                          0x1F0003 /* TIMER_ALL_ACCESS */);
    }
    return hr_timer;
}
#endif
/* 1 if the event (HANDLE) was signaled before ms; ev may be NULL = just sleep */
int wait_event_ms(void *ev, double ms) {
#ifdef _WIN32
    HANDLE t = get_timer();
    if (ms <= 0)
        return ev ? WaitForSingleObject((HANDLE)ev, 0) == WAIT_OBJECT_0 : 0;
    if (t) {
        LARGE_INTEGER due;
        due.QuadPart = -(LONGLONG)(ms * 10000.0);
        SetWaitableTimer(t, &due, 0, NULL, NULL, FALSE);
        if (ev) {
            HANDLE h[2] = {(HANDLE)ev, t};
            return WaitForMultipleObjects(2, h, FALSE, INFINITE) == WAIT_OBJECT_0;
        }
        WaitForSingleObject(t, INFINITE);
        return 0;
    }
    if (ev)
        return WaitForSingleObject((HANDLE)ev, (DWORD)(ms + 0.999)) == WAIT_OBJECT_0;
    Sleep((DWORD)(ms + 0.5));
    return 0;
#else
    (void)ev;
    (void)ms;
    return 0;
#endif
}
/* Sleeps ms milliseconds, precisely (a high-resolution timer, not the 15 ms tick of Sleep). */
void sleep_ms(double ms) {
#ifdef _WIN32
    if (ms <= 0) {
        Sleep(0);
        return;
    }
    wait_event_ms(NULL, ms);
#endif
}

static FILE *logf_ = NULL;
/* to the error.log file only (informational notices that must not go to the console) */
void host_log_file(const char *fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    if (!logf_) {
        char p[600];
        path_join(p, sizeof p, app_dir(), "error.log");
        logf_ = fopen(p, "a");
    }
    if (logf_) {
        fprintf(logf_, "%s\n", buf);
        fflush(logf_);
    }
}
/* Writes a message to the console (if there is one) and to error.log next to the program. */
void host_log(const char *fmt, ...) {
    char buf[1024];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    fprintf(stderr, "[dow] %s\n", buf);
    fflush(stderr);
    if (!logf_) {
        char p[600];
        path_join(p, sizeof p, app_dir(), "error.log");
        logf_ = fopen(p, "a");
    }
    if (logf_) {
        fprintf(logf_, "%s\n", buf);
        fflush(logf_);
    }
}

/* The folder of the executable. */
const char *app_dir(void) {
    static char dir[600];
    if (!dir[0]) {
#ifdef _WIN32
        char p[600];
        DWORD n = GetModuleFileNameA(NULL, p, sizeof p);
        while (n > 0 && p[n - 1] != 0x5C && p[n - 1] != '/')
            n--;
        if (n > 0)
            n--;
        p[n] = 0;
        snprintf(dir, sizeof dir, "%s", p);
#endif
        for (char *c = dir; *c; c++)
            if (*c == 0x5C)
                *c = '/';
    }
    return dir;
}

/* Joins two parts of a path with a slash. */
void path_join(char *out, size_t n, const char *a, const char *b) {
    snprintf(out, n, "%s/%s", a, b);
}

/* Is there a file (not a folder) at that path? */
int file_exists(const char *p) {
#ifdef _WIN32
    DWORD a = GetFileAttributesA(p);
    return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
#else
    return 0;
#endif
}
/* Is there a folder at that path? */
int dir_exists(const char *p) {
#ifdef _WIN32
    DWORD a = GetFileAttributesA(p);
    return a != INVALID_FILE_ATTRIBUTES && (a & FILE_ATTRIBUTE_DIRECTORY);
#else
    return 0;
#endif
}

/* Reads a whole file into a new buffer (NULL on failure); *n = its size. */
uint8_t *read_file(const char *p, size_t *n) {
    FILE *f = fopen(p, "rb");
    if (!f)
        return NULL;
    fseek(f, 0, SEEK_END);
    long l = ftell(f);
    fseek(f, 0, SEEK_SET);
    uint8_t *b = (uint8_t *)malloc((size_t)l + 1);
    if (!b) {
        fclose(f);
        return NULL;
    }
    size_t r = fread(b, 1, (size_t)l, f);
    fclose(f);
    b[r] = 0;
    if (n)
        *n = r;
    return b;
}
/* Writes a whole file; returns 1 on success. */
int write_file(const char *p, const void *d, size_t n) {
    char tmp[700];
    snprintf(tmp, sizeof tmp, "%s.tmp", p);
    FILE *f = fopen(tmp, "wb");
    if (!f)
        return 0;
    size_t w = fwrite(d, 1, n, f);
    fclose(f);
    if (w != n) {
        remove(tmp);
        return 0;
    }
#ifdef _WIN32
    return MoveFileExA(tmp, p, MOVEFILE_REPLACE_EXISTING) != 0;
#else
    return rename(tmp, p) == 0;
#endif
}
