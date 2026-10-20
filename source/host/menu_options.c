/*
 * menu_options.c - What the options menu contains, and what every option does.
 *
 * The menu library (menu.c) only draws items and moves between them; every item is created here together with the
 * functions that read and change the value it shows:
 *
 *     g_xxx()    returns the current value          s_xxx(v)   changes it (and tells whoever must know)
 *     xx_nch() / xx_txt(i) / xx_get() / xx_set(i)   the same for a list of choices: how many, the text of each, the
 *                                                   current one, and choose one
 *
 * Most options only change a variable of app.c and send an event to the emulation thread (emu_post). The texts are ids
 * of the language files (data/languages/*.xml); the menu translates them every time it draws.
 *
 * The file is ordered like the menu: the Game, Sound, Video, Motion, Controls and System tabs, and at the end the
 * code that builds the menu from all of them (menu_build).
 */
#include "app.h"

/* ================================================================================================ Game tab */

static int g_god(void) {
    return god_mode;
}
static void s_god(int v) {
    god_mode = v;
    emu_post(EV_GOD, v, 0);
}
static int g_money(void) {
    return infinite_money;
}
static void s_money(int v) {
    infinite_money = v;
    emu_post(EV_MONEY, v, 0);
}

/* Is there a saved game in that slot? */
static int slot_file_ok(int n) {
    char p[760];
    snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n);
    return file_exists(p);
}

static char slot_lbl[3][64]; /* text of each save slot: its date or "empty" */

static void slot_labels(void) {
    for (int n = 1; n <= 3; n++) {
        char p[760];
        snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n);
#ifdef _WIN32
        WIN32_FILE_ATTRIBUTE_DATA fa;
        SYSTEMTIME st, lt;
        if (GetFileAttributesExA(p, GetFileExInfoStandard, &fa)) {
            FileTimeToSystemTime(&fa.ftLastWriteTime, &st);
            SystemTimeToTzSpecificLocalTime(NULL, &st, &lt);
            trf(slot_lbl[n - 1], 64, "val.slot.date", n, lt.wDay, lt.wMonth, lt.wHour, lt.wMinute);
        } else
#endif
            trf(slot_lbl[n - 1], 64, "val.slot.empty", n);
    }
}
static int sl_nch(void) {
    return 3;
}
static const char *sl_txt(int i) {
    slot_labels();
    return slot_lbl[i];
}
static int sl_get(void) {
    return save_slot - 1;
}
static void sl_set(int i) {
    save_slot = i + 1;
}
static void act_goshop(void) {
    emu_post(EV_GOSHOP, 0, 0);
    close_menu();
}
static void act_save(void) {
    emu_post(EV_SAVE, save_slot, 0);
}
static void act_load(void) {
    if (slot_file_ok(save_slot)) {
        emu_post(EV_LOAD, save_slot, 0);
        close_menu();
    } else {
        char t[96];
        trf(t, sizeof t, "msg.slot_empty", save_slot);
        menu_status(menu_cur, t);
        menu_drawn_sig[0] = 0;
    }
}
static void act_restart(void) {
    emu_post(EV_RESTART, 0, 0);
    close_menu();
}

/* ================================================================================================ Sound tab */

static int g_snd(void) {
    return sound_on;
}
static void s_snd(int v) {
    sound_on = v;
    emu_post(EV_SOUND, v, 0);
}
static double g_vol(void) {
    return master_volume;
}
static void s_vol(double v) {
    master_volume = v;
    emu_post(EV_VOL, v, 0);
}
static double g_vx0(void) {
    return part_volume[0];
}
static void s_vx0(double v) {
    part_volume[0] = v;
    emu_post(EV_VOLX, 0, v);
}
static double g_vx1(void) {
    return part_volume[1];
}
static void s_vx1(double v) {
    part_volume[1] = v;
    emu_post(EV_VOLX, 1, v);
}
static double g_vx2(void) {
    return part_volume[2];
}
static void s_vx2(double v) {
    part_volume[2] = v;
    emu_post(EV_VOLX, 2, v);
}
static double g_vx3(void) {
    return part_volume[3];
}
static void s_vx3(double v) {
    part_volume[3] = v;
    emu_post(EV_VOLX, 3, v);
}
static int g_mch(void) {
    return multichannel_fx;
}
static void s_mch(int v) {
    multichannel_fx = v;
    emu_post(EV_MCH, v, 0);
}
static int g_pan(void) {
    return fx_pan;
}
static void s_pan(int v) {
    fx_pan = v;
    emu_post(EV_PAN, v, 0);
}
static int g_mp3(void) {
    return mp3_on;
}
static void s_mp3(int v) {
    mp3_on = v;
    emu_post(EV_MP3, v, 0);
}

