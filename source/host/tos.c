/*
 * tos.c - Mini-TOS and interrupt scheduler.
 *
 * Serves what the game expects from an Atari ST: minimal GEMDOS / BIOS / XBIOS calls (files from the game folder),
 * the intelligent keyboard (IKBD: joystick and keys through the ACIA), and the clocks: for every VBL period it delivers
 * the VBL interrupt, the timer B firings (palette changes by zones), timer C (music), timer A (4-bit samples) and the
 * keyboard.
 */
#include "host.h"
#ifdef _WIN32
#include <windows.h>
#endif

Tos tos;
/* The loader shows a text screen at start-up: the text at full brightness, a fade to dark (while the original machine
   reads the 800 KB of the game from disk) and a fade back, then the intro. Here the read is instant, so the game is
   held once, with the text at full brightness, for "loading_seconds" (set by main; 0 = no wait). */
double g_load_seconds = 0.0;
static double hold_until = 0;
/* The text screen of the loader is shown by the game as: the text at once, a fade to dark grey, (the disk read), a fade
   back, then the screen is cleared, a black wait and the intro. What is shown instead: a fade in of the text,
   "loading_seconds" of text, a fade out to black and straight to the intro. The game is held (stall) while the text is
   shown; the palette that is presented is the one the text has, dimmed; and the black wait after the text is cut short
   (skip_black_delay, used by the delay loop at $75A4). */
/* seconds (real time: outside the game the emulation runs at the monitor rate) */
#define DIR_FADE_IN  0.5
#define DIR_FADE_OUT 0.45
static int loader_state = 0, loader_skip_black = 0;
static uint8_t loader_palette[32];
static double loader_fade_in_start = 0, loader_fade_out_start = 0, loader_text_end = 0;
/* seconds of black between the fade out of the text and the intro */
#define DIR_BLACK 1.0
/* Called when the game starts reading its own file: holds it once on the loader's text screen for "loading_seconds"
 * (the real machine took that long to read the disk). Returns 1 if the hold started. */
int hold_loader_text(void) {
    if (g_load_seconds <= 0)
        return 0;
    hold_until = loader_text_end = now_s() + DIR_FADE_IN + g_load_seconds;
    g_load_seconds = 0;
    loader_state = 1;
    loader_fade_in_start = 0;
    return 1;
}
/* the game resumes after the black wait that the director holds: its own one is not needed */
int skip_black_delay(void) {
    if (!loader_skip_black || hold_until > 0)
        return 0;
    loader_skip_black = 0;
    return 1;
}

/* runtime/machine.c API */
extern uint8_t *get_ym_regs(void);
extern int get_ym_sel(void);
extern void set_ym_sel(int);
extern void ym_clear(void);
extern int ym_take(void *out, int max);
extern void acia_push(uint32_t b);
extern void acia_clear(void);
extern int acia_len(void);
extern int acia_peek(int i);
extern int ikbd_take(uint8_t *out, int max);
extern void cpu_reset_io(void);
extern void core_reset(void);
extern int core_run(CPU *c, int32_t budget);
extern int cpu_run_isr(CPU *c, uint32_t handler, int level, int force, int n, const double *phases, int32_t budget);
extern int cpu_run_batch(CPU *c, uint32_t handler, int n, const double *phases, int32_t budget);

/* ---- registers of the game (g_cpu while it is yielded) ---- */
/* ---- host registers (g_cpu while the game is yielded) ---- */
static uint32_t cpu_sr(void) {
    CPU *c = &g_cpu;
    return c->srh | (c->X << 4) | (c->N << 3) | (c->Z << 2) | (c->V << 1) | c->C;
}
/* Sets the status register; swaps the stack pointers when the supervisor bit changes. */
static void cpu_set_sr(uint32_t v) {
    CPU *c = &g_cpu;
    v &= 0xFFFF;
    if ((v ^ c->srh) & 0x2000) {
        uint32_t t = c->a[7];
        c->a[7] = c->osp;
        c->osp = t;
    }
    c->srh = v & 0xFF00;
    c->X = (v >> 4) & 1;
    c->N = (v >> 3) & 1;
    c->Z = (v >> 2) & 1;
    c->V = (v >> 1) & 1;
    c->C = v & 1;
}
/* The supervisor stack pointer (a7 or the saved one, depending on the mode). */
static uint32_t cpu_isp(void) {
    return (g_cpu.srh & 0x2000) ? g_cpu.a[7] : g_cpu.osp;
}
/* The user stack pointer. */
static uint32_t cpu_usp(void) {
    return (g_cpu.srh & 0x2000) ? g_cpu.osp : g_cpu.a[7];
}
/* Sets the user stack pointer. */
static void cpu_set_usp(uint32_t v) {
    if (g_cpu.srh & 0x2000)
        g_cpu.osp = v;
    else
        g_cpu.a[7] = v;
}
/* Sets the supervisor stack pointer. */
static void cpu_set_isp(uint32_t v) {
    if (g_cpu.srh & 0x2000)
        g_cpu.a[7] = v;
    else
        g_cpu.osp = v;
}

/* ---- game files ---- */
/* file handles: they are not reused (6, 7, 8...) and the game may leave many open */
#define MAXH 8192
static FILE *files[MAXH];
static int nexth = 6;
/* the game must yield control on returning from the call */
static int stop_flag = 0;
static int polls = 0;
static uint8_t ikbd_cmd[8];
static int ikbd_n = 0;

