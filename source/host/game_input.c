/*
 * game_input.c - Everything the window loop does with the inputs in one frame: the events of the window (keys), the
 * XInput pads, and the navigation of the menu with either. The result is a stream of events for the emulation thread
 * (emu_post): joystick state of the two ports, Atari keys, "hold position" and aim of each pad, weapon changes...
 *
 *   keyboard   arrows + fire keys -> joystick bits of port 1 (kbd_state); the other keys -> Atari scancodes
 *   pad 1      joystick of port 1 too (it adds to the keyboard)         pad 2    joystick of port 2
 *   menu open  nothing reaches the game: the keys / the pad move through the menu instead
 *
 * With "fast input polling" on (fastinput.c) a thread reads the joystick, hold and aim 1000 times per second and this
 * file leaves those three to it; everything else is still done here.
 */
#include "app.h"

uint8_t key_held[512];        /* keys pressed while the menu was open: ignored by the game until they are released */
int pad_fire_blocked[2];      /* the fire button pressed to accept in the menu does not fire until it is released */
volatile int input_ready = 0; /* set once the window loop runs (the fast-input thread waits for it) */
int sent_hold[2] = {0, 0};    /* last "hold position" / aim values sent to the emulation per pad (-1 = send again) */
int sent_aim[2] = {0, 0};

static int kbd_state = 0;                  /* joystick bits of the keyboard (arrows, fire keys) */
static int sent_joy1 = -1, sent_joy0 = -1; /* last joystick state sent for each port (-1 = send again) */
static double menu_repeat_at[2];           /* pad navigation: when the held direction repeats */
static int menu_prev_dirs[2];              /* pad navigation: directions held in the previous frame */

/* Keys of the pad that must be released after a short time (the "1 OR 2 PLAYER GAME?" screen needs a key press). */
static double key_release_at[8];
static int key_release_scancode[8];
static int n_key_releases = 0;

/* Value of the EV_AIM event for pad i: its right-stick direction (J_* bits), plus 0x80 if the stick also fires. */
int pad_aim_code(const PadRead *pr, int n, int i) {
    int a = i < n ? pr[i].aim : 0;
    return a ? (a | (pad_aim_autofire ? 0x80 : 0)) : 0;
}

/* Used by the automatic tests to press keys of the joystick without a keyboard. */
void input_set_kbd_state(int bits) {
    kbd_state = bits;
}

/* ---------------------------------------------------------------------------------------------- keys */

/* A key event while the menu is open: redefining a key, or moving through the menu. */
static void key_in_menu(const VEvent *ev) {
    int vk = ev->vk, down = ev->down;
    /* redefining a key or a button */
    if (rebind_kind && (down || ev->rep)) {
        if (!ev->rep) {
            key_held[vk & 511] = 1;
            if (vk == VK_ESCAPE) { /* cancel */
                rebind_kind = 0;
                menu_status(menu_cur, tr("msg.cancelled"));
            } else if (vk == VK_DELETE) { /* leave the action unassigned */
                if (rebind_kind == 1)
                    kbd_set_bind(rebind_action, 0);
                else
                    pad_set_bind(rebind_action, 0);
                rebind_kind = 0;
                menu_status(menu_cur, tr("msg.unassigned"));
            } else if (rebind_kind == 1 && vk > 0 && vk < 256 && vk != VK_F11) { /* assign the key */
                kbd_set_bind(rebind_action, vk);
                char kn[64], t[160];
                key_name(kbd_norm_vk(vk), kn, sizeof kn);
                trf(t, sizeof t, "msg.assigned", key_act_label(rebind_action), kn);
                rebind_kind = 0;
                menu_status(menu_cur, t);
            }
            menu_drawn_sig[0] = 0;
        }
        return;
    }
    /* A key pressed with the menu open does not count as a game key press until it is released (otherwise accepting
     * with the left Ctrl, which is fire, would fire as soon as the menu closes). */
    key_held[vk & 511] = (uint8_t)down;
    if (!down)
        return;
    if (vk == VK_PRIOR)
        menu_nav(menu_cur, "tabprev");
    else if (vk == VK_NEXT)
        menu_nav(menu_cur, "tabnext");
    else if (vk == VK_TAB)
        menu_nav(menu_cur, (GetKeyState(VK_SHIFT) & 0x8000) ? "tabprev" : "tabnext");
    else if (vk == VK_UP)
        menu_nav(menu_cur, "up");
    else if (vk == VK_DOWN)
        menu_nav(menu_cur, "down");
    else if (vk == VK_LEFT)
        menu_nav(menu_cur, "left");
    else if (vk == VK_RIGHT)
        menu_nav(menu_cur, "right");
    else if (vk == VK_RETURN || vk == 0x10D /* numeric pad Enter */ || vk == VK_SPACE || vk == 'Z' ||
             vk == VK_RCONTROL || vk == VK_LCONTROL)
        menu_activate(menu_cur);
    else if (vk == VK_ESCAPE || vk == VK_BACK)
        menu_back();
    else if (vk == VK_F11) {
        fullscreen = !fullscreen;
        video_set_fullscreen(fullscreen);
    }
}

