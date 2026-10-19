/*
 * machine.c - The Atari STE "machine" around the game code: memory, I/O (video, MFP, YM2149, keyboard), interrupts,
 *             and the context switch between the host and the game.
 *
 * The game runs in its own FIBER (cooperative thread): when it needs to yield control (VBL wait, capture hook,
 * instruction the host must resolve...) it calls yield_to_host(), which switches to the host's fiber. This way every game routine is
 * a plain C function, with its call stack, without chopping up the code.
 */
#include "cpu.h"
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

static uint8_t g_ram[RAMSZ];
static uint8_t g_rom[ROMSZ];
static uint8_t g_io[0x8000];
uint8_t *ram = g_ram, *rom = g_rom, *io = g_io;
CPU g_cpu;

uint32_t d0, d1, d2, d3, d4, d5, d6, d7;
uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
uint8_t  fx, fn, fz, fv, fc;
uint32_t sr_sys, other_sp, ret_addr, resume_pc;

/* ------------------------------------------------------------------------------------------------ YM2149 and keyboard */
typedef struct { double ph; uint8_t reg, val; } YmEv;
#define YM_MAX 262144
static YmEv ym_ev[YM_MAX];
static int ym_n = 0;
static uint8_t ym_regs[16];
static int ym_sel = 0;
int g_ym_tag = 0;                                          /* != 0: the next write to YM registers 7-9 is made by the timer A sample routine ($E6xx): it is tagged (register + 16) */
#define RXQ 4096
static uint8_t rxq[RXQ];
static int rx_h = 0, rx_n = 0;
#define TXQ 256
static uint8_t txq[TXQ];                                   /* bytes the game writes to the keyboard (IKBD commands) for the host to handle */
static int tx_n = 0;

API CPU *get_cpu(void) { return &g_cpu; }
API uint8_t *get_ram(void) { return g_ram; }
API uint8_t *get_rom(void) { return g_rom; }
API uint8_t *get_io(void) { return g_io; }
API uint8_t *get_ym_regs(void) { return ym_regs; }
API int  get_ym_sel(void) { return ym_sel; }
API void set_ym_sel(int v) { ym_sel = v & 15; }
API void set_host(void *hook, void *ikbd) { (void)hook; (void)ikbd; }       /* compatibility: here the hooks and the keyboard are handled by RC_HOOK / ikbd_take() */
API int ym_ev_size(void) { return (int)sizeof(YmEv); }
API void ym_clear(void) { ym_n = 0; }
API int ym_take(YmEv *out, int max) {
    int n = ym_n < max ? ym_n : max;
    memcpy(out, ym_ev, (size_t)n * sizeof(YmEv));
    if (n < ym_n) memmove(ym_ev, ym_ev + n, (size_t)(ym_n - n) * sizeof(YmEv));
    ym_n -= n; return n;
}
API void acia_push(uint32_t b) { if (rx_n < RXQ) { rxq[(rx_h + rx_n) % RXQ] = (uint8_t)b; rx_n++; } }
API void acia_clear(void) { rx_h = rx_n = 0; }
API int  acia_len(void) { return rx_n; }
API int  acia_peek(int i) { return (i < rx_n) ? rxq[(rx_h + i) % RXQ] : -1; }
API int  ikbd_take(uint8_t *out, int max) { int n = tx_n < max ? tx_n : max; memcpy(out, txq, (size_t)n); tx_n = 0; return n; }
static int acia_pop(void) { if (!rx_n) return 0; int v = rxq[rx_h]; rx_h = (rx_h + 1) % RXQ; rx_n--; return v; }

/* ------------------------------------------------------------------------------------------------ E/S */
uint32_t io_read(uint32_t a, int size) {
    if (a >= ROMBASE && a < ROMBASE + ROMSZ) {
        uint32_t o = a - ROMBASE, v = 0;
        for (int i = 0; i < size && o + i < ROMSZ; i++) v = (v << 8) | g_rom[o + i];
        return v;
    }
    if (a < 0xFF8000u) return 0;
    uint32_t off = a & 0x7FFF, v = 0;
    if (off == 0x7C00) g_io[0x7C00] = 0x02 | (rx_n ? 0x81 : 0);                /* state of the keyboard ACIA */
    else if (off == 0x7C02) g_io[0x7C02] = (uint8_t)acia_pop();                 /* dato recibido */
    else if (off == 0x7C04) g_io[0x7C04] = 0x02;
    else if (off == 0x0800) g_io[0x0800] = ym_regs[ym_sel & 15];                /* read of the chosen YM register */
    for (int i = 0; i < size && off + i < 0x8000; i++) v = (v << 8) | g_io[off + i];
    return v;
}

