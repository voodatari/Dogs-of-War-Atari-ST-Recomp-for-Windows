/*
 * cpu.h - Motorola 68000 state and the macros used by the game's generated C code.
 *
 * The game code (generated/*.c) is translated instruction by instruction from the original 68000. Every game routine is a plain
 * C function (sub_XXXXXX or its readable name). The registers are global variables with the same name as in the
 * processor (d0..d7, a0..a6, a7 = active stack) and the flags (X N Z V C) are fx, fn, fz, fv, fc.
 *
 * A BSR/JSR call is an ordinary C call: besides calling the function, the return address is pushed on the emulated stack
 * (the game reads and writes that stack and the RAM state must be identical to the Atari's). RTS pops it and does "return".
 */
#ifndef CPU_H
#define CPU_H
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API
#endif

#define RAMSZ   0x400000u            /* 4 MB of Atari STE RAM */
#define ROMBASE 0xE00000u
#define ROMSZ   0x100000u
#define NHOOK_MAX 8192
#define NPATCH 8

/* Reasons why the game code yields control to the host */
enum { RC_STOP = 0, RC_SOFT = 1, RC_MISS = 2, RC_BUDGET = 3, RC_DONE = 4, RC_HOOK = 5 };

/* CPU state visible to the host. It is identical to the monolithic core's as before. */
typedef struct CPU {
    uint32_t d[8], a[8];           /* a[7] = pila activa */
    uint32_t osp;                  /* the other stack (USP in supervisor mode, SSP in user mode) */
    uint32_t pc;
    uint32_t X, N, Z, V, C;
    uint32_t srh;                  /* system byte of the SR in its position (e.g. 0x2700) */
    int32_t  stop, inject, skip_idle, irq_done, irq_active;
    int32_t  budget;
    uint32_t irq_target;
    double   ph;                   /* phase inside the VBL used to stamp the writes to the YM */
    uint8_t  patch[NPATCH];
    uint8_t  hook_on[NHOOK_MAX];
    uint32_t soft_pc;              /* address of the instruction the host must resolve (RC_SOFT / RC_MISS) */
    int32_t  rc;                   /* reason core_run exited */
    int32_t  batch;                /* 1 while a batch routine runs (the final RTE acts as a 4-byte RTS) */
    uint32_t batch_sp;
    uint32_t hook_addr;            /* address of the hook that caused RC_HOOK */
} CPU;

extern CPU g_cpu;
extern uint8_t *ram, *rom, *io;

/* ---- registros ---- */
extern uint32_t d0, d1, d2, d3, d4, d5, d6, d7;
extern uint32_t a0, a1, a2, a3, a4, a5, a6, a7;
extern uint8_t  fx, fn, fz, fv, fc;                  /* indicadores X N Z V C (0 o 1) */
extern uint32_t sr_sys, other_sp;                    /* system byte of the SR (0x2700...) and the inactive stack */
extern uint32_t ret_addr;                            /* last return address popped by RTS */
extern uint32_t resume_pc;                           /* != 0: the function must resume at that address (only when loading a state) */

/* ---- memory (big-endian, like the 68000) ---- */
uint32_t io_read(uint32_t a, int size);
void     io_write(uint32_t a, int size, uint32_t v);
static inline uint32_t rd8(uint32_t a)  { a &= 0xFFFFFFu; return a < RAMSZ ? ram[a] : io_read(a, 1); }
static inline uint32_t rd16(uint32_t a) { a &= 0xFFFFFFu; if (a < RAMSZ - 1) { uint16_t v; memcpy(&v, ram + a, 2); return (uint32_t)__builtin_bswap16(v); } return io_read(a, 2); }
static inline uint32_t rd32(uint32_t a) { a &= 0xFFFFFFu; if (a < RAMSZ - 3) { uint32_t v; memcpy(&v, ram + a, 4); return __builtin_bswap32(v); } return io_read(a, 4); }
static inline void wr8(uint32_t a, uint32_t v)  { a &= 0xFFFFFFu; if (a < RAMSZ) ram[a] = (uint8_t)v; else io_write(a, 1, v); }
static inline void wr16(uint32_t a, uint32_t v) { a &= 0xFFFFFFu; if (a < RAMSZ - 1) { uint16_t x = __builtin_bswap16((uint16_t)v); memcpy(ram + a, &x, 2); } else io_write(a, 2, v); }
static inline void wr32(uint32_t a, uint32_t v) { a &= 0xFFFFFFu; if (a < RAMSZ - 3) { uint32_t x = __builtin_bswap32(v); memcpy(ram + a, &x, 4); } else io_write(a, 4, v); }