/* A key event while the game is running. */
static void key_in_game(const VEvent *ev) {
    int vk = ev->vk, down = ev->down;
    /* a key still held from the menu is ignored until it is released */
    if (key_held[vk & 511]) {
        if (!down)
            key_held[vk & 511] = 0;
        return;
    }
    if (down && vk == VK_ESCAPE) {
        open_menu();
        kbd_state = 0;
        return;
    }
    if (down && vk == VK_F11) {
        fullscreen = !fullscreen;
        video_set_fullscreen(fullscreen);
    }
    if (down && !ev->rep && vk == VK_F9) { /* reload the graphics pack */
        gfxpack_reload();
        present_pack_changed(presenter);
    }
    /* Page Up / Page Down: more / less emulation margin (latency) */
    if (down && (vk == VK_PRIOR || vk == VK_NEXT)) {
        double *margin = pflags.interp ? &lead_with_interp : &lead_without_interp;
        *margin += vk == VK_PRIOR ? 0.5 : -0.5;
        if (*margin < (pflags.interp ? 0.5 : 0.0))
            *margin = pflags.interp ? 0.5 : 0.0;
        if (*margin > 6)
            *margin = 6;
        apply_lead();
    }
    int joy_bit = kbd_joy_bit(vk), action = kbd_action(vk);
    if (joy_bit) {
        kbd_state = down ? (kbd_state | joy_bit) : (kbd_state & ~joy_bit);
        /* A joystick key that is also a letter (Z = second fire key): on the screens that take typing (the name in
         * the high-score table) it is typed too. */
        int scancode = kbd_scancode(vk);
        if (scancode && !ev->rep) {
            static uint8_t typed[512]; /* keys whose press was sent as a letter: their release must be sent too */
            if (down) {
                if (!strcmp(present_screen_kind(presenter), "other")) {
                    typed[vk & 511] = 1;
                    emu_post(EV_KEY, scancode, 1);
                }
            } else if (typed[vk & 511]) {
                typed[vk & 511] = 0;
                emu_post(EV_KEY, scancode, 0);
            }
        }
    } else if (action == KA_WNEXT) { /* the game's own key for the next weapon is A (scancode 0x1E) */
        if (!ev->rep)
            emu_post(EV_KEY, 0x1E, down);
    } else if (action == KA_WPREV) {
        if (down && !ev->rep)
            emu_post(EV_WPN, 0x1E, -1);
    } else if (action == KA_ROCKET) {
        if (down && !ev->rep)
            emu_post(EV_QUICK, 0x1E, 0);
    } else if (action == KA_GRENADE) {
        if (down && !ev->rep)
            emu_post(EV_QUICK, 0x1E, 1);
    } else {
        int scancode = kbd_scancode(vk);
        if (scancode && !ev->rep)
            emu_post(EV_KEY, scancode, down);
    }
}

/* All the events the window has queued. */
static void window_events(void) {
    VEvent ev;
    while (video_poll(&ev)) {
        if (ev.type == VE_QUIT)
            app_running = 0;
        else if (ev.type == VE_KEY) {
            if (menu_is_open(menu_cur))
                key_in_menu(&ev);
            else
                key_in_game(&ev);
        } else if (ev.type == VE_RESIZE)
            menu_drawn_sig[0] = 0; /* the menu image depends on the window size */
    }
}

/* ---------------------------------------------------------------------------------------------- pads */

/* Redefining a pad button: the first button pressed (not one that was already held) is assigned. Returns 1 while the
 * pads must not reach anything else (waiting for the button, or for every button to be released after assigning). */