#ifdef _WIN32
/* looks for `name` case-insensitively inside `dir` */
static int ci_find(const char *dir, const char *name, char *out, size_t n) {
    char pat[700];
    snprintf(pat, sizeof pat, "%s\\*", dir);
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat, &fd);
    if (h == INVALID_HANDLE_VALUE)
        return 0;
    int ok = 0;
    do {
        if (_stricmp(fd.cFileName, name) == 0) {
            snprintf(out, n, "%s", fd.cFileName);
            ok = 1;
            break;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    return ok;
}
#else
static int ci_find(const char *dir, const char *name, char *out, size_t n) {
    (void)dir;
    snprintf(out, n, "%s", name);
    return 1;
}
#endif

/* Turns a GEMDOS path of the game ("A:\FILES\DOW", relative to the current directory) into a real path inside the
 * game folder, looking every part up case-insensitively. Returns 0 if a part does not exist. */
static int resolve_path(const char *name, char *out, size_t n) {
    char tmp[600];
    size_t k;
    if (name[0] && name[1] == ':')
        name += 2;
    if (name[0] != '\\' && name[0] != '/' && tos.curdir[0])
        snprintf(tmp, sizeof tmp, "%s/%s", tos.curdir, name);
    else
        snprintf(tmp, sizeof tmp, "%s", name);
    for (k = 0; tmp[k]; k++)
        if (tmp[k] == '\\')
            tmp[k] = '/';
    char cur[700];
    snprintf(cur, sizeof cur, "%s", tos.folder);
    char *p = tmp;
    while (*p == '/')
        p++;
    while (*p) {
        char part[260];
        size_t i = 0;
        while (*p && *p != '/' && i < sizeof part - 1)
            part[i++] = *p++;
        part[i] = 0;
        while (*p == '/')
            p++;
        if (!i || (i == 1 && part[0] == '.'))
            continue;
        char hit[260];
        if (!ci_find(cur, part, hit, sizeof hit))
            return 0;
        size_t l = strlen(cur);
        snprintf(cur + l, sizeof cur - l, "/%s", hit);
    }
    snprintf(out, n, "%s", cur);
    return 1;
}
/* Reads a C string from the Atari memory. */
static void cstr_at(uint32_t a, char *out, size_t n) {
    size_t i = 0;
    while (i + 1 < n) {
        uint8_t c = (uint8_t)mr8(a + i);
        if (!c)
            break;
        out[i++] = (char)c;
    }
    out[i] = 0;
}
/* Does s end with suf (case-insensitive)? */
static int ends_with_i(const char *s, const char *suf) {
    size_t a = strlen(s), b = strlen(suf);
    if (b > a)
        return 0;
    for (size_t i = 0; i < b; i++) {
        char x = s[a - b + i], y = suf[i];
        if (x >= 'a' && x <= 'z')
            x -= 32;
        if (y >= 'a' && y <= 'z')
            y -= 32;
        if (x != y)
            return 0;
    }
    return 1;
}

/* ---- the sound chip ---- */
/* ---- YM ---- */
void ym_events_clear(void) {
    ym_clear();
}
/* Takes the register writes the game made to the sound chip during this VBL, in order of their instant inside it. */
int ym_events_take(YmEvent *out, int max) {
    int n = ym_take(out, max);
    /* stable order by instant */
    for (int i = 1; i < n; i++) {
        YmEvent t = out[i];
        int j = i - 1;
        while (j >= 0 && out[j].ph > t.ph) {
            out[j + 1] = out[j];
            j--;
        }
        out[j + 1] = t;
    }
    return n;
}
/* Sets the sound chip registers (used when loading a state). */
void ym_set_regs(const uint8_t *r, int sel) {
    memcpy(get_ym_regs(), r, 16);
    set_ym_sel(sel);
}
/* Reads the sound chip registers (and the selected register). */
void ym_get_regs(uint8_t *r, int *sel) {
    memcpy(r, get_ym_regs(), 16);
    if (sel)
        *sel = get_ym_sel();
}

/* ---- ACIA / intelligent keyboard ---- */
/* ---- ACIA / IKBD ---- */
static void ikbd_byte(uint8_t b) {
    if (ikbd_n < 8)
        ikbd_cmd[ikbd_n++] = b;
    /* reset: 0x80 0x01 -> 0xF1 (self-test passed) */
    if (ikbd_cmd[0] == 0x80) {
        if (ikbd_n == 2) {
            ikbd_n = 0;
            acia_push(0xF1);
        }
    }
    /* the rest of the commands (joystick, mouse...) are ignored */
    else
        ikbd_n = 0;
}
/* Delivers what the game wrote to the keyboard port since the last call to ikbd_byte. */
static void drain_ikbd(void) {
    uint8_t b[256];
    int n = ikbd_take(b, 256);
    for (int i = 0; i < n; i++)
        ikbd_byte(b[i]);
}
/* Log of the host's inputs to the game (joystick, keys, patches, memory writes): to replay EXACTLY a session that gets
 * corrupted (emu.c dumps it next to the previous snapshot) */
#define NLOG 20000
static struct {
    uint32_t vbl;
    char t;
    int a, b;
} input_log[NLOG];
static int n_input_log = 0;
/* Adds an entry to the input log: t = J joystick, K key, F patch switch, P memory write; a and b are its two values. */
void tos_ilog(char t, int a, int b) {
    if (n_input_log == NLOG) {
        memmove(input_log, input_log + NLOG / 2, sizeof input_log[0] * (NLOG / 2));
        n_input_log = NLOG / 2;
    }
    input_log[n_input_log].vbl = tos.vbl;
    input_log[n_input_log].t = t;
    input_log[n_input_log].a = a;
    input_log[n_input_log].b = b;
    n_input_log++;
}
/* Drops the log entries older than vbl. */
void tos_ilog_trim(uint32_t vbl) {
    int k = 0;
    while (k < n_input_log && input_log[k].vbl < vbl)
        k++;
    if (k) {
        memmove(input_log, input_log + k, sizeof input_log[0] * (size_t)(n_input_log - k));
        n_input_log -= k;
    }
}
/* Empties the log. */
void tos_ilog_clear(void) {
    n_input_log = 0;
}
/* Writes the log (from VBL `from`) to a text file; used for the corruption reports. */
int tos_ilog_write(const char *path, uint32_t from) {
    FILE *f = fopen(path, "w");
    if (!f)
        return 0;
    /* state of the patches at the start (later changes are logged) */
    fprintf(f, "0 F 1 %d\n0 F 0 %d\n", g_cpu.patch[1], g_cpu.patch[0]);
    for (int i = 0; i < n_input_log; i++)
        if (input_log[i].vbl >= from)
            fprintf(f, "%u %c %d %d\n", input_log[i].vbl - from, input_log[i].t, input_log[i].a, input_log[i].b);
    fclose(f);
    return 1;
}
/* Sends a joystick report to the game through the keyboard port: port 0 / 1 and the state (bits 0-3 directions, bit 7
 * fire). */
void tos_joystick(int port, int state) {
    tos_ilog('J', port, state);
    acia_push(0xFE + (port ? 1 : 0));
    acia_push(state & 0x8F);
}
/* Sends a key scancode (press, or release with bit 7 set) through the keyboard port. */
void tos_key(int sc) {
    tos_ilog('K', sc, 0);
    acia_push(sc & 0xFF);
}

/* ---- start-up ---- */
/* ---- start-up ---- */
void tos_init(const char *game_folder) {
    memset(&tos, 0, sizeof tos);
    hold_until = 0;
    loader_state = 0;
    loader_skip_black = 0;
    snprintf(tos.folder, sizeof tos.folder, "%s", game_folder);
    tos.vbl_rate = 50.0;
    tos.song_now = -1;
    tos.heap = 0x100000;
    tos.scrbase = SCREEN_DEFAULT;
    tos.kbd = 0x4F00;
    for (int i = 0; i < 3; i++)
        tos.wait_hit[i] = (uint32_t)-99;
    cpu_reset_io();
    memset(ram, 0, RAMSZ);
    memset(rom, 0, ROMSZ);
    /* header of the TOS ROM (only what HAGA queries: version) and stub page (each one an RTE) */
    rom[0] = 0x60;
    rom[1] = 0x30;
    rom[2] = 0x02;
    rom[3] = 0x06;
    rom[8] = 0x00;
    rom[9] = 0xE0;
    rom[0x1A] = 0x19;
    rom[0x1B] = 0x89;
    for (int i = 0; i < 0x400; i += 2) {
        rom[0x800 + i] = 0x4E;
        rom[0x800 + i + 1] = 0x73;
    }
    g_cpu.srh = 0x2700;
    g_cpu.stop = 0;
}

/* "Empty" machine to boot from scratch: vectors to the stubs, system variables, cookie jar and kbdvbase */
void tos_machine_init(void) {
    for (uint32_t n = 0; n < 256; n++)
        mw32(n * 4, STUB_BASE + 4 * n);
    static const uint32_t sv[][2] = {{0x42E, 0x400000},   {0x432, 0x800},  {0x436, 0x3F8000},   {0x44E, 0x3F8000},
                                     {0x454, 8},          {0x456, 0x4F40}, {0x4F2, 0xE00000},   {0x4C6, 0},
                                     {0x4A6, 2},          {0x4A2, 0},      {0x426, 0x752019F3}, {0x42A, 0x237698AA},
                                     {0x43A, 0x31415926}, {0x4C2, 4},      {0x472, 0}};
    for (size_t i = 0; i < sizeof sv / sizeof sv[0]; i++)
        mw32(sv[i][0], sv[i][1]);
    /* memcntlr: bank 1 = 4 MB (HAGA computes the RAM from here) */
    mw8(0x424, 0x0A);
    static const struct {
        const char *k;
        uint32_t v;
    } ck[] = {{"_CPU", 0}, {"_VDO", 0x00010000}, {"_MCH", 0x00010000}, {"_SND", 2}, {"_FPU", 0}, {"", 0}};
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 4; j++)
            mw8(0x4F80 + 8u * i + j, ck[i].k[0] && j < (int)strlen(ck[i].k) ? (uint32_t)(uint8_t)ck[i].k[j] : 0);
        mw32(0x4F84 + 8u * i, ck[i].v);
    }
    mw32(0x5A0, 0x4F80);
    tos.kbd = 0x4F00;
    for (uint32_t i = 0; i < 10; i++)
        mw32(tos.kbd + 4 * i, STUB_BASE + 0x100 + 4 * i);
    mw32(0x4F40, 0);
    tos.scrbase = SCREEN_DEFAULT;
    io[0x201] = (SCREEN_DEFAULT >> 16) & 0xFF;
    io[0x203] = (SCREEN_DEFAULT >> 8) & 0xFF;
    io[0x20D] = SCREEN_DEFAULT & 0xFF;
}

