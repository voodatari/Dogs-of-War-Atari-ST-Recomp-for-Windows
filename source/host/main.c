/*
 * main.c - Dogs of War, native port: start-up, the window loop, shutdown.
 *
 * Threads of the program:
 *   - this one (window thread): owns the window, reads the inputs, builds and draws one image per monitor refresh;
 *   - the emulation thread (emu.c): runs the game's code at 50 VBL per second and publishes what it draws;
 *   - the audio device's thread (audioout.c) and, optionally, the fast-input thread (fastinput.c).
 *
 * Start-up order matters: the options are read before the window opens (they decide the shader and the audio), the
 * graphics dump is made before the presenter needs it, and the menu is built after the video library knows which
 * shaders exist. See main() for the sequence; the rest of the program is described in app.h.
 */
#include "app.h"

/* ---------------------------------------------------------------------------------------------- start-up */

/* Options of the command line: --original / --no-fix / --fix (enemy-script fix), --no-sound / --mute, --volume x,
 * --pack name (graphics pack). *fix_cli = -1 when the fix was not given, else 0 / 1. Some DOW_* variables do the same
 * for the automatic tests. */
static void parse_command_line(int argc, char **argv, int *no_sound, int *fix_cli) {
    *no_sound = 0;
    *fix_cli = -1;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--original") || !strcmp(a, "--no-fix"))
            *fix_cli = 0;
        else if (!strcmp(a, "--fix"))
            *fix_cli = 1;
        else if (!strcmp(a, "--no-sound") || !strcmp(a, "--mute"))
            *no_sound = 1;
        else if (!strcmp(a, "--volume") && i + 1 < argc)
            master_volume = atof(argv[++i]);
        else if (!strcmp(a, "--pack") && i + 1 < argc)
            snprintf(pack_from_cmdline, sizeof pack_from_cmdline, "%s", argv[++i]);
    }
    if (getenv("DOW_FIX") && getenv("DOW_FIX")[0] == '0')
        *fix_cli = 0;
    if (getenv("DOW_SONIDO") && getenv("DOW_SONIDO")[0] == '0')
        *no_sound = 1;
    if (getenv("DOW_VOL"))
        master_volume = atof(getenv("DOW_VOL"));
    if (getenv("DOW_GRAFICO"))
        snprintf(pack_from_cmdline, sizeof pack_from_cmdline, "%s", getenv("DOW_GRAFICO"));
}

/* Development shortcuts that do one job and exit: DOW_BOOT_OUT (write the state the original loader leaves) and
 * DOW_DUMP_OUT (make the graphics dump in that folder). Returns -1 to go on, else the exit code of the program. */
static int development_tools(void) {
    if (getenv("DOW_BOOT_OUT")) {
        State *bs = boot_state_make(game_data_dir);
        int ok = bs && state_write_file(bs, getenv("DOW_BOOT_OUT"));
        host_log("start-up state: %s", ok ? "written" : "FAILED");
        return ok ? 0 : 1;
    }
    if (getenv("DOW_DUMP_OUT")) {
        double t0 = now_s();
        int ok = 0;
        if (!dump_begin(game_data_dir, getenv("DOW_DUMP_OUT")))
            return 1;
        while (!dump_finished(&ok))
            Sleep(100);
        host_log("graphics dump: %s in %.1f s", ok ? "ok" : "FAILED", now_s() - t0);
        return ok ? 0 : 1;
    }
    return -1;
}

/* Monitor refresh rate (50..165) and the scale at which the game image is rendered (DOW_SCALE overrides it). */
static void choose_display_size(void) {
    monitor_hz = video_refresh_hz();
    if (monitor_hz < 50)
        monitor_hz = 50;
    if (monitor_hz > 165)
        monitor_hz = 165;
    render_scale = (int)env_d("DOW_SCALE", monitor_hz <= 75 ? 3 : 2);
    if (render_scale < 1)
        render_scale = 1;
    if (render_scale > RS_MAX)
        render_scale = RS_MAX;
}

/* Opens the window. The video library is opened twice: first without shaders, only to learn whether OpenGL works
 * (a message box explains if not), then with the shader folder. Returns 0 on failure. */
