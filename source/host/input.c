/*
 * input.c - Keyboard and XInput pads -> joystick and Atari ST keys, with redefinable keys and buttons ("Controls" window of the menu).
 *
 * Pads: left stick or D-pad = directions (dead zone, 8 directions; not redefinable). The remaining buttons are assigned to actions; by default:
 * A = fire (in "1 OR 2 PLAYER GAME?" = key 1 / 2), RT = fire, LT = hold position, RB / LB = next / previous weapon, B = rocket launcher, X = grenade, Start = menu,
 * Back = leave the mission. The first pad is player 1 and the second one is player 2.
 */
#include "input.h"
#include "lang.h"
#include "cfg.h"
#ifdef _WIN32
#include <windows.h>
#endif

#ifdef _WIN32
/* ------------------------------------------------------------------------------------------------ teclas redefinibles */
int key_bind[KA_COUNT];
static const struct { const char *cfg, *label, *hint; int def; } KA[KA_COUNT] = {
    { "key_up", "ctl.key.move_up", "ctl.key.move_up.hint", VK_UP },
    { "key_down", "ctl.key.move_down", "ctl.key.move_down.hint", VK_DOWN },
    { "key_left", "ctl.key.move_left", "ctl.key.move_left.hint", VK_LEFT },
    { "key_right", "ctl.key.move_right", "ctl.key.move_right.hint", VK_RIGHT },
    { "key_fire", "ctl.key.fire", "ctl.key.fire.hint", VK_CONTROL },
    { "key_fire2", "ctl.key.fire_second_key", "ctl.key.fire_second_key.hint", 'Z' },
    { "key_wnext", "ctl.key.next_weapon", "ctl.key.next_weapon.hint", 'A' },
    { "key_wprev", "ctl.key.previous_weapon", "ctl.key.previous_weapon.hint", 0 },
    { "key_rocket", "ctl.key.quick_rocket_launcher", "ctl.key.quick_rocket_launcher.hint", 0 },
    { "key_grenade", "ctl.key.quick_grenade", "ctl.key.quick_grenade.hint", 0 },
};
const char *key_act_label(int a) { return tr(KA[a].label); }
const char *key_act_label_id(int a) { return KA[a].label; }                  /* text ids: the menu translates them every time it draws */
const char *key_act_hint(int a) { return KA[a].hint; }
int kbd_norm_vk(int vk) { return (vk == VK_LCONTROL || vk == VK_RCONTROL) ? VK_CONTROL : vk; }
int kbd_action(int vk) { vk = kbd_norm_vk(vk); if (!vk) return -1; for (int a = 0; a < KA_COUNT; a++) if (key_bind[a] == vk) return a; return -1; }
void kbd_set_bind(int act, int vk) { vk = kbd_norm_vk(vk); if (vk) for (int a = 0; a < KA_COUNT; a++) if (key_bind[a] == vk) key_bind[a] = 0; key_bind[act] = vk; }
int kbd_joy_bit(int vk) {
    switch (kbd_action(vk)) { case KA_UP: return J_UP; case KA_DOWN: return J_DOWN; case KA_LEFT: return J_LEFT; case KA_RIGHT: return J_RIGHT; case KA_FIRE: case KA_FIRE2: return J_FIRE; }
    return 0;
}
int kbd_joy_keys(int *vks) {
    int n = 0;
    for (int a = KA_UP; a <= KA_FIRE2; a++) { int v = key_bind[a]; if (!v) continue; if (v == VK_CONTROL) { vks[n++] = VK_LCONTROL; vks[n++] = VK_RCONTROL; } else vks[n++] = v; }
    return n;
}
void key_name(int vk, char *out, size_t n) {
    static const struct { int vk; const char *nm; } T[] = { { 0, "(none)" }, { VK_UP, "Up" }, { VK_DOWN, "Down" }, { VK_LEFT, "Left" }, { VK_RIGHT, "Right" }, { VK_CONTROL, "Ctrl" }, { VK_LCONTROL, "Left Ctrl" }, { VK_RCONTROL, "Right Ctrl" },
        { VK_SHIFT, "Shift" }, { VK_MENU, "Alt" }, { VK_SPACE, "Space" }, { VK_RETURN, "Enter" }, { 0x10D, "Num Enter" }, { VK_TAB, "Tab" }, { VK_BACK, "Backspace" }, { VK_INSERT, "Insert" }, { VK_DELETE, "Delete" },
        { VK_HOME, "Home" }, { VK_END, "End" }, { VK_PRIOR, "Page Up" }, { VK_NEXT, "Page Down" }, { VK_ADD, "Num +" }, { VK_SUBTRACT, "Num -" }, { VK_MULTIPLY, "Num *" }, { VK_DIVIDE, "Num /" }, { VK_DECIMAL, "Num ." }, { VK_CAPITAL, "Caps Lock" },
        { VK_ESCAPE, "Esc" }, { VK_PAUSE, "Pause" }, { VK_SCROLL, "Scroll Lock" }, { VK_NUMLOCK, "Num Lock" }, { VK_SNAPSHOT, "Print Screen" }, { VK_APPS, "Menu" }, { VK_LWIN, "Left Win" }, { VK_RWIN, "Right Win" },
        { VK_LSHIFT, "Left Shift" }, { VK_RSHIFT, "Right Shift" }, { VK_LMENU, "Left Alt" }, { VK_RMENU, "Right Alt" } };
    for (size_t i = 0; i < sizeof T / sizeof *T; i++) if (T[i].vk == vk) { char id[64]; snprintf(id, sizeof id, "key.%s", T[i].nm); snprintf(out, n, "%s", tr(id)); return; }
    if ((vk >= '0' && vk <= '9') || (vk >= 'A' && vk <= 'Z')) { snprintf(out, n, "%c", vk); return; }
    if (vk >= VK_NUMPAD0 && vk <= VK_NUMPAD9) { snprintf(out, n, "Num %d", vk - VK_NUMPAD0); return; }
    if (vk >= VK_F1 && vk <= VK_F24) { snprintf(out, n, "F%d", vk - VK_F1 + 1); return; }
    char b[64] = ""; UINT sc = MapVirtualKeyA((UINT)vk, MAPVK_VK_TO_VSC);
    if (sc && GetKeyNameTextA((LONG)(sc << 16), b, sizeof b) > 0) snprintf(out, n, "%s", b); else snprintf(out, n, "Key %d", vk);
}
int kbd_scancode(int vk) {
    if (vk >= '1' && vk <= '9') return 2 + (vk - '1');
    if (vk == '0') return 11;
    if (vk >= VK_NUMPAD1 && vk <= VK_NUMPAD9) return 2 + (vk - VK_NUMPAD1);
    if (vk == VK_NUMPAD0) return 11;
    switch (vk) {
    case VK_RETURN: case 0x10D: return 0x1C; case VK_SPACE: return 0x39; case 'P': return 0x19;
    case 'Q': return 0x10; case 'W': return 0x11; case 'E': return 0x12; case 'R': return 0x13; case 'T': return 0x14; case 'Y': return 0x15; case 'U': return 0x16; case 'I': return 0x17; case 'O': return 0x18;
    case 'A': return 0x1E; case 'S': return 0x1F; case 'D': return 0x20; case 'F': return 0x21; case 'G': return 0x22; case 'H': return 0x23; case 'J': return 0x24; case 'K': return 0x25; case 'L': return 0x26;
    case 'X': return 0x2D; case 'C': return 0x2E; case 'B': return 0x30; case 'N': return 0x31; case 'M': return 0x32;
    case VK_F1: return 0x3B; case VK_F2: return 0x3C; case VK_F3: return 0x3D; case VK_F4: return 0x3E; case VK_F5: return 0x3F;
    }
    return 0;
}

