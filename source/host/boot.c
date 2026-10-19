/*
 * boot.c - Start-up of the game: what the original loader (RUNME.TOS + HAGA) leaves in memory when it jumps into the game.
 *
 * The original start-up is a chain of three programs: RUNME.TOS, the program HAGA unpacks from it (a trainer/loader that probes the machine, prints a menu, reads FILES/DOW and patches it)
 * and the HAGA file itself, which stays resident as a library of routines. All their code, and the game's, is translated to C (generated/), so no 68000 code is executed from any file:
 * FILES/DOW is the only original file and the game only reads data from it (graphics, levels, tables, text). The loader always leaves the same memory on this machine; what is built here:
 *
 *   $2C00   the 60000 bytes of FILES/DOW from offset $5141C (the game's block: its tables and text; the instructions in it are never executed)
 *   $81000  the same block (the game unpacks its graphics from there)
 *   LOADER_DATA   the few variables of the loader's hook code ($2000) and of the HAGA library ($3F8000) that the translated code reads (TOS 2.06, 4 MB)
 *   PC = $2C00, supervisor mode, stack at $2400
 */
#include "host.h"

/* The data of the loader that the translated code reads (the rest of its memory is code, which runs as C, or is never used). Taken from the memory the real loader leaves. */
static const struct { uint32_t a; uint16_t n; uint8_t v[26]; } LOADER_DATA[] = {
    { 0x0020EA, 26, { 0x44, 0x4F, 0x57, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x80, 0x00, 0x00, 0x08, 0x10, 0x00, 0x00, 0x0C, 0x80, 0x00, 0xFF, 0x00 } },   /* hook: "DOW", cache bounds, buffer ($C8000) */
    { 0x00217A, 4, { 0x00, 0x3F, 0x80, 0x00 } },                                                                                                                                  /* hook: address of the library ($3F8000) */
    { 0x3F8008, 2, { 0x0F, 0xCE } }, { 0x3F8083, 1, { 0x03 } },                                                                                                                  /* library: entry of its routine table, parameters */
    { 0x3F8094, 16, { 0x00, 0x3E, 0x20, 0xB4, 0x07, 0x4C, 0xFF, 0x00, 0x00, 0x3C, 0xC1, 0x68, 0x00, 0x3F, 0x7F, 0x24 } },                                                       /* library: addresses of its work buffers ($3E20B4 backup of the low memory) */
    { 0x3F80AF, 1, { 0x00 } },
    { 0x3F902A, 2, { 0x4E, 0x41 } }, { 0x3F9116, 2, { 0x4E, 0x41 } }, { 0x3F9128, 2, { 0x4E, 0x41 } },                                                                            /* library: its three "trap #1" instructions */
};

static uint8_t *load_game_file(const char *dir, const char *rel, size_t *n) { char p[800]; snprintf(p, sizeof p, "%s/%s", dir, rel); return read_file(p, n); }

State *boot_state_make(const char *game_dir) {
    size_t nd = 0; uint8_t *dow = load_game_file(game_dir, "FILES/DOW", &nd);
    State *st = NULL;
    if (!dow || nd < 0x5141C + 60000) { host_log("Boot: FILES/DOW is missing or is not the expected version"); goto done; }
    tos_init(game_dir); memset(ram, 0, RAMSZ); memset(io, 0, 0x8000); tos_machine_init();
    /* the game */
    memcpy(ram + 0x81000, dow + 0x5141C, 60000);                                    /* the loader also leaves the file's block at $81000 (the game unpacks its graphics from there) */
    memcpy(ram + 0x2C00, dow + 0x5141C, 60000);
    /* the variables of the loader (hook code at $2000 and HAGA library at $3F8000) */
    for (size_t i = 0; i < sizeof LOADER_DATA / sizeof LOADER_DATA[0]; i++) memcpy(ram + LOADER_DATA[i].a, LOADER_DATA[i].v, LOADER_DATA[i].n);
    /* hardware as the loader leaves it: video base $078000, its palette, MFP/keyboard masked */
    io[0x201] = 0x07; io[0x203] = 0x80; io[0x20D] = 0x00; tos.scrbase = 0x078000;
    { uint8_t pal[32]; pal[0] = 0x05; pal[1] = 0x62; for (int i = 1; i < 16; i++) { pal[2 * i] = 0x00; pal[2 * i + 1] = 0x07; } memcpy(io + 0x240, pal, 32); }
    io[0xE21] = 0xFC; io[0x7C00] = 0x02; io[0x7C02] = 0x13;
    mw32(0x462, 13); mw32(0x466, 13); mw32(0x4BA, 52);                                    /* TOS clocks: VBLs and 200 Hz ticks counted up to the moment the loader jumps into the game */
    memcpy(ram + 0x3E20B4, ram + 8, 0x4FF8);                                                /* copy of the low memory (vectors and system variables) that the loader keeps at $3E20B4 to restore them */
    snprintf(tos.curdir, sizeof tos.curdir, "FILES");                                   /* the loader leaves the current directory at FILES (the game opens "DOW" from there) */
    /* CPU */
    memset(&g_cpu, 0, sizeof g_cpu); { static const uint32_t d[8] = { 0, 0x4E00, 0x30FFFF, 0x100000, 0x70007, 0x70007, 0x70007, 0x80007 }, a[7] = { 0x1492A0, 0x7A68, 0x22BC, 0x149204, 0x3F98E0, 0, 0x3F7F24 };
      for (int i = 0; i < 8; i++) g_cpu.d[i] = d[i]; for (int i = 0; i < 7; i++) g_cpu.a[i] = a[i]; }
    g_cpu.a[7] = 0x2400; g_cpu.osp = 0; g_cpu.srh = 0x2700; g_cpu.pc = 0x2C00; tos.vbl = 13;      /* program entry: the loader jumps to $2C10, the second instruction group of the same start-up code */
    st = state_new(); tos_save_state(st);
done:
    free(dow); return st;
}