static int open_video(int test_run) {
    int gl_ok = video_open(320 * render_scale, 200 * render_scale, 320 * render_scale, 200 * render_scale, "");
    char shaders_dir[700];
    snprintf(shaders_dir, sizeof shaders_dir, "%s/shaders", app_dir_path);
    if (!gl_ok) {
        MessageBoxA(NULL, "OpenGL is not available.", "Dogs of War", MB_OK | MB_ICONERROR);
        return 0;
    }
    video_close();
    gl_ok = video_open(320 * render_scale, 200 * render_scale, 320 * render_scale, 200 * render_scale, shaders_dir);
    if (!gl_ok)
        return 0;
    apply_video_cfg();
    /* fullscreen unless the options say otherwise (automatic tests start in a window) */
    if (cfg_bool("fullscreen", !test_run)) {
        fullscreen = 1;
        video_set_fullscreen(1);
    }
    return 1;
}

/* Opens the sound device (the target delay depends on the emulation margin) and applies the low-latency option. */
static void open_audio(void) {
    if (sound_on || cfg_bool("sound", 1))
        audio_out = audio_open(AUDIO_FS, (lead + 1.0) * 20.0 + 30.0);
    if (audio_out && low_latency_audio && !audio_set_lowlat(audio_out, 1))
        low_latency_audio = 0;
    if (audio_out)
        audio_set_target(audio_out, aud_target_ms());
    /* (DOW_FAKEAUDIO: tests without a sound device) */
    if (!audio_out && !getenv("DOW_FAKEAUDIO"))
        sound_on = 0;
}

/* First run: the graphics dump (assets/original) does not exist yet. The program plays the game by itself, with
 * scripted input, and writes it; it takes a few seconds and the window shows a message meanwhile. */
static void make_graphics_dump_if_missing(void) {
    char index[700];
    snprintf(index, sizeof index, "%s/original/index.json", assets_root);
    if (getenv("DOW_STATE") || getenv("DOW_NODUMP") || file_exists(index))
        return;
    char out[700];
    snprintf(out, sizeof out, "%s/original", assets_root);
    video_title("Dogs of War - preparing the graphics (first run, a few seconds)...");
    int ok = 0;
    if (dump_begin(game_data_dir, out)) {
        while (!dump_finished(&ok)) {
            VEvent ev;
            while (video_poll(&ev)) { /* keep the window alive */
            }
            Sleep(30);
        }
    }
    host_log("graphics dump: %s", ok ? "ready" : "FAILED (graphics packs will not be available)");
}

/* Starts the emulation thread and sends it the options that are not part of its configuration. Returns 0 on failure
 * (a message box explains). */
static int start_emulation(void) {
    /* time the original loader's copyright text stays on screen (not during automatic tests) */
    g_load_seconds = cfg_num("loading_seconds", 3.0);
    if (g_load_seconds < 0)
        g_load_seconds = 0;
    if (g_load_seconds > 30)
        g_load_seconds = 30;
    if (getenv("DOW_TEST") && !getenv("DOW_LOAD_WAIT"))
        g_load_seconds = 0;

    char music_dir[700];
    pack_dir(music_pack, music_dir, sizeof music_dir);
    EmuConfig ec;
    memset(&ec, 0, sizeof ec);
    ec.game_dir = game_data_dir;
    ec.state_path = getenv("DOW_STATE"); /* tests can start from a saved state */
    ec.music_dir = music_dir;
    ec.save_dir = save_dir;
    ec.audio = audio_out;
    ec.lead = lead;
    ec.fix = script_fix;
    ec.god = getenv("DOW_GOD") != NULL;
    ec.money = getenv("DOW_MONEY") != NULL;
    ec.sound = sound_on;
    ec.mp3 = mp3_on;
    ec.multi = multichannel_fx;
    ec.pan = fx_pan;
    ec.vol = master_volume;
    ec.t_anchor = now_s() + 0.5;
    god_mode = ec.god;
    infinite_money = ec.money;
    if (!emu_start(&ec)) {
        MessageBoxA(NULL, "Could not start the game from the original files in the data folder.", "Dogs of War",
                    MB_OK | MB_ICONERROR);
        return 0;
    }
    emu_post(EV_GAMEHZ, game_rate(), 0);
    fast_input_start();
    for (int i = 0; i < 4; i++)
        emu_post(EV_VOLX, i, part_volume[i]);
    emu_post(EV_HZ, monitor_hz, 0);
    return 1;
}

/* ---------------------------------------------------------------------------------------------- the window loop */

