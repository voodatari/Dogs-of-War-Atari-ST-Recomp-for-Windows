/*
 * main.c - Dogs of War, native 60 fps port): window, input, menu, presenter and the emulation thread.
 */
#include "emu.h"
#include "video.h"
#include "menu.h"
#include "lang.h"
#include "input.h"
#include "cfg.h"
#include "gfxpack.h"
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#endif

#define LEAD_DEFAULT 3.6

/* ------------------------------------------------------------------------------------------------ global application state */
static Presenter *pres; static Menu *menu, *menu_main, *menu_ctl; static AudioOut *aud;      
static int S = 3, hz = 60; static double lead = LEAD_DEFAULT;
/* Latency (tests): minimum emulation margin + fast input polling / low-latency WASAPI audio / hard sync with the GPU. */
static int fastinp = 0, ll_audio = 0; static int last_hold[2] = { 0, 0 }; static double lead_norm = LEAD_DEFAULT, lead_ni = 0.5, atrim = 0.0; static int gmatch = 0;
static volatile int g_menu_open_f = 0; static volatile int *p_kstate = NULL; static volatile uint8_t *p_held = NULL; static volatile int *p_padhold = NULL;
static int fastin_on(void) { return fastinp && (!getenv("DOW_TEST") || getenv("DOW_FASTIN_TEST")); }
static int snd = 1, mp3v = 1, mchv = 0, panv = 0, fixv = 1, godv = 0, moneyv = 0; static double volm = 1.0;
static int slot = 1; static char app[600], cfg_path[700], default_path[700], legacy_path[700], game_dir[600], assets_root[700], save_dir[700];
static char music_root[700], music_pack[80] = "arrange";                       /* music pack = subfolder of music ("" = the mp3 files loose in music) */
static int rb_kind = 0, rb_act = -1, rb_prev_ok = 0, rb_wait_release = 0;       /* redefining controls: 0 nothing, 1 waiting for a key, 2 waiting for a pad button */
static int running = 1, fs_state = 0, ov_on = 0; static char menu_sig[600] = ""; static char menu_sig_last[600] = "";
static char pack_cli[64] = ""; static int pack_chosen = 0;

static double env_d(const char *k, double d) { const char *v = getenv(k); return v ? atof(v) : d; }

static double vx[4] = { 1, 1, 1, 1 };                                   /* volumes per type: MP3, chip music, sampled effects, chip effects */
static void save_cfg(void) {
    if (getenv("DOW_TEST")) return;                                    /* automatic tests do not touch options.json */
    cfg_set_num("version", 3); cfg_set_str("language", lang_current()); cfg_set_bool("fullscreen", fs_state); cfg_set_bool("sound", snd); cfg_set_num("volume", volm); cfg_set_num("volume_mp3", vx[0]); cfg_set_num("volume_chip_music", vx[1]); cfg_set_num("volume_samples", vx[2]); cfg_set_num("volume_chip_fx", vx[3]); cfg_set_bool("mp3_music", mp3v); cfg_set_bool("multichannel_sfx", mchv);
    cfg_set_bool("stereo_pan", panv); cfg_set_bool("subpixel_smoothing", pflags.subpx); cfg_set_bool("scroll_fix", pflags.sfix); cfg_set_bool("frame_interpolation", pflags.interp); cfg_set_num("latency_nointerp", lead_ni); cfg_set_bool("game_speed_match", gmatch); cfg_set_bool("enemy_script_fix", fixv); cfg_set_num("latency", lead_norm); cfg_set_bool("fast_input", fastinp); cfg_set_bool("low_latency_audio", ll_audio); cfg_set_num("audio_sync_trim_ms", atrim); cfg_set_bool("extrapolate", pflags.extrap);
    cfg_set_bool("filter_x", pflags.xfilter); cfg_set_bool("filter_y", pflags.yfilter); cfg_set_bool("residual_layer", pflags.res);
    if (pack_chosen) cfg_set_str("graphics_pack", gfxpack_name());
    cfg_set_str("preset", vcfg.preset); if (vcfg.external[0]) cfg_set_str("external", vcfg.external); else cfg_set_str("external", "");
    cfg_set_num("shader_source", vcfg.src_mode); cfg_set_num("filter", vcfg.filter); cfg_set_num("aspect", vcfg.aspect);
    cfg_set_num("params.p_scan", vcfg.p.p_scan); cfg_set_num("params.p_mask", vcfg.p.p_mask); cfg_set_num("params.p_curv", vcfg.p.p_curv); cfg_set_num("params.p_bright", vcfg.p.p_bright); cfg_set_num("params.p_vig", vcfg.p.p_vig); cfg_set_num("params.p_bloom", vcfg.p.p_bloom);
    cfg_set_str("music_pack", music_pack); input_cfg_save();
    cfg_save(cfg_path);
}