/* Switches one of the game-code patches (0 = immunity, 1 = enemy-script fix) and logs it. */
void tos_set_patch(int slot, int on) {
    tos_ilog('F', slot, on);
    if (slot >= 0 && slot < NPATCH)
        g_cpu.patch[slot] = on ? 1 : 0;
}

/* ---- machine state ---- */
/* ---- state ---- */
void tos_load_state(const State *s) {
    memcpy(ram, s->ram, RAMSZ);
    memcpy(io, s->io, 0x8000);
    CPU *c = &g_cpu;
    for (int i = 0; i < 8; i++)
        c->d[i] = s->d[i];
    for (int i = 0; i < 7; i++)
        c->a[i] = s->a[i];
    c->a[7] = s->a7;
    c->osp = (s->sr & 0x2000) ? s->usp : s->isp;
    c->srh = s->sr & 0xFF00;
    c->X = (s->sr >> 4) & 1;
    c->N = (s->sr >> 3) & 1;
    c->Z = (s->sr >> 2) & 1;
    c->V = (s->sr >> 1) & 1;
    c->C = s->sr & 1;
    c->pc = s->pc;
    tos.vbl = s->vbl;
    tos.scrbase = s->scrbase;
    tos.heap = s->heap;
    snprintf(tos.curdir, sizeof tos.curdir, "%s", s->curdir);
    tos.nzones = s->nzones;
    for (int i = 0; i < s->nzones; i++)
        tos.zones[i] = s->zones[i];
    ym_set_regs(s->ym, s->ymsel);
    ym_clear();
    acia_clear();
    for (int i = 0; i < s->nrx; i++)
        acia_push(s->rx[i]);
    tos.song_now = s->song;
    core_reset();
    c->stop = 0;
    c->inject = 0;
    c->skip_idle = 0;
    c->irq_target = 0;
    /* the RAM of the snapshots may carry patches already applied (immunity, fix): the core has its own switches */
    if (mr16(0xAA42) == 0x4E75)
        mw16(0xAA42, 0x48E7);
    if (mr16(0xB13E) == 0x6086)
        mw16(0xB13E, 0x4E75);
    ikbd_n = 0;
    stop_flag = 0;
}