void io_write(uint32_t a, int size, uint32_t value) {
    if (a < 0xFF8000u) return;
    uint32_t off = a & 0x7FFF;
    for (int i = 0; i < size && off + i < 0x8000; i++) g_io[off + i] = (uint8_t)(value >> (8 * (size - 1 - i)));
    if (off == 0x7C02) { if (tx_n < TXQ) txq[tx_n++] = (uint8_t)value; }
    else if (off == 0x0800 || off == 0x0802) {                                  /* YM2149: $FF8800 chooses the register, $FF8802 writes the data */
        uint32_t v = (size >= 2 ? (value >> 8) : value) & 0xFF;
        if (off == 0x0800) ym_sel = (v < 16) ? (int)(v & 15) : 15;
        else {
            int r = ym_sel & 15; ym_regs[r] = (uint8_t)v;
            int tag = (g_ym_tag && r >= 7 && r <= 9) ? 16 : 0; g_ym_tag = 0;
            if (r < 14 && ym_n < YM_MAX) { ym_ev[ym_n].ph = g_cpu.ph; ym_ev[ym_n].reg = (uint8_t)(r | tag); ym_ev[ym_n].val = (uint8_t)v; ym_n++; }
        }
    }
}

/* ------------------------------------------------------------------------------------------------ SR and shifts */
uint32_t get_sr(void) { return sr_sys | get_ccr(); }
void set_sr(uint32_t v) {
    v &= 0xFFFF;
    if ((v ^ sr_sys) & 0x2000) { uint32_t t = a7; a7 = other_sp; other_sp = t; }      /* supervisor <-> user: switches stacks */
    sr_sys = v & 0xFF00; set_ccr(v);
}

uint32_t shift_op(int type, int left, uint32_t v, uint32_t cnt, int bits) {
    uint64_t m = (bits == 32) ? 0xFFFFFFFFull : ((1ull << bits) - 1), msb = 1ull << (bits - 1), x = v & m, r;
    uint32_t c = 0, vv = 0;
    if (cnt == 0) { r = x; c = (type == 2) ? fx : 0; }                           /* ROX: C = X; resto: C = 0; X no cambia */
    else if (type == 0) {                                                        /* AS: arithmetic shift */
        if (left) {
            if (cnt >= (uint32_t)bits) { r = 0; c = (cnt == (uint32_t)bits) ? (x & 1) : 0; vv = (x != 0); }
            else { r = (x << cnt) & m; c = (x >> (bits - cnt)) & 1; uint64_t top = x >> (bits - 1 - cnt), all = (1ull << (cnt + 1)) - 1; vv = !(top == 0 || top == all); }
        } else {
            int64_t sx = (x & msb) ? (int64_t)(x | ~m) : (int64_t)x;
            if (cnt >= (uint32_t)bits) { r = (x & msb) ? m : 0; c = (x & msb) ? 1 : 0; }
            else { r = (uint64_t)(sx >> cnt) & m; c = (x >> (cnt - 1)) & 1; }
        }
        fx = c;
    } else if (type == 1) {                                                      /* LS: logical shift */
        if (left) { if (cnt > (uint32_t)bits) { r = 0; c = 0; } else { r = (cnt == (uint32_t)bits) ? 0 : ((x << cnt) & m); c = (x >> (bits - cnt)) & 1; } }
        else { if (cnt > (uint32_t)bits) { r = 0; c = 0; } else { r = (cnt == (uint32_t)bits) ? 0 : (x >> cnt); c = (x >> (cnt - 1)) & 1; } }
        fx = c;
    } else if (type == 3) {                                                      /* RO: rotation */
        uint32_t k = cnt % bits;
        if (left) { r = k ? (((x << k) | (x >> (bits - k))) & m) : x; c = r & 1; }
        else { r = k ? (((x >> k) | (x << (bits - k))) & m) : x; c = (r & msb) ? 1 : 0; }
    } else {                                                                     /* ROX: rotation by count+1 positions going through X */
        uint64_t w = x | ((uint64_t)fx << bits); uint32_t n = bits + 1, k = cnt % n; uint64_t M = (1ull << n) - 1;
        if (left) w = k ? (((w << k) | (w >> (n - k))) & M) : w; else w = k ? (((w >> k) | (w << (n - k))) & M) : w;
        r = w & m; c = (uint32_t)((w >> bits) & 1); fx = c;
    }
    fn = (r & msb) ? 1 : 0; fz = (r == 0); fv = vv; fc = c;
    return (uint32_t)r;
}

