/*
 * devtest.c - Scripted input for automatic tests (development only; nothing here runs unless DOW_TEST is set).
 *
 * The test scripts run the program unattended and look at its log or its screenshots, so the "player" is replaced by
 * environment variables. They are called once per frame with the seconds elapsed since the loop started:
 *
 *   (default)         the soldier walks up, left and right and fires, to see the game move
 *   DOW_NOINPUT=1     no input at all
 *   DOW_PUSH=bits     hold those joystick bits (J_*) all the time, firing every few frames
 *   DOW_JOYSEQ="t:bits,t:bits..."   joystick state (J_* bits) from second t on
 *   DOW_KEYS="t:scancode,..."       Atari keys pressed at second t for 0.25 s
 *   DOW_HOLD="a:b"                  "hold position" pressed between seconds a and b
 *   DOW_AIM=bits [DOW_AIM_FROM, DOW_AIM_TO]   right stick pushed in that direction between those seconds
 *   DOW_GOD_AT=t                    invulnerability from second t
 *   DOW_MSEQ="t:action,..."         drives the menu: open close up down left right enter tabnext tabprev
 *
 * The scripted mission of the twin-stick test (DOW_AUTOMISSION) runs inside the emulation thread, see emu.c.
 */
#include "app.h"

void devtest_frame(double et) {
    if (!getenv("DOW_TEST"))
        return;
    /* default input: walk up and fire, turning left and right now and then (25 steps per second) */
    int f = (int)(et * 25), want = J_UP | (((f / 30) % 6 == 1) ? J_LEFT : 0) | (((f / 30) % 6 == 3) ? J_RIGHT : 0) |
                                   ((f % 10) < 5 ? J_FIRE : 0);
    if (getenv("DOW_PUSH"))
        want = atoi(getenv("DOW_PUSH")) | ((f % 6) < 3 ? J_FIRE : 0);
    if (getenv("DOW_NOINPUT"))
        want = 0;
    /* "t:state,t:state,...": scripted joystick (seconds : bits) */
    if (getenv("DOW_JOYSEQ")) {
        static double times[128];
        static int states[128], n = -1;
        if (n < 0) {
            n = 0;
            char buf[1500];
            snprintf(buf, sizeof buf, "%s", getenv("DOW_JOYSEQ"));
            for (char *tok = strtok(buf, ","); tok && n < 128; tok = strtok(NULL, ",")) {
                double t;
                int v;
                if (sscanf(tok, "%lf:%i", &t, &v) == 2) {
                    times[n] = t;
                    states[n] = v;
                    n++;
                }
            }
        }
        want = 0;
        for (int i = 0; i < n; i++)
            if (et >= times[i])
                want = states[i];
    }
    if (et > 1.0)
        input_set_kbd_state(want);

    /* "t:scancode,...": Atari keys, each pressed for 0.25 s */
    const char *keys = getenv("DOW_KEYS");
    static int key_state[32]; /* 0 waiting, 1 pressed, 2 released */
    static double key_release[32];
    static int n_keys = -1;
    static double key_time[32];
    static int key_code[32];
    if (keys) {
        if (n_keys < 0) {
            n_keys = 0;
            char buf[400];
            snprintf(buf, sizeof buf, "%s", keys);
            for (char *tok = strtok(buf, ","); tok && n_keys < 32; tok = strtok(NULL, ",")) {
                double t;
                int sc;
                if (sscanf(tok, "%lf:%i", &t, &sc) == 2) {
                    key_time[n_keys] = t;
                    key_code[n_keys] = sc;
                    n_keys++;
                }
            }
        }
        for (int i = 0; i < n_keys; i++) {
            if (!key_state[i] && et >= key_time[i]) {
                key_state[i] = 1;
                key_release[i] = et + 0.25;
                emu_post(EV_KEY, key_code[i], 1);
            } else if (key_state[i] == 1 && et >= key_release[i]) {
                key_state[i] = 2;
                emu_post(EV_KEY, key_code[i], 0);
            }
        }
    }

    /* "a:b": "hold position" (LT) held between seconds a and b */
    if (getenv("DOW_HOLD")) {
        static int holding = 0;
        double from = 0, to = 0;
        sscanf(getenv("DOW_HOLD"), "%lf:%lf", &from, &to);
        int want_hold = et >= from && et < to;
        if (want_hold != holding) {
            holding = want_hold;
            emu_post(EV_HOLD, 0, want_hold);
        }
    }

    /* right stick (J_* bits) pushed between DOW_AIM_FROM and DOW_AIM_TO seconds */
    if (getenv("DOW_AIM")) {
        static int sent = -1;
        int bits = atoi(getenv("DOW_AIM"));
        double from = env_d("DOW_AIM_FROM", 0), to = env_d("DOW_AIM_TO", 1e9);
        int want_aim = (et >= from && et < to) ? (bits | (pad_aim_autofire ? 0x80 : 0)) : 0;
        if (want_aim != sent) {
            sent = want_aim;
            emu_post(EV_AIM, 0, want_aim);
        }
    }

    if (getenv("DOW_GOD_AT") && et > atof(getenv("DOW_GOD_AT")) && !god_mode) {
        god_mode = 1;
        emu_post(EV_GOD, 1, 0);
    }

    /* "t:action,...": drives the menu by script */
    if (getenv("DOW_MSEQ")) {
        static double times[64];
        static char actions[64][12];
        static int n = -1, done[64];
        if (n < 0) {
            n = 0;
            char buf[800];
            snprintf(buf, sizeof buf, "%s", getenv("DOW_MSEQ"));
            for (char *tok = strtok(buf, ","); tok && n < 64; tok = strtok(NULL, ",")) {
                char *colon = strchr(tok, ':');
                if (colon) {
                    *colon = 0;
                    times[n] = atof(tok);
                    snprintf(actions[n], 12, "%s", colon + 1);
                    n++;
                }
            }
        }
        for (int i = 0; i < n; i++)
            if (!done[i] && et >= times[i]) {
                done[i] = 1;
                const char *a = actions[i];
                if (!strcmp(a, "open"))
                    open_menu();
                else if (!strcmp(a, "close"))
                    close_menu();
                else if (!strcmp(a, "enter"))
                    menu_activate(menu_cur);
                else
                    menu_nav(menu_cur, a);
            }
    }
}