/* ------------------------------------------------------------------------------------------------ menu options */
static void close_menu(void);
static int g_god(void) { return godv; } static void s_god(int v) { godv = v; emu_post(EV_GOD, v, 0); }
static int g_money(void) { return moneyv; } static void s_money(int v) { moneyv = v; emu_post(EV_MONEY, v, 0); }
static int g_snd(void) { return snd; } static void s_snd(int v) { snd = v; emu_post(EV_SOUND, v, 0); }
static double g_vol(void) { return volm; } static void s_vol(double v) { volm = v; emu_post(EV_VOL, v, 0); }
static double g_vx0(void) { return vx[0]; } static void s_vx0(double v) { vx[0] = v; emu_post(EV_VOLX, 0, v); }
static double g_vx1(void) { return vx[1]; } static void s_vx1(double v) { vx[1] = v; emu_post(EV_VOLX, 1, v); }
static double g_vx2(void) { return vx[2]; } static void s_vx2(double v) { vx[2] = v; emu_post(EV_VOLX, 2, v); }
static double g_vx3(void) { return vx[3]; } static void s_vx3(double v) { vx[3] = v; emu_post(EV_VOLX, 3, v); }
static int g_mch(void) { return mchv; } static void s_mch(int v) { mchv = v; emu_post(EV_MCH, v, 0); }
static int g_pan(void) { return panv; } static void s_pan(int v) { panv = v; emu_post(EV_PAN, v, 0); }
static int g_mp3(void) { return mp3v; } static void s_mp3(int v) { mp3v = v; emu_post(EV_MP3, v, 0); }
static int lg_nch(void) { return lang_count() > 0 ? lang_count() : 1; } static const char *lg_txt(int i) { return lang_count() > 0 ? lang_name(i) : "English"; } static int lg_get(void) { return lang_index(); }
static void lg_set(int i) { if (i >= 0 && i < lang_count()) { lang_set(lang_code(i)); menu_sig_last[0] = 0; } }
static int g_fs(void) { return fs_state; } static void s_fs(int v) { fs_state = v; video_set_fullscreen(v); }
static int g_subpx(void) { return pflags.subpx; } static void s_subpx(int v) { pflags.subpx = v; }
static double aud_target_ms(void) {                                       /* target delay of the audio buffer (ms) */
    double t;
    if (ll_audio && aud) t = lead * 20.0 + 25.0 - audio_device_ms(aud);   /* low latency: the TOTAL delay (buffer + device) equals the image's: emulation margin + ~25 ms of video pipeline */
    else t = (lead + 1.0) * 20.0 + 30.0;                                  /* winmm: the usual formula */
    t += atrim; return t < 30.0 ? 30.0 : t;
}
static int last_j1_reset = 0;
static double game_rate(void) {                                          /* game VBL/s: 50 (original) or, without interpolation and with "Match screen refresh", the monitor frequency divided by k (the divisor closest to 50 VBL/s) */
    if (pflags.interp || !gmatch) return 50.0;
    int k = (int)floor(hz / 50.0 + 0.5); if (k < 1) k = 1; return (double)hz / k;
}
static void apply_lead(void) { lead = pflags.interp ? lead_norm : lead_ni; emu_post(EV_LEAD, lead, 0); emu_post(EV_GAMEHZ, game_rate(), 0); if (aud) audio_set_target(aud, aud_target_ms()); }
static int g_interp(void) { return pflags.interp; } static void s_interp(int v) { pflags.interp = v; apply_lead(); }
static int gs_nch(void) { return 2; } static const char *gs_txt(int i) { return i ? "val.speed.1" : "val.speed.0"; } static int gs_get(void) { return gmatch; } static void gs_set(int i) { gmatch = i; apply_lead(); }
static int g_fastin(void) { return fastinp; } static void s_fastin(int v) { fastinp = v; last_j1_reset = 1; }
static int g_llaud(void) { return ll_audio; } static void s_llaud(int v) { ll_audio = v; if (aud) { if (!audio_set_lowlat(aud, v)) ll_audio = 0; audio_set_target(aud, aud_target_ms()); } else ll_audio = 0; }
static double g_atrim(void) { return atrim; } static void s_atrim(double v) { atrim = v; if (aud) audio_set_target(aud, aud_target_ms()); }
static int a_nch(void) { return 4; } static const char *a_txt(int i) { static const char *V[4] = { "val.aspect.0", "val.aspect.1", "val.aspect.2", "val.aspect.3" }; return V[i & 3]; } static int a_get(void) { return vcfg.aspect; } static void a_set(int i) { vcfg.aspect = i; }
static const char *a_hint(int i) { static const char *H[4] = { "valhint.aspect.0", "valhint.aspect.1", "valhint.aspect.2", "valhint.aspect.3" }; return H[i & 3]; }
static const char *ss_hint(int i) { static const char *H[3] = { "valhint.source.0", "valhint.source.1", "valhint.source.2" }; return H[i % 3]; }
static const char *f_hint(int i) { static const char *H[3] = { "valhint.filter.0", "valhint.filter.1", "valhint.filter.2" }; return H[i % 3]; }
static const char *gs_hint(int i) { return i ? "valhint.speed.1" : "valhint.speed.0"; }
static const char *pk_hint(int i);
static int ss_nch(void) { return 3; } static const char *ss_txt(int i) { static const char *V[3] = { "val.source.0", "val.source.1", "val.source.2" }; return V[i % 3]; } static int ss_get(void) { return vcfg.src_mode; } static void ss_set(int i) { vcfg.src_mode = i; }
static int f_nch(void) { return 3; } static const char *f_txt(int i) { static const char *V[3] = { "val.filter.0", "val.filter.1", "val.filter.2" }; return V[i % 3]; } static int f_get(void) { return vcfg.filter; } static void f_set(int i) { vcfg.filter = i; }
static char shname[80][80];
static int sh_count(void) { int n = 0; for (int i = 0; i < 4; i++) snprintf(shname[n++], 80, "%s", VID_PRESETS[i]); if (!strcmp(vcfg.preset, "Custom")) snprintf(shname[n++], 80, "Custom"); for (int i = 0; i < video_external_count() && n < 78; i++) snprintf(shname[n++], 80, "External: %s", video_external_name(i)); return n; }
static const char *sh_txt(int i) {                                                  /* shown name (shname keeps the internal one) */
    static char t[2][120]; static int k = 0; k ^= 1; sh_count(); const char *v = shname[i];
    if (!strncmp(v, "External: ", 10)) trf(t[k], sizeof t[k], "val.external", v + 10); else { char id[100]; snprintf(id, sizeof id, "val.preset.%s", v); snprintf(t[k], sizeof t[k], "%s", tr(id)); }
    return t[k];
}
static int sh_get(void) { int n = sh_count(); char cur[80]; if (vcfg.external[0]) snprintf(cur, 80, "External: %s", vcfg.external); else snprintf(cur, 80, "%s", vcfg.preset); for (int i = 0; i < n; i++) if (!strcmp(shname[i], cur)) return i; return 0; }
static void sh_set(int i) { sh_count(); const char *v = shname[i]; if (!strncmp(v, "External: ", 10)) video_use_external(v + 10); else if (strcmp(v, "Custom")) { video_use_external(""); video_set_preset(v); } }
static const char *sh_hint(int i) {
    static char id[100]; sh_count(); const char *v = shname[i];
    if (!strncmp(v, "External: ", 10)) return "valhint.external";
    snprintf(id, sizeof id, "valhint.preset.%s", v); return id;
}
static void custom(void) { snprintf(vcfg.preset, sizeof vcfg.preset, "Custom"); video_use_external(""); }
#define PARAM(name, field) static double g_##name(void) { return vcfg.p.field; } static void s_##name(double v) { vcfg.p.field = (float)v; custom(); }
PARAM(scan, p_scan) PARAM(mask, p_mask) PARAM(curv, p_curv) PARAM(bright, p_bright) PARAM(vig, p_vig) PARAM(bloom, p_bloom)

static int slot_file_ok(int n) { char p[760]; snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n); return file_exists(p); }
static char slot_lbl[3][64];
static void slot_labels(void) {
    for (int n = 1; n <= 3; n++) {
        char p[760]; snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n);
#ifdef _WIN32
        WIN32_FILE_ATTRIBUTE_DATA fa; SYSTEMTIME st, lt; if (GetFileAttributesExA(p, GetFileExInfoStandard, &fa)) { FileTimeToSystemTime(&fa.ftLastWriteTime, &st); SystemTimeToTzSpecificLocalTime(NULL, &st, &lt); trf(slot_lbl[n - 1], 64, "val.slot.date", n, lt.wDay, lt.wMonth, lt.wHour, lt.wMinute); }
        else
#endif
        trf(slot_lbl[n - 1], 64, "val.slot.empty", n);
    }
}
static int sl_nch(void) { return 3; } static const char *sl_txt(int i) { slot_labels(); return slot_lbl[i]; } static int sl_get(void) { return slot - 1; } static void sl_set(int i) { slot = i + 1; }
static void act_goshop(void) { emu_post(EV_GOSHOP, 0, 0); close_menu(); }
static void act_save(void) { emu_post(EV_SAVE, slot, 0); }
static void act_load(void) { if (slot_file_ok(slot)) { emu_post(EV_LOAD, slot, 0); close_menu(); } else { char t[96]; trf(t, sizeof t, "msg.slot_empty", slot); menu_status(menu, t); menu_sig[0] = 0; menu_sig_last[0] = 0; } }
static void act_restart(void) { emu_post(EV_RESTART, 0, 0); close_menu(); }
static void act_quit(void) { running = 0; }
static void act_close(void) { close_menu(); }
static const char *pk_hint(int i) { return i == 0 ? "valhint.pack.original" : "valhint.pack.other"; }
static const char *pk_disp(int i);
static int pk_nch(void) { return 1 + gfxpack_list_count(); } static const char *pk_txt(int i) { return i == 0 ? "original" : gfxpack_list_name(i - 1); }
static const char *pk_disp(int i) { return i == 0 ? "val.pack.original" : gfxpack_list_name(i - 1); }
static int pk_get(void) { const char *n = gfxpack_name(); for (int i = 0; i < pk_nch(); i++) if (!strcmp(pk_txt(i), n)) return i; return 0; }
static void pk_set(int i) { gfxpack_select(pk_txt(i)); pack_chosen = 1; present_pack_changed(pres); char t[160]; if (i) trf(t, sizeof t, "msg.pack_replaced", pk_txt(i), gfxpack_replaced()); else trf(t, sizeof t, "msg.pack_original"); menu_status(menu, t); menu_sig_last[0] = 0; }

/* ------------------------------------------------------------------------------------------------ music packs (subfolders of music) */
#define MAXPACK 32
static char packs[MAXPACK][80]; static int npacks = 0;                          /* "" = the mp3 files loose directly in music */
static int has_audio_files(const char *dir) {
#ifdef _WIN32
    char pat[760]; snprintf(pat, sizeof pat, "%s/*", dir); WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat, &fd); if (h == INVALID_HANDLE_VALUE) return 0; int found = 0;
    do { if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue; const char *e = strrchr(fd.cFileName, '.'); if (e && (!_stricmp(e, ".mp3") || !_stricmp(e, ".ogg") || !_stricmp(e, ".wav") || !_stricmp(e, ".wma") || !_stricmp(e, ".m4a"))) { found = 1; break; } } while (FindNextFileA(h, &fd));
    FindClose(h); return found;
#else
    (void)dir; return 0;
#endif
}
static int pack_cmp(const void *a, const void *b) { return _stricmp((const char *)a, (const char *)b); }
static void scan_packs(void) {
    npacks = 0;
#ifdef _WIN32
    if (has_audio_files(music_root)) packs[npacks++][0] = 0;
    char pat[760]; snprintf(pat, sizeof pat, "%s/*", music_root); WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat, &fd); int first = npacks;
    if (h != INVALID_HANDLE_VALUE) {
        do { if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) || fd.cFileName[0] == '.' || npacks >= MAXPACK) continue; char d[800]; snprintf(d, sizeof d, "%s/%s", music_root, fd.cFileName); if (has_audio_files(d)) snprintf(packs[npacks++], 80, "%s", fd.cFileName); } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
    qsort(packs[first], (size_t)(npacks - first), 80, pack_cmp);