/* ------------------------------------------------------------------------------------------------ registers <-> host CPU */
void sync_regs_out(void);
void sync_regs_in(void);
static void store_regs(void) {
    CPU *c = &g_cpu;
    c->d[0] = d0; c->d[1] = d1; c->d[2] = d2; c->d[3] = d3; c->d[4] = d4; c->d[5] = d5; c->d[6] = d6; c->d[7] = d7;
    c->a[0] = a0; c->a[1] = a1; c->a[2] = a2; c->a[3] = a3; c->a[4] = a4; c->a[5] = a5; c->a[6] = a6; c->a[7] = a7;
    c->X = fx; c->N = fn; c->Z = fz; c->V = fv; c->C = fc; c->srh = sr_sys; c->osp = other_sp;
}
static void load_regs(void) {
    CPU *c = &g_cpu;
    d0 = c->d[0]; d1 = c->d[1]; d2 = c->d[2]; d3 = c->d[3]; d4 = c->d[4]; d5 = c->d[5]; d6 = c->d[6]; d7 = c->d[7];
    a0 = c->a[0]; a1 = c->a[1]; a2 = c->a[2]; a3 = c->a[3]; a4 = c->a[4]; a5 = c->a[5]; a6 = c->a[6]; a7 = c->a[7];
    fx = c->X; fn = c->N; fz = c->Z; fv = c->V; fc = c->C; sr_sys = c->srh; other_sp = c->osp;
}

API void regs_store(void) { store_regs(); }
API void regs_load(void) { load_regs(); }
void sync_regs_out(void) { store_regs(); }
void sync_regs_in(void) { load_regs(); }

/* ------------------------------------------------------------------------------------------------ game fiber */
#ifdef _WIN32
static LPVOID host_fiber = 0, game_fiber = 0;
#endif
static int in_game = 0;                    /* 1 while control is in the game fiber */

void yield_to_host(int rc) {
    if (!in_game) return;                  /* in an interrupt (it runs in the host's context) nothing is yielded */
    g_cpu.rc = rc; store_regs();
#ifdef _WIN32
    SwitchToFiber(host_fiber);
#endif
    load_regs();                           /* the host may have changed registers or memory */
}

#ifdef TRACE_DYN
#include <stdio.h>
void call_dynamic(uint32_t pc) { fprintf(stderr, "DYN %06X a7=%06X ret=%06X\n", pc & 0xFFFFFFu, a7, rd32(a7)); game_dispatch(pc & 0xFFFFFFu); }
#else
uint32_t dyn_hist[32][2]; int dyn_n = 0;                      /* last 32 dynamic calls/jumps (target, a7): for the report when the game goes astray */
void call_dynamic(uint32_t pc) { dyn_hist[dyn_n & 31][0] = pc & 0xFFFFFFu; dyn_hist[dyn_n & 31][1] = a7; dyn_n++; game_dispatch(pc & 0xFFFFFFu); }
#endif

/* Interrupts and RTE: see cpu_irq. The RTE of a batch routine (timers) acts as a 4-byte RTS. */
void rte_return(void) {
    CPU *c = &g_cpu;
    if (c->batch && a7 == c->batch_sp) { c->batch = 0; ret_addr = rd32(a7); a7 += 4; return; }
    uint32_t sr = rd16(a7); ret_addr = rd32(a7 + 2); a7 += 6; set_sr(sr);
}

/* When a state is loaded the game may be stopped in the middle of several nested calls: the call stack is rebuilt from the 68000
 * stack ("resume ladder"): the routine that contains the PC is resumed; when it does RTS, its return address indicates the routine
 * that follows, and so on until the game enters its main loop (which does not end). */
static void game_main(void *unused) {
    (void)unused;
    uint32_t pc = g_cpu.pc & 0xFFFFFF;
    for (;;) {
        resume_pc = 0;
        game_resume(pc);                   /* returns when the routine executes RTS */
        pc = ret_addr & 0xFFFFFF;
    }
}
#ifdef _WIN32
static VOID CALLBACK game_entry(LPVOID p) { game_main(p); }
#endif

API void core_reset(void) {                /* forgets the call stack (e.g. when loading a state) */
#ifdef _WIN32
    if (game_fiber && !in_game) { DeleteFiber(game_fiber); }
    game_fiber = 0;
#endif
    in_game = 0; resume_pc = 0;
}