/* Saves the machine into a snapshot. */
void tos_save_state(State *s) {
    memcpy(s->ram, ram, RAMSZ);
    memcpy(s->io, io, 0x8000);
    CPU *c = &g_cpu;
    for (int i = 0; i < 8; i++)
        s->d[i] = c->d[i];
    for (int i = 0; i < 7; i++)
        s->a[i] = c->a[i];
    s->a7 = c->a[7];
    s->usp = cpu_usp();
    s->isp = cpu_isp();
    s->sr = cpu_sr();
    s->pc = c->pc;
    s->vbl = tos.vbl;
    s->scrbase = tos.scrbase;
    s->heap = tos.heap;
    snprintf(s->curdir, sizeof s->curdir, "%s", tos.curdir);
    s->nzones = tos.nzones;
    for (int i = 0; i < tos.nzones; i++)
        s->zones[i] = tos.zones[i];
    ym_get_regs(s->ym, &s->ymsel);
    int n = acia_len();
    if (n > 256)
        n = 256;
    s->nrx = n;
    for (int i = 0; i < n; i++)
        s->rx[i] = (uint8_t)acia_peek(i);
    s->song = tos.song_now;
}

/* ---- exceptions and system calls ---- */
/* ---- exceptions and system calls ---- */
static int exception_(int vec, uint32_t retpc) {
    CPU *c = &g_cpu;
    uint32_t sr = cpu_sr();
    cpu_set_sr((sr | 0x2000) & ~0x8000u);
    uint32_t sp = c->a[7] - 6;
    mw16(sp, sr);
    mw32(sp + 2, retpc);
    c->a[7] = sp;
    c->pc = mr32((uint32_t)vec * 4);
    return 1;
}
/* Line-A instructions (graphics primitives of the original TOS): only the "get the line-A variables" call is faked. */
static int linea_(uint32_t op, uint32_t pc) {
    if (op == 0xA000) {
        g_cpu.a[0] = 0x4E00;
        g_cpu.d[0] = 0x4E00;
        g_cpu.a[1] = 0x4E80;
        g_cpu.a[2] = 0x4EC0;
    }
    g_cpu.pc = pc + 2;
    return 1;
}
/* Sets the video base address registers (the screen the game shows). */
static void set_base_(uint32_t a) {
    io[0x201] = (a >> 16) & 0xFF;
    io[0x203] = (a >> 8) & 0xFF;
    io[0x20D] = a & 0xFF;
}
/* The game polls the keyboard in a loop: every few polls the call yields control to the host so it does not spin. */
static void idle_poll(void) {
    polls++;
    if ((polls & 3) == 0)
        stop_flag = 1;
}
/* A bump allocator for the game's GEMDOS Malloc calls (0xFFFFFFFF asks how much there is: answers 2 MB). */
static uint32_t malloc_(uint32_t n) {
    if (n == 0xFFFFFFFFu)
        return 0x200000;
    uint32_t a = (tos.heap + 15) & ~15u;
    tos.heap = a + n;
    return a;
}

/* GEMDOS call (TRAP #1) number fn with its arguments on the stack at sp: console, files in the game folder, memory.
 * Returns the value for d0. Unknown calls return 0. */
static uint32_t gemdos(uint32_t fn, uint32_t sp) {
#define A2(o) mr16(sp + (o))
#define A4(o) mr32(sp + (o))
    if (fn == 0x20) { /* Super */
        uint32_t arg = A4(2), sr = cpu_sr();
        if (arg == 0) {
            if (sr & 0x2000)
                return 0xFFFFFFFFu;
            uint32_t cur = g_cpu.a[7], old = cpu_isp();
            cpu_set_sr(sr | 0x2000);
            g_cpu.a[7] = cur;
            return old;
        }
        {
            uint32_t cur = g_cpu.a[7];
            cpu_set_isp(arg);
            cpu_set_usp(cur);
            cpu_set_sr(sr & ~0x2000u);
            return 0;
        }
    }
    /* Pterm0 / Pterm: the program ends */
    if (fn == 0x00 || fn == 0x4C) {
        tos.done = 1;
        stop_flag = 1;
        return 0;
    }
    /* Cconin / Crawcin / Cnecin: wait for a key */
    if (fn == 0x01 || fn == 0x07 || fn == 0x08) {
        if (tos.nkeys) {
            uint32_t k = tos.keys[0];
            memmove(tos.keys, tos.keys + 1, 4 * (size_t)(--tos.nkeys));
            return k;
        }
        stop_flag = 1;
        return 0;
    }
    /* Cconout: character to the console (ignored) */
    if (fn == 0x02)
        return 0;
    /* Crawio: key or output without waiting */
    if (fn == 0x06) {
        if (A2(2) != 0xFF)
            return 0;
        if (tos.nkeys) {
            uint32_t k = tos.keys[0];
            memmove(tos.keys, tos.keys + 1, 4 * (size_t)(--tos.nkeys));
            return k;
        }
        idle_poll();
        return 0;
    }
    /* Cconws: string to the console (ignored) */
    if (fn == 0x09)
        return 0;
    /* Cconis: is a key waiting? */
    if (fn == 0x0B) {
        if (tos.nkeys)
            return 0xFFFFFFFFu;
        idle_poll();
        return 0;
    }
    /* Dsetdrv: the drive map */
    if (fn == 0x0E)
        return 0x4;
    /* Dgetdrv: current drive */
    if (fn == 0x19)
        return 2;
    /* Fsetdta */
    if (fn == 0x1A) {
        tos.dta = A4(2);
        return 0;
    }
    /* Fgetdta */
    if (fn == 0x2F)
        return tos.dta;
    /* Tgetdate / Tgettime: a fixed value */
    if (fn == 0x2A || fn == 0x2C)
        return 0x2A2B;
    if (fn == 0x3B) { /* Dsetpath */
        char p[260], real[700];
        cstr_at(A4(2), p, sizeof p);
        if (resolve_path(p, real, sizeof real) && dir_exists(real)) {
            size_t fl = strlen(tos.folder);
            const char *rel = real + fl;
            while (*rel == '/' || *rel == '\\')
                rel++;
            snprintf(tos.curdir, sizeof tos.curdir, "%s", rel);
            return 0;
        }
        return 0xFFFFFFDEu;
    }
    /* Dgetpath */
    if (fn == 0x47)
        return 0;
    if (fn == 0x3D) { /* Fopen */
        char p[260], real[700];
        cstr_at(A4(2), p, sizeof p);
        if (resolve_path(p, real, sizeof real) && file_exists(real) && !ends_with_i(real, ".HST")) {
            int h = nexth;
            if (h >= MAXH)
                return 0xFFFFFFDFu;
            files[h] = fopen(real, "rb");
            if (!files[h])
                return 0xFFFFFFDFu;
            nexth = h + 1;
            if (getenv("DOW_FOPEN"))
                host_log_file("Fopen %s -> h%d (vbl %u)", real, h, tos.vbl);
            return (uint32_t)h;
        }
        return 0xFFFFFFDFu;
    }
    /* Fclose */
    if (fn == 0x3E) {
        uint32_t h = A2(2);
        if (getenv("DOW_FOPEN"))
            host_log_file("Fclose h%u (vbl %u)", h, tos.vbl);
        if (h < MAXH && files[h]) {
            fclose(files[h]);
            files[h] = NULL;
        }
        return 0;
    }
    if (fn == 0x3F) { /* Fread(h, n, buf) */
        uint32_t h = A2(2), n = A4(4), buf = A4(8);
        if (h >= MAXH || !files[h])
            return 0xFFFFFFDFu;
        if ((uint64_t)buf + n > RAMSZ)
            n = buf < RAMSZ ? RAMSZ - buf : 0;
        size_t got = n ? fread(ram + buf, 1, n, files[h]) : 0;
        if (getenv("DOW_FOPEN"))
            host_log_file("Fread h%u n=%u -> $%X got=%u (vbl %u)", h, n, buf, (unsigned)got, tos.vbl);
        return (uint32_t)got;
    }
    /* Fseek(off, h, mode) */
    if (fn == 0x42) {
        int32_t off = (int32_t)A4(2);
        uint32_t h = mr16(sp + 6), mode = mr16(sp + 8);
        if (h >= MAXH || !files[h])
            return 0xFFFFFFDFu;
        fseek(files[h], off, mode == 0 ? SEEK_SET : mode == 1 ? SEEK_CUR : SEEK_END);
        return (uint32_t)ftell(files[h]);
    }
    /* Malloc / Mxalloc */
    if (fn == 0x48 || fn == 0x44)
        return malloc_(A4(2));
    /* Mfree / Mshrink */
    if (fn == 0x49 || fn == 0x4A)
        return 0;
    /* Fsfirst / Fsnext: no directory listing */
    if (fn == 0x4E || fn == 0x4F)
        return 0xFFFFFFCEu;
    return 0;
}

