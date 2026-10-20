/*
 * fastinput.c - Optional "fast input polling" (option of the Motion tab).
 *
 * Normally the window loop reads the pads and the keyboard once per image (every ~16 ms at 60 Hz) and blocks waiting
 * for the screen refresh. With the option on, this thread reads the XInput pads and the joystick keys every
 * millisecond and hands the state to the emulation as soon as it changes, so an input is never more than ~1 ms late.
 *
 * It follows the same rules as the window loop (game_input.c):
 *   - nothing is sent while the menu is open;
 *   - a key or pad button used to accept something in the menu does not count until it is released;
 *   - keyboard keys only count while the game window is in the foreground.
 * While the thread is active, the window loop does not send the joystick, hold or aim state itself.
 */
#include "app.h"

/* True when the thread must be the one that reads the inputs. Automatic tests drive the inputs themselves, so they
 * only use it when DOW_FASTIN_TEST is set. */
int fast_input_active(void) {
    return fast_input && (!getenv("DOW_TEST") || getenv("DOW_FASTIN_TEST"));
}

#ifdef _WIN32
static DWORD WINAPI fast_input_thread(LPVOID arg) {
    (void)arg;
    timeBeginPeriod(1);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    /* what was last sent to the emulation (-1 = nothing yet, so the next read is always sent) */
    int last_joy1 = -1, last_joy0 = -1, last_hold[2] = {-1, -1}, last_aim[2] = {-1, -1};
    while (app_running) {
        /* idle: the option is off, the menu is open, or the window loop has not started yet */
        if (!fast_input_active() || menu_open_flag || !input_ready) {
            last_joy1 = last_joy0 = -1;
            last_hold[0] = last_hold[1] = -1;
            last_aim[0] = last_aim[1] = -1;
            Sleep(fast_input_active() ? 1 : 20);
            continue;
        }
        PadRead pad[2];
        int npads = pads_read(pad);
        /* joystick bits of the keyboard keys (arrows, fire keys) */
        int kbd = 0;
        if (getenv("DOW_FASTIN_TEST") || GetForegroundWindow() == (HWND)video_hwnd()) {
            int keys[16], nkeys = kbd_joy_keys(keys);
            for (int i = 0; i < nkeys; i++)
                if ((GetAsyncKeyState(keys[i]) & 0x8000) && !key_held[keys[i] & 511])
                    kbd |= kbd_joy_bit(keys[i]);
        }
        int st[2] = {0, 0};
        for (int i = 0; i < npads && i < 2; i++) {
            st[i] = pad[i].st;
            if (pad_fire_blocked[i] && (st[i] & J_FIRE))
                st[i] &= ~J_FIRE;
        }
        int joy1 = kbd | st[0], joy0 = st[1]; /* the keyboard and pad 1 are player 1; pad 2 is player 2 */
        for (int i = 0; i < 2; i++) {
            int hold = i < npads ? pad[i].hold : 0;
            if (hold != last_hold[i]) {
                last_hold[i] = hold;
                emu_post(EV_HOLD, i, hold);
            }
            int aim = pad_aim_code(pad, npads, i);
            if (aim != last_aim[i]) {
                last_aim[i] = aim;
                emu_post(EV_AIM, i, aim);
            }
        }
        if (joy1 != last_joy1) {
            last_joy1 = joy1;
            emu_post(EV_JOY, joy1, 0);
            if (getenv("DOW_FASTIN_LOG"))
                host_log_file("fastin j1=%02X t=%.4f", joy1, now_s());
        }
        if (joy0 != last_joy0) {
            last_joy0 = joy0;
            emu_post(EV_JOY0, joy0, 0);
        }
        Sleep(1);
    }
    return 0;
}
#endif

void fast_input_start(void) {
#ifdef _WIN32
    CloseHandle(CreateThread(NULL, 0, fast_input_thread, NULL, 0, NULL));
#endif
}