#endif
}
static int pack_index(const char *name) { for (int i = 0; i < npacks; i++) if (!_stricmp(packs[i], name)) return i; return -1; }
static void pack_dir(const char *name, char *out, size_t n) { if (name[0]) snprintf(out, n, "%s/%s", music_root, name); else snprintf(out, n, "%s", music_root); }
static void pack_resolve(const char *want) {                                     /* leaves in music_pack a pack that exists (the requested one, or else the usual one / the first) */
    int i = pack_index(want); if (i < 0) i = pack_index("arrange"); if (i < 0) i = npacks > 0 ? 0 : -1;
    snprintf(music_pack, sizeof music_pack, "%s", i >= 0 ? packs[i] : want);
}
static int mpk_nch(void) { return npacks; }
static const char *mpk_txt(int i) { static char t[2][96]; static int k = 0; k ^= 1; if (i < 0 || i >= npacks) { t[k][0] = 0; return t[k]; } if (!packs[i][0]) snprintf(t[k], sizeof t[k], "%s", tr("val.musicfolder")); else { snprintf(t[k], sizeof t[k], "%s", packs[i]); if (t[k][0] >= 'a' && t[k][0] <= 'z') t[k][0] = (char)(t[k][0] - 32); } return t[k]; }
static int mpk_get(void) { int i = pack_index(music_pack); return i < 0 ? 0 : i; }
static void mpk_set(int i) { if (i < 0 || i >= npacks) return; snprintf(music_pack, sizeof music_pack, "%s", packs[i]); char d[800]; pack_dir(music_pack, d, sizeof d); emu_set_music_dir(d); char t[200]; trf(t, sizeof t, "msg.music_pack", mpk_txt(i)); menu_status(menu, t); menu_sig_last[0] = 0; }

/* ------------------------------------------------------------------------------------------------ video from the options */
static void apply_video_cfg(void) {
    const char *pn = cfg_str("preset", "Soft CRT"); video_use_external(""); if (!strcmp(pn, "Custom")) snprintf(vcfg.preset, sizeof vcfg.preset, "Custom"); else video_set_preset(pn);
    vcfg.filter = (int)cfg_num("filter", vcfg.filter); vcfg.src_mode = (int)cfg_num("shader_source", 0); if (vcfg.src_mode < 0 || vcfg.src_mode > 2) vcfg.src_mode = 0; vcfg.aspect = (int)cfg_num("aspect", 1); if (vcfg.filter < 0 || vcfg.filter > 2) vcfg.filter = 1; if (vcfg.aspect < 0 || vcfg.aspect > 3) vcfg.aspect = 1;
    if (cfg_has("params.p_scan")) { vcfg.p.p_scan = (float)cfg_num("params.p_scan", vcfg.p.p_scan); vcfg.p.p_mask = (float)cfg_num("params.p_mask", vcfg.p.p_mask); vcfg.p.p_curv = (float)cfg_num("params.p_curv", vcfg.p.p_curv); vcfg.p.p_bright = (float)cfg_num("params.p_bright", vcfg.p.p_bright); vcfg.p.p_vig = (float)cfg_num("params.p_vig", vcfg.p.p_vig); vcfg.p.p_bloom = (float)cfg_num("params.p_bloom", vcfg.p.p_bloom); }
    const char *ex = cfg_str("external", ""); for (int i = 0; i < video_external_count(); i++) if (!strcmp(video_external_name(i), ex)) video_use_external(ex);
}

/* ------------------------------------------------------------------------------------------------ controls (keys and pad buttons) */
static void menu_back(void) {                                                    /* Esc / B: from the controls window it goes back to the menu; from the menu it closes */
    if (menu == menu_ctl) { rb_kind = 0; menu_hide(menu_ctl); menu = menu_main; menu_sig_last[0] = 0; } else close_menu();
}
static void start_rebind(int kind, int act) {
    rb_kind = kind; rb_act = act; rb_prev_ok = 0; menu_sig_last[0] = 0;
    menu_status(menu, tr(kind == 1 ? "msg.press_new_key" : "msg.press_new_button"));
}
static const char *ctl_text(int kind, int act) {
    static char t[2][64]; static int k = 0; k ^= 1;
    if (rb_kind == kind && rb_act == act) { snprintf(t[k], sizeof t[k], "%s", tr(kind == 1 ? "val.press_key" : "val.press_button")); return t[k]; }
    if (kind == 1) key_name(key_bind[act], t[k], sizeof t[k]); else pad_button_name(pad_bind[act], t[k], sizeof t[k]);
    return t[k];
}
#define CTLROW(N) static const char *kv##N(void) { return ctl_text(1, N); } static void ka##N(void) { start_rebind(1, N); } \
                  static const char *pv##N(void) { return ctl_text(2, N); } static void pa##N(void) { start_rebind(2, N); }
CTLROW(0) CTLROW(1) CTLROW(2) CTLROW(3) CTLROW(4) CTLROW(5) CTLROW(6) CTLROW(7) CTLROW(8) CTLROW(9)
static const char *(*const KV[KA_COUNT])(void) = { kv0, kv1, kv2, kv3, kv4, kv5, kv6, kv7, kv8, kv9 };
static void (*const KAF[KA_COUNT])(void) = { ka0, ka1, ka2, ka3, ka4, ka5, ka6, ka7, ka8, ka9 };
static const char *(*const PV[PBA_COUNT])(void) = { pv0, pv1, pv2, pv3, pv4, pv5, pv6, pv7, pv8 };
static void (*const PAF[PBA_COUNT])(void) = { pa0, pa1, pa2, pa3, pa4, pa5, pa6, pa7, pa8 };
static void act_ctl_defaults(void) { input_defaults(); rb_kind = 0; menu_status(menu, tr("msg.controls_restored")); menu_sig_last[0] = 0; }
static void act_ctl_back(void) { menu_back(); }
static void open_controls(void) { rb_kind = 0; menu = menu_ctl; menu_show(menu_ctl); menu_sig_last[0] = 0; }

/* ------------------------------------------------------------------------------------------------ restore the default options (default.json) */
static void act_restore(void) {
    cfg_clear(); cfg_load(default_path);                                          /* without default.json (or without some key) the program's factory values apply */
    s_snd(cfg_bool("sound", 1)); s_vol(cfg_num("volume", 1.0));
    s_vx0(cfg_num("volume_mp3", 1.0)); s_vx1(cfg_num("volume_chip_music", 1.0)); s_vx2(cfg_num("volume_samples", 1.0)); s_vx3(cfg_num("volume_chip_fx", 1.0));
    s_mp3(cfg_bool("mp3_music", 1)); s_mch(cfg_bool("multichannel_sfx", 0)); s_pan(cfg_bool("stereo_pan", 0));
    s_fs(cfg_bool("fullscreen", 1)); s_subpx(cfg_bool("subpixel_smoothing", 1)); pflags.sfix = cfg_bool("scroll_fix", 1);
    pflags.extrap = cfg_bool("extrapolate", 1); pflags.xfilter = cfg_bool("filter_x", 1); pflags.yfilter = cfg_bool("filter_y", 1); pflags.res = cfg_bool("residual_layer", 1);
    lead_norm = cfg_num("latency", LEAD_DEFAULT); if (lead_norm < 0.5) lead_norm = 0.5; if (lead_norm > 6) lead_norm = 6;
    lead_ni = cfg_num("latency_nointerp", 0.5); if (lead_ni < 0.0) lead_ni = 0.0; if (lead_ni > 6) lead_ni = 6;
    gmatch = cfg_bool("game_speed_match", 0); s_interp(cfg_bool("frame_interpolation", 1));
    s_fastin(cfg_bool("fast_input", 0)); s_llaud(cfg_bool("low_latency_audio", 0)); s_atrim(cfg_num("audio_sync_trim_ms", 0.0));
    apply_video_cfg();
    { const char *pn = cfg_str("graphics_pack", ""); int k = 0; for (int i = 0; i < pk_nch(); i++) if (!_stricmp(pk_txt(i), pn)) k = i; pk_set(k); }
    scan_packs(); pack_resolve(cfg_str("music_pack", "arrange")); { char d[800]; pack_dir(music_pack, d, sizeof d); emu_set_music_dir(d); }
    input_cfg_load();
    save_cfg(); menu_status(menu, tr("msg.options_restored")); menu_sig_last[0] = 0;
}