/* Music pack: the list is the subfolders of music/ found by musicpacks.c. */
static int mpk_nch(void) {
    return pack_count();
}
static const char *mpk_txt(int i) {
    static char t[2][96]; /* two buffers: the menu may ask for two texts before drawing */
    static int k = 0;
    k ^= 1;
    if (i < 0 || i >= pack_count()) {
        t[k][0] = 0;
        return t[k];
    }
    if (!pack_name(i)[0])
        snprintf(t[k], sizeof t[k], "%s", tr("val.musicfolder"));
    else {
        snprintf(t[k], sizeof t[k], "%s", pack_name(i));
        if (t[k][0] >= 'a' && t[k][0] <= 'z') /* shown capitalised */
            t[k][0] = (char)(t[k][0] - 32);
    }
    return t[k];
}
static int mpk_get(void) {
    int i = pack_find(music_pack);
    return i < 0 ? 0 : i;
}
static void mpk_set(int i) {
    if (i < 0 || i >= pack_count())
        return;
    snprintf(music_pack, sizeof music_pack, "%s", pack_name(i));
    char d[800];
    pack_dir(music_pack, d, sizeof d);
    emu_set_music_dir(d);
    char t[200];
    trf(t, sizeof t, "msg.music_pack", mpk_txt(i));
    menu_status(menu_cur, t);
    menu_drawn_sig[0] = 0;
}

/* ================================================================================================ Video tab */

static int g_fs(void) {
    return fullscreen;
}
static void s_fs(int v) {
    fullscreen = v;
    video_set_fullscreen(v);
}
static int a_nch(void) {
    return 4;
}
static const char *a_txt(int i) {
    static const char *V[4] = {"val.aspect.0", "val.aspect.1", "val.aspect.2", "val.aspect.3"};
    return V[i & 3];
}
static int a_get(void) {
    return vcfg.aspect;
}
static void a_set(int i) {
    vcfg.aspect = i;
}
static const char *a_hint(int i) {
    static const char *H[4] = {"valhint.aspect.0", "valhint.aspect.1", "valhint.aspect.2", "valhint.aspect.3"};
    return H[i & 3];
}
static int ss_nch(void) {
    return 3;
}
static const char *ss_txt(int i) {
    static const char *V[3] = {"val.source.0", "val.source.1", "val.source.2"};
    return V[i % 3];
}
static int ss_get(void) {
    return vcfg.src_mode;
}
static void ss_set(int i) {
    vcfg.src_mode = i;
}
static const char *ss_hint(int i) {
    static const char *H[3] = {"valhint.source.0", "valhint.source.1", "valhint.source.2"};
    return H[i % 3];
}
static int f_nch(void) {
    return 3;
}
static const char *f_txt(int i) {
    static const char *V[3] = {"val.filter.0", "val.filter.1", "val.filter.2"};
    return V[i % 3];
}
static int f_get(void) {
    return vcfg.filter;
}
static void f_set(int i) {
    vcfg.filter = i;
}
static const char *f_hint(int i) {
    static const char *H[3] = {"valhint.filter.0", "valhint.filter.1", "valhint.filter.2"};
    return H[i % 3];
}

/* "Shader / CRT effect": the built-in presets, "Custom" (when a slider was moved) and the external shaders found. */
static char shname[80][80]; /* internal names of the choices; sh_count() refreshes them */

