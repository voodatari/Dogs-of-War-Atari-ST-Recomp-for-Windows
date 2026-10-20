/*
 * app.c - The state shared by the window-thread code (see app.h), and the options: where they are read from, how
 * they are saved, and the few calculations that depend on several of them.
 *
 * Options live in options.json next to the executable. When it does not exist yet the file of earlier versions
 * (opciones.json) or, failing that, default.json (the factory values) is read. The file is rewritten when the menu
 * closes and at exit, but never during an automatic test (DOW_TEST).
 */
#include "app.h"

/* ---- the objects of the running program ---- */
Presenter *presenter;
AudioOut *audio_out;
Menu *menu_options, *menu_controls, *menu_cur;
volatile int app_running = 1;

/* ---- display ---- */
int render_scale = 3;
int monitor_hz = 60;
int fullscreen = 0;
int overlay_on = 0;
char menu_drawn_sig[600] = "";

/* ---- options chosen by the user ---- */
int sound_on = 1, mp3_on = 1, multichannel_fx = 0, fx_pan = 0;
double master_volume = 1.0;
double part_volume[4] = {1, 1, 1, 1};
int script_fix = 1;
int god_mode = 0, infinite_money = 0;
int save_slot = 1;
int fast_input = 0;
int low_latency_audio = 0;
double audio_trim_ms = 0.0;
int match_refresh = 0;
double lead = LEAD_DEFAULT;
double lead_with_interp = LEAD_DEFAULT, lead_without_interp = 0.5;
int gfx_pack_chosen = 0;
char music_pack[80] = "arrange";

/* ---- folders and files ---- */
char app_dir_path[600], options_path[700], defaults_path[700], legacy_options_path[700];
char game_data_dir[600], assets_root[700], save_dir[700], music_root[700];
char pack_from_cmdline[64] = "";

double env_d(const char *name, double def) {
    const char *v = getenv(name);
    return v ? atof(v) : def;
}

/* ---------------------------------------------------------------------------------------------- folders */

/* The folders and files of the program, all relative to the executable (assets can be moved with DOW_ASSETS_DIR). */
void set_paths(void) {
    snprintf(app_dir_path, sizeof app_dir_path, "%s", app_dir());
    snprintf(options_path, sizeof options_path, "%s/options.json", app_dir_path);
    snprintf(defaults_path, sizeof defaults_path, "%s/default.json", app_dir_path);
    snprintf(legacy_options_path, sizeof legacy_options_path, "%s/opciones.json", app_dir_path);
    snprintf(music_root, sizeof music_root, "%s/music", app_dir_path);
    snprintf(save_dir, sizeof save_dir, "%s/saves", app_dir_path);
    const char *ad = getenv("DOW_ASSETS_DIR");
    if (ad)
        snprintf(assets_root, sizeof assets_root, "%s", ad);
    else
        snprintf(assets_root, sizeof assets_root, "%s/assets", app_dir_path);
}

/* The original game file FILES/DOW must be in the "data" folder next to the executable. */
int find_game_dir(void) {
    char p[800];
    snprintf(p, sizeof p, "%s/data/FILES/DOW", app_dir_path);
    if (file_exists(p))
        snprintf(game_data_dir, sizeof game_data_dir, "%s/data", app_dir_path);
    else
        game_data_dir[0] = 0;
    return game_data_dir[0] != 0;
}

/* ---------------------------------------------------------------------------------------------- loading */

/* Reads the options file into the variables of this module and of the libraries it configures (language, controls,
 * music pack, picture flags...). fix_cli: -1 = not given on the command line, else 0 / 1 (the enemy-script fix);
 * no_sound: --no-sound was given. Environment variables of the form DOW_* override some options (they exist for tests
 * and are not documented for players). */
