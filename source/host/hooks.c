/* hooks.c - table of game-code hooks (the generator puts HOOK(slot, address) at every address of HOST_HOOKS and of the scene units). */
#include "host.h"

#define HT 512
typedef struct { uint32_t addr; void (*fn)(uint32_t); } HookEnt;
static HookEnt htab[HT];

static unsigned hslot(uint32_t a) { return (a * 2654435761u >> 12) & (HT - 1); }

void hook_register(uint32_t addr, void (*fn)(uint32_t)) {
    unsigned h = hslot(addr);
    while (htab[h].addr && htab[h].addr != addr) h = (h + 1) & (HT - 1);
    htab[h].addr = addr; htab[h].fn = fn;
    int slot = game_hook_slot(addr);
    if (slot < 0) host_log("WARNING: address %06X has no hook in the generated code", addr);
    else g_cpu.hook_on[slot] = 1;
}

void hooks_enable(uint32_t addr, int on) { int slot = game_hook_slot(addr); if (slot >= 0) g_cpu.hook_on[slot] = on ? 1 : 0; }

void host_hook(uint32_t addr) {
    unsigned h = hslot(addr);
    while (htab[h].addr) { if (htab[h].addr == addr) { htab[h].fn(addr); return; } h = (h + 1) & (HT - 1); }
}

int hook_registered(uint32_t addr) { unsigned h = hslot(addr); while (htab[h].addr) { if (htab[h].addr == addr) return 1; h = (h + 1) & (HT - 1); } return 0; }