static int sh_count(void) {
    int n = 0;
    for (int i = 0; i < 4; i++)
        snprintf(shname[n++], 80, "%s", VID_PRESETS[i]);
    if (!strcmp(vcfg.preset, "Custom"))
        snprintf(shname[n++], 80, "Custom");
    for (int i = 0; i < video_external_count() && n < 78; i++)
        snprintf(shname[n++], 80, "External: %s", video_external_name(i));
    return n;
}
/* shown name (shname keeps the internal one) */
static const char *sh_txt(int i) {
    static char t[2][120];
    static int k = 0;
    k ^= 1;
    sh_count();
    const char *v = shname[i];
    if (!strncmp(v, "External: ", 10))
        trf(t[k], sizeof t[k], "val.external", v + 10);
    else {
        char id[100];
        snprintf(id, sizeof id, "val.preset.%s", v);
        snprintf(t[k], sizeof t[k], "%s", tr(id));
    }
    return t[k];
}
static int sh_get(void) {
    int n = sh_count();
    char cur[80];
    if (vcfg.external[0])
        snprintf(cur, 80, "External: %s", vcfg.external);
    else
        snprintf(cur, 80, "%s", vcfg.preset);
    for (int i = 0; i < n; i++)
        if (!strcmp(shname[i], cur))
            return i;
    return 0;
}
static void sh_set(int i) {
    sh_count();
    const char *v = shname[i];
    if (!strncmp(v, "External: ", 10))
        video_use_external(v + 10);
    else if (strcmp(v, "Custom")) {
        video_use_external("");
        video_set_preset(v);
    }
}
static const char *sh_hint(int i) {
    static char id[100];
    sh_count();
    const char *v = shname[i];
    if (!strncmp(v, "External: ", 10))
        return "valhint.external";
    snprintf(id, sizeof id, "valhint.preset.%s", v);
    return id;
}
static void custom(void) {
    snprintf(vcfg.preset, sizeof vcfg.preset, "Custom");
    video_use_external("");
}

/* The sliders of the built-in effects: a g_xxx / s_xxx pair per parameter. Moving one switches to the "Custom" preset.
 */
/* clang-format off */
#define PARAM(name, field)                                                                                             \
    static double g_##name(void) { return vcfg.p.field; }                                                              \
    static void s_##name(double v) { vcfg.p.field = (float)v; custom(); }
PARAM(scan, p_scan)
PARAM(mask, p_mask)
PARAM(curv, p_curv)
PARAM(bright, p_bright)
PARAM(vig, p_vig)
PARAM(bloom, p_bloom)
/* clang-format on */

/* The sliders only apply to the built-in effects (an external shader has its own parameters). */

static int sh_internal(void) {
    return !vcfg.external[0];
}

/* Graphics pack: "original" or one of the folders of assets/. */

static const char *pk_hint(int i) {
    return i == 0 ? "valhint.pack.original" : "valhint.pack.other";
}
static int pk_nch(void) {
    return 1 + gfxpack_list_count();
}
static const char *pk_txt(int i) {
    return i == 0 ? "original" : gfxpack_list_name(i - 1);
}
static const char *pk_disp(int i) {
    return i == 0 ? "val.pack.original" : gfxpack_list_name(i - 1);
}
static int pk_get(void) {
    const char *n = gfxpack_name();
    for (int i = 0; i < pk_nch(); i++)
        if (!strcmp(pk_txt(i), n))
            return i;
    return 0;
}
static void pk_set(int i) {
    gfxpack_select(pk_txt(i));
    gfx_pack_chosen = 1;
    present_pack_changed(presenter);
    char t[160];
    if (i)
        trf(t, sizeof t, "msg.pack_replaced", pk_txt(i), gfxpack_replaced());
    else
        trf(t, sizeof t, "msg.pack_original");
    menu_status(menu_cur, t);
    menu_drawn_sig[0] = 0;
}

/* ================================================================================================ Motion tab */