/* ---- partial write of data registers (a MOVE.B/.W only changes the low part) ---- */
#define SETB(r, v) ((r) = ((r) & 0xFFFFFF00u) | ((uint32_t)(v) & 0xFFu))
#define SETW(r, v) ((r) = ((r) & 0xFFFF0000u) | ((uint32_t)(v) & 0xFFFFu))
#define SX8(x)  ((uint32_t)(int32_t)(int8_t)(x))
#define SX16(x) ((uint32_t)(int32_t)(int16_t)(x))

/* ---- indicadores ---- */
/* They are updated only when a later instruction reads them (the generator decides this with a flag liveness analysis). */
#define FLAGS_LOGIC(r, m, s)        do { fn = !!((r) & (s)); fz = (((r) & (m)) == 0); fv = 0; fc = 0; } while (0)
#define FLAGS_ADD(sv, dv, r, m, s, setx) do { fn = !!((r) & (s)); fz = (((r) & (m)) == 0); fv = !!((~((sv) ^ (dv)) & ((sv) ^ (r))) & (s)); fc = (((r) & (m)) < ((sv) & (m))); if (setx) fx = fc; } while (0)
#define FLAGS_SUB(sv, dv, r, m, s, setx) do { fn = !!((r) & (s)); fz = (((r) & (m)) == 0); fv = !!((((sv) ^ (dv)) & ((dv) ^ (r))) & (s)); fc = (((sv) & (m)) > ((dv) & (m))); if (setx) fx = fc; } while (0)
static inline uint32_t get_ccr(void) { return ((uint32_t)fx << 4) | ((uint32_t)fn << 3) | ((uint32_t)fz << 2) | ((uint32_t)fv << 1) | fc; }
static inline void set_ccr(uint32_t v) { fx = (v >> 4) & 1; fn = (v >> 3) & 1; fz = (v >> 2) & 1; fv = (v >> 1) & 1; fc = v & 1; }

/* branch conditions (cc = condition field of the instruction) */
#define COND_0   1                                 /* T  */
#define COND_1   0                                 /* F  */
#define COND_2   (!fc && !fz)                      /* HI: higher (unsigned) */
#define COND_3   (fc || fz)                        /* LS: lower or same (unsigned) */
#define COND_4   (!fc)                             /* CC: carry clear */
#define COND_5   (fc)                              /* CS: carry set */
#define COND_6   (!fz)                             /* NE */
#define COND_7   (fz)                              /* EQ */
#define COND_8   (!fv)                             /* VC */
#define COND_9   (fv)                              /* VS */
#define COND_10  (!fn)                             /* PL */
#define COND_11  (fn)                              /* MI */
#define COND_12  (fn == fv)                        /* GE */
#define COND_13  (fn != fv)                        /* LT */
#define COND_14  (!fz && fn == fv)                 /* GT */
#define COND_15  (fz || fn != fv)                  /* LE */
#define COND(n) COND_##n

#define FL_LOGIC FLAGS_LOGIC
#define FL_ADD FLAGS_ADD
#define FL_SUB FLAGS_SUB
#define MISS(a) do { game_dispatch(a); return; } while (0)          /* code that was not translated: the host is told (see game_dispatch in generated/dispatch.c) */
uint32_t shift_op(int type, int left, uint32_t v, uint32_t cnt, int bits);   /* ASx LSx ROXx ROx; updates the flags */

/* ---- status register ---- */
void set_sr(uint32_t v);                           /* switches stacks if the supervisor bit changes */
uint32_t get_sr(void);

/* ---- calls and returns ---- */
#define PUSH32(v)      do { a7 -= 4; wr32(a7, (v)); } while (0)
#define PUSH16(v)      do { a7 -= 2; wr16(a7, (v)); } while (0)
static inline uint32_t POP32(void) { uint32_t v = rd32(a7); a7 += 4; return v; }
static inline uint32_t POP16(void) { uint32_t v = rd16(a7); a7 += 2; return v; }
/* A routine can return "higher up" than usual (it discards its return address with ADDQ.L #4,A7 and does RTS, or reads it as a pointer to
 * data): then RTS leaves in ret_addr the address that is really returned to, which is not the one of this call. The caller must also
 * finish ("return") so that the C call stack follows the 68000 stack; the one that did expect that address continues. */