/* BIOS call (TRAP #13). */
static uint32_t bios(uint32_t fn, uint32_t sp) {
    if (fn == 1) {
        if (tos.nkeys)
            return 0xFFFFFFFFu;
        idle_poll();
        return 0;
    }
    if (fn == 2) {
        if (tos.nkeys) {
            uint32_t k = tos.keys[0];
            memmove(tos.keys, tos.keys + 1, 4 * (size_t)(--tos.nkeys));
            return k;
        }
        stop_flag = 1;
        return 0;
    }
    if (fn == 3)
        return 0;
    if (fn == 5) {
        uint32_t v = A2(2), ad = A4(4);
        uint32_t old = mr32(v * 4);
        if (ad != 0xFFFFFFFFu)
            mw32(v * 4, ad);
        return old;
    }
    if (fn == 6)
        return 20;
    if (fn == 8)
        return 0xFFFFFFFFu;
    if (fn == 0x0B)
        return 0;
    if (fn == 0x0A)
        return 0x4;
    return 0;
}

/* XBIOS call (TRAP #14): screen address, palette, random numbers, keyboard vectors... Returns 1 if handled (the value
 * goes in *ret). */
static int xbios(uint32_t fn, uint32_t sp, uint32_t *ret) {
    *ret = 0;
    if (fn == 2 || fn == 3) {
        *ret = tos.scrbase;
        return 1;
    }
    if (fn == 4)
        return 1;
    if (fn == 5) {
        uint32_t ph = A4(6);
        if (ph != 0xFFFFFFFFu && ph) {
            tos.scrbase = ph;
            set_base_(ph);
        }
        return 1;
    }
    if (fn == 6) {
        uint32_t p = A4(2);
        for (int i = 0; i < 32; i++)
            io[0x240 + i] = (uint8_t)mr8(p + (uint32_t)i);
        return 1;
    }
    if (fn == 7) {
        uint32_t n = A2(2), cc = A2(4);
        if (cc != 0xFFFF) {
            io[0x240 + 2 * (n & 15)] = (uint8_t)(cc >> 8);
            io[0x240 + 2 * (n & 15) + 1] = (uint8_t)cc;
        }
        return 1;
    }
    if (fn == 8) {
        *ret = 0xFFFFFFFFu;
        return 1;
    }
    if (fn == 0x11) {
        *ret = (uint32_t)(now_s() * 1000.0) & 0xFFFFFF;
        return 1;
    }
    if (fn == 0x15)
        return 1;
    if (fn == 0x17) {
        *ret = 0x2A2B2A2Bu;
        return 1;
    }
    if (fn == 0x1C)
        return 1;
    if (fn == 0x22) {
        *ret = tos.kbd;
        return 1;
    }
    if (fn == 0x25) {
        stop_flag = 1;
        return 1;
    }
    if (fn == 0x26)
        return 1;
    return 1;
}

/* A TRAP instruction: if the game installed its own handler the exception is delivered; otherwise the TOS call is
 * answered here. */
static int trap_(int n, uint32_t pc) {
    uint32_t vec = mr32((uint32_t)(32 + n) * 4);
    /* the program installed its handler: real delivery */
    if (vec != STUB_VEC((uint32_t)(32 + n)))
        return exception_(32 + n, pc + 2);
    uint32_t sp = g_cpu.a[7], fn = mr16(sp), ret = 0;
    if (n == 1)
        ret = gemdos(fn, sp);
    else if (n == 13)
        ret = bios(fn, sp);
    else if (n == 14)
        xbios(fn, sp, &ret);
    g_cpu.d[0] = ret;
    if (!tos.done)
        g_cpu.pc = pc + 2;
    return 1;
}