void load_options(int fix_cli, int no_sound) {
    /* without options.json: the one from earlier versions (opciones.json) or, failing that, default.json */
    if (file_exists(options_path))
        cfg_load(options_path);
    else if (file_exists(legacy_options_path))
        cfg_load(legacy_options_path);
    else
        cfg_load(defaults_path);
    lang_set(cfg_str("language", "en"));
    input_cfg_load();
    scan_packs();
    pack_resolve(cfg_str("music_pack", "arrange"));
    /* options of version 1 files did not use these keys the same way */
    if ((int)cfg_num("version", 1) < 2) {
        cfg_remove("preset");
        cfg_remove("external");
        cfg_remove("filter");
        cfg_remove("aspect");
        cfg_remove("fullscreen");
    }
    /* enemy-script fix: only in options.json (on by default) or by the command line */
    script_fix = fix_cli >= 0 ? fix_cli : cfg_bool("enemy_script_fix", 1);
    sound_on = cfg_bool("sound", 1) && !no_sound;
    if (!getenv("DOW_VOL") && cfg_has("volume") && master_volume == 1.0)
        master_volume = cfg_num("volume", 1.0);
    mp3_on = cfg_bool("mp3_music", 1);
    multichannel_fx = cfg_bool("multichannel_sfx", 0);
    fx_pan = cfg_bool("stereo_pan", 0);
    pflags.subpx = cfg_bool("subpixel_smoothing", 1);
    pflags.sfix = cfg_bool("scroll_fix", 1) && !getenv("DOW_NOSFIX");
    pflags.interp = cfg_bool("frame_interpolation", 1) && !getenv("DOW_NOINTERP");
    match_refresh = cfg_bool("game_speed_match", 0) || getenv("DOW_SPEEDMATCH") != NULL;
    pflags.extrap = cfg_bool("extrapolate", 1);
    pflags.xfilter = cfg_bool("filter_x", 1);
    pflags.yfilter = cfg_bool("filter_y", 1);
    pflags.res = cfg_bool("residual_layer", 1);
    if (cfg_has("latency") && !getenv("DOW_LEAD")) {
        lead_with_interp = cfg_num("latency", LEAD_DEFAULT);
        if (lead_with_interp < 0.5)
            lead_with_interp = 0.5;
        if (lead_with_interp > 6)
            lead_with_interp = 6;
    }
    fast_input = cfg_bool("fast_input", 0);
    low_latency_audio = cfg_bool("low_latency_audio", 0);
    audio_trim_ms = cfg_num("audio_sync_trim_ms", 0.0);
    if (getenv("DOW_FASTIN"))
        fast_input = getenv("DOW_FASTIN")[0] != '0';
    lead_without_interp = cfg_num("latency_nointerp", 0.5);
    if (lead_without_interp < 0.0)
        lead_without_interp = 0.0;
    if (lead_without_interp > 6)
        lead_without_interp = 6;
    lead = pflags.interp ? lead_with_interp : lead_without_interp;
    if (getenv("DOW_LEAD")) {
        lead_with_interp = atof(getenv("DOW_LEAD"));
        lead = pflags.interp ? lead_with_interp : lead_without_interp;
    }
    part_volume[0] = cfg_num("volume_mp3", 1.0);
    part_volume[1] = cfg_num("volume_chip_music", 1.0);
    part_volume[2] = cfg_num("volume_samples", 1.0);
    part_volume[3] = cfg_num("volume_chip_fx", 1.0);
    for (int i = 0; i < 4; i++) {
        if (part_volume[i] < 0)
            part_volume[i] = 0;
        if (part_volume[i] > 1)
            part_volume[i] = 1;
    }
}

/* Video options (needs the video library to be open, because the external shaders are listed from its folder). */
void apply_video_cfg(void) {
    const char *pn = cfg_str("preset", "Soft CRT");
    video_use_external("");
    if (!strcmp(pn, "Custom"))
        snprintf(vcfg.preset, sizeof vcfg.preset, "Custom");
    else
        video_set_preset(pn);
    vcfg.filter = (int)cfg_num("filter", vcfg.filter);
    vcfg.src_mode = (int)cfg_num("shader_source", 0);
    if (vcfg.src_mode < 0 || vcfg.src_mode > 2)
        vcfg.src_mode = 0;
    vcfg.aspect = (int)cfg_num("aspect", 1);
    if (vcfg.filter < 0 || vcfg.filter > 2)
        vcfg.filter = 1;
    if (vcfg.aspect < 0 || vcfg.aspect > 3)
        vcfg.aspect = 1;
    if (cfg_has("params.p_scan")) {
        vcfg.p.p_scan = (float)cfg_num("params.p_scan", vcfg.p.p_scan);
        vcfg.p.p_mask = (float)cfg_num("params.p_mask", vcfg.p.p_mask);
        vcfg.p.p_curv = (float)cfg_num("params.p_curv", vcfg.p.p_curv);
        vcfg.p.p_bright = (float)cfg_num("params.p_bright", vcfg.p.p_bright);
        vcfg.p.p_vig = (float)cfg_num("params.p_vig", vcfg.p.p_vig);
        vcfg.p.p_bloom = (float)cfg_num("params.p_bloom", vcfg.p.p_bloom);
    }
    const char *ex = cfg_str("external", "");
    for (int i = 0; i < video_external_count(); i++)
        if (!strcmp(video_external_name(i), ex))
            video_use_external(ex);
}