#ifdef _WIN32
API void core_forget_host(void) { host_fiber = 0; }          /* the thread that was running it (e.g. the first boot's) no longer exists: the next core_run converts its thread again */
#else
API void core_forget_host(void) {}
#endif
API int core_run(CPU *c, int32_t budget) {
    (void)c;
    g_cpu.budget = budget; g_cpu.stop = 0; g_cpu.rc = 0;
#ifdef _WIN32
    if (!host_fiber) host_fiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(0);
    load_regs();
    if (!game_fiber) game_fiber = CreateFiber(16u << 20, game_entry, 0);
    in_game = 1; SwitchToFiber(game_fiber); in_game = 0;
    return g_cpu.rc;
#else
    return RC_MISS;
#endif
}

/* ------------------------------------------------------------------------------------------------ interrupts */
API int cpu_irq(CPU *c, uint32_t handler, int level, int force) {
    uint32_t sr = c->srh | (c->X << 4) | (c->N << 3) | (c->Z << 2) | (c->V << 1) | c->C;
    if (!force && ((sr >> 8) & 7) >= (uint32_t)level) return 0;
    uint32_t nsr = ((sr | 0x2000) & ~0x8000u & ~0x0700u) | ((uint32_t)level << 8);
    if (!(sr & 0x2000)) { uint32_t t = c->a[7]; c->a[7] = c->osp; c->osp = t; }
    c->srh = nsr & 0xFF00;
    uint32_t sp = c->a[7] - 6;
    wr16(sp, sr); wr32(sp + 2, c->pc); c->a[7] = sp;
    c->pc = handler;
    return 1;
}

/* Runs the interrupt routine cpu_irq has just armed (the final RTE pops the frame and the PC goes back to the game's). */
API int cpu_run_isr(CPU *c, uint32_t handler, int level, int force, int n, const double *phases, int32_t budget) {
    (void)budget;
    for (int i = 0; i < n; i++) {
        c->ph = phases ? phases[i] : (n > 1 ? (double)i / n : 0.0);
        if (!cpu_irq(c, handler, level, force)) return -1;
        load_regs(); resume_pc = 0; c->batch = 0; c->budget = 0x7FFFFFFF;
        game_dispatch(handler);                 /* ends with RTE: pops the frame and restores the SR */
        store_regs(); c->pc = ret_addr;
    }
    c->ph = 0.0;
    return RC_STOP;
}

/* n runs of an interrupt routine as a SUBROUTINE (without changing the SR; the final RTE acts as RTS): timers C and A. */
API int cpu_run_batch(CPU *c, uint32_t handler, int n, const double *phases, int32_t budget) {
    (void)budget;
    uint32_t sr0 = c->srh | (c->X << 4) | (c->N << 3) | (c->Z << 2) | (c->V << 1) | c->C, pc0 = c->pc;
    load_regs();
    for (int i = 0; i < n; i++) {
        g_cpu.ph = phases ? phases[i] : (n > 1 ? (double)i / n : 0.0);
        a7 -= 4; wr32(a7, 0xE00C0Eu); a7 -= 4; wr32(a7, handler); a7 += 4;       /* pea ret; move.l VEC,-(a7); rts */
        g_cpu.batch_sp = a7; g_cpu.batch = 1; resume_pc = 0; g_cpu.budget = 0x7FFFFFFF;
        game_dispatch(handler);
        g_cpu.batch = 0;
    }
    if (((sr_sys >> 8) ^ (sr0 >> 8)) & 0x20) { uint32_t t = a7; a7 = other_sp; other_sp = t; }
    sr_sys = sr0 & 0xFF00; set_ccr(sr0); store_regs();
    c->pc = pc0; c->ph = 0.0;
    return RC_STOP;
}

API void cpu_reset_io(void) { memset(g_io, 0, sizeof g_io); ym_n = 0; memset(ym_regs, 0, sizeof ym_regs); ym_sel = 0; rx_h = rx_n = tx_n = 0; }

#ifdef CHECK_RET
#include <stdio.h>
void nonlocal_return(const char *fn, uint32_t expected, uint32_t actual) {
    static const char *seen[64]; static int n = 0;
    for (int i = 0; i < n; i++) if (seen[i] == fn) return;
    if (n < 64) seen[n++] = fn;
    fprintf(stderr, "RETORNO NO LOCAL: la llamada en %s esperaba volver a %06X y la rutina volvio a %06X\n", fn, expected, actual); fflush(stderr);
}
#endif