/* The game code asked the host to execute an instruction it does not run itself (TRAP, line-A, illegal): handles it.
 * 0 = unsupported. */
static int soft_(uint32_t pc) {
    uint32_t op = mr16(pc);
    if (op >= 0x4E40 && op <= 0x4E4F)
        return trap_((int)(op & 15), pc);
    if ((op & 0xF000) == 0xA000)
        return linea_(op, pc);
    if (op == 0x4AFC)
        return exception_(4, pc);
    host_log("unsupported instruction %04X at %06X", op, pc);
    return 0;
}

/* ---- running the game ---- */
/* ---- execution ---- */
int tos_run_slice(int32_t budget) {
    stop_flag = 0;
    /* the loader text is being held on screen: the game does not run */
    if (hold_until > 0) {
        if (now_s() < hold_until)
            return 1;
        hold_until = 0;
    }
    for (;;) {
        int rc = core_run(&g_cpu, budget);
        drain_ikbd();
        /* the game waits in a sound / key loop */
        if (rc == RC_STOP) {
            uint32_t pc = g_cpu.pc;
            if (pc == 0xF91E)
                tos.wait_hit[0] = tos.vbl;
            else if (pc == 0x3972)
                tos.wait_hit[1] = tos.vbl;
            else if (pc == 0xFDF0)
                tos.wait_hit[2] = tos.vbl;
        }
        if (rc == RC_SOFT) {
            uint32_t pc = g_cpu.soft_pc;
            g_cpu.pc = pc;
            if (soft_(pc)) {
                if (stop_flag || tos.done)
                    break;
                continue;
            }
            return -1;
        }
        if (rc == RC_MISS) {
            host_log("the game jumped to an invalid address %06X (vbl %u): corrupted state", g_cpu.soft_pc, tos.vbl);
            {
                extern uint32_t dyn_hist[32][2];
                extern int dyn_n;
                for (int i = 0; i < 12; i++) {
                    int k = (dyn_n - 1 - i) & 31;
                    host_log("  dynamic jump -%d: %06X (a7 %06X)", i, dyn_hist[k][0], dyn_hist[k][1]);
                }
            }
            host_log("  d %08X %08X %08X %08X %08X %08X %08X %08X", d0, d1, d2, d3, d4, d5, d6, d7);
            host_log("  a %08X %08X %08X %08X %08X %08X %08X %08X  ret %08X sr %04X", a0, a1, a2, a3, a4, a5, a6, a7,
                     ret_addr, sr_sys);
            host_log("  stack %08X %08X %08X %08X %08X %08X %08X %08X", mr32(a7), mr32(a7 + 4), mr32(a7 + 8),
                     mr32(a7 + 12), mr32(a7 + 16), mr32(a7 + 20), mr32(a7 + 24), mr32(a7 + 28));
            return -1;
        }
        break;
    }
    return tos.done ? 0 : 1;
}

/* Is the interrupt `bit` enabled and unmasked in this pair of MFP registers? */
int tos_mfp_enabled(int ie, int im, int bit) {
    return (io[ie] & (1 << bit)) && (io[im] & (1 << bit));
}

/* Runs an interrupt handler of the game (vector number, priority level). Returns 0 if it failed. */
int tos_run_isr(int vecn, int level) {
    int rc = cpu_run_isr(&g_cpu, mr32((uint32_t)vecn * 4), level, 0, 1, NULL, 2000000);
    drain_ikbd();
    return rc != -1;
}

/* Runs the handler n times, at the instants of the frame given in ph (fractions of the VBL), for the timer
 * interrupts. */
static void run_isr_batch(uint32_t vec, int n, const double *ph) {
    if (n <= 0)
        return;
    cpu_run_batch(&g_cpu, vec, n, ph, 2000000);
    drain_ikbd();
}

/* Copies the 16 palette registers (32 bytes). */
void tos_pal16(uint8_t *pal32) {
    memcpy(pal32, io + 0x240, 32);
}
/* Starts a new palette zone at this scan line: the game changes the palette in the middle of the screen with timer B.
 */
static void add_zone(int line) {
    if (tos.nzones < MAXZONES) {
        tos.zones[tos.nzones].line = line;
        tos_pal16(tos.zones[tos.nzones].pal);
        tos.nzones++;
    }
}

static const int MFP_PRESCALE[8] = {0, 4, 10, 16, 50, 64, 100, 200};

/* ------------------------------------------------------------------------------------------------ fades
   The game darkens the screen (end of a mission) by subtracting 1 from every component of the palette of 512 colours, a
   step every ~5 VBL: the hues drift and the steps are big. Here that is recognised (the palette is the one it had
   before, minus k in every component, k = 1..8; one or two colours may be something else, like the text) and what the
   presentation gets is the original palette with a smooth multiplicative dimming (a lighter one) at the full 8-bit
   colour: the zone palette keeps the colours (bit 15 of the word = "dim this colour") and in the top bits of words 0-2
   the 8-bit factor (see pal_to_rgb). The palette registers the game reads are not touched. */
