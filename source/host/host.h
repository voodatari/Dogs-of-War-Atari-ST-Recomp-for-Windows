/*
 * host.h - Native host of Dogs of War: common declarations.
 *
 * The host surrounds the game code (generated/*.c, runtime/machine.c) with everything the original machine did around
 * it: the mini-TOS (tos.c), scene capture (scene.c), the presenter (present.c / render.c), audio (audioout.c, ym.c,
 * mixer.c, mp3.c), input (input.c) and the window (video.c). The application layer that owns the window and the options
 * is described in app.h.
 */
#ifndef HOST_H
#define HOST_H
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "cpu.h"

#define STUB_BASE      0xE00800u
#define STUB_VEC(n)    (STUB_BASE + 4u * (n))
#define SCREEN_DEFAULT 0x3F8000u

/* ---- utilities ---- */
double now_s(void); /* seconds (high-resolution monotonic clock) */
void sleep_ms(double ms);
/* precise wait for a Windows event (HANDLE) or for a time; 1 if the event was signaled */
int wait_event_ms(void *ev, double ms);
void host_log(const char *fmt, ...);
void host_log_file(const char *fmt, ...); /* to error.log only */
const char *app_dir(void);                /* folder of the executable */
void path_join(char *out, size_t n, const char *a, const char *b);
int file_exists(const char *p);
int dir_exists(const char *p);
uint8_t *read_file(const char *p, size_t *n); /* malloc; NULL on failure */
int write_file(const char *p, const void *d, size_t n);

/* read/write of the Atari memory from the host (big-endian) */
static inline uint32_t mr8(uint32_t a) {
    return a < RAMSZ ? ram[a] : 0;
}
static inline uint32_t mr16(uint32_t a) {
    return a + 1 < RAMSZ ? ((uint32_t)ram[a] << 8) | ram[a + 1] : 0;
}
static inline uint32_t mr32(uint32_t a) {
    return a + 3 < RAMSZ
               ? ((uint32_t)ram[a] << 24) | ((uint32_t)ram[a + 1] << 16) | ((uint32_t)ram[a + 2] << 8) | ram[a + 3]
               : 0;
}
static inline void mw8(uint32_t a, uint32_t v) {
    if (a < RAMSZ)
        ram[a] = (uint8_t)v;
}
static inline void mw16(uint32_t a, uint32_t v) {
    if (a + 1 < RAMSZ) {
        ram[a] = (uint8_t)(v >> 8);
        ram[a + 1] = (uint8_t)v;
    }
}
static inline void mw32(uint32_t a, uint32_t v) {
    if (a + 3 < RAMSZ) {
        ram[a] = (uint8_t)(v >> 24);
        ram[a + 1] = (uint8_t)(v >> 16);
        ram[a + 2] = (uint8_t)(v >> 8);
        ram[a + 3] = (uint8_t)v;
    }
}

/* png.c */
uint8_t *png_read_rgba(const char *path, int *w, int *h);    /* RGBA (malloc) o NULL */
uint8_t *png_read_indexed(const char *path, int *w, int *h); /* indices of a palette PNG */
/* channels 3 or 4 */
int png_write(const char *path, const uint8_t *px, int w, int h, int channels);
/* 8-bit palette PNG */
int png_write_indexed(const char *path, const uint8_t *idx, int w, int h, const uint8_t *plte768);

/* ---- machine state (snapshots) ---- */
typedef struct {
    int line;
    uint8_t pal[32];
} Zone;
#define MAXZONES 8
typedef struct State {
    uint8_t *ram; /* 4 MB (malloc) */
    uint8_t io[0x8000];
    uint32_t pc, vbl, sr, usp, isp, a7, heap, scrbase, d[8], a[7];
    int32_t song; /* current song (-1 = none) */
    int nrx;
    uint8_t rx[256];
    int nzones;
    Zone zones[MAXZONES];
    uint8_t ym[16];
    int ymsel;
    char curdir[260];
} State;
State *state_new(void);
void state_free(State *s);
State *state_clone(const State *s);
int state_read_file(State *s, const char *path); /* DOWS format */
int state_write_file(const State *s, const char *path);

/* ---- mini-TOS (tos.c) ---- */
typedef struct Tos {
    uint32_t vbl;
    int nzones;
    Zone zones[MAXZONES];
    int done;
    uint32_t heap, scrbase, kbd, dta;
    char curdir[260], folder[260];
    double vbl_rate, tc_acc;
    int song_now; /* -1 = none */
    /* last VBL in which the game waited at $F91E, $3972, $FDF0 */
    uint32_t wait_hit[3];
    int timepolls;
    /* keys for Cconin (boot only) */
    uint32_t keys[8];
    int nkeys;
    /* instant of every call of the timer C batch */
    double ph_times[512];
    int nph;
    /* the next write to the YM comes from the sample routine */
    int ym_tag;
} Tos;
extern Tos tos;
/* dump.c: graphics dump (assets/original) made by playing the game with scripted input in a thread with a big stack */
int dump_begin(const char *game_dir, const char *out_dir); /* 1 if the thread started */
int dump_finished(int *ok);
void dump_status(char *out, size_t n);
void tos_init(const char *game_folder);
/* vectors, system variables, cookies: the machine as BootCore._init_machine leaves it */
void tos_machine_init(void);
/* the machine as the original loader leaves it when it jumps into the game (boot.c), built from the original files;
 * NULL if they are missing */
State *boot_state_make(const char *game_dir);
void tos_load_state(const State *s);
void tos_save_state(State *s);
/* seconds the loader text stays on screen at start-up (the game is held once); 0 = no wait */
extern double g_load_seconds;
/* runs until the game yields (VBL wait...); 0 if the game ended */
int tos_run_slice(int32_t budget);
/* one VBL period: clocks, VBL, palette zones (timer B), timer C, timer A, keyboard */
void tos_vbl(void);
/* input log: J port state, K scancode, F patch on, P address value (16-bit write) */
void tos_ilog(char t, int a, int b);
void tos_ilog_trim(uint32_t vbl);
void tos_ilog_clear(void);
int tos_ilog_write(const char *path, uint32_t from);
void tos_joystick(int port, int state); /* joystick packet to the ACIA */
void tos_key(int scancode);             /* keyboard byte to the ACIA */
void tos_screen_idx(uint8_t *idx /*200x320*/, Zone *zones, int *nzones);
void tos_pal16(uint8_t *pal32);
/* calls a game routine from a safe point (like BootCore.call_sub) */
int tos_call_sub(uint32_t addr, uint32_t d0);
void tos_set_patch(int slot, int on);
extern uint8_t ym_regs_copy[16];
int tos_mfp_enabled(int ie, int im, int bit);
/* delivers the interrupt and runs its routine; 0 if it could not be delivered */
int tos_run_isr(int vecn, int level);
/* delivers the pending ACIA bytes (as in tos_vbl) */
void tos_deliver_keys(void);

/* YM2149: write events collected since the last call */
typedef struct {
    double ph;
    uint8_t reg, val;
} YmEvent;
int ym_events_take(YmEvent *out, int max); /* sorted by instant (stable) */
void ym_events_clear(void);
void ym_set_regs(const uint8_t *r, int sel);
void ym_get_regs(uint8_t *r, int *sel);

/* ---- ganchos (hooks.c) ---- */
/* call from the game code (HOOK) with the registers already dumped into g_cpu */
void host_hook(uint32_t addr);
void hook_register(uint32_t addr, void (*fn)(uint32_t)); /* registers and enables a hook */
void hooks_enable(uint32_t addr, int on);
void sync_regs_out(void);
void sync_regs_in(void);

#endif