/* ---------------------------------------------------------------------------------------------- saving */

/* Writes every option to options.json. Automatic tests do not touch it. */
void save_options(void) {
    if (getenv("DOW_TEST"))
        return;
    cfg_set_num("version", 3);
    cfg_set_str("language", lang_current());
    cfg_set_bool("fullscreen", fullscreen);
    cfg_set_bool("sound", sound_on);
    cfg_set_num("volume", master_volume);
    cfg_set_num("volume_mp3", part_volume[0]);
    cfg_set_num("volume_chip_music", part_volume[1]);
    cfg_set_num("volume_samples", part_volume[2]);
    cfg_set_num("volume_chip_fx", part_volume[3]);
    cfg_set_bool("mp3_music", mp3_on);
    cfg_set_bool("multichannel_sfx", multichannel_fx);
    cfg_set_bool("stereo_pan", fx_pan);
    cfg_set_bool("subpixel_smoothing", pflags.subpx);
    cfg_set_bool("scroll_fix", pflags.sfix);
    cfg_set_bool("frame_interpolation", pflags.interp);
    cfg_set_num("latency_nointerp", lead_without_interp);
    cfg_set_bool("game_speed_match", match_refresh);
    cfg_set_bool("enemy_script_fix", script_fix);
    cfg_set_num("latency", lead_with_interp);
    cfg_set_bool("fast_input", fast_input);
    cfg_set_bool("low_latency_audio", low_latency_audio);
    cfg_set_num("audio_sync_trim_ms", audio_trim_ms);
    cfg_set_bool("extrapolate", pflags.extrap);
    cfg_set_bool("filter_x", pflags.xfilter);
    cfg_set_bool("filter_y", pflags.yfilter);
    cfg_set_bool("residual_layer", pflags.res);
    if (gfx_pack_chosen)
        cfg_set_str("graphics_pack", gfxpack_name());
    cfg_set_str("preset", vcfg.preset);
    cfg_set_str("external", vcfg.external[0] ? vcfg.external : "");
    cfg_set_num("shader_source", vcfg.src_mode);
    cfg_set_num("filter", vcfg.filter);
    cfg_set_num("aspect", vcfg.aspect);
    cfg_set_num("params.p_scan", vcfg.p.p_scan);
    cfg_set_num("params.p_mask", vcfg.p.p_mask);
    cfg_set_num("params.p_curv", vcfg.p.p_curv);
    cfg_set_num("params.p_bright", vcfg.p.p_bright);
    cfg_set_num("params.p_vig", vcfg.p.p_vig);
    cfg_set_num("params.p_bloom", vcfg.p.p_bloom);
    cfg_set_str("music_pack", music_pack);
    input_cfg_save();
    cfg_save(options_path);
}

/* ---------------------------------------------------------------------------------------------- latency */

/* Target delay of the audio buffer (ms). */
double aud_target_ms(void) {
    double t;
    if (low_latency_audio && audio_out)
        /* low latency: the TOTAL delay (buffer + device) equals the image's: emulation margin + ~25 ms of video
         * pipeline */
        t = lead * 20.0 + 25.0 - audio_device_ms(audio_out);
    else
        /* winmm: the usual formula */
        t = (lead + 1.0) * 20.0 + 30.0;
    t += audio_trim_ms;
    return t < 30.0 ? 30.0 : t;
}

/* Game VBL per second: 50 (the original) or, without interpolation and with "Match screen refresh", the monitor
 * frequency divided by k (the divisor that gives the value closest to 50 VBL/s). */
double game_rate(void) {
    if (pflags.interp || !match_refresh)
        return 50.0;
    int k = (int)floor(monitor_hz / 50.0 + 0.5);
    if (k < 1)
        k = 1;
    return (double)monitor_hz / k;
}

/* Called when the margin or the game speed may have changed: tells the emulation and the audio device. */
void apply_lead(void) {
    lead = pflags.interp ? lead_with_interp : lead_without_interp;
    emu_post(EV_LEAD, lead, 0);
    emu_post(EV_GAMEHZ, game_rate(), 0);
    if (audio_out)
        audio_set_target(audio_out, aud_target_ms());
}