/* ------------------------------------------------------------------------------------------------ redefinable gamepad buttons */
int pad_bind[PBA_COUNT];
#define XB_UP 0x0001
#define XB_DOWN 0x0002
#define XB_LEFT 0x0004
#define XB_RIGHT 0x0008
#define XB_START 0x0010
#define XB_BACK 0x0020
#define XB_L3 0x0040
#define XB_R3 0x0080
#define XB_LB 0x0100
#define XB_RB 0x0200
#define XB_A 0x1000
#define XB_B 0x2000
#define XB_X 0x4000
#define XB_Y 0x8000
static const struct { int mask; const char *nm; } PBN[] = { { 0, "none" }, { XB_A, "A" }, { XB_B, "B" }, { XB_X, "X" }, { XB_Y, "Y" }, { XB_LB, "LB" }, { XB_RB, "RB" }, { PB_LT, "LT" }, { PB_RT, "RT" },
    { XB_BACK, "Back" }, { XB_START, "Start" }, { XB_L3, "L3" }, { XB_R3, "R3" } };
#define NPBN ((int)(sizeof PBN / sizeof *PBN))
static const struct { const char *cfg, *label, *hint; int def; } PA[PBA_COUNT] = {
    { "pad_fire", "ctl.pad.fire", "ctl.pad.fire.hint", XB_A },
    { "pad_fire2", "ctl.pad.fire_second_button", "ctl.pad.fire_second_button.hint", PB_RT },
    { "pad_hold", "ctl.pad.hold_position", "ctl.pad.hold_position.hint", PB_LT },
    { "pad_wnext", "ctl.pad.next_weapon", "ctl.pad.next_weapon.hint", XB_RB },
    { "pad_wprev", "ctl.pad.previous_weapon", "ctl.pad.previous_weapon.hint", XB_LB },
    { "pad_rocket", "ctl.pad.quick_rocket_launcher", "ctl.pad.quick_rocket_launcher.hint", XB_B },
    { "pad_grenade", "ctl.pad.quick_grenade", "ctl.pad.quick_grenade.hint", XB_X },
    { "pad_menu", "ctl.pad.options_menu", "ctl.pad.options_menu.hint", XB_START },
    { "pad_exit", "ctl.pad.leave_the_mission", "ctl.pad.leave_the_mission.hint", XB_BACK },
};
const char *pad_act_label(int a) { return tr(PA[a].label); }
const char *pad_act_label_id(int a) { return PA[a].label; }
const char *pad_act_hint(int a) { return PA[a].hint; }
int pad_assignable(int mask) { for (int i = 1; i < NPBN; i++) if (PBN[i].mask == mask) return 1; return 0; }
void pad_set_bind(int act, int mask) { if (mask) for (int a = 0; a < PBA_COUNT; a++) if (pad_bind[a] == mask) pad_bind[a] = 0; pad_bind[act] = mask; }
void pad_button_name(int mask, char *out, size_t n) { for (int i = 0; i < NPBN; i++) if (PBN[i].mask == mask) { snprintf(out, n, "%s", mask ? PBN[i].nm : tr("key.(none)")); return; } snprintf(out, n, "?"); }