static int g_interp(void) {
    return pflags.interp;
}
static void s_interp(int v) {
    pflags.interp = v;
    apply_lead();
}
static int g_subpx(void) {
    return pflags.subpx;
}
static void s_subpx(int v) {
    pflags.subpx = v;
}
static int gs_nch(void) {
    return 2;
}
static const char *gs_txt(int i) {
    return i ? "val.speed.1" : "val.speed.0";
}
static int gs_get(void) {
    return match_refresh;
}
static void gs_set(int i) {
    match_refresh = i;
    apply_lead();
}
static const char *gs_hint(int i) {
    return i ? "valhint.speed.1" : "valhint.speed.0";
}
static int g_fastin(void) {
    return fast_input;
}
static void s_fastin(int v) {
    fast_input = v;
    resend_input = 1;
}
static int g_llaud(void) {
    return low_latency_audio;
}
static void s_llaud(int v) {
    low_latency_audio = v;
    if (audio_out) {
        if (!audio_set_lowlat(audio_out, v))
            low_latency_audio = 0;
        audio_set_target(audio_out, aud_target_ms());
    } else
        low_latency_audio = 0;
}
static double g_atrim(void) {
    return audio_trim_ms;
}
static void s_atrim(double v) {
    audio_trim_ms = v;
    if (audio_out)
        audio_set_target(audio_out, aud_target_ms());
}

/* ================================================================================================ Controls tab */

static int g_dual(void) {
    return pad_dual_stick;
}
static void s_dual(int v) {
    pad_dual_stick = v;
    resend_input = 1;
}
static int g_autofire(void) {
    return pad_aim_autofire;
}
static void s_autofire(int v) {
    pad_aim_autofire = v;
    resend_input = 1;
}

/* The window "Redefine keys and gamepad buttons" has a row per key action and per pad button action. A row shows the
 * key (or button) now assigned (kvN / pvN) and starts redefining it when chosen (kaN / paN). The menu needs plain
 * function pointers, so the four functions of each row N are written by a macro. */
/* clang-format off */
#define CTLROW(N)                                                                                                      \
    static const char *kv##N(void) { return ctl_text(1, N); }                                                          \
    static void ka##N(void) { start_rebind(1, N); }                                                                    \
    static const char *pv##N(void) { return ctl_text(2, N); }                                                          \
    static void pa##N(void) { start_rebind(2, N); }
CTLROW(0) CTLROW(1) CTLROW(2) CTLROW(3) CTLROW(4) CTLROW(5) CTLROW(6) CTLROW(7) CTLROW(8) CTLROW(9)
    /* clang-format on */
    static const char *(*const KV[KA_COUNT])(void) = {kv0, kv1, kv2, kv3, kv4, kv5, kv6, kv7, kv8, kv9};
static void (*const KAF[KA_COUNT])(void) = {ka0, ka1, ka2, ka3, ka4, ka5, ka6, ka7, ka8, ka9};
static const char *(*const PV[PBA_COUNT])(void) = {pv0, pv1, pv2, pv3, pv4, pv5, pv6, pv7, pv8};
static void (*const PAF[PBA_COUNT])(void) = {pa0, pa1, pa2, pa3, pa4, pa5, pa6, pa7, pa8};

static void act_ctl_defaults(void) {
    input_defaults();
    rebind_kind = 0;
    menu_status(menu_cur, tr("msg.controls_restored"));
    menu_drawn_sig[0] = 0;
}
static void act_ctl_back(void) {
    menu_back();
}
static void open_controls(void) {
    rebind_kind = 0;
    menu_cur = menu_controls;
    menu_show(menu_controls);
    menu_drawn_sig[0] = 0;
}

/* ================================================================================================ System tab */