static void M(int kind, const char *label, const char *hint) { MItem it; memset(&it, 0, sizeof it); it.kind = kind; snprintf(it.label, sizeof it.label, "%s", label); it.hint = hint; menu_add(menu, &it); }
static MItem *last_item(void);
static void add_bool(const char *l, const char *h, int (*g)(void), void (*s)(int)) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_BOOL; snprintf(it.label, sizeof it.label, "%s", l); it.hint = h; it.getb = g; it.setb = s; menu_add(menu, &it); }
static void add_choice(const char *l, const char *h, int (*n)(void), const char *(*t)(int), int (*g)(void), void (*s)(int)) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_CHOICE; snprintf(it.label, sizeof it.label, "%s", l); it.hint = h; it.nch = n; it.chtext = t; it.getch = g; it.setch = s; menu_add(menu, &it); }
static void add_slider(const char *l, const char *h, double lo, double hi, double st, const char *fmt, double (*g)(void), void (*s)(double)) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_SLIDER; snprintf(it.label, sizeof it.label, "%s", l); it.hint = h; it.lo = lo; it.hi = hi; it.step = st; it.fmt = fmt; it.getd = g; it.setd = s; menu_add(menu, &it); }
static void last_enabled(int (*e)(void)) { menu_last_enabled(menu, e); }
static int sh_internal(void) { return !vcfg.external[0]; }
static void add_action(const char *l, const char *h, void (*a)(void)) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_ACTION; snprintf(it.label, sizeof it.label, "%s", l); it.hint = h; it.act = a; menu_add(menu, &it); }
static void add_tab(const char *l, const char *hint) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_TAB; snprintf(it.label, sizeof it.label, "%s", l); it.hint = hint; menu_add(menu, &it); }
static void add_sep(const char *l, int plain) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_SEP; snprintf(it.label, sizeof it.label, "%s", l); it.plain = plain; menu_add(menu, &it); }

static void add_image(const char *path) { MItem it; memset(&it, 0, sizeof it); it.kind = MK_IMAGE; it.path = _strdup(path); menu_add(menu, &it); }
static void build_controls_menu(void) {                                        /* the window to redefine the keys and the pad buttons */
    menu = menu_ctl = menu_new("title.controls");
    add_sep("sep.keyboard", 0);
    for (int a = 0; a < KA_COUNT; a++) { add_action(key_act_label_id(a), key_act_hint(a), KAF[a]); menu_last_valtext(menu, KV[a]); }
    add_sep("sep.gamepad_both_pads", 0);
    for (int a = 0; a < PBA_COUNT; a++) { add_action(pad_act_label_id(a), pad_act_hint(a), PAF[a]); menu_last_valtext(menu, PV[a]); }
    add_sep("", 0);
    add_action("menu.restore_default_controls", "menu.restore_default_controls.hint", act_ctl_defaults);
    add_action("menu.back", "menu.back.hint", act_ctl_back);
}
static void build_menu(int have_audio, int have_gl, int have_pack) {
    menu = menu_main = menu_new("title.options");
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
        add_choice("menu.aspect_ratio", "menu.aspect_ratio.hint", a_nch, a_txt, a_get, a_set); menu_last_valhint(menu, a_hint);
        add_choice("menu.shader_source", "menu.shader_source.hint", ss_nch, ss_txt, ss_get, ss_set); menu_last_valhint(menu, ss_hint);
        add_choice("menu.scaling_filter", "menu.scaling_filter.hint", f_nch, f_txt, f_get, f_set); menu_last_valhint(menu, f_hint);
        add_choice("menu.shader_crt_effect", "menu.shader_crt_effect.hint", sh_count, sh_txt, sh_get, sh_set); menu_last_valhint(menu, sh_hint);
        add_slider("  menu.scanlines", "menu.scanlines.hint", 0, 1, 0.05, NULL, g_scan, s_scan); last_enabled(sh_internal);
        add_slider("  menu.phosphor_mask", "menu.phosphor_mask.hint", 0, 1, 0.05, NULL, g_mask, s_mask); last_enabled(sh_internal);
        add_slider("  menu.curvature", "menu.curvature.hint", 0, 1, 0.05, NULL, g_curv, s_curv); last_enabled(sh_internal);
        add_slider("  menu.brightness", "menu.brightness.hint", 0.5, 1.8, 0.05, "%.2f", g_bright, s_bright); last_enabled(sh_internal);
        add_slider("  menu.vignette", "menu.vignette.hint", 0, 1, 0.05, NULL, g_vig, s_vig); last_enabled(sh_internal);
        add_slider("  menu.glow", "menu.glow.hint", 0, 1, 0.05, NULL, g_bloom, s_bloom); last_enabled(sh_internal);
    }
    if (have_pack) add_choice("menu.graphics_pack", "menu.graphics_pack.hint", pk_nch, pk_disp, pk_get, pk_set); menu_last_valhint(menu, pk_hint);
    add_tab("tab.motion", "tab.motion.hint");
    add_bool("menu.frame_interpolation", "menu.frame_interpolation.hint", g_interp, s_interp);
    add_bool("menu.sub_pixel_smoothing", "menu.sub_pixel_smoothing.hint", g_subpx, s_subpx);
    add_choice("  menu.game_speed_no_interpolation", "menu.game_speed_no_interpolation.hint", gs_nch, gs_txt, gs_get, gs_set); menu_last_valhint(menu, gs_hint);
    add_sep("sep.latency", 0);
    add_bool("menu.fast_input_polling", "menu.fast_input_polling.hint", g_fastin, s_fastin);
    if (have_audio) {
        add_bool("menu.low_latency_audio", "menu.low_latency_audio.hint", g_llaud, s_llaud);
        add_slider("  menu.audio_sync_trim_ms", "menu.audio_sync_trim_ms.hint", -100.0, 100.0, 5.0, "%.0f", g_atrim, s_atrim);
    }
    add_tab("tab.controls", "tab.controls.hint");
    add_action("menu.keys_and_gamepad_buttons", "menu.keys_and_gamepad_buttons.hint", open_controls);
    { char ip[800]; snprintf(ip, sizeof ip, "%s/assets/controller.png", app); add_image(ip); }
    add_tab("tab.system", "tab.system.hint");
    add_choice("menu.language", "menu.language.hint", lg_nch, lg_txt, lg_get, lg_set);
    add_action("menu.restore_default_options", "menu.restore_default_options.hint", act_restore);
    add_sep("sep.credits", 0); add_sep("Claude Sonnet 5.5 & VooD", 1); add_sep("", 0);
    add_action("menu.continue", "menu.continue.hint", act_close);
    add_action("menu.quit", "menu.quit.hint", act_quit);
    (void)M; (void)last_item;
    build_controls_menu(); menu = menu_main;
}
static MItem *last_item(void) { return NULL; }

/* ------------------------------------------------------------------------------------------------ game */
static void open_menu(void) { { char cur[80]; snprintf(cur, sizeof cur, "%s", music_pack); scan_packs(); pack_resolve(cur); } rb_kind = 0; menu = menu_main; g_menu_open_f = 1; menu_show(menu); emu_post(EV_HOLD, 0, 0); emu_post(EV_HOLD, 1, 0); last_hold[0] = last_hold[1] = 0; emu_post(EV_JOY, 0, 0); emu_post(EV_JOY0, 0, 0); emu_post(EV_PAUSE, 1, 0); menu_sig_last[0] = 0; }
static void close_menu(void) { rb_kind = 0; menu_hide(menu_ctl); menu_hide(menu_main); menu = menu_main; g_menu_open_f = 0; last_j1_reset = 1; emu_post(EV_PAUSE, 0, 0); save_cfg(); }

/* The original game file FILES/DOW must be in the "data" folder next to the executable. */
static void find_game_dir(void) {
    char p[800]; snprintf(p, sizeof p, "%s/data/FILES/DOW", app);
    if (file_exists(p)) snprintf(game_dir, sizeof game_dir, "%s/data", app); else game_dir[0] = 0;
}

#ifdef _WIN32
/* Fast input polling ("Fast input polling" option): the main loop only reads the pad and the keyboard once per image (every ~16 ms) and stays blocked waiting for the refresh. This thread polls the
   XInput pads and the joystick keys (arrows, Ctrl, Z) every millisecond and hands the state to the emulation as soon as it changes. It follows the same rules as the main loop: nothing with the menu open,
   a key or button used to accept the menu does not count until it is released, and only with the window in the foreground. */