/* Timing of the loop: what is measured to keep the pace and what the automatic tests report. */
typedef struct {
    double t_start; /* when the loop started */
    double vs;      /* expected time of the next screen refresh (see pace_frame) */
    int have_vs;
    double tick_t;        /* when the previous frame ended (frame limiter without vsync) */
    double t_prev;        /* ... and the one before, for the frame interval */
    double intervals[64]; /* the last frame intervals in ms (to find out whether vsync works) */
    int n_intervals;
    int vsync_ok;      /* -1 not known yet, 0 the swap does not wait for the refresh, 1 it does */
    int vsync_samples; /* frames since the video mode last changed */
    int frames;
    double prof[5];  /* seconds spent: presenter, upload, drawing, waiting for the map, idle (tests report them) */
    uint8_t *frozen; /* the game image kept while the menu is open (the game is paused behind it) */
    int shot_n;      /* screenshots taken so far (DOW_SHOTS) */
    double next_shot;
} Loop;

/* Draws one frame: the game image (or the frozen one if the menu is open), the menu over it, screenshots. */
static void draw_frame(Loop *lp, int menu_open, double elapsed) {
    if (!menu_open)
        emu_post(EV_TICK, 0, 0); /* lets the emulation run one more step in "tick" mode */
    if (!menu_open)
        present_drain(presenter);
    /* on the world map the presenter is given a moment to finish the image of the menu-like screens */
    if (!menu_open && !strcmp(present_screen_kind(presenter), "map")) {
        double tq = now_s();
        present_wait_menu(presenter, 0.006);
        lp->prof[3] += now_s() - tq;
    }
    if (!lp->have_vs) {
        lp->vs = now_s() + 1.0 / monitor_hz;
        lp->have_vs = 1;
    }
    double show_at = lp->vs + 0.5 / monitor_hz; /* the time this image will be on screen */
    if (menu_open)
        present_drain(presenter);
    {
        char txt[256];
        double t;
        if (present_take_text(presenter, txt, sizeof txt, &t)) { /* a message from the emulation thread */
            menu_status(menu_cur, txt);
            menu_drawn_sig[0] = 0;
        }
    }

    const uint8_t *img;
    int is_menu_img = 0;
    size_t image_bytes = (size_t)320 * 200 * render_scale * render_scale * 3;
    if (menu_open && lp->frozen)
        img = lp->frozen;
    else {
        double tq = now_s();
        img = present_frame(presenter, show_at, &is_menu_img);
        lp->prof[0] += now_s() - tq;
        if (menu_open) { /* keep the last image while the menu is open */
            lp->frozen = (uint8_t *)malloc(image_bytes);
            memcpy(lp->frozen, img, image_bytes);
            img = lp->frozen;
        }
    }
    if (!menu_open && lp->frozen) {
        free(lp->frozen);
        lp->frozen = NULL;
    }
    {
        double tq = now_s();
        video_upload(img);
        lp->prof[1] += now_s() - tq;
    }

    /* the menu is drawn into an overlay only when what it shows changes */
    int rx, ry, rw, rh;
    video_rect(&rx, &ry, &rw, &rh);
    if (menu_open) {
        char sig[600];
        menu_signature(menu_cur, rw, rh, sig, sizeof sig);
        if (strcmp(sig, menu_drawn_sig)) {
            snprintf(menu_drawn_sig, sizeof menu_drawn_sig, "%s", sig);
            uint8_t *overlay = menu_draw(menu_cur, rw, rh);
            video_overlay(overlay, rw, rh);
            free(overlay);
            overlay_on = 1;
        }
    } else if (overlay_on) {
        overlay_on = 0;
        menu_drawn_sig[0] = 0;
        video_overlay(NULL, 0, 0);
    }

    /* DOW_SHOTS=folder: screenshots, every DOW_SHOT_EVERY seconds or (without it) 40 of them after DOW_SHOT_T seconds
     */
    const char *shots = getenv("DOW_SHOTS");
    double shot_every = env_d("DOW_SHOT_EVERY", 0);
    if (shots && (shot_every > 0 ? elapsed >= lp->next_shot : (elapsed > env_d("DOW_SHOT_T", 8) && lp->shot_n < 40))) {
        char path[800];
        if (shot_every > 0) {
            snprintf(path, sizeof path, "%s/t%07d.png", shots, (int)(elapsed * 1000));
            lp->next_shot += shot_every;
        } else
            snprintf(path, sizeof path, "%s/f%02d.png", shots, lp->shot_n++);
        video_screenshot(path);
    }

    {
        double tq = now_s();
        video_draw();
        lp->prof[2] += now_s() - tq;
    }
    lp->frames++;
}