static int pad_rebind(const PadRead *pr, int npads, int menu_open) {
    if (menu_open && rebind_kind == 2) {
        static int prev_raw[2]; /* buttons held in the previous frame */
        /* the XInput button bits that can be assigned: A B X Y, LB RB, LT RT (as PB_*), Back, Start, L3, R3 */
        static const int assignable[] = {XB_A,  XB_B,  XB_X,    XB_Y,     XB_LB, XB_RB,
                                         PB_LT, PB_RT, XB_BACK, XB_START, XB_L3, XB_R3};
        for (int i = 0; i < npads && i < 2 && rebind_kind == 2; i++) {
            int newly_pressed = rebind_prev_ok ? (pr[i].raw & ~prev_raw[i]) : 0;
            prev_raw[i] = pr[i].raw;
            for (size_t q = 0; q < sizeof assignable / sizeof *assignable; q++)
                if (newly_pressed & assignable[q]) {
                    pad_set_bind(rebind_action, assignable[q]);
                    char bn[32], t[160];
                    pad_button_name(assignable[q], bn, sizeof bn);
                    trf(t, sizeof t, "msg.assigned", pad_act_label(rebind_action), bn);
                    rebind_kind = 0;
                    rebind_wait_release = 1;
                    menu_status(menu_cur, t);
                    menu_drawn_sig[0] = 0;
                    break;
                }
        }
        rebind_prev_ok = 1;
        return 1;
    }
    if (rebind_wait_release) {
        int any = 0;
        for (int i = 0; i < npads && i < 2; i++)
            any |= pr[i].raw;
        if (any)
            return 1;
        rebind_wait_release = 0;
    }
    return 0;
}

/* The buttons of a pad that do something in the game: menu, weapons, rocket, grenade, exit the mission, and the fire
 * button on the "1 OR 2 PLAYER GAME?" screen (where it must act as the key 1 / 2). Each button acts when it goes from
 * released to pressed. *menu_open is updated because a button can open or close the menu. */
static void pad_actions(const PadRead *pr, int npads, double now, int *menu_open) {
    static int was_menu[2], was_wprev[2], was_rocket[2], was_grenade[2], was_fire[2], was_wnext[2], was_exit[2];
    for (int i = 0; i < npads; i++) {
        int menu_btn = !!(pr[i].acts & PA_MENU), wprev = !!(pr[i].acts & PA_WBACK), rocket = !!(pr[i].acts & PA_ROCKET),
            grenade = !!(pr[i].acts & PA_GRENADE), exit_btn = !!(pr[i].acts & PA_EXIT);
        int atari_key_a = i == 0 ? 0x1E : 0x4E; /* the key of each player's weapon change: A, and keypad 4 */
        if (menu_btn && !was_menu[i]) {
            if (*menu_open)
                close_menu();
            else
                open_menu();
            *menu_open = menu_is_open(menu_cur);
        }
        was_menu[i] = menu_btn;
        if (wprev && !was_wprev[i] && !*menu_open)
            emu_post(EV_WPN, atari_key_a, -1);
        was_wprev[i] = wprev;
        if (rocket && !was_rocket[i]) {
            if (*menu_open) { /* in the menu the rocket button (B) goes back */
                menu_back();
                *menu_open = menu_is_open(menu_cur);
            } else
                emu_post(EV_QUICK, atari_key_a, 0);
        }
        was_rocket[i] = rocket;
        if (grenade && !was_grenade[i] && !*menu_open)
            emu_post(EV_QUICK, atari_key_a, 1);
        was_grenade[i] = grenade;
        int fire = !!(pr[i].st & J_FIRE);
        if (fire && !was_fire[i] && !strcmp(present_screen_kind(presenter), "players") && !*menu_open &&
            n_key_releases < 8) {
            int key = i == 0 ? 0x02 : 0x03; /* the keys 1 and 2 */
            emu_post(EV_KEY, key, 1);
            key_release_at[n_key_releases] = now + 0.12;
            key_release_scancode[n_key_releases] = key;
            n_key_releases++;
        }
        was_fire[i] = fire;
        if (pr[i].wnext && !was_wnext[i])
            emu_post(EV_KEY, atari_key_a, 1);
        else if (!pr[i].wnext && was_wnext[i])
            emu_post(EV_KEY, atari_key_a, 0);
        was_wnext[i] = pr[i].wnext;
        if (exit_btn && !was_exit[i])
            emu_post(EV_EXIT, 0, 0);
        was_exit[i] = exit_btn;
    }
    /* release the keys pressed for the "1 OR 2 PLAYER GAME?" screen once their time is up */
    for (int k = 0; k < n_key_releases;) {
        if (now >= key_release_at[k]) {
            emu_post(EV_KEY, key_release_scancode[k], 0);
            key_release_at[k] = key_release_at[n_key_releases - 1];
            key_release_scancode[k] = key_release_scancode[n_key_releases - 1];
            n_key_releases--;
        } else
            k++;
    }
}

