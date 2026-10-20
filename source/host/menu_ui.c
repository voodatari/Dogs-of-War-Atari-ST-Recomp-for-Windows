/*
 * menu_ui.c - Showing and hiding the options menu, and the state of "redefine a key / a pad button".
 *
 * The menu pauses the game: opening it releases every input the game was holding and pauses the emulation clock;
 * closing it saves the options. Only the window thread calls these functions (the fast-input thread just reads
 * menu_open_flag).
 */
#include "app.h"

volatile int menu_open_flag = 0;

/* Redefining controls (window "Keys and gamepad buttons"): which kind of input is awaited and for which action. */
int rebind_kind = 0;
int rebind_action = -1;
int rebind_prev_ok = 0;
int rebind_wait_release = 0;

/* Set to make the next frame send the whole joystick / aim / hold state to the emulation again. */
int resend_input = 0;

/* Opens the options menu: stops the game's inputs, refreshes the list of music packs and pauses the emulation. */
void open_menu(void) {
    {
        char cur[80];
        snprintf(cur, sizeof cur, "%s", music_pack);
        scan_packs();
        pack_resolve(cur);
    }
    rebind_kind = 0;
    menu_cur = menu_options;
    menu_open_flag = 1;
    menu_show(menu_cur);
    emu_post(EV_HOLD, 0, 0);
    emu_post(EV_HOLD, 1, 0);
    sent_hold[0] = sent_hold[1] = 0;
    emu_post(EV_AIM, 0, 0);
    emu_post(EV_AIM, 1, 0);
    sent_aim[0] = sent_aim[1] = 0;
    emu_post(EV_JOY, 0, 0);
    emu_post(EV_JOY0, 0, 0);
    emu_post(EV_PAUSE, 1, 0);
    menu_drawn_sig[0] = 0;
}

/* Closes the menu (and the controls window), resumes the game and saves the options. */
void close_menu(void) {
    rebind_kind = 0;
    menu_hide(menu_controls);
    menu_hide(menu_options);
    menu_cur = menu_options;
    menu_open_flag = 0;
    resend_input = 1;
    emu_post(EV_PAUSE, 0, 0);
    save_options();
}

/* Esc / B: from the controls window it goes back to the menu; from the menu it closes. */
void menu_back(void) {
    if (menu_cur == menu_controls) {
        rebind_kind = 0;
        menu_hide(menu_controls);
        menu_cur = menu_options;
        menu_drawn_sig[0] = 0;
    } else
        close_menu();
}

/* The player chose an action of the controls window: wait for the key (kind 1) or pad button (kind 2) to assign. */
void start_rebind(int kind, int action) {
    rebind_kind = kind;
    rebind_action = action;
    rebind_prev_ok = 0;
    menu_drawn_sig[0] = 0;
    menu_status(menu_cur, tr(kind == 1 ? "msg.press_new_key" : "msg.press_new_button"));
}

/* Text shown at the right of an action of the controls window: its key / button, or "press a key..." while it is
 * being redefined. */
const char *ctl_text(int kind, int action) {
    static char t[2][64]; /* two buffers: the menu may ask for two texts before drawing */
    static int k = 0;
    k ^= 1;
    if (rebind_kind == kind && rebind_action == action) {
        snprintf(t[k], sizeof t[k], "%s", tr(kind == 1 ? "val.press_key" : "val.press_button"));
        return t[k];
    }
    if (kind == 1)
        key_name(key_bind[action], t[k], sizeof t[k]);
    else
        pad_button_name(pad_bind[action], t[k], sizeof t[k]);
    return t[k];
}