static int lg_nch(void) {
    return lang_count() > 0 ? lang_count() : 1;
}
static const char *lg_txt(int i) {
    return lang_count() > 0 ? lang_name(i) : "English";
}
static int lg_get(void) {
    return lang_index();
}
static void lg_set(int i) {
    if (i >= 0 && i < lang_count()) {
        lang_set(lang_code(i));
        menu_drawn_sig[0] = 0;
    }
}
/* ---- restore the default options (default.json) ---- */
static void act_restore(void) {
    /* without default.json (or without some key) the program's factory values apply */
    cfg_clear();
    cfg_load(defaults_path);
    s_snd(cfg_bool("sound", 1));
    s_vol(cfg_num("volume", 1.0));
    s_vx0(cfg_num("volume_mp3", 1.0));
    s_vx1(cfg_num("volume_chip_music", 1.0));
    s_vx2(cfg_num("volume_samples", 1.0));
    s_vx3(cfg_num("volume_chip_fx", 1.0));
    s_mp3(cfg_bool("mp3_music", 1));
    s_mch(cfg_bool("multichannel_sfx", 0));
    s_pan(cfg_bool("stereo_pan", 0));
    s_fs(cfg_bool("fullscreen", 1));
    s_subpx(cfg_bool("subpixel_smoothing", 1));
    pflags.sfix = cfg_bool("scroll_fix", 1);
    pflags.extrap = cfg_bool("extrapolate", 1);
    pflags.xfilter = cfg_bool("filter_x", 1);
    pflags.yfilter = cfg_bool("filter_y", 1);
    pflags.res = cfg_bool("residual_layer", 1);
    lead_with_interp = cfg_num("latency", LEAD_DEFAULT);
    if (lead_with_interp < 0.5)
        lead_with_interp = 0.5;
    if (lead_with_interp > 6)
        lead_with_interp = 6;
    lead_without_interp = cfg_num("latency_nointerp", 0.5);
    if (lead_without_interp < 0.0)
        lead_without_interp = 0.0;
    if (lead_without_interp > 6)
        lead_without_interp = 6;
    match_refresh = cfg_bool("game_speed_match", 0);
    s_interp(cfg_bool("frame_interpolation", 1));
    s_fastin(cfg_bool("fast_input", 0));
    s_llaud(cfg_bool("low_latency_audio", 0));
    s_atrim(cfg_num("audio_sync_trim_ms", 0.0));
    apply_video_cfg();
    {
        const char *pn = cfg_str("graphics_pack", "");
        int k = 0;
        for (int i = 0; i < pk_nch(); i++)
            if (!_stricmp(pk_txt(i), pn))
                k = i;
        pk_set(k);
    }
    scan_packs();
    pack_resolve(cfg_str("music_pack", "arrange"));
    {
        char d[800];
        pack_dir(music_pack, d, sizeof d);
        emu_set_music_dir(d);
    }
    input_cfg_load();
    save_options();
    menu_status(menu_cur, tr("msg.options_restored"));
    menu_drawn_sig[0] = 0;
}
static void act_close(void) {
    close_menu();
}
static void act_quit(void) {
    app_running = 0;
}

/* ================================================================================================ building the menu */

/* Appends an item of each kind to the menu being built (menu_cur). */

static void add_bool(const char *l, const char *h, int (*g)(void), void (*s)(int)) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_BOOL;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.hint = h;
    it.getb = g;
    it.setb = s;
    menu_add(menu_cur, &it);
}
static void add_choice(const char *l, const char *h, int (*n)(void), const char *(*t)(int), int (*g)(void),
                       void (*s)(int)) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_CHOICE;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.hint = h;
    it.nch = n;
    it.chtext = t;
    it.getch = g;
    it.setch = s;
    menu_add(menu_cur, &it);
}
static void add_slider(const char *l, const char *h, double lo, double hi, double st, const char *fmt,
                       double (*g)(void), void (*s)(double)) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_SLIDER;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.hint = h;
    it.lo = lo;
    it.hi = hi;
    it.step = st;
    it.fmt = fmt;
    it.getd = g;
    it.setd = s;
    menu_add(menu_cur, &it);
}
static void last_enabled(int (*e)(void)) {
    menu_last_enabled(menu_cur, e);
}
static void add_action(const char *l, const char *h, void (*a)(void)) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_ACTION;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.hint = h;
    it.act = a;
    menu_add(menu_cur, &it);
}
static void add_tab(const char *l, const char *hint) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_TAB;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.hint = hint;
    menu_add(menu_cur, &it);
}
static void add_sep(const char *l, int plain) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_SEP;
    snprintf(it.label, sizeof it.label, "%s", l);
    it.plain = plain;
    menu_add(menu_cur, &it);
}
static void add_image(const char *path) {
    MItem it;
    memset(&it, 0, sizeof it);
    it.kind = MK_IMAGE;
    it.path = _strdup(path);
    menu_add(menu_cur, &it);
}