void input_defaults(void) { for (int a = 0; a < KA_COUNT; a++) key_bind[a] = KA[a].def; for (int a = 0; a < PBA_COUNT; a++) pad_bind[a] = PA[a].def; }
void input_cfg_load(void) {
    input_defaults();
    for (int a = 0; a < KA_COUNT; a++) if (cfg_has(KA[a].cfg)) key_bind[a] = kbd_norm_vk((int)cfg_num(KA[a].cfg, KA[a].def));
    for (int a = 0; a < PBA_COUNT; a++) if (cfg_has(PA[a].cfg)) { const char *v = cfg_str(PA[a].cfg, ""); for (int i = 0; i < NPBN; i++) if (!_stricmp(v, PBN[i].nm)) pad_bind[a] = PBN[i].mask; }
}
void input_cfg_save(void) {
    for (int a = 0; a < KA_COUNT; a++) cfg_set_num(KA[a].cfg, key_bind[a]);
    for (int a = 0; a < PBA_COUNT; a++) { const char *nm = "none"; for (int i = 0; i < NPBN; i++) if (PBN[i].mask == pad_bind[a]) nm = PBN[i].nm; cfg_set_str(PA[a].cfg, nm); }
}

/* ------------------------------------------------------------------------------------------------ XInput (carga dinamica) */
typedef struct { WORD wButtons; BYTE bLeftTrigger, bRightTrigger; SHORT sThumbLX, sThumbLY, sThumbRX, sThumbRY; } XPad;
typedef struct { DWORD dwPacketNumber; XPad Gamepad; } XState;
typedef DWORD (WINAPI *PFN_XGetState)(DWORD, XState *);
static PFN_XGetState xget = NULL; static int xtried = 0;
static int connected[4]; static double last_poll[4]; static int ncon = 0;

int pads_count(void) { return ncon; }
void pads_names(char *out, size_t n) { snprintf(out, n, "%d", ncon); }

