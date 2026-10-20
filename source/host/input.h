/*
 * input.h - Keyboard and gamepads: everything becomes ST joystick state and Atari keys.
 */
#ifndef INPUT_H
#define INPUT_H
#include "host.h"

#define J_UP    1
#define J_DOWN  2
#define J_LEFT  4
#define J_RIGHT 8
#define J_FIRE  0x80

/* ---- redefinable keys. Each action has ONE key (Windows VK; 0 = unassigned). Left and right Ctrl count as the same
 * key (VK_CONTROL). */
enum { KA_UP, KA_DOWN, KA_LEFT, KA_RIGHT, KA_FIRE, KA_FIRE2, KA_WNEXT, KA_WPREV, KA_ROCKET, KA_GRENADE, KA_COUNT };
extern int key_bind[KA_COUNT];
/* id of the label and of the hint: for the menu, which translates them when it draws */
const char *key_act_label_id(int a);
const char *key_act_label(int a); /* "Move up", "Fire"... */
const char *key_act_hint(int a);
int kbd_action(int vk);  /* action the key is assigned to, or -1 */
int kbd_norm_vk(int vk); /* Ctrl derecho -> Ctrl izquierdo, etc. */
/* assigns (taking it away from the action that had it); vk = 0 leaves it unassigned */
void kbd_set_bind(int act, int vk);
/* text of the key ("Up", "Ctrl", "Z", "Num 5", "(none)") */
void key_name(int vk, char *out, size_t n);
/* keyboard: joystick bits or ST scancode of a Windows key (VK); 0 if unused */
int kbd_joy_bit(int vk);
int kbd_scancode(int vk);
/* direction and fire keys that must be polled (Ctrl with both variants) */
int kbd_joy_keys(int *vks);

/* ---- redefinable gamepad buttons (the same layout for both pads). One button = one XInput bit; the triggers are PB_LT
 * / PB_RT. */
/* XInput button bits (the D-pad bits stay in input.c) */
#define XB_START 0x0010
#define XB_BACK  0x0020
#define XB_L3    0x0040
#define XB_R3    0x0080
#define XB_LB    0x0100
#define XB_RB    0x0200
#define XB_A     0x1000
#define XB_B     0x2000
#define XB_X     0x4000
#define XB_Y     0x8000
#define PB_LT    0x10000
#define PB_RT    0x20000
enum { PBA_FIRE, PBA_FIRE2, PBA_HOLD, PBA_WNEXT, PBA_WPREV, PBA_ROCKET, PBA_GRENADE, PBA_MENU, PBA_EXIT, PBA_COUNT };
extern int pad_bind[PBA_COUNT];
const char *pad_act_label_id(int a);
const char *pad_act_label(int a);
const char *pad_act_hint(int a);
/* mask = 0 leaves it unassigned; if another action had the button, it is taken away from it */
void pad_set_bind(int act, int mask);
void pad_button_name(int mask, char *out, size_t n);
/* action (PBA_*) assigned to that button, or -1 */
int pad_action_of(int mask);
/* buttons that can be assigned (not the D-pad nor the sticks) */
int pad_assignable(int mask);

/* ---- defaults and saving into the options */
void input_defaults(void); /* factory controls (keyboard and pad) */
/* reads key_* and pad_* from the loaded options (what is missing stays at its default) */
void input_cfg_load(void);
void input_cfg_save(void);

enum { PA_ROCKET = 1, PA_GRENADE = 2, PA_WBACK = 4, PA_MENU = 8, PA_EXIT = 16 };
typedef struct PadRead {
    int st;
    int wnext;
    int acts;
    int hold;
    int raw;
    int aim;
} PadRead; /* joystick state, next weapon pressed, actions pressed, hold position, the raw buttons (XInput bits +
              PB_LT/PB_RT) and the direction of the right stick (J_* bits, 8 directions; 0 = released or twin-stick off)
            */
/* twin-stick: the left stick moves, the right stick aims (and fires) */
extern int pad_dual_stick;
/* the right stick also holds the fire button while it is pushed */
extern int pad_aim_autofire;
int pads_read(PadRead out[2]); /* number of pads (0..2) */
void pads_names(char *out, size_t n);
int pads_count(void);
#endif