/* With the menu open the pad moves through it instead of reaching the game: direction = move (with auto-repeat),
 * fire = accept, LB / RB = previous / next tab. */
static void pad_menu_navigation(const PadRead *pr, int npads, double now) {
    static const struct {
        int bit;
        const char *name;
    } dirs[4] = {{J_UP, "up"}, {J_DOWN, "down"}, {J_LEFT, "left"}, {J_RIGHT, "right"}};
    static int prev_raw[2]; /* buttons of the previous frame (for LB / RB) */
    for (int i = 0; i < npads; i++) {
        int cur = pr[i].st;
        for (int d = 0; d < 4; d++)
            if (cur & dirs[d].bit) {
                if (!(menu_prev_dirs[i] & dirs[d].bit)) { /* first press: move, repeat after 0.40 s */
                    menu_nav(menu_cur, dirs[d].name);
                    menu_repeat_at[i] = now + 0.40;
                } else if (now >= menu_repeat_at[i]) { /* held: repeat every 0.09 s */
                    menu_nav(menu_cur, dirs[d].name);
                    menu_repeat_at[i] = now + 0.09;
                }
            }
        if ((cur & J_FIRE) && !(menu_prev_dirs[i] & J_FIRE))
            menu_activate(menu_cur);
        int raw = pr[i].raw;
        if ((raw & XB_LB) && !(prev_raw[i] & XB_LB))
            menu_nav(menu_cur, "tabprev");
        if ((raw & XB_RB) && !(prev_raw[i] & XB_RB))
            menu_nav(menu_cur, "tabnext");
        prev_raw[i] = raw;
        menu_prev_dirs[i] = cur;
    }
}

/* ---------------------------------------------------------------------------------------------- one frame */

/* Handles the window events and the pads of this frame and sends the resulting inputs to the emulation. now = the
 * time of the start of the frame. Returns 1 if the menu is open. */
int input_frame(double now) {
    window_events();

    PadRead pr[2];
    int npads = pads_read(pr);
    int menu_open = menu_is_open(menu_cur);
    if (pad_rebind(pr, npads, menu_open))
        npads = 0;

    /* "hold position" (LT) and the right stick (aim): with fast polling the thread sends them */
    if (!fast_input_active()) {
        for (int i = 0; i < 2; i++) {
            int hold = (i < npads && !menu_open) ? pr[i].hold : 0;
            if (hold != sent_hold[i]) {
                sent_hold[i] = hold;
                emu_post(EV_HOLD, i, hold);
            }
        }
        for (int i = 0; i < 2; i++) {
            int aim = menu_open ? 0 : pad_aim_code(pr, npads, i);
            if (aim != sent_aim[i]) {
                sent_aim[i] = aim;
                emu_post(EV_AIM, i, aim);
            }
        }
    }

    pad_actions(pr, npads, now, &menu_open);

    if (menu_open) {
        pad_menu_navigation(pr, npads, now);
        npads = 0; /* the pad does not reach the game */
    }

    /* The fire button used to accept in the menu does not fire until it is released. */
    if (menu_open) {
        for (int i = 0; i < 2; i++)
            pad_fire_blocked[i] = 1;
    } else
        for (int i = 0; i < npads && i < 2; i++) {
            if (pad_fire_blocked[i] && (pr[i].st & J_FIRE))
                pr[i].st &= ~J_FIRE;
            else
                pad_fire_blocked[i] = 0;
        }

    /* the joystick of each port: port 1 = keyboard + pad 1, port 0 = pad 2 */
    int joy1 = menu_open ? 0 : (kbd_state | (npads > 0 ? pr[0].st : 0)), joy0 = npads > 1 ? pr[1].st : 0;
    menu_open_flag = menu_open;
    if (resend_input) { /* something asks to send everything again (the menu closed, an option changed) */
        resend_input = 0;
        sent_joy1 = sent_joy0 = -1;
        sent_hold[0] = sent_hold[1] = -1;
        sent_aim[0] = sent_aim[1] = -1;
    }
    /* with fast input polling the polling thread sends the joystick (1 kHz) */
    if (!fast_input_active()) {
        if (joy1 != sent_joy1) {
            sent_joy1 = joy1;
            emu_post(EV_JOY, joy1, 0);
        }
        if (joy0 != sent_joy0) {
            sent_joy0 = joy0;
            emu_post(EV_JOY0, joy0, 0);
        }
    }
    return menu_open;
}