/* Keeps the pace of the loop at the monitor's refresh rate. The swap normally waits for the refresh (vsync); the
 * time of the next refresh (lp->vs) is followed with a slow correction. If the intervals show that the swap does not
 * wait (vsync_ok = 0) the loop sleeps instead. Returns the length of the last frame in seconds. */
static double pace_frame(Loop *lp) {
    double now = now_s(), interval = now - lp->t_prev;
    lp->t_prev = now;
    if (lp->n_intervals < 64)
        lp->intervals[lp->n_intervals++] = interval * 1000.0;
    else {
        memmove(lp->intervals, lp->intervals + 1, sizeof(double) * 63);
        lp->intervals[63] = interval * 1000.0;
    }
    double err = now - lp->vs;
    if (fabs(err) > 0.5 / monitor_hz)
        lp->vs = now + 1.0 / monitor_hz; /* lost the refresh: start again from now */
    else
        lp->vs = lp->vs + 1.0 / monitor_hz + 0.05 * err;

    /* another video mode (window / fullscreen) may synchronise differently: measure again */
    {
        static int fullscreen_seen = -1;
        int fs = video_is_fullscreen();
        if (fs != fullscreen_seen) {
            fullscreen_seen = fs;
            lp->vsync_ok = -1;
            lp->vsync_samples = 0;
            lp->n_intervals = 0;
        }
        lp->vsync_samples++;
    }
    if (lp->vsync_ok < 0 && lp->vsync_samples > 90) { /* median of the last intervals: about the refresh period? */
        double sorted[64];
        memcpy(sorted, lp->intervals, sizeof(double) * (size_t)lp->n_intervals);
        for (int a = 1; a < lp->n_intervals; a++) { /* insertion sort */
            double x = sorted[a];
            int b = a - 1;
            while (b >= 0 && sorted[b] > x) {
                sorted[b + 1] = sorted[b];
                b--;
            }
            sorted[b + 1] = x;
        }
        lp->vsync_ok = sorted[lp->n_intervals / 2] > 0.8 * 1000.0 / monitor_hz;
    }
    /* without real vsync: limit to the monitor frequency (like a clock tick) */
    if (lp->vsync_ok == 0) {
        double rest = lp->tick_t + 1.0 / monitor_hz - now_s();
        if (rest > 0) {
            double tq = now_s();
            sleep_ms(rest * 1000.0);
            lp->prof[4] += now_s() - tq;
        }
    }
    lp->tick_t = now_s();
    return interval;
}

/* The loop: one iteration per monitor refresh until the window is closed, the emulation stops, an error is reported
 * or (tests) DOW_SECONDS have passed. */
static void run_window_loop(Loop *lp) {
    double limit = env_d("DOW_SECONDS", 0);
    lp->t_start = lp->tick_t = lp->t_prev = now_s();
    lp->vsync_ok = -1;
    lp->next_shot = env_d("DOW_SHOT_FROM", env_d("DOW_SHOT_EVERY", 0));
    input_ready = 1;
    while (app_running && emu_running) {
        double now = now_s(), elapsed = now - lp->t_start;
        devtest_frame(elapsed);
        if (limit > 0 && elapsed > limit) {
            if (getenv("DOW_SHOT") && !getenv("DOW_SHOTS"))
                video_screenshot(getenv("DOW_SHOT"));
            break;
        }
        int menu_open = input_frame(now);
        draw_frame(lp, menu_open, elapsed);

        const char *video_error = video_take_error();
        if (video_error)
            host_log("%s", video_error);
        const char *emu_error = present_error(presenter);
        if (emu_error) { /* the emulation found something it cannot continue from */
            host_log("Emulation ERROR: %s", emu_error);
            video_title("Dogs of War - EMULATION ERROR (see error.log next to the program)");
            sleep_ms(4000);
            break;
        }
        double interval = pace_frame(lp);
        if (lp->frames % 120 == 0) { /* the title shows the frame rate */
            char title[200];
            snprintf(title, sizeof title, "Dogs of War - %.0f fps%s%s%s", 1.0 / (interval > 0 ? interval : 1),
                     god_mode ? "  [GOD MODE]" : "", script_fix ? "  [FIX]" : "", infinite_money ? "  [MONEY]" : "");
            video_title(title);
        }
    }
}

/* ---------------------------------------------------------------------------------------------- shutdown */