/* the window to redefine the keys and the pad buttons */
static void build_controls_menu(void) {
    menu_cur = menu_controls = menu_new("title.controls");
    add_sep("sep.keyboard", 0);
    for (int a = 0; a < KA_COUNT; a++) {
        add_action(key_act_label_id(a), key_act_hint(a), KAF[a]);
        menu_last_valtext(menu_cur, KV[a]);
    }
    add_sep("sep.gamepad_both_pads", 0);
    for (int a = 0; a < PBA_COUNT; a++) {
        add_action(pad_act_label_id(a), pad_act_hint(a), PAF[a]);
        menu_last_valtext(menu_cur, PV[a]);
    }
    add_sep("", 0);
    add_action("menu.restore_default_controls", "menu.restore_default_controls.hint", act_ctl_defaults);
    add_action("menu.back", "menu.back.hint", act_ctl_back);
}
/* Creates the options menu (menu_options) and the controls window (menu_controls). have_audio / have_gl / have_pack:
 * the tabs and items that need a sound device, OpenGL shaders or a graphics pack are only created if they exist. */
void menu_build(int have_audio, int have_gl, int have_pack) {
    menu_cur = menu_options = menu_new("title.options");
    add_tab("tab.game", "tab.game.hint");
    add_action("menu.back_to_the_mission_map", "menu.back_to_the_mission_map.hint", act_goshop);
    add_choice("menu.save_slot", "menu.save_slot.hint", sl_nch, sl_txt, sl_get, sl_set);
    add_action("menu.save_game", "menu.save_game.hint", act_save);
    add_action("menu.load_game", "menu.load_game.hint", act_load);
    add_action("menu.restart_game_1_or_2_players", "menu.restart_game_1_or_2_players.hint", act_restart);
    add_bool("menu.invulnerability_testing", "menu.invulnerability_testing.hint", g_god, s_god);
    add_bool("menu.infinite_money_testing", "menu.infinite_money_testing.hint", g_money, s_money);
    if (have_audio) {
        add_tab("tab.sound", "tab.sound.hint");
        add_bool("menu.sound", "menu.sound.hint", g_snd, s_snd);
        add_slider("menu.volume", "menu.volume.hint", 0.0, 1.0, 0.1, NULL, g_vol, s_vol);
        add_slider("  menu.music_mp3", "menu.music_mp3.hint", 0.0, 1.0, 0.1, NULL, g_vx0, s_vx0);
        add_slider("  menu.music_ym2149f", "menu.music_ym2149f.hint", 0.0, 1.0, 0.1, NULL, g_vx1, s_vx1);
        add_slider("  menu.sampled_fx", "menu.sampled_fx.hint", 0.0, 1.0, 0.1, NULL, g_vx2, s_vx2);
        add_slider("  menu.fx_ym2149f", "menu.fx_ym2149f.hint", 0.0, 1.0, 0.1, NULL, g_vx3, s_vx3);
        add_bool("menu.multichannel_fx", "menu.multichannel_fx.hint", g_mch, s_mch);
        add_bool("menu.fx_stereo_panning", "menu.fx_stereo_panning.hint", g_pan, s_pan);
        add_bool("menu.mp3_music", "menu.mp3_music.hint", g_mp3, s_mp3);
        add_choice("menu.music_pack", "menu.music_pack.hint", mpk_nch, mpk_txt, mpk_get, mpk_set);
    }
    add_tab("tab.video", "tab.video.hint");
    add_bool("menu.fullscreen_f11", "menu.fullscreen_f11.hint", g_fs, s_fs);
    if (have_gl) {
        add_choice("menu.aspect_ratio", "menu.aspect_ratio.hint", a_nch, a_txt, a_get, a_set);
        menu_last_valhint(menu_cur, a_hint);
        add_choice("menu.shader_source", "menu.shader_source.hint", ss_nch, ss_txt, ss_get, ss_set);
        menu_last_valhint(menu_cur, ss_hint);
        add_choice("menu.scaling_filter", "menu.scaling_filter.hint", f_nch, f_txt, f_get, f_set);
        menu_last_valhint(menu_cur, f_hint);
        add_choice("menu.shader_crt_effect", "menu.shader_crt_effect.hint", sh_count, sh_txt, sh_get, sh_set);
        menu_last_valhint(menu_cur, sh_hint);
        add_slider("  menu.scanlines", "menu.scanlines.hint", 0, 1, 0.05, NULL, g_scan, s_scan);
        last_enabled(sh_internal);
        add_slider("  menu.phosphor_mask", "menu.phosphor_mask.hint", 0, 1, 0.05, NULL, g_mask, s_mask);
        last_enabled(sh_internal);
        add_slider("  menu.curvature", "menu.curvature.hint", 0, 1, 0.05, NULL, g_curv, s_curv);
        last_enabled(sh_internal);
        add_slider("  menu.brightness", "menu.brightness.hint", 0.5, 1.8, 0.05, "%.2f", g_bright, s_bright);
        last_enabled(sh_internal);
        add_slider("  menu.vignette", "menu.vignette.hint", 0, 1, 0.05, NULL, g_vig, s_vig);
        last_enabled(sh_internal);
        add_slider("  menu.glow", "menu.glow.hint", 0, 1, 0.05, NULL, g_bloom, s_bloom);
        last_enabled(sh_internal);
    }
    if (have_pack)
        add_choice("menu.graphics_pack", "menu.graphics_pack.hint", pk_nch, pk_disp, pk_get, pk_set);
    menu_last_valhint(menu_cur, pk_hint);
    add_tab("tab.motion", "tab.motion.hint");
    add_bool("menu.frame_interpolation", "menu.frame_interpolation.hint", g_interp, s_interp);
    add_bool("menu.sub_pixel_smoothing", "menu.sub_pixel_smoothing.hint", g_subpx, s_subpx);
    add_choice("  menu.game_speed_no_interpolation", "menu.game_speed_no_interpolation.hint", gs_nch, gs_txt, gs_get,
               gs_set);
    menu_last_valhint(menu_cur, gs_hint);
    add_sep("sep.latency", 0);
    add_bool("menu.fast_input_polling", "menu.fast_input_polling.hint", g_fastin, s_fastin);
    if (have_audio) {
        add_bool("menu.low_latency_audio", "menu.low_latency_audio.hint", g_llaud, s_llaud);
        add_slider("  menu.audio_sync_trim_ms", "menu.audio_sync_trim_ms.hint", -100.0, 100.0, 5.0, "%.0f", g_atrim,
                   s_atrim);
    }
    add_tab("tab.controls", "tab.controls.hint");
    add_bool("menu.dual_stick", "menu.dual_stick.hint", g_dual, s_dual);
    add_bool("  menu.dual_stick_autofire", "menu.dual_stick_autofire.hint", g_autofire, s_autofire);
    add_action("menu.keys_and_gamepad_buttons", "menu.keys_and_gamepad_buttons.hint", open_controls);
    /* the picture of the pad, drawn by the program from the current controls */
    add_image("@padmap");
    add_tab("tab.system", "tab.system.hint");
    add_choice("menu.language", "menu.language.hint", lg_nch, lg_txt, lg_get, lg_set);
    add_action("menu.restore_default_options", "menu.restore_default_options.hint", act_restore);
    add_sep("sep.credits", 0);
    add_sep("Claude Sonnet 5.5 & VooD", 1);
    add_sep("", 0);
    add_action("menu.continue", "menu.continue.hint", act_close);
    add_action("menu.quit", "menu.quit.hint", act_quit);
    build_controls_menu();
    menu_cur = menu_options;
}