int pads_read(PadRead out[2]) {
    if (!xtried) { xtried = 1; HMODULE h = LoadLibraryA("xinput1_4.dll"); if (!h) h = LoadLibraryA("xinput1_3.dll"); if (!h) h = LoadLibraryA("xinput9_1_0.dll"); if (h) xget = (PFN_XGetState)GetProcAddress(h, "XInputGetState"); }
    if (!xget) return 0;
    double now = now_s(); int idx = 0; ncon = 0;
    for (int i = 0; i < 4; i++) {
        if (!connected[i] && now - last_poll[i] < 1.0) continue;                  /* disconnected pads are polled only once per second */
        XState s; DWORD r = xget((DWORD)i, &s); last_poll[i] = now; connected[i] = (r == ERROR_SUCCESS);
        if (r != ERROR_SUCCESS) continue;
        ncon++;
        if (idx >= 2) continue;
        const float DZ = 0.35f * 32768.0f; int st = 0; XPad *p = &s.Gamepad;
        int raw = p->wButtons & ~0x000F; if (p->bLeftTrigger > 0.30f * 255) raw |= PB_LT; if (p->bRightTrigger > 0.30f * 255) raw |= PB_RT;      /* buttons without the D-pad; the triggers count as buttons */
        if ((p->wButtons & XB_UP) || p->sThumbLY > DZ) st |= J_UP;
        if ((p->wButtons & XB_DOWN) || p->sThumbLY < -DZ) st |= J_DOWN;
        if ((p->wButtons & XB_LEFT) || p->sThumbLX < -DZ) st |= J_LEFT;
        if ((p->wButtons & XB_RIGHT) || p->sThumbLX > DZ) st |= J_RIGHT;
        if ((pad_bind[PBA_FIRE] && (raw & pad_bind[PBA_FIRE])) || (pad_bind[PBA_FIRE2] && (raw & pad_bind[PBA_FIRE2]))) st |= J_FIRE;
        if ((st & J_UP) && (st & J_DOWN)) st &= ~(J_UP | J_DOWN);
        if ((st & J_LEFT) && (st & J_RIGHT)) st &= ~(J_LEFT | J_RIGHT);
        out[idx].st = st; out[idx].raw = raw; out[idx].acts = 0;
        out[idx].wnext = pad_bind[PBA_WNEXT] && (raw & pad_bind[PBA_WNEXT]) ? 1 : 0; out[idx].hold = pad_bind[PBA_HOLD] && (raw & pad_bind[PBA_HOLD]) ? 1 : 0;
        if (pad_bind[PBA_ROCKET] && (raw & pad_bind[PBA_ROCKET])) out[idx].acts |= PA_ROCKET;
        if (pad_bind[PBA_GRENADE] && (raw & pad_bind[PBA_GRENADE])) out[idx].acts |= PA_GRENADE;
        if (pad_bind[PBA_WPREV] && (raw & pad_bind[PBA_WPREV])) out[idx].acts |= PA_WBACK;
        if (pad_bind[PBA_MENU] && (raw & pad_bind[PBA_MENU])) out[idx].acts |= PA_MENU;
        if (pad_bind[PBA_EXIT] && (raw & pad_bind[PBA_EXIT])) out[idx].acts |= PA_EXIT;
        idx++;
    }
    return idx;
}
#else
int key_bind[KA_COUNT]; int pad_bind[PBA_COUNT];
const char *key_act_label(int a) { (void)a; return ""; } const char *key_act_label_id(int a) { (void)a; return ""; } const char *key_act_hint(int a) { (void)a; return ""; }
int kbd_action(int vk) { (void)vk; return -1; } int kbd_norm_vk(int vk) { return vk; } void kbd_set_bind(int a, int vk) { (void)a; (void)vk; }
void key_name(int vk, char *o, size_t n) { (void)vk; snprintf(o, n, ""); }
int kbd_joy_bit(int vk) { (void)vk; return 0; }
int kbd_scancode(int vk) { (void)vk; return 0; }
int kbd_joy_keys(int *v) { (void)v; return 0; }
const char *pad_act_label(int a) { (void)a; return ""; } const char *pad_act_label_id(int a) { (void)a; return ""; } const char *pad_act_hint(int a) { (void)a; return ""; }
void pad_set_bind(int a, int m) { (void)a; (void)m; } void pad_button_name(int m, char *o, size_t n) { (void)m; snprintf(o, n, ""); } int pad_assignable(int m) { (void)m; return 0; }
void input_defaults(void) {} void input_cfg_load(void) {} void input_cfg_save(void) {}
int pads_read(PadRead out[2]) { (void)out; return 0; }
int pads_count(void) { return 0; }
void pads_names(char *o, size_t n) { snprintf(o, n, "0"); }
#endif