/* brightness at each level of the original fade (it ends in black) */
static const double FADE_BRIGHTNESS[9] = {1.0, 0.90, 0.80, 0.68, 0.54, 0.40, 0.26, 0.12, 0.0};
static uint8_t fade_ref[32];
static int fade_level = 0, fade_len = 5;
static double fade_from = 1.0, fade_to = 1.0;
static uint32_t fade_t0 = 0, fade_last_change = 0;
/* Component c (0 = red, 1 = green, 2 = blue; 3 bits) of colour i of an ST palette. */
static int fd_comp(const uint8_t *pal, int i, int c) {
    int v = (pal[2 * i] << 8) | pal[2 * i + 1];
    return (v >> (8 - 4 * c)) & 7;
}
/* Is colour i of P the colour of the reference palette R with k taken from every component (the game's fade step)? */
static int fd_color_ok(const uint8_t *R, const uint8_t *P, int i, int k) {
    for (int c = 0; c < 3; c++) {
        int r = fd_comp(R, i, c) - k;
        if (r < 0)
            r = 0;
        if (fd_comp(P, i, c) != r)
            return 0;
    }
    return 1;
}
/* returns k (1..8) or 0 */
static int fd_match(const uint8_t *R, const uint8_t *P) {
    for (int k = 1; k <= 8; k++) {
        int mism = 0, changed = 0;
        for (int i = 0; i < 16; i++) {
            if (!fd_color_ok(R, P, i, k))
                mism++;
            else
                for (int c = 0; c < 3; c++)
                    if (fd_comp(R, i, c) != fd_comp(P, i, c)) {
                        changed++;
                        break;
                    }
        }
        if (mism <= 2 && changed >= 4)
            return k;
    }
    return 0;
}
/* the palette ref (all 16 colours) dimmed by f; see pal_to_rgb */
static void put_dim(uint8_t *o, const uint8_t *ref, double f) {
    int f8 = (int)(f * 255.0 + 0.5);
    if (f8 < 0)
        f8 = 0;
    if (f8 > 255)
        f8 = 255;
    for (int i = 0; i < 16; i++) {
        o[2 * i] = (uint8_t)((ref[2 * i] & 0x0F) | 0x80);
        o[2 * i + 1] = ref[2 * i + 1];
    }
    o[0] = (uint8_t)((o[0] & 0x8F) | (((f8 >> 5) & 7) << 4));
    o[2] = (uint8_t)((o[2] & 0x8F) | (((f8 >> 2) & 7) << 4));
    o[4] = (uint8_t)((o[4] & 0x8F) | ((f8 & 3) << 4));
}
/* 1 = it has set the palette of this VBL */
static int loader_text_director(void) {
    if (!loader_state)
        return 0;
    /* the screen was cleared: the text is over; black for a second */
    if (tos.nzones != 1) {
        if (loader_state == 2) {
            loader_state = 0;
            loader_skip_black = 1;
            hold_until = now_s() + DIR_BLACK;
        }
        return 0;
    }
    double now = now_s();
    if (loader_fade_in_start == 0) {
        memcpy(loader_palette, tos.zones[0].pal, 32);
        loader_fade_in_start = now;
        loader_fade_out_start = 0;
    }
    if (loader_state == 1) {
        double f = (now - loader_fade_in_start) / DIR_FADE_IN;
        if (f > 1.0)
            f = 1.0;
        put_dim(tos.zones[0].pal, loader_palette, f);
        if (now >= loader_text_end) {
            loader_state = 2;
            loader_fade_out_start = now;
        }
        return 1;
    }
    double f = 1.0 - (now - loader_fade_out_start) / DIR_FADE_OUT;
    if (f < 0)
        f = 0;
    put_dim(tos.zones[0].pal, loader_palette, f);
    if (now - loader_fade_out_start > 3.0) {
        loader_state = 0;
        loader_skip_black = 1;
        hold_until = now + DIR_BLACK;
    }
    return 1;
}
/* Recognises the game's own fades to black (steps of 1 in every palette component) and replaces them with a smooth
 * one in the palette sent to the presenter. */
static void fade_filter(void) {
    if (tos.nzones != 1) {
        fade_level = 0;
        fade_to = fade_from = 1.0;
        return;
    }
    uint8_t P[32];
    memcpy(P, tos.zones[0].pal, 32);
    int k = 0;
    if (memcmp(P, fade_ref, 32) == 0)
        k = 0;
    else {
        k = fd_match(fade_ref, P);
        if (!k)
            memcpy(fade_ref, P, 32);
    }
    if (k != fade_level) {
        double cur =
            fade_from + (fade_to - fade_from) *
                            ((tos.vbl - fade_t0) >= (uint32_t)fade_len ? 1.0 : (double)(tos.vbl - fade_t0) / fade_len);
        int dk = k - fade_level;
        uint32_t gap = tos.vbl - fade_last_change;
        if (k > 0 && (dk == 1 || dk == -1)) {
            fade_from = cur;
            fade_to = FADE_BRIGHTNESS[k];
            fade_len = gap >= 2 && gap <= 12 ? (int)gap : 5;
        } else {
            fade_from = fade_to = FADE_BRIGHTNESS[k];
            fade_len = 1;
        }
        fade_t0 = tos.vbl;
        fade_last_change = tos.vbl;
        fade_level = k;
    }
    if (!k)
        return;
    double f =
        fade_from + (fade_to - fade_from) *
                        ((tos.vbl - fade_t0) >= (uint32_t)fade_len ? 1.0 : (double)(tos.vbl - fade_t0) / fade_len);
    int f8 = (int)(f * 255.0 + 0.5);
    if (f8 < 0)
        f8 = 0;
    if (f8 > 255)
        f8 = 255;
    uint8_t *o = tos.zones[0].pal;
    for (int i = 0; i < 16; i++)
        if (fd_color_ok(fade_ref, P, i, k)) {
            o[2 * i] = (uint8_t)((fade_ref[2 * i] & 0x0F) | 0x80);
            o[2 * i + 1] = fade_ref[2 * i + 1];
        }
    o[0] = (uint8_t)((o[0] & 0x8F) | (((f8 >> 5) & 7) << 4));
    o[2] = (uint8_t)((o[2] & 0x8F) | (((f8 >> 2) & 7) << 4));
    o[4] = (uint8_t)((o[4] & 0x8F) | ((f8 & 3) << 4));
}

/* ---- one VBL period ---- */
/* One VBL period of the machine: the VBL interrupt, the timer B firings (palette zones), timer C (music) and timer A
 * (4-bit samples) in their proper instants, and the keyboard interrupts. */