#ifdef CHECK_RET
void nonlocal_return(const char *fn, uint32_t expected, uint32_t actual);
#define CHECK_NONLOCAL(ret) nonlocal_return(__func__, (ret), ret_addr)
#else
#define CHECK_NONLOCAL(ret) ((void)0)
#endif
#define CALL(fn_, ret) do { PUSH32(ret); fn_(); if (ret_addr != (ret)) { CHECK_NONLOCAL(ret); return; } } while (0)                     /* BSR / JSR to a known routine */
#define CALL_DYN(ea, ret) do { uint32_t t_ = (ea); PUSH32(ret); call_dynamic(t_); if (ret_addr != (ret)) { CHECK_NONLOCAL(ret); return; } } while (0)   /* JSR (An) etc. */
#define RTS()          do { ret_addr = rd32(a7); a7 += 4; return; } while (0)
#define RTR()          do { set_ccr(rd16(a7)); ret_addr = rd32(a7 + 2); a7 += 6; return; } while (0)
#define JUMP_DYN(ea)   do { uint32_t t_ = (ea); call_dynamic(t_); return; } while (0)    /* JMP (An), jump tables... */
void call_dynamic(uint32_t pc);                    /* runs the routine that starts at pc (starts it as a call) */
void rte_return(void);                             /* RTE: ver machine.c */
#define RTE()          do { rte_return(); return; } while (0)

/* ---- handing control to the host ---- */
void yield_to_host(int rc);                        /* saves the registers in g_cpu, returns to core_run() and reloads them on resume */
#define BUDGET_CHECK(addr) do { if (--g_cpu.budget < 0) { g_cpu.pc = (addr); yield_to_host(RC_BUDGET); } } while (0)
#ifdef HOST_DIRECT
/* Native host: the hook is a direct C call (with the registers dumped in g_cpu and reloaded afterwards), without switching fibers */
void sync_regs_out(void); void sync_regs_in(void); void host_hook(uint32_t addr);
#define HOOK(slot, addr)   do { if (__builtin_expect(g_cpu.hook_on[slot], 0)) { g_cpu.hook_addr = (addr); sync_regs_out(); host_hook(addr); sync_regs_in(); } } while (0)
#else
#define HOOK(slot, addr)   do { if (__builtin_expect(g_cpu.hook_on[slot], 0)) { g_cpu.hook_addr = (addr); yield_to_host(RC_HOOK); } } while (0)
#endif
#define SOFT(addr)         do { g_cpu.pc = (addr); g_cpu.soft_pc = (addr); yield_to_host(RC_SOFT); if (g_cpu.pc != (addr) + 2u) { call_dynamic(g_cpu.pc); return; } } while (0)
/* The game's wait loops: they yield control once per VBL (see HOOK/IDLE_* below) */
#define STOP_WAIT(next)    do { g_cpu.pc = (next); yield_to_host(RC_STOP); } while (0)      /* STOP instruction: waits for the next interrupt */
#define IDLE_ENTRY()       do { g_cpu.inject = 1; } while (0)
#define IDLE_SPIN(addr)    do { if (g_cpu.inject) { g_cpu.inject = 0; g_cpu.pc = (addr); yield_to_host(RC_STOP); } } while (0)
#define IDLE_EVERY(addr)   do { if (g_cpu.skip_idle) g_cpu.skip_idle = 0; else { g_cpu.pc = (addr); yield_to_host(RC_STOP); g_cpu.skip_idle = 0; } } while (0)
#define IDLE_DELAY(addr, n) do { if (g_cpu.skip_idle) g_cpu.skip_idle = 0; else if (d7 > (n) + 1u) { d7 -= (n) - 1u; g_cpu.pc = (addr); yield_to_host(RC_STOP); g_cpu.skip_idle = 0; } } while (0)

/* ---- game routines (generated/) ---- */
void game_dispatch(uint32_t pc);                   /* calls the routine that starts at pc (generated) */
void game_resume(uint32_t pc);                     /* resumes the routine that contains pc (only when loading a state) */
int  game_hook_slot(uint32_t addr);                /* hook index of an address, or -1 */
extern const int game_hook_count;
uint32_t game_hook_addr(int i);                    /* address of hook i (0..game_hook_count-1) */

#endif