/* What the automatic tests print at the end: CPU time, where the loop spent its time, frames and audio gaps. */
static void report_test_statistics(const Loop *lp, uint32_t vbl_start) {
#ifdef _WIN32
    FILETIME created, exited, kernel, user;
    GetProcessTimes(GetCurrentProcess(), &created, &exited, &kernel, &user);
    double cpu = (((uint64_t)kernel.dwHighDateTime << 32 | kernel.dwLowDateTime) +
                  ((uint64_t)user.dwHighDateTime << 32 | user.dwLowDateTime)) /
                 1e7;
    GetThreadTimes(GetCurrentThread(), &created, &exited, &kernel, &user);
    host_log("main thread: CPU %.1f s (kernel %.1f s)",
             (((uint64_t)kernel.dwHighDateTime << 32 | kernel.dwLowDateTime) +
              ((uint64_t)user.dwHighDateTime << 32 | user.dwLowDateTime)) /
                 1e7,
             ((uint64_t)kernel.dwHighDateTime << 32 | kernel.dwLowDateTime) / 1e7);
    double seconds = now_s() - lp->t_start;
    host_log("test: presenter %.1f s, upload %.1f s, drawing %.1f s, screen wait %.1f s, idle %.1f s (hz %d, vsync %d)",
             lp->prof[0], lp->prof[1], lp->prof[2], lp->prof[3], lp->prof[4], monitor_hz, lp->vsync_ok);
    host_log("test: %d frames in %.1f s (%.1f fps), CPU %.1f s, %u game VBL (%.1f per second)", lp->frames, seconds,
             lp->frames / seconds, cpu, tos.vbl - vbl_start, (tos.vbl - vbl_start) / seconds);
#else
    (void)lp;
    (void)vbl_start;
#endif
}

/* Audio statistics: in a test the full report goes to the log; in normal use only problems are written, to error.log.
 */
static void report_audio_statistics(void) {
    int underruns, drops;
    long played;
    if (!audio_out)
        return;
    audio_stats(audio_out, &underruns, &drops, &played);
    if (getenv("DOW_TEST"))
        host_log("audio: %d gaps, %d drops, %ld samples; device %.0f ms + buffer %.0f ms (target %.0f ms) = %.0f ms "
                 "delay; emulation margin %.1f VBL",
                 underruns, drops, played, audio_device_ms(audio_out), audio_level_ms(audio_out), aud_target_ms(),
                 audio_device_ms(audio_out) + audio_level_ms(audio_out), lead);
    else if (underruns || drops)
        host_log_file("audio: %d gaps, %d buffer trims in this session", underruns, drops);
}

/* ---------------------------------------------------------------------------------------------- main */

int main(int argc, char **argv) {
    set_paths();
    int no_sound, fix_cli;
    script_fix = 1;
    parse_command_line(argc, argv, &no_sound, &fix_cli);
    if (!find_game_dir()) {
        MessageBoxA(NULL,
                    "The original game file was not found.\nCopy the FILES\\DOW file of the original Dogs of War into "
                    "the \"data\\FILES\" folder next to dowport.exe.",
                    "Dogs of War", MB_OK | MB_ICONERROR);
        return 1;
    }
    {
        char languages[800];
        snprintf(languages, sizeof languages, "%s/languages", game_data_dir);
        lang_init(languages);
    }
    int exit_code = development_tools();
    if (exit_code >= 0)
        return exit_code;

    choose_display_size();
    int test_run = getenv("DOW_TEST") || getenv("DOW_SECONDS");
    load_options(fix_cli, no_sound);
    if (!open_video(test_run))
        return 1;
    open_audio();
    presenter = present_new(render_scale);
    present_set_ahead(presenter, 0.6 * 60.0 / monitor_hz);
    make_graphics_dump_if_missing();
    video_title("Dogs of War");
    /* graphics packs (sets of replacement sheets), then the menu, which lists them */
    int have_pack = gfxpack_init(assets_root, render_scale,
                                 pack_from_cmdline[0] ? pack_from_cmdline : cfg_str("graphics_pack", ""));
    gfxpack_attach();
    menu_build(audio_out != NULL, !video_is_software(), have_pack);
    if (!start_emulation())
        return 1;

    Loop loop;
    memset(&loop, 0, sizeof loop);
    uint32_t vbl_start = tos.vbl;
    run_window_loop(&loop);

    emu_post(EV_QUIT, 0, 0);
    emu_join();
    if (getenv("DOW_ATLAS")) {
        extern void scene_dump_atlas(void);
        scene_dump_atlas();
    }
    save_options();
    if (getenv("DOW_TEST"))
        report_test_statistics(&loop, vbl_start);
    report_audio_statistics();
    if (audio_out)
        audio_close(audio_out);
    video_close();
    return 0;
}