static DWORD WINAPI fastin_thread(LPVOID arg) {
    (void)arg; timeBeginPeriod(1); SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    int l1 = -1, l0 = -1, lh[2] = { -1, -1 }; 
    while (running) {
        if (!fastin_on() || g_menu_open_f || !p_held) { l1 = l0 = -1; lh[0] = lh[1] = -1; Sleep(fastin_on() ? 1 : 20); continue; }
        PadRead pr[2]; int n = pads_read(pr); int kb = 0;
        if (getenv("DOW_FASTIN_TEST") || GetForegroundWindow() == (HWND)video_hwnd()) { int ks[16], nk = kbd_joy_keys(ks); for (int i = 0; i < nk; i++) if ((GetAsyncKeyState(ks[i]) & 0x8000) && !p_held[ks[i] & 511]) kb |= kbd_joy_bit(ks[i]); }
        int st[2] = { 0, 0 }; for (int i = 0; i < n && i < 2; i++) { st[i] = pr[i].st; if (p_padhold[i] && (st[i] & J_FIRE)) st[i] &= ~J_FIRE; }
        int j1 = kb | st[0], j0 = st[1];
        for (int i = 0; i < 2; i++) { int h = i < n ? pr[i].hold : 0; if (h != lh[i]) { lh[i] = h; emu_post(EV_HOLD, i, h); } }
        if (j1 != l1) { l1 = j1; emu_post(EV_JOY, j1, 0); if (getenv("DOW_FASTIN_LOG")) host_log_file("fastin j1=%02X t=%.4f", j1, now_s()); }
        if (j0 != l0) { l0 = j0; emu_post(EV_JOY0, j0, 0); }
        Sleep(1);
    }
    return 0;
}
#endif