void tos_vbl(void) {
    tos.vbl++;
    /* TOS system variables: only while the TOS still has control (default VBL vector); afterwards that low RAM belongs
     * to the game */
    if (mr32(0x70) == STUB_VEC(28)) {
        mw32(0x466, mr32(0x466) + 1);
        mw32(0x462, mr32(0x462) + 1);
        mw32(0x4BA, mr32(0x4BA) + 4);
    }
    g_cpu.skip_idle = 1;
    tos.nzones = 0;
    if (mr32(0x70) != STUB_VEC(28)) {
        tos_run_isr(28, 4);
        add_zone(0);
        /* timer B (event count): every firing changes the palette from there downwards */
        int line = 0;
        for (int k = 0; k < 3; k++) {
            if ((io[0x7A1B] & 0x0F) != 8 || !tos_mfp_enabled(0x7A07, 0x7A13, 0))
                break;
            if (mr32(0x120) == STUB_VEC(72))
                break;
            line += io[0x7A21] ? io[0x7A21] : 256;
            io[0x7A1B] = 0;
            if (!tos_run_isr(72, 6))
                break;
            add_zone(line);
        }
    }
    /* timer C: the game programs it through registers (at ~48.6 Hz for the music); unprogrammed it is the TOS's (200
     * Hz) */
    int pre = MFP_PRESCALE[(io[0x7A1D] >> 4) & 7], tcd = io[0x7A23] ? io[0x7A23] : 256;
    double fc = pre ? 2457600.0 / ((double)pre * tcd) : 0.0;
    if (io[0x7A1D] == 0 && io[0x7A23] == 0)
        fc = 200.0;
    double per = fc / tos.vbl_rate, acc0 = tos.tc_acc;
    tos.tc_acc += per;
    int ntc = (int)(tos.tc_acc + 1e-9);
    tos.tc_acc -= ntc;
    if (ntc > 500)
        ntc = 500;
    tos.nph = 0;
    if (per > 0)
        for (int k = 0; k < ntc; k++) {
            double v = (k + 1 - acc0) / per;
            tos.ph_times[tos.nph++] = v > 0.999 ? 0.999 : v;
        }
    if (mr32(0x114) != STUB_VEC(69) && tos_mfp_enabled(0x7A09, 0x7A15, 5))
        run_isr_batch(mr32(0x114), ntc, tos.nph == ntc ? tos.ph_times : NULL);
    tos.nph = 0;
    /* timer A (4-bit samples) */
    int tacr = io[0x7A19] & 0x0F;
    if (tacr && mr32(0x134) != STUB_VEC(77) && tos_mfp_enabled(0x7A07, 0x7A13, 5)) {
        int div = MFP_PRESCALE[tacr & 7];
        if (!div)
            div = 1;
        int data = io[0x7A1F] ? io[0x7A1F] : 256;
        int n = (int)(2457600.0 / ((double)div * data) / tos.vbl_rate);
        /* the idle handler: bclr #5,$fffa0f ; rte */
        static const uint8_t idle_isr[10] = {0x08, 0xB9, 0x00, 0x05, 0x00, 0xFF, 0xFA, 0x0F, 0x4E, 0x73};
        uint32_t h = mr32(0x134);
        int idle = h + 10 <= RAMSZ && memcmp(ram + h, idle_isr, 10) == 0;
        if (!idle)
            run_isr_batch(h, n < 400 ? n : 400, NULL);
    }
    if (!loader_text_director())
        fade_filter();
    tos_deliver_keys();
}

/* keyboard / joystick (ACIA): up to 4 interrupts */
void tos_deliver_keys(void) {
    for (int k = 0; k < 4; k++)
        if (acia_len() && mr32(0x118) != STUB_VEC(70) && tos_mfp_enabled(0x7A09, 0x7A15, 6))
            tos_run_isr(70, 6);
}

/* The screen as 320x200 colour indexes (decoding the 4 bitplanes at the video base) and its palette zones. */
void tos_screen_idx(uint8_t *idx, Zone *zones, int *nz) {
    uint32_t base = ((uint32_t)io[0x201] << 16) | ((uint32_t)io[0x203] << 8) | io[0x20D];
    if (base + 32000 > RAMSZ)
        base = SCREEN_DEFAULT;
    for (int y = 0; y < 200; y++)
        for (int g = 0; g < 20; g++) {
            const uint8_t *p = ram + base + (uint32_t)y * 160 + (uint32_t)g * 8;
            uint32_t w0 = (p[0] << 8) | p[1], w1 = (p[2] << 8) | p[3], w2 = (p[4] << 8) | p[5], w3 = (p[6] << 8) | p[7];
            uint8_t *o = idx + y * 320 + g * 16;
            for (int b = 0; b < 16; b++) {
                int s = 15 - b;
                o[b] = (uint8_t)(((w0 >> s) & 1) | (((w1 >> s) & 1) << 1) | (((w2 >> s) & 1) << 2) |
                                 (((w3 >> s) & 1) << 3));
            }
        }
    if (tos.nzones) {
        memcpy(zones, tos.zones, sizeof(Zone) * (size_t)tos.nzones);
        *nz = tos.nzones;
    } else {
        zones[0].line = 0;
        tos_pal16(zones[0].pal);
        *nz = 1;
    }
}

int tos_call_sub3(uint32_t addr, uint32_t d0, uint32_t d1, uint32_t d2);
/* Calls a routine of the game (address, d0) from the host and returns, leaving the registers as they were. */
int tos_call_sub(uint32_t addr, uint32_t d0) {
    return tos_call_sub3(addr, d0, g_cpu.d[1], g_cpu.d[2]);
}
/* The same with d0, d1 and d2. */
int tos_call_sub3(uint32_t addr, uint32_t d0, uint32_t d1, uint32_t d2) {
    CPU saved = g_cpu;
    g_cpu.d[0] = d0;
    g_cpu.d[1] = d1;
    g_cpu.d[2] = d2;
    int rc = cpu_run_batch(&g_cpu, addr, 1, NULL, 2000000);
    drain_ikbd();
    uint32_t pc0 = saved.pc;
    for (int i = 0; i < 8; i++)
        g_cpu.d[i] = saved.d[i];
    for (int i = 0; i < 8; i++)
        g_cpu.a[i] = saved.a[i];
    g_cpu.osp = saved.osp;
    g_cpu.srh = saved.srh;
    g_cpu.X = saved.X;
    g_cpu.N = saved.N;
    g_cpu.Z = saved.Z;
    g_cpu.V = saved.V;
    g_cpu.C = saved.C;
    g_cpu.pc = pc0;
    return rc;
}