int main(int argc, char **argv) {
    snprintf(app, sizeof app, "%s", app_dir());
    int no_sound = 0, log_on = 0, fix_cli = -1; fixv = 1; (void)log_on;
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i];
        if (!strcmp(a, "--original") || !strcmp(a, "--no-fix")) fix_cli = 0;
        else if (!strcmp(a, "--fix")) fix_cli = 1;
        else if (!strcmp(a, "--no-sound") || !strcmp(a, "--mute")) no_sound = 1;
        else if (!strcmp(a, "--volume") && i + 1 < argc) volm = atof(argv[++i]);
        else if (!strcmp(a, "--pack") && i + 1 < argc) snprintf(pack_cli, sizeof pack_cli, "%s", argv[++i]);
    }
    if (getenv("DOW_FIX") && getenv("DOW_FIX")[0] == '0') fix_cli = 0;
    if (getenv("DOW_SONIDO") && getenv("DOW_SONIDO")[0] == '0') no_sound = 1;
    if (getenv("DOW_VOL")) volm = atof(getenv("DOW_VOL"));
    if (getenv("DOW_GRAFICO")) snprintf(pack_cli, sizeof pack_cli, "%s", getenv("DOW_GRAFICO"));
    find_game_dir();
    if (!game_dir[0]) { MessageBoxA(NULL, "The original game file was not found.\nCopy the FILES\\DOW file of the original Dogs of War into the \"data\\FILES\" folder next to dowport.exe.", "Dogs of War", MB_OK | MB_ICONERROR); return 1; }
    { char ld[800]; snprintf(ld, sizeof ld, "%s/languages", game_dir); lang_init(ld); }
    snprintf(cfg_path, sizeof cfg_path, "%s/options.json", app); snprintf(default_path, sizeof default_path, "%s/default.json", app); snprintf(legacy_path, sizeof legacy_path, "%s/opciones.json", app); snprintf(music_root, sizeof music_root, "%s/music", app); snprintf(save_dir, sizeof save_dir, "%s/saves", app);
    const char *ad = getenv("DOW_ASSETS_DIR"); if (ad) snprintf(assets_root, sizeof assets_root, "%s", ad); else snprintf(assets_root, sizeof assets_root, "%s/assets", app);
    if (getenv("DOW_BOOT_OUT")) { State *bs = boot_state_make(game_dir); int okb = bs && state_write_file(bs, getenv("DOW_BOOT_OUT")); host_log("start-up state: %s", okb ? "written" : "FAILED"); return okb ? 0 : 1; }       /* development: the state the loader leaves, to a file */
    if (getenv("DOW_DUMP_OUT")) {                                              /* development: graphics dump into the given folder, then exit */
        double t0d = now_s(); int okd = 0; if (!dump_begin(game_dir, getenv("DOW_DUMP_OUT"))) return 1;
        while (!dump_finished(&okd)) Sleep(100);
        host_log("graphics dump: %s in %.1f s", okd ? "ok" : "FAILED", now_s() - t0d); return okd ? 0 : 1;
    }
    hz = video_refresh_hz(); if (hz < 50) hz = 50; if (hz > 165) hz = 165;
    S = (int)env_d("DOW_SCALE", hz <= 75 ? 3 : 2); if (S < 1) S = 1; if (S > RS_MAX) S = RS_MAX;
    int test_run = getenv("DOW_TEST") || getenv("DOW_SECONDS");
    if (file_exists(cfg_path)) cfg_load(cfg_path); else if (file_exists(legacy_path)) cfg_load(legacy_path); else cfg_load(default_path);   /* without options.json: the one from earlier versions (opciones.json) or, failing that, default.json */
    lang_set(cfg_str("language", "en")); input_cfg_load(); scan_packs(); pack_resolve(cfg_str("music_pack", "arrange"));
    if ((int)cfg_num("version", 1) < 2) { cfg_remove("preset"); cfg_remove("external"); cfg_remove("filter"); cfg_remove("aspect"); cfg_remove("fullscreen"); for (const char *k = "params.p_scan"; 0; ) (void)k; }
    fixv = fix_cli >= 0 ? fix_cli : cfg_bool("enemy_script_fix", 1);                          /* enemy-script fix: only in options.json (on by default) or by the command line */
    snd = cfg_bool("sound", 1) && !no_sound; if (!getenv("DOW_VOL") && cfg_has("volume") && volm == 1.0) volm = cfg_num("volume", 1.0); mp3v = cfg_bool("mp3_music", 1); mchv = cfg_bool("multichannel_sfx", 0); panv = cfg_bool("stereo_pan", 0);
    pflags.subpx = cfg_bool("subpixel_smoothing", 1); pflags.sfix = cfg_bool("scroll_fix", 1) && !getenv("DOW_NOSFIX"); pflags.interp = cfg_bool("frame_interpolation", 1) && !getenv("DOW_NOINTERP"); gmatch = cfg_bool("game_speed_match", 0) || getenv("DOW_SPEEDMATCH") != NULL; pflags.extrap = cfg_bool("extrapolate", 1); pflags.xfilter = cfg_bool("filter_x", 1); pflags.yfilter = cfg_bool("filter_y", 1); pflags.res = cfg_bool("residual_layer", 1);
    if (cfg_has("latency") && !getenv("DOW_LEAD")) { lead_norm = cfg_num("latency", LEAD_DEFAULT); if (lead_norm < 0.5) lead_norm = 0.5; if (lead_norm > 6) lead_norm = 6; }
    fastinp = cfg_bool("fast_input", 0); ll_audio = cfg_bool("low_latency_audio", 0); atrim = cfg_num("audio_sync_trim_ms", 0.0);
    if (getenv("DOW_FASTIN")) fastinp = getenv("DOW_FASTIN")[0] != '0';
    lead_ni = cfg_num("latency_nointerp", 0.5); if (lead_ni < 0.0) lead_ni = 0.0; if (lead_ni > 6) lead_ni = 6;
    lead = pflags.interp ? lead_norm : lead_ni;
    if (getenv("DOW_LEAD")) { lead_norm = atof(getenv("DOW_LEAD")); lead = pflags.interp ? lead_norm : lead_ni; }
    /* video */
    int gl_ok = video_open(320 * S, 200 * S, 320 * S, 200 * S, ""); char sh_dir[700]; snprintf(sh_dir, sizeof sh_dir, "%s/shaders", app);
    if (!gl_ok) { MessageBoxA(NULL, "OpenGL is not available.", "Dogs of War", MB_OK | MB_ICONERROR); return 1; }
    video_close(); gl_ok = video_open(320 * S, 200 * S, 320 * S, 200 * S, sh_dir);
    if (!gl_ok) return 1;
    apply_video_cfg();
    if (cfg_bool("fullscreen", !test_run)) { fs_state = 1; video_set_fullscreen(1); }
    /* audio */
    if (snd || cfg_bool("sound", 1)) aud = audio_open(AUDIO_FS, (lead + 1.0) * 20.0 + 30.0);
    if (aud && ll_audio && !audio_set_lowlat(aud, 1)) ll_audio = 0;
    if (aud) audio_set_target(aud, aud_target_ms());
    if (!aud && !getenv("DOW_FAKEAUDIO")) snd = 0;                             /* (DOW_FAKEAUDIO: tests without a sound device) */
    pres = present_new(S); present_set_ahead(pres, 0.6 * 60.0 / hz);
    /* first run: the graphics dump (assets/original) is made from the original files */
    EmuConfig ec; memset(&ec, 0, sizeof ec); char music_dir[700]; pack_dir(music_pack, music_dir, sizeof music_dir);
    char dump_idx[700]; snprintf(dump_idx, sizeof dump_idx, "%s/original/index.json", assets_root);
    if (!getenv("DOW_STATE") && !getenv("DOW_NODUMP") && !file_exists(dump_idx)) {      /* no graphics dump yet: the program plays the game by itself and writes it (a few seconds) */
        char out[700]; snprintf(out, sizeof out, "%s/original", assets_root); video_title("Dogs of War - preparing the graphics (first run, a few seconds)...");
        int okd = 0; if (dump_begin(game_dir, out)) { while (!dump_finished(&okd)) { VEvent ev0; while (video_poll(&ev0)) {} Sleep(30); } }
        host_log("graphics dump: %s", okd ? "ready" : "FAILED (graphics packs will not be available)");
    }
    video_title("Dogs of War");
    /* graphics packs */
    int have_pack = gfxpack_init(assets_root, S, pack_cli[0] ? pack_cli : cfg_str("graphics_pack", ""));
    gfxpack_attach();
    build_menu(aud != NULL, !video_is_software(), have_pack);
    /* emulation */
    g_load_seconds = cfg_num("loading_seconds", 3.0); if (g_load_seconds < 0) g_load_seconds = 0; if (g_load_seconds > 30) g_load_seconds = 30; if (getenv("DOW_TEST") && !getenv("DOW_LOAD_WAIT")) g_load_seconds = 0;
    ec.game_dir = game_dir; ec.state_path = getenv("DOW_STATE"); ec.music_dir = music_dir; ec.save_dir = save_dir; ec.audio = aud; ec.lead = lead; ec.fix = fixv; ec.god = getenv("DOW_GOD") != NULL; ec.money = getenv("DOW_MONEY") != NULL;
    ec.sound = snd; ec.mp3 = mp3v; ec.multi = mchv; ec.pan = panv; ec.vol = volm; ec.t_anchor = now_s() + 0.5; godv = ec.god; moneyv = ec.money;
    if (!emu_start(&ec)) { MessageBoxA(NULL, "Could not start the game from the original files in the data folder.", "Dogs of War", MB_OK | MB_ICONERROR); return 1; }
    emu_post(EV_GAMEHZ, game_rate(), 0);
    CloseHandle(CreateThread(NULL, 0, fastin_thread, NULL, 0, NULL));
    vx[0] = cfg_num("volume_mp3", 1.0); vx[1] = cfg_num("volume_chip_music", 1.0); vx[2] = cfg_num("volume_samples", 1.0); vx[3] = cfg_num("volume_chip_fx", 1.0);
    for (int i = 0; i < 4; i++) { if (vx[i] < 0) vx[i] = 0; if (vx[i] > 1) vx[i] = 1; emu_post(EV_VOLX, i, vx[i]); }
    emu_post(EV_HZ, hz, 0); uint32_t vbl_start = tos.vbl;
    /* drawing loop */
    double prof[5] = { 0, 0, 0, 0, 0 }, tick_t = now_s(); double t_start = now_s(); int frames = 0; int state = 0; static uint8_t held[512]; static int padhold[2]; int last_j1 = -1, last_j0 = -1; p_kstate = &state; p_held = held; p_padhold = padhold; static int pg_menu[2], pg_wb[2], pg_rock[2], pg_gren[2], pg_fire[2], pg_wnext[2];
    double vs = 0; int have_vs = 0; double limit = env_d("DOW_SECONDS", 0); const char *shots = getenv("DOW_SHOTS"); double shot_every = env_d("DOW_SHOT_EVERY", 0), next_shot = env_d("DOW_SHOT_FROM", shot_every); int shot_n = 0;
    uint8_t *frozen = NULL; static double key_rel_t[8]; static int key_rel_sc[8]; int nkr = 0; double t_prev = now_s(); int vsync_ok = -1, vs_n = 0; double ivs[64]; int niv = 0;
    int mp_prev[2] = { 0, 0 }; double mp_rep[2] = { 0, 0 };
    const char *joyenv = getenv("DOW_JOY"); (void)joyenv;
    while (running && emu_running) {
        double tl0 = now_s(); (void)tl0;
        double et = tl0 - t_start;
        if (getenv("DOW_TEST")) {                                           /* development only: automatic input */
            int f = (int)(et * 25), want = J_UP | (((f / 30) % 6 == 1) ? J_LEFT : 0) | (((f / 30) % 6 == 3) ? J_RIGHT : 0) | ((f % 10) < 5 ? J_FIRE : 0);
            if (getenv("DOW_PUSH")) { want = atoi(getenv("DOW_PUSH")) | ((f % 6) < 3 ? J_FIRE : 0); }
            if (getenv("DOW_NOINPUT")) want = 0;
            if (getenv("DOW_JOYSEQ")) {                                       /* "t:state,t:state,...": scripted joystick (seconds : bits) */
                static double jt[128]; static int js[128], jn = -1;
                if (jn < 0) { jn = 0; char buf[1500]; snprintf(buf, sizeof buf, "%s", getenv("DOW_JOYSEQ")); for (char *tok = strtok(buf, ","); tok && jn < 128; tok = strtok(NULL, ",")) { double t; int v; if (sscanf(tok, "%lf:%i", &t, &v) == 2) { jt[jn] = t; js[jn] = v; jn++; } } }
                want = 0; for (int i = 0; i < jn; i++) if (et >= jt[i]) want = js[i];
            }
            if (et > 1.0) state = want;
            const char *keys = getenv("DOW_KEYS"); static int kdone[32]; static double krel[32]; static int kn = -1; static double kt[32]; static int ks[32];
            if (keys) {
                if (kn < 0) { kn = 0; char buf[400]; snprintf(buf, sizeof buf, "%s", keys); for (char *tok = strtok(buf, ","); tok && kn < 32; tok = strtok(NULL, ",")) { double t; int sc; if (sscanf(tok, "%lf:%i", &t, &sc) == 2) { kt[kn] = t; ks[kn] = sc; kn++; } } }
                for (int i = 0; i < kn; i++) { if (!kdone[i] && et >= kt[i]) { kdone[i] = 1; krel[i] = et + 0.25; emu_post(EV_KEY, ks[i], 1); } else if (kdone[i] == 1 && et >= krel[i]) { kdone[i] = 2; emu_post(EV_KEY, ks[i], 0); } }
            }
            if (getenv("DOW_HOLD")) { static int hs_ = 0; double h0 = 0, h1 = 0; sscanf(getenv("DOW_HOLD"), "%lf:%lf", &h0, &h1); int want_h = et >= h0 && et < h1; if (want_h != hs_) { hs_ = want_h; emu_post(EV_HOLD, 0, want_h); } }       /* test: L2 held between h0 and h1 s */
            if (getenv("DOW_GOD_AT") && et > atof(getenv("DOW_GOD_AT")) && !godv) { godv = 1; emu_post(EV_GOD, 1, 0); }
            if (getenv("DOW_MSEQ")) {                                         /* "t:action,...": drives the menu by script (open close up down left right enter) */
                static double mt[64]; static char ma[64][12]; static int mn = -1, mdone[64];
                if (mn < 0) { mn = 0; char buf[800]; snprintf(buf, sizeof buf, "%s", getenv("DOW_MSEQ")); for (char *tok = strtok(buf, ","); tok && mn < 64; tok = strtok(NULL, ",")) { char *c = strchr(tok, ':'); if (c) { *c = 0; mt[mn] = atof(tok); snprintf(ma[mn], 12, "%s", c + 1); mn++; } } }
                for (int i = 0; i < mn; i++) if (!mdone[i] && et >= mt[i]) {
                    mdone[i] = 1; const char *a = ma[i];
                    if (!strcmp(a, "open")) open_menu(); else if (!strcmp(a, "close")) close_menu(); else if (!strcmp(a, "enter")) menu_activate(menu); else menu_nav(menu, a);
                }
            }
        }
        if (limit > 0 && et > limit) { if (getenv("DOW_SHOT") && !shots) video_screenshot(getenv("DOW_SHOT")); break; }
        VEvent ev;
        while (video_poll(&ev)) {
            if (ev.type == VE_QUIT) running = 0;
            else if (ev.type == VE_KEY) {
                int vk = ev.vk, down = ev.down;
                if (menu_is_open(menu)) {
                    if (rb_kind && (down || ev.rep)) {                                      /* redefining a key or a button */
                        if (!ev.rep) {
                            held[vk & 511] = 1;
                            if (vk == VK_ESCAPE) { rb_kind = 0; menu_status(menu, tr("msg.cancelled")); }
                            else if (vk == VK_DELETE) { if (rb_kind == 1) kbd_set_bind(rb_act, 0); else pad_set_bind(rb_act, 0); rb_kind = 0; menu_status(menu, tr("msg.unassigned")); }
                            else if (rb_kind == 1 && vk > 0 && vk < 256 && vk != VK_F11) { kbd_set_bind(rb_act, vk); char kn[64], t[160]; key_name(kbd_norm_vk(vk), kn, sizeof kn); trf(t, sizeof t, "msg.assigned", key_act_label(rb_act), kn); rb_kind = 0; menu_status(menu, t); }
                            menu_sig_last[0] = 0;
                        }
                        continue;
                    }
                    held[vk & 511] = (uint8_t)down;                          /* key pressed with the menu open: it does not count as a game key press until it is released (otherwise accepting with Ctrl left fire pressed on closing) */
                    if (down) {
                        if (vk == VK_PRIOR) menu_nav(menu, "tabprev"); else if (vk == VK_NEXT) menu_nav(menu, "tabnext"); else if (vk == VK_TAB) menu_nav(menu, (GetKeyState(VK_SHIFT) & 0x8000) ? "tabprev" : "tabnext");
                        else if (vk == VK_UP) menu_nav(menu, "up"); else if (vk == VK_DOWN) menu_nav(menu, "down"); else if (vk == VK_LEFT) menu_nav(menu, "left"); else if (vk == VK_RIGHT) menu_nav(menu, "right");
                        else if (vk == VK_RETURN || vk == 0x10D || vk == VK_SPACE || vk == 'Z' || vk == VK_RCONTROL || vk == VK_LCONTROL) menu_activate(menu);
                        else if (vk == VK_ESCAPE || vk == VK_BACK) menu_back();
                        else if (vk == VK_F11) { fs_state = !fs_state; video_set_fullscreen(fs_state); }
                    }
                    continue;
                }
                if (held[vk & 511]) { if (!down) held[vk & 511] = 0; continue; }
                if (down && vk == VK_ESCAPE) { open_menu(); state = 0; continue; }
                if (down && vk == VK_F11) { fs_state = !fs_state; video_set_fullscreen(fs_state); }
                if (down && !ev.rep) {
                    if (vk == VK_F9) { gfxpack_reload(); present_pack_changed(pres); }
                }
                if (down && (vk == VK_PRIOR || vk == VK_NEXT)) { { double *pl = pflags.interp ? &lead_norm : &lead_ni; *pl += vk == VK_PRIOR ? 0.5 : -0.5; if (*pl < (pflags.interp ? 0.5 : 0.0)) *pl = pflags.interp ? 0.5 : 0.0; if (*pl > 6) *pl = 6; apply_lead(); } }
                int jb = kbd_joy_bit(vk), ka = kbd_action(vk);
                if (jb) state = down ? (state | jb) : (state & ~jb);
                else if (ka == KA_WNEXT) { if (!ev.rep) emu_post(EV_KEY, 0x1E, down); }
                else if (ka == KA_WPREV) { if (down && !ev.rep) emu_post(EV_WPN, 0x1E, -1); }
                else if (ka == KA_ROCKET) { if (down && !ev.rep) emu_post(EV_QUICK, 0x1E, 0); }
                else if (ka == KA_GRENADE) { if (down && !ev.rep) emu_post(EV_QUICK, 0x1E, 1); }
                else { int sc = kbd_scancode(vk); if (sc && !ev.rep) emu_post(EV_KEY, sc, down); }
            } else if (ev.type == VE_RESIZE) { menu_sig_last[0] = 0; }
        }
        PadRead pr[2]; int npads = pads_read(pr); int menu_open = menu_is_open(menu);
        if (menu_open && rb_kind == 2) {                                     /* redefining a button: the first one pressed (not one that was already held) */
            static int rprev[2]; static const int ASSIGN[] = { 0x1000, 0x2000, 0x4000, 0x8000, 0x0100, 0x0200, 0x10000, 0x20000, 0x0020, 0x0010, 0x0040, 0x0080 };
            for (int i = 0; i < npads && i < 2 && rb_kind == 2; i++) {
                int nb = rb_prev_ok ? (pr[i].raw & ~rprev[i]) : 0; rprev[i] = pr[i].raw;
                for (size_t q = 0; q < sizeof ASSIGN / sizeof *ASSIGN; q++) if (nb & ASSIGN[q]) { pad_set_bind(rb_act, ASSIGN[q]); char bn[32], t[160]; pad_button_name(ASSIGN[q], bn, sizeof bn); trf(t, sizeof t, "msg.assigned", pad_act_label(rb_act), bn); rb_kind = 0; rb_wait_release = 1; menu_status(menu, t); menu_sig_last[0] = 0; break; }
            }
            rb_prev_ok = 1; npads = 0;
        } else if (rb_wait_release) { int any = 0; for (int i = 0; i < npads && i < 2; i++) any |= pr[i].raw; if (any) npads = 0; else rb_wait_release = 0; }
        if (!fastin_on()) for (int i = 0; i < 2; i++) { int h = (i < npads && !menu_open) ? pr[i].hold : 0; if (h != last_hold[i]) { last_hold[i] = h; emu_post(EV_HOLD, i, h); } }       /* L2: hold position (with fast polling the thread sends it) */
        for (int i = 0; i < npads; i++) {                                    /* A dispara / acepta; R1 R2 = arma siguiente; L1 L2 = anterior; B = cohetes; X = granadas; Start = menu; Back = salir */
            int menu_btn = !!(pr[i].acts & PA_MENU), wb = !!(pr[i].acts & PA_WBACK), rk = !!(pr[i].acts & PA_ROCKET), gr = !!(pr[i].acts & PA_GRENADE), ex = !!(pr[i].acts & PA_EXIT);
            if (menu_btn && !pg_menu[i]) { if (menu_open) close_menu(); else open_menu(); menu_open = menu_is_open(menu); }
            pg_menu[i] = menu_btn;
            if (wb && !pg_wb[i] && !menu_open) emu_post(EV_WPN, i == 0 ? 0x1E : 0x4E, -1);
            pg_wb[i] = wb;
            if (rk && !pg_rock[i]) { if (menu_open) { menu_back(); menu_open = menu_is_open(menu); } else emu_post(EV_QUICK, i == 0 ? 0x1E : 0x4E, 0); }
            pg_rock[i] = rk;
            if (gr && !pg_gren[i] && !menu_open) emu_post(EV_QUICK, i == 0 ? 0x1E : 0x4E, 1);
            pg_gren[i] = gr;
            int fire = !!(pr[i].st & J_FIRE);
            if (fire && !pg_fire[i] && !strcmp(present_screen_kind(pres), "players") && !menu_open && nkr < 8) { emu_post(EV_KEY, i == 0 ? 0x02 : 0x03, 1); key_rel_t[nkr] = tl0 + 0.12; key_rel_sc[nkr] = i == 0 ? 0x02 : 0x03; nkr++; }
            pg_fire[i] = fire;
            if (pr[i].wnext && !pg_wnext[i]) emu_post(EV_KEY, i == 0 ? 0x1E : 0x4E, 1); else if (!pr[i].wnext && pg_wnext[i]) emu_post(EV_KEY, i == 0 ? 0x1E : 0x4E, 0);
            pg_wnext[i] = pr[i].wnext;
            static int pg_exit[2]; if (ex && !pg_exit[i]) emu_post(EV_EXIT, 0, 0); pg_exit[i] = ex;
        }
        for (int k = 0; k < nkr; ) { if (tl0 >= key_rel_t[k]) { emu_post(EV_KEY, key_rel_sc[k], 0); key_rel_t[k] = key_rel_t[nkr - 1]; key_rel_sc[k] = key_rel_sc[nkr - 1]; nkr--; } else k++; }
        if (menu_open) {                                                     /* the pad drives the menu and does not reach the game */
            for (int i = 0; i < npads; i++) {
                int cur = pr[i].st;
                static const struct { int bit; const char *nm; } D[4] = { { J_UP, "up" }, { J_DOWN, "down" }, { J_LEFT, "left" }, { J_RIGHT, "right" } };
                for (int d = 0; d < 4; d++) if (cur & D[d].bit) { if (!(mp_prev[i] & D[d].bit)) { menu_nav(menu, D[d].nm); mp_rep[i] = tl0 + 0.40; } else if (tl0 >= mp_rep[i]) { menu_nav(menu, D[d].nm); mp_rep[i] = tl0 + 0.09; } }
                if ((cur & J_FIRE) && !(mp_prev[i] & J_FIRE)) menu_activate(menu);
                { static int sh_prev[2]; int rw = pr[i].raw; if ((rw & 0x0100) && !(sh_prev[i] & 0x0100)) menu_nav(menu, "tabprev"); if ((rw & 0x0200) && !(sh_prev[i] & 0x0200)) menu_nav(menu, "tabnext"); sh_prev[i] = rw; }       /* L1 / R1 (XInput shoulder buttons): previous / next tab */
                mp_prev[i] = cur;
            }
            npads = 0;
        }
        if (menu_open) { for (int i = 0; i < 2; i++) padhold[i] = 1; }                                         /* same with the pads: the button used to accept does not fire until it is released */
        else for (int i = 0; i < npads && i < 2; i++) { if (padhold[i] && (pr[i].st & J_FIRE)) pr[i].st &= ~J_FIRE; else padhold[i] = 0; }
        int j1 = menu_open ? 0 : (state | (npads > 0 ? pr[0].st : 0)), j0 = npads > 1 ? pr[1].st : 0;
        g_menu_open_f = menu_open;
        if (last_j1_reset) { last_j1_reset = 0; last_j1 = last_j0 = -1; last_hold[0] = last_hold[1] = -1; }
        if (!fastin_on()) {                                                 /* with fast input polling the polling thread sends the joystick (1 kHz) */
            if (j1 != last_j1) { last_j1 = j1; emu_post(EV_JOY, j1, 0); }
            if (j0 != last_j0) { last_j0 = j0; emu_post(EV_JOY0, j0, 0); }
        }
        if (!menu_open) emu_post(EV_TICK, 0, 0);
        int is_menu_img = 0;
        if (!menu_open) { present_drain(pres); }
        if (!menu_open && !strcmp(present_screen_kind(pres), "map")) { double tq = now_s(); present_wait_menu(pres, 0.006); prof[3] += now_s() - tq; }
        else if (!menu_open && present_umode_h(pres) == 0 && 0) present_wait_menu(pres, 0.006);
        double t0 = now_s(); if (!have_vs) { vs = t0 + 1.0 / hz; have_vs = 1; }
        double at = vs + 0.5 / hz;
        if (menu_open) present_drain(pres);
        { char txt[256]; double tt; if (present_take_text(pres, txt, sizeof txt, &tt)) { menu_status(menu, txt); menu_sig_last[0] = 0; } }
        const uint8_t *img; uint8_t *copy = NULL;
        if (menu_open && frozen) img = frozen;
        else { double tq = now_s(); img = present_frame(pres, at, &is_menu_img); prof[0] += now_s() - tq; if (menu_open) { frozen = (uint8_t *)malloc((size_t)320 * 200 * S * S * 3); memcpy(frozen, img, (size_t)320 * 200 * S * S * 3); img = frozen; } }
        (void)copy;
        if (!menu_open && frozen) { free(frozen); frozen = NULL; }
        { double tq = now_s(); video_upload(img); prof[1] += now_s() - tq; }
        int rx, ry, rw, rh; video_rect(&rx, &ry, &rw, &rh);
        if (menu_open) {
            char sig[600]; menu_signature(menu, rw, rh, sig, sizeof sig);
            if (strcmp(sig, menu_sig_last)) { snprintf(menu_sig_last, sizeof menu_sig_last, "%s", sig); uint8_t *ov = menu_draw(menu, rw, rh); video_overlay(ov, rw, rh); free(ov); ov_on = 1; }
        } else if (ov_on) { ov_on = 0; menu_sig_last[0] = 0; video_overlay(NULL, 0, 0); }
        if (shots && (shot_every > 0 ? et >= next_shot : (et > env_d("DOW_SHOT_T", 8) && shot_n < 40))) {
            char p[800]; if (shot_every > 0) { snprintf(p, sizeof p, "%s/t%07d.png", shots, (int)(et * 1000)); next_shot += shot_every; } else snprintf(p, sizeof p, "%s/f%02d.png", shots, shot_n++);
            video_screenshot(p);
        }
        { double tq = now_s(); video_draw(); prof[2] += now_s() - tq; } frames++;
        { const char *er = video_take_error(); if (er) host_log("%s", er); }
        const char *perr = present_error(pres); if (perr) { host_log("Emulation ERROR: %s", perr); video_title("Dogs of War - EMULATION ERROR (see error.log next to the program)"); sleep_ms(4000); break; }
        double t3 = now_s(); double interval = t3 - t_prev; t_prev = t3; if (niv < 64) ivs[niv++] = interval * 1000.0; else { memmove(ivs, ivs + 1, sizeof(double) * 63); ivs[63] = interval * 1000.0; }
        double err = t3 - vs; if (fabs(err) > 0.5 / hz) vs = t3 + 1.0 / hz; else vs = vs + 1.0 / hz + 0.05 * err;
        { static int fs_seen = -1; int fsn = video_is_fullscreen(); if (fsn != fs_seen) { fs_seen = fsn; vsync_ok = -1; vs_n = 0; niv = 0; } vs_n++; }       /* another synchronization mode: it is measured again */
        if (vsync_ok < 0 && vs_n > 90) { double tmp[64]; memcpy(tmp, ivs, sizeof(double) * (size_t)niv); for (int a = 1; a < niv; a++) { double x = tmp[a]; int b = a - 1; while (b >= 0 && tmp[b] > x) { tmp[b + 1] = tmp[b]; b--; } tmp[b + 1] = x; } vsync_ok = tmp[niv / 2] > 0.8 * 1000.0 / hz; }
        if (vsync_ok == 0) { double rest = tick_t + 1.0 / hz - now_s(); if (rest > 0) { double tq = now_s(); sleep_ms(rest * 1000.0); prof[4] += now_s() - tq; } }       /* without real vsync: limits to the monitor frequency (like clock.tick) */
        tick_t = now_s();
        if (frames % 120 == 0) { char title[200]; snprintf(title, sizeof title, "Dogs of War - %.0f fps%s%s%s", 1.0 / (interval > 0 ? interval : 1), godv ? "  [GOD MODE]" : "", fixv ? "  [FIX]" : "", moneyv ? "  [MONEY]" : ""); video_title(title); }
    }
    emu_post(EV_QUIT, 0, 0); emu_join();
    if (getenv("DOW_ATLAS")) { extern void scene_dump_atlas(void); scene_dump_atlas(); }
    save_cfg();
    if (getenv("DOW_TEST")) {
#ifdef _WIN32
        FILETIME c_, e_, k_, u_; GetProcessTimes(GetCurrentProcess(), &c_, &e_, &k_, &u_);
        double cpu = (((uint64_t)k_.dwHighDateTime << 32 | k_.dwLowDateTime) + ((uint64_t)u_.dwHighDateTime << 32 | u_.dwLowDateTime)) / 1e7;
        { FILETIME c2, e2, k2, u2; GetThreadTimes(GetCurrentThread(), &c2, &e2, &k2, &u2); host_log("main thread: CPU %.1f s (kernel %.1f s)", (((uint64_t)k2.dwHighDateTime << 32 | k2.dwLowDateTime) + ((uint64_t)u2.dwHighDateTime << 32 | u2.dwLowDateTime)) / 1e7, ((uint64_t)k2.dwHighDateTime << 32 | k2.dwLowDateTime) / 1e7); }
        host_log("test: presenter %.1f s, upload %.1f s, drawing %.1f s, screen wait %.1f s, idle %.1f s (hz %d, vsync %d)", prof[0], prof[1], prof[2], prof[3], prof[4], hz, vsync_ok); host_log("test: %d frames in %.1f s (%.1f fps), CPU %.1f s, %u game VBL (%.1f per second)", frames, now_s() - t_start, frames / (now_s() - t_start), cpu, tos.vbl - vbl_start, (tos.vbl - vbl_start) / (now_s() - t_start));
#endif
    }
    if (aud && getenv("DOW_TEST")) { int u, d; long pu; audio_stats(aud, &u, &d, &pu); host_log("audio: %d gaps, %d drops, %ld samples; device %.0f ms + buffer %.0f ms (target %.0f ms) = %.0f ms delay; emulation margin %.1f VBL", u, d, pu, audio_device_ms(aud), audio_level_ms(aud), aud_target_ms(), audio_device_ms(aud) + audio_level_ms(aud), lead); }
    if (aud && !getenv("DOW_TEST")) { int u, d; long pu; audio_stats(aud, &u, &d, &pu); if (u || d) host_log_file("audio: %d gaps, %d buffer trims in this session", u, d); }
    if (aud) audio_close(aud);
    video_close();
    return 0;
}
