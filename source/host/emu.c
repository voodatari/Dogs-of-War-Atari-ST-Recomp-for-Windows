/*
 * emu.c - Emulation thread: game clock, input with minimum hold, audio, publication of frames and fixed screens, machine snapshots
 * (map and shop, saved games) and the game fixes hooked into the code (horizontal scroll, sound).
 */
#include "emu.h"
#include "lang.h"
#ifdef _WIN32
#include <windows.h>
#endif

volatile int emu_running = 0;
#define J_UP 1
#define J_DOWN 2
#define J_LEFT 4
#define J_RIGHT 8
#define J_FIRE 0x80
#define HOLD 3
#define ATRASO_MAX 6.0

static EmuConfig cfg;
#ifdef _WIN32
static HANDLE th = NULL; static HANDLE ev_signal = NULL; static CRITICAL_SECTION ev_cs;
#endif
typedef struct Ev { int type; double a, b; } Ev;
#define EVCAP 1024
static Ev evq[EVCAP]; static int ev_h = 0, ev_n = 0;

static char music_req[700];
void emu_set_music_dir(const char *path) { snprintf(music_req, sizeof music_req, "%s", path); emu_post(EV_MUSICDIR, 0, 0); }
void emu_post(int type, double a, double b) {
#ifdef _WIN32
    EnterCriticalSection(&ev_cs);
    if (ev_n < EVCAP) { evq[(ev_h + ev_n) % EVCAP].type = type; evq[(ev_h + ev_n) % EVCAP].a = a; evq[(ev_h + ev_n) % EVCAP].b = b; ev_n++; }
    LeaveCriticalSection(&ev_cs); SetEvent(ev_signal);
#endif
}
static int ev_pop(Ev *e) {
    int ok = 0;
#ifdef _WIN32
    EnterCriticalSection(&ev_cs);
    if (ev_n) { *e = evq[ev_h]; ev_h = (ev_h + 1) % EVCAP; ev_n--; ok = 1; }
    LeaveCriticalSection(&ev_cs);
#endif
    return ok;
}

/* ------------------------------------------------------------------------------------------------ state */
static double clk_t, clk_v, clk_r = 50.0; static double lead = 3.6;
static double game_hz = 50.0; static int menu_clock = 0;                 /* game_hz: VBL per second during the game (50 original; with "Match screen refresh" the monitor frequency / k); menu_clock: the clock follows the drawing (menus) */
static int paused = 0, ticks = 0; static double menu_hz = 60.0;
static int audio_on = 1; static double vol = 1.0;
static double vol_mp3 = 1.0, vol_ay = 1.0, vol_smp = 1.0, vol_fx = 1.0;            /* volumes per type: MP3 music, chip music, sampled effects, chip effects */
static int mp3_on = 0, mp3_active = 0, sfx_multi = 0, sfx_pan = 0;
static int fix_on = 0, god_on = 0, money_on = 0;
static int cur_song = -1, song0 = 0;
/* Playback of the mp3 files: pb_song is the song that is HEARD. The ones that end by themselves (music_loops == 0) play once and whole: the game waits for them (virtual flag $424F2) and a song change or a stop
   that arrives while they play is queued in pend_song (-2 = nothing, -1 = silence) until they finish. */
static int pb_song = -1, pend_song = -2, pb_done = 0, vf_on = 0; static uint8_t vf_real;
static Music *music; static Looper looper; static YM *ym;
static int skip_prev_fire = 0, skip_key = 0, skip_fade = 0, skip_total = 0;               /* a button pressed while a song that ends by itself plays fades it out (see audio_chunk) */
static void pb_reset(int s) { pb_song = s; pend_song = -2; pb_done = 0; looper.pos = 0; vf_on = 0; skip_fade = skip_total = 0; }
static int pb_once(void) { return mp3_on && audio_on && pb_song >= 0 && !music_loops(pb_song) && !pb_done && (music_get(music, pb_song, NULL) != NULL || music_pending(music, pb_song)); }          /* an mp3 of the ones that end by themselves is playing (or loading) */
int hold_fail_screen(void) { return pb_once() && pb_song == 7; }                                          /* the mission failed screen waits while the mp3 of its song plays (see game_00A000.c) */
static int pb_fixed(void) { return mp3_on && audio_on && pb_song >= 0 && !music_loops(pb_song) && (music_get(music, pb_song, NULL) != NULL || music_pending(music, pb_song)); }
static int have_frame = 0; static uint32_t last_frame_vbl = 0;
static int last_gp = 0; static int want_song = 0;
static char kind_now[16] = "";
static uint8_t last_menu_idx[64000]; static Zone last_menu_zones[MAXZONES]; static int last_menu_nz = -1;
static int wj[2] = { 0, 0 }, jj[2] = { 0, 0 }, fo[2] = { 0, 0 }; static uint32_t fv_[2] = { 0, 0 }; static int force_fire[2] = { 0, 0 };
typedef struct KeyEv { int scan, down; } KeyEv;
static KeyEv keyq[256]; static int nkeyq = 0; static uint32_t down_at[256]; static uint32_t key_free = 0;
static uint32_t fire_last = (uint32_t)-999;
static double t_rep = 0, busy = 0; static int steps = 0; static double stepmax = 0; static int slow = 0, stalls = 0;
static char save_dir[500];
#define SHOP_PAL_HEX "0000076205550443023301520520002107760122053203200131001100470542"
#define MAP_PAL_HEX "0000077700120014002501360246035604670740063005200353024201310020"
static uint8_t SHOP_PAL[32], MAP_PAL[32];

static double tvf(void) { return clk_v + (now_s() - clk_t) * clk_r; }
static void post_rate(void) { Msg *m = msg_new(MSG_RATE); m->t0 = clk_t; m->vbl0 = clk_v; m->rate = clk_r; msgq_push(m); }
static void set_rate(double rate, double add) { double now = now_s(); clk_v = tvf() + add; clk_t = now; clk_r = rate; tos.vbl_rate = rate; post_rate(); }
static void say(const char *txt);
static void say_id(const char *id) { say(tr(id)); }
static void say(const char *txt) { Msg *m = msg_new(MSG_TEXT); snprintf(m->text, sizeof m->text, "%s", txt); msgq_push(m); }
static void hex2bin(const char *h, uint8_t *o, int n) { for (int i = 0; i < n; i++) { unsigned v; sscanf(h + 2 * i, "%2x", &v); o[i] = (uint8_t)v; } }

/* ------------------------------------------------------------------------------------------------ game patches and hooks */
#define POKE16(a, v) do { tos_ilog('P', (int)(a), (int)(v)); mw16((a), (v)); } while (0)
static void apply_fix(void) { tos_set_patch(1, fix_on); }                /* enemy-script fix ($B13E: RTS -> BRA.B to $B0C6) */
static void set_god(int on) { god_on = on; tos_set_patch(0, on); }       /* immunity: $AA42 (start of the player's death) with an RTS on top */

static void hk_song(uint32_t a) {
    (void)a; int n = (int)(g_cpu.d[0] & 0xFFFF);
    if (getenv("DOW_SONGLOG")) host_log_file("song %d (vbl %u, pc %06X, ret %06X)", n, tos.vbl, g_cpu.pc, mr32(g_cpu.a[7]));
    if (n == 0) song0 = 1; cur_song = n; tos.song_now = n; vf_on = 0;
    if (pb_once()) pend_song = n; else pb_reset(n);
}
static void hk_stop(uint32_t a) {                                          /* (the start-song routine itself calls it first: return $41C28) */
    (void)a; if (mr32(g_cpu.a[7]) == 0x41C28) return;
    cur_song = -1; tos.song_now = -1; vf_on = 0;
    if (pb_once()) pend_song = -1; else pb_reset(-1);
}
static const struct { uint32_t ret; int reg; int off; } PAN_SITES[] = {      /* register with the object the sound comes from (depending on where the game calls from) */
    { 0x6A4A, 4, 0 }, { 0x6B3C, 3, 0 }, { 0x6CEA, 4, 0 }, { 0x72EE, 4, 2 }, { 0xA660, 6, 0 }, { 0xACF0, 4, 0 }, { 0xB9CC, 4, 0 },
    { 0x9442, 4, 2 }, { 0x9542, 4, 2 }, { 0xA9CC, 4, 2 } };
static float pan_of(void) {
    uint32_t ret = mr32(g_cpu.a[7]); int k;
    for (k = 0; k < 10; k++) if (PAN_SITES[k].ret == ret) break;
    if (k == 10) return 0.5f;
    int x; if (PAN_SITES[k].reg == 3) x = (int)(g_cpu.d[3] & 0xFFFF); else x = (int)mr16((g_cpu.a[PAN_SITES[k].reg] & 0xFFFFFF) + (uint32_t)PAN_SITES[k].off);
    float p = (float)((x - 100.0) / 192.0); return p < 0 ? 0 : p > 1 ? 1 : p;       /* 0 = left edge of the game area, 1 = right edge (game x = screen + 36) */
}
static void hk_sample(uint32_t a) {
    int n = (int)(g_cpu.d[0] & 0xFFFF); if (n > 8) return;
    if (sfx_multi || sfx_pan) mixer_start(n, a == 0xE6EC ? 0 : 1, 0.0, sfx_pan ? pan_of() : 0.5f);
}
static void hk_ymtag(uint32_t a) { (void)a; extern int g_ym_tag; g_ym_tag = 1; }
static void hk_hscroll(uint32_t a) {                                      /* the horizontal scroll ($49A0..$4A50) does not shift the lists of explosions ($1472) nor of mines/turrets ($18E6, 20 entries: the 21st would be the first vehicle slot at $1A76): d5 is added here */
    (void)a; if (mr32(0x4A3C) != 0x3E3C0013u) return;
    uint32_t d5 = g_cpu.d[5] & 0xFFFF, base = (g_cpu.a[5] & 0xFFFFFF) + 0x1472;
    for (int k = 0; k < 17; k++) { uint32_t p = base + 14u * (uint32_t)k; mw16(p, mr16(p) + d5); }
    base = (g_cpu.a[5] & 0xFFFFFF) + 0x18E6;
    for (int k = 0; k < 20; k++) { uint32_t p = base + 20u * (uint32_t)k; mw16(p, mr16(p) + d5); }
}
static uint32_t mute_saved[3]; static int mute_on[3];
static void hk_mute_a(uint32_t a) {
    int k = a == 0x41DD8 ? 0 : a == 0x41E00 ? 1 : 2;
    if (mp3_active) { mute_saved[k] = g_cpu.d[1] & 0xFF; mute_on[k] = 1; g_cpu.d[1] &= ~0xFFu; }
    else mute_on[k] = 0;
}
static uint32_t drv_base = 0;                                                          /* sound driver data ($FCA/$FCB/$FCC > 0: a chip effect is playing on channel A/B/C) */
static void hk_fx(uint32_t a) { (void)a; drv_base = g_cpu.a[3] & 0xFFFFFF; }          /* chip effects: the driver's data pointer */
static int fx_active(int ch) { return drv_base && (int8_t)mr8(drv_base + 0xFCA + (uint32_t)ch) > 0; }
static void hk_mute_b(uint32_t a) { int k = a == 0x41DDC ? 0 : a == 0x41E04 ? 1 : 2; if (mute_on[k]) { g_cpu.d[1] = (g_cpu.d[1] & ~0xFFu) | mute_saved[k]; mute_on[k] = 0; } }
/* Pad: L2 = hold position. The player control routine ($9306, just before the movement steps) checks the flag +$56 of the player structure ($2DA for player 1, $376 for player 2): if it is non-zero
   he does not move, but the directions still turn the soldier (the fire direction comes from other flags). The game sets it only when fire is pressed with no direction; here it is forced while L2 is held. */
static int hold_on[2] = { 0, 0 };
/* Twin-stick (right stick = aim): aim_dir[p] is the right-stick direction (J_* bits = the game's direction bits) while the game is being played, else 0. aim_fire[p]: it also holds fire.
   host_aim() (called by the player's direction routine, $9CAC) makes it the facing / fire direction ($36 of the player structure) whatever the left stick does; and while it is pushed the
   "fire without direction = stand still" lock (+$56) is released, so the left stick can move the soldier while he keeps shooting where the right stick points. */
static int aim_in[2] = { 0, 0 }, aim_fire[2] = { 0, 0 }; static uint8_t aim_dir[2] = { 0, 0 };
uint32_t host_aim(uint32_t a4, uint32_t d7) { a4 &= 0xFFFFFF; int pl = a4 == 0x2DA ? 0 : a4 == 0x376 ? 1 : -1; return (pl >= 0 && aim_dir[pl]) ? aim_dir[pl] : d7; }
static void hk_hold(uint32_t a) { (void)a; uint32_t a4 = g_cpu.a[4] & 0xFFFFFF; int pl = a4 == 0x2DA ? 0 : a4 == 0x376 ? 1 : -1; if (pl >= 0 && hold_on[pl]) mw8(a4 + 0x56, 0xFF); else if (pl >= 0 && aim_dir[pl]) mw16(a4 + 0x56, 0); }
static void hk_audiofix(uint32_t a) { (void)a; g_cpu.Z = 1; }              /* while a sample plays the driver does not write the music's A/B channels: that jump is cancelled */

static void install_hooks(void) {
    hook_register(0x9306, hk_hold); hook_register(0x41BCC, hk_song); hook_register(0x41BE8, hk_stop); hook_register(0xE6EC, hk_sample); hook_register(0xE756, hk_sample); hook_register(0x4A50, hk_hscroll);
    static const uint32_t ta[6] = { 0xE678, 0xE69C, 0xE6C0, 0xE6D6, 0xE708, 0xE772 }; for (int i = 0; i < 6; i++) hook_register(ta[i], hk_ymtag);
    hook_register(0x41DD8, hk_mute_a); hook_register(0x41E00, hk_mute_a); hook_register(0x41E28, hk_mute_a);
    hook_register(0x41DDC, hk_mute_b); hook_register(0x41E04, hk_mute_b); hook_register(0x41E2C, hk_mute_b);
    hook_register(0x41E68, hk_fx); hook_register(0x41E9C, hk_fx); hook_register(0x41ED0, hk_fx); hook_register(0x41EE2, hk_audiofix); hook_register(0x41EF8, hk_audiofix); hook_register(0x41F32, hk_audiofix);
}

/* ------------------------------------------------------------------------------------------------ audio per VBL */
/* the MP3 of the song that is heard, mixed into mix (n stereo frames); press = a new fire or key press */
static void mix_mp3(float *mix, int n, const int16_t *arr, int mfr, int press) {
    static int16_t loop[8192 * 2];
    if (!music_loops(pb_song)) {                                       /* song that ends by itself: one pass, at full volume (the driver's fade-out is the chip version's) */
        if (!pb_done) {
            if (press && !skip_total) { skip_total = skip_fade = (int)(0.7 * AUDIO_FS); if (getenv("DOW_SONGLOG")) host_log_file("song %d skipped with a button (vbl %u)", pb_song, tos.vbl); }
            if (looper_take_once(&looper, arr, mfr, n, loop)) { pb_done = 1; if (getenv("DOW_SONGLOG")) host_log_file("mp3 of song %d finished (vbl %u, pending %d)", pb_song, tos.vbl, pend_song); }
            float gain = (float)(1.0 / 32768.0) * (float)vol_mp3;
            for (int i = 0; i < n; i++) {
                float g = gain; if (skip_total) { g *= (float)skip_fade / (float)skip_total; if (skip_fade > 0) skip_fade--; }
                mix[2 * i] += loop[2 * i] * g; mix[2 * i + 1] += loop[2 * i + 1] * g;
            }
            if (skip_total && skip_fade == 0) { pb_done = 1; skip_total = 0; }             /* faded out: the game goes on (its wait for the song ends) */
        }
    } else {
        int g = (int)mr8(0x42502); float gain = (float)(1.0 / 32768.0) * (g ? (g / 13.0f < 1.0f ? g / 13.0f : 1.0f) : 1.0f);
        { int ls, lb; if (music_loop_points(music, pb_song, &ls, &lb)) looper_take_lp(&looper, arr, mfr, ls, lb, n, loop); else looper_take(&looper, arr, mfr, n, loop); }
        gain *= (float)vol_mp3; for (int i = 0; i < n * 2; i++) mix[i] += loop[i] * gain;
    }
}

/* With the menu open the game is paused but the MP3 music goes on (the chip music needs the game to run, so it is silent). */
static double pa_next = 0;
static void audio_paused(void) {
    if (!audio_on || !mp3_on || !cfg.audio) { pa_next = 0; return; }
    double now = now_s(); if (pa_next < now - 0.1) pa_next = now;
    int mfr = 0; const int16_t *arr = music_get(music, pb_song, &mfr); if (!arr) return;
    while (pa_next <= now) {
        static float mix[8192 * 2]; static int16_t pcm[8192 * 2]; int n = (int)(AUDIO_FS * 0.02); if (n > 8000) n = 8000;
        memset(mix, 0, sizeof(float) * (size_t)n * 2); mix_mp3(mix, n, arr, mfr, 0);
        if (pb_done && pend_song != -2) { int nx = pend_song; pb_reset(nx); break; }
        for (int i = 0; i < n * 2; i++) { float v = mix[i] * (float)vol; v = v < -1.0f ? -1.0f : v > 1.0f ? 1.0f : v; pcm[i] = (int16_t)(v * 32767.0f); }
        audio_push(cfg.audio, pcm, n); pa_next += 0.02;
    }
}

static void audio_chunk(void) {
    static YmEvent ev[65536]; static float mono[8192], mix[8192 * 2]; static int16_t pcm[8192 * 2];
    int ne = ym_events_take(ev, 65536);
    if (!audio_on) { uint8_t r[16]; ym_get_regs(r, NULL); ym_set_state(ym, r); return; }
    int fire_now = ((wj[0] | wj[1]) & 0x80) != 0, press = (fire_now && !skip_prev_fire) || skip_key; skip_prev_fire = fire_now; skip_key = 0;        /* new fire or key press */
    int mfr = 0; const int16_t *arr = mp3_on ? music_get(music, pb_song, &mfr) : NULL;
    int want_mute = arr != NULL || (mp3_on && music_pending(music, pb_song));
    if (want_mute != mp3_active) { mp3_active = want_mute; if (want_mute) for (int i = 0; i < ne; i++) if (ev[i].reg == 8 || ev[i].reg == 9 || ev[i].reg == 10) ev[i].val = 0; }
    int n = ym_samples_for(ym, 1.0 / tos.vbl_rate); if (n > 8000) n = 8000;
    int mixer = sfx_multi || sfx_pan;
    if (mixer) { int k = 0; for (int i = 0; i < ne; i++) if (ev[i].reg < 16) ev[k++] = ev[i]; ne = k; }
    { float g[3]; for (int c = 0; c < 3; c++) g[c] = (float)(fx_active(c) ? vol_fx : vol_ay); ym_set_channel_gains(ym, g); }      /* mixer: a channel is an effect or the music */
    ym_render(ym, n, ev, ne, mono);
    if (getenv("DOW_AUDIOLOG")) { double e = 0; for (int i = 0; i < n; i++) e += mono[i] * mono[i]; if (ne || e > 1e-6) host_log_file("audio vbl %u ev %d rms %.4f mp3on %d active %d arr %d song %d", tos.vbl, ne, sqrt(e / n), mp3_on, mp3_active, arr != NULL, pb_song); }
    for (int i = 0; i < n; i++) { mix[2 * i] = mono[i]; mix[2 * i + 1] = mono[i]; }
    if (mixer) {
        static float mv[8192 * 2]; mixer_render(n, mv);
        for (int i = 0; i < n * 2; i++) { float v = mix[i] + mv[i], ap = fabsf(v); if (ap > 0.8f) v = (v < 0 ? -1.0f : 1.0f) * (0.8f + 0.2f * tanhf((ap - 0.8f) / 0.2f)); mix[i] = v; }
    } else mixer_reset();
    if (arr) mix_mp3(mix, n, arr, mfr, press);
    if (pb_done && pend_song != -2) { int nx = pend_song; pb_reset(nx); }                         /* the song being heard finished: the one that was waiting plays now */
    for (int i = 0; i < n * 2; i++) { float v = mix[i] * (float)vol; v = v < -1.0f ? -1.0f : v > 1.0f ? 1.0f : v; pcm[i] = (int16_t)(v * 32767.0f); }
    if (cfg.audio) audio_push(cfg.audio, pcm, n);
}

/* ------------------------------------------------------------------------------------------------ machine snapshots */
static State *snap_shop = NULL, *pre_map = NULL; static int have_shop = 0, have_premap = 0;
static State *map_hist[3]; static uint32_t map_hist_vbl[3]; static int nmap_hist = 0;
static uint32_t last_snap_shop = (uint32_t)-999, last_snap_map = (uint32_t)-999;
static State *take_state(State *dst) { if (!dst) dst = state_new(); tos_save_state(dst); return dst; }

static void reset_after_load(void) {
    mixer_reset(); mixer_drop_cache(); ym_events_clear(); { uint8_t r[16]; ym_get_regs(r, NULL); ym_set_state(ym, r); }
    wj[0] = wj[1] = 0; jj[0] = jj[1] = 0; fo[0] = fo[1] = 0; nkeyq = 0; { extern void acia_clear(void); acia_clear(); }
    scene_new_segment(); scene_clear_frames(); have_frame = 0; cur_song = tos.song_now; pb_reset(cur_song); last_menu_nz = -1;
}
static int wpn_active, qjob_active;
static void restore_state(const State *st) {
    tos_load_state(st); reset_after_load();
    force_fire[0] = force_fire[1] = 0; wpn_active = 0; qjob_active = 0;
    last_snap_shop = last_snap_map = tos.vbl - 999; nmap_hist = 0;
    set_rate(game_hz, (double)tos.vbl - lead - tvf()); menu_clock = 0;
    apply_fix(); if (god_on) set_god(1);
}
static void to_title(void) {
    State *st = boot_state_make(cfg.game_dir); if (!st) return;
    song0 = 0; cur_song = -1; tos_load_state(st); state_free(st); reset_after_load(); wpn_active = qjob_active = 0;
    set_rate(game_hz, (double)tos.vbl - lead - tvf()); menu_clock = 0; apply_fix(); if (god_on) set_god(1);
}
static void screen_kind(int gameplay, char *out) {
    if (gameplay) { strcpy(out, "game"); return; }
    if (tos.vbl - tos.wait_hit[2] <= 2) { strcpy(out, "players"); return; }          /* waiting at "1 OR 2 PLAYER GAME?" */
    int shop = 0, map = 0; uint8_t p16[32]; tos_pal16(p16);
    if (!memcmp(p16, SHOP_PAL, 32)) shop = 1; if (!memcmp(p16, MAP_PAL, 32)) map = 1;
    for (int i = 0; i < tos.nzones; i++) { if (!memcmp(tos.zones[i].pal, SHOP_PAL, 32)) shop = 1; if (!memcmp(tos.zones[i].pal, MAP_PAL, 32)) map = 1; }
    strcpy(out, shop ? "shop" : map ? "map" : "otro");
}

static void go_map(void) {
    if (!strcmp(kind_now, "map")) { say_id("msg.already_on_map"); return; }
    if (!have_premap) { say_id("msg.no_map_yet"); return; }
    State *c = state_clone(pre_map); restore_state(c); state_free(c); say_id("msg.back_on_map");
}
static void save_slot(int n) {
    State *st = NULL; int tmp = 0;
    if (!strcmp(kind_now, "map")) { st = take_state(NULL); tmp = 1; }
    else if ((!strcmp(kind_now, "shop") || !strcmp(kind_now, "game")) && have_premap) st = pre_map;
    else { say_id("msg.cannot_save"); return; }
    char p[700], msg[120]; snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n);
#ifdef _WIN32
    CreateDirectoryA(save_dir, NULL);
#endif
    if (state_write_file(st, p)) { trf(msg, sizeof msg, tmp ? "msg.saved_map" : "msg.saved_before", n); say(msg); } else say_id("msg.save_failed");
    if (tmp) state_free(st);
}
static void load_slot(int n) {
    char p[700], msg[80]; snprintf(p, sizeof p, "%s/slot%d.dows", save_dir, n);
    State *st = state_new();
    if (!file_exists(p) || !state_read_file(st, p)) { state_free(st); trf(msg, sizeof msg, "msg.slot_empty", n); say(msg); return; }
    restore_state(st); if (!pre_map) pre_map = state_new(); { uint8_t *r = pre_map->ram; *pre_map = *st; pre_map->ram = r; memcpy(r, st->ram, RAMSZ); } have_premap = 1; have_shop = 0; last_snap_map = tos.vbl; state_free(st);
    trf(msg, sizeof msg, "msg.slot_loaded", n); say(msg);
}
static void restart_players(void) {
    to_title(); have_shop = 0; have_premap = 0; nmap_hist = 0; kind_now[0] = 0;
    for (int n = 0; n < 6000; n++) {
        tos_joystick(1, ((n / 8) % 2 == 0) ? 0x80 : 0);
        if (tos_run_slice(2000000) < 0) break; tos_vbl(); { YmEvent e[4096]; ym_events_take(e, 4096); }
        if (tos.vbl - tos.wait_hit[2] <= 1) break;
    }
    tos_joystick(1, 0); jj[1] = 0; scene_clear_frames(); have_frame = 0; ym_events_clear(); { uint8_t r[16]; ym_get_regs(r, NULL); ym_set_state(ym, r); }
    set_rate(game_hz, (double)tos.vbl - lead - tvf()); menu_clock = 0; say_id("msg.restarted");
}

/* ------------------------------------------------------------------------------------------------ weapons: weapon change and the pad's B/X */
static const struct { int sc; uint32_t pb; } PB[2] = { { 0x1E, 0x2DA }, { 0x4E, 0x376 } };
static int pb_of(int sc) { return sc == 0x1E ? 0 : sc == 0x4E ? 1 : -1; }
typedef struct WJob { int sc, ph; uint32_t t0; int single; } WJob;
static WJob wjob;
static void weapon(int sc, int dir) {
    if (dir >= 0 || pb_of(sc) < 0) { tos_key(sc); wjob.sc = sc; wjob.ph = 1; wjob.t0 = tos.vbl; wjob.single = 1; wpn_active = 1; return; }
    if (wpn_active || qjob_active || strcmp(kind_now, "game")) return;
    uint32_t pb = PB[pb_of(sc)].pb; int cats[19], nc = 0, cur = (int)mr16(pb + 0x0C);
    for (int i = 0; i < 19; i++) if (mr8(pb + 0x66 + (uint32_t)i)) cats[nc++] = i;
    int idx = -1; for (int i = 0; i < nc; i++) if (cats[i] == cur) idx = i;
    if (nc > 1 && idx >= 0) {
        int tgt = cats[((idx - 2) % nc + nc) % nc];                         /* $0C is left in the category before the previous one and pressed ONCE */
        POKE16(pb + 0x0C, (uint32_t)tgt); tos_key(sc); wjob.sc = sc; wjob.ph = 1; wjob.t0 = tos.vbl; wjob.single = 1; wpn_active = 1;
    }
}
static void wpn_tick(void) {
    if (!wpn_active) return;
    if (wjob.ph == 1 && tos.vbl - wjob.t0 >= 2) { tos_key(wjob.sc | 0x80); wjob.ph = 2; wjob.t0 = tos.vbl; }
    else if (wjob.ph == 2 && tos.vbl - wjob.t0 >= 3) wpn_active = 0;
}
static const int QCAT_ROCKET[3] = { 14, 15, 16 }, QCAT_GRENADE[1] = { 18 };
static int qammo_idx(int c) { return c == 14 ? 8 : c == 15 ? 0xC : c == 16 ? 6 : 0; }
typedef struct QJob { int sc; uint32_t pb; int port, T, C0, v60, v62; int st; int tries, fires; uint32_t t0; int a0; int wait; } QJob;
static QJob qj;
enum { QS_SW1, QS_BACK, QS_FIRE0, QS_FIRE1, QS_FIRE2, QS_RESTORE };
static int goto_cat(uint32_t pb, int sc, int goal) {
    int cats[19], nc = 0; for (int i = 0; i < 19; i++) if (mr8(pb + 0x66 + (uint32_t)i)) cats[nc++] = i;
    int idx = -1; for (int i = 0; i < nc; i++) if (cats[i] == goal) idx = i;
    if (idx < 0) return 0;
    POKE16(pb + 0x0C, (uint32_t)cats[(idx - 1 + nc) % nc]); tos_key(sc); wjob.sc = sc; wjob.ph = 1; wjob.t0 = tos.vbl; wjob.single = 1; wpn_active = 1; return 1;
}
static void quick(int sc, int kind /* 0 cohete 1 granada */) {
    if (qjob_active || wpn_active || strcmp(kind_now, "game") || pb_of(sc) < 0) return;
    uint32_t pb = PB[pb_of(sc)].pb; int cand = -1; const int *cl = kind == 0 ? QCAT_ROCKET : QCAT_GRENADE; int ncl = kind == 0 ? 3 : 1;
    for (int i = 0; i < ncl; i++) { int c = cl[i]; if (mr8(pb + 0x66 + (uint32_t)c) && mr16(pb + 0x7A + 2u * (uint32_t)qammo_idx(c)) > 0) { cand = c; break; } }
    if (cand < 0) return;
    memset(&qj, 0, sizeof qj); qj.sc = sc; qj.pb = pb; qj.port = sc == 0x1E ? 1 : 0; qj.T = cand; qj.C0 = (int)mr16(pb + 0x0C); qj.v60 = (int)mr16(pb + 0x60); qj.v62 = (int)mr16(pb + 0x62); qj.st = QS_SW1; qj.t0 = tos.vbl; qjob_active = 1;
}
static void quick_tick(void) {
    if (!qjob_active || wpn_active) return;
    QJob *j = &qj; uint32_t v = tos.vbl, pb = j->pb; int port = j->port, st = j->st;
    if (st == QS_SW1 || st == QS_BACK) {
        int goal = st == QS_SW1 ? j->T : j->C0;
        if (!j->wait) {
            if ((int)mr16(pb + 0x0C) == goal || !goto_cat(pb, j->sc, goal)) { j->st = st == QS_SW1 ? QS_FIRE0 : QS_RESTORE; j->wait = 0; j->t0 = v; return; }
            j->wait = 1; j->t0 = v; return;
        }
        if (v - j->t0 >= 4) {
            j->wait = 0;
            if ((int)mr16(pb + 0x0C) == goal) { j->st = st == QS_SW1 ? QS_FIRE0 : QS_RESTORE; j->t0 = v; }
            else { j->tries++; if (j->tries > 3) { j->st = st == QS_BACK ? QS_RESTORE : QS_BACK; j->tries = 0; } }
        }
    } else if (st == QS_FIRE0 && v - j->t0 >= 3) { j->a0 = (int)mr16(pb + 0x7A + 2u * (uint32_t)qammo_idx(j->T)); force_fire[port] = 1; j->t0 = v; j->st = QS_FIRE1; }
    else if (st == QS_FIRE1 && v - j->t0 >= 6) { force_fire[port] = 0; j->t0 = v; j->st = QS_FIRE2; }
    else if (st == QS_FIRE2 && v - j->t0 >= 8) { if ((int)mr16(pb + 0x7A + 2u * (uint32_t)qammo_idx(j->T)) < j->a0 || j->fires >= 2) j->st = QS_BACK; else { j->fires++; j->st = QS_FIRE0; } }
    else if (st == QS_RESTORE) { if ((int)mr16(pb + 0x0C) == j->C0) { POKE16(pb + 0x60, (uint32_t)j->v60); POKE16(pb + 0x62, (uint32_t)j->v62); } force_fire[port] = 0; qjob_active = 0; }
}

/* ------------------------------------------------------------------------------------------------ window events */
static int handle_event(const Ev *e) {
    switch (e->type) {
    case EV_QUIT: return 1;
    case EV_HZ: menu_hz = e->a; break;
    case EV_LEAD: lead = e->a; break;
    case EV_HOLD: { int pl = (int)e->a; if (pl >= 0 && pl < 2) hold_on[pl] = e->b != 0; } break;
    case EV_AIM: { int pl = (int)e->a; if (pl >= 0 && pl < 2) { aim_in[pl] = (int)e->b & 0x0F; aim_fire[pl] = ((int)e->b & 0x80) != 0; } } break;
    case EV_GAMEHZ: game_hz = e->a; if (!menu_clock) set_rate(game_hz, 0.0); break;
    case EV_TICK: if (ticks < 3) ticks++; break;
    case EV_GOD: set_god((int)e->a); break;
    case EV_MONEY: money_on = (int)e->a; break;
    case EV_FIX: fix_on = (int)e->a; apply_fix(); break;
    case EV_SOUND: audio_on = (int)e->a; break;
    case EV_VOL: vol = e->a; break;
    case EV_VOLX: { int w = (int)e->a; double v = e->b; if (w == 0) vol_mp3 = v; else if (w == 1) vol_ay = v; else if (w == 2) { vol_smp = v; ym_set_sample_gain(ym, v); mixer_set_gain(v); } else if (w == 3) vol_fx = v; break; }
    case EV_PAUSE:
        if (e->a) paused = 1;
        else { if (paused) set_rate(clk_r, -(tvf() + lead - (double)tos.vbl)); paused = 0; }
        break;
    case EV_MCH: sfx_multi = (int)e->a; mixer_options(sfx_multi, sfx_pan); break;
    case EV_PAN: sfx_pan = (int)e->a; mixer_options(sfx_multi, sfx_pan); break;
    case EV_MP3: mp3_on = (int)e->a; if (mp3_on) music_prefetch(music); break;
    case EV_MUSICDIR: music_set_dir(music, music_req); looper.pos = 0; if (mp3_on) music_prefetch(music); break;
    case EV_JOY: wj[1] = (int)e->a; break;
    case EV_JOY0: wj[0] = (int)e->a; break;
    case EV_KEY: if (e->b) skip_key = 1; if (nkeyq < 256) { keyq[nkeyq].scan = (int)e->a; keyq[nkeyq].down = (int)e->b; nkeyq++; } break;
    case EV_EXIT:
        if (last_gp && mr16(0x206) == 0) mw16(0x206, 0xFFFF); else to_title();
        break;
    case EV_GOSHOP: go_map(); break;
    case EV_SAVE: save_slot((int)e->a); break;
    case EV_LOAD: load_slot((int)e->a); break;
    case EV_RESTART: restart_players(); break;
    case EV_WPN: weapon((int)e->a, (int)e->b); break;
    case EV_QUICK: quick((int)e->a, (int)e->b); break;
    }
    return 0;
}

/* Development only (DOW_TEST + DOW_AUTOMISSION=<mission value of the map boxes>): from the map it goes into that mission by itself (cursor -> mission -> EXIT of the shop), and once in the mission it plays a
   fixed twin-stick script (left stick / right stick per phase, 100 VBL each) so the facing sprite ($36) can be checked in the log of DOW_PLAYERLOG without a pad. */
static int auto_arrived(int tx, int ty, int tol) {
    int x = (int)mr16(0x1EC), y = (int)mr16(0x1EE), dx = tx - x, dy = ty - y, st = 0;
    if (abs(dx) > tol) st |= dx > 0 ? 8 : 4; if (abs(dy) > tol) st |= dy > 0 ? 2 : 1;
    wj[1] = st; return st == 0;
}
static void autopilot(uint32_t v) {
    static int step = 0, wait = 0, arrived = 0; static uint32_t g0 = 0, last_pulse = 0; static int fire_left = 0;
    if (fire_left > 0) { wj[1] |= 0x80; fire_left--; return; }
    if (wait > 0) { wait--; wj[1] = 0; return; }
    if (!strcmp(kind_now, "map")) {
        int want = atoi(getenv("DOW_AUTOMISSION")), tx = -1, ty = -1;
        for (uint32_t a = 0xBF98; ; a += 10) { int xmin = (int16_t)mr16(a); if (xmin < 0) break; if ((int16_t)mr16(a + 8) == want) { tx = (xmin + (int16_t)mr16(a + 2)) / 2; ty = ((int16_t)mr16(a + 4) + (int16_t)mr16(a + 6)) / 2; break; } }
        if (tx < 0) return;
        if (auto_arrived(tx, ty, 1)) { if (++arrived > 25) { fire_left = 8; wait = 150; arrived = 0; } } else arrived = 0;
    } else if (!strcmp(kind_now, "shop")) {
        if (auto_arrived(288, 132, 3)) { if (++arrived > 20) { fire_left = 8; wait = 100; arrived = 0; } } else arrived = 0;
    } else if (!strcmp(kind_now, "game")) {
        if (!g0) { g0 = v; host_log("autopilot: mission started at vbl %u", v); }
        uint32_t t = v - g0; int j = 0, aim = 0;
        if (t >= 200 && t < 300) j = 1;
        else if (t >= 300 && t < 400) { j = 1; aim = 8; }                  /* left: up, right stick: right */
        else if (t >= 400 && t < 500) { j = 4; aim = 9; }                      /* left: left, right stick: up-right */
        else if (t >= 500 && t < 600) { j = 0; aim = 2; }                      /* left: neutral, right stick: down */
        else if (t >= 600 && t < 700) { j = 8; aim = 0; }                      /* left: right, right stick released */
        else if (t >= 700 && t < 800) { j = 2 | 4; aim = 4; }                  /* left: down-left, right stick: left */
        else if (t == 800) host_log("autopilot: script finished");
        wj[1] = j; aim_in[0] = aim; aim_fire[0] = aim != 0;
    } else if (v - last_pulse > 90 && g0 == 0) { last_pulse = v; fire_left = 6; }      /* the loading screens: fire until the mission starts */
}
static int pump(uint32_t v) {
    Ev e; while (ev_pop(&e)) if (handle_event(&e)) return 1;
    if (getenv("DOW_TEST") && getenv("DOW_AUTOMISSION")) autopilot(v);
    for (int pl = 0; pl < 2; pl++) aim_dir[pl] = (uint8_t)(last_gp ? aim_in[pl] : 0);          /* the right stick only aims during the mission (in the menus and the shop it does nothing) */
    for (int port = 0; port < 2; port++) {                                  /* fire is held at least HOLD VBL (the game polls once per logic frame) */
        int pl = port ? 0 : 1;                                              /* joystick port 1 = player 1 */
        int want = (wj[port] & 0x80) || force_fire[port] || (aim_dir[pl] && aim_fire[pl]);
        if (want && !fo[port]) { fo[port] = 1; fv_[port] = v; }
        if (!want && fo[port] && v - fv_[port] >= HOLD) fo[port] = 0;
        int st = (wj[port] & 0x0F) | (fo[port] ? 0x80 : 0);
        if (st != jj[port]) { jj[port] = st; tos_joystick(port, st); }
    }
    if (nkeyq) {
        int scan = keyq[0].scan, down = keyq[0].down;
        if (down) { if (v >= key_free) { memmove(keyq, keyq + 1, sizeof(KeyEv) * (size_t)(--nkeyq)); tos_key(scan); down_at[scan & 255] = v; key_free = v + 1; } }
        else if (v - down_at[scan & 255] >= HOLD) { memmove(keyq, keyq + 1, sizeof(KeyEv) * (size_t)(--nkeyq)); tos_key(scan | 0x80); down_at[scan & 255] = (uint32_t)-999; key_free = v + 1; }
    }
    return 0;
}

/* ------------------------------------------------------------------------------------------------ one VBL */
static void on_frame_cb(Frame *f) {
    if (getenv("DOW_PLAYERLOG")) host_log_file("player vbl %u x=%d y=%d  facing L=%02X R=%02X U=%02X D=%02X  still56=%02X%02X  input E=%02X F=%02X 10=%02X 11=%02X fire=%02X  face36=%02X", f->vbl, (int)mr16(0x2DA + 2), (int)mr16(0x2DA + 4), mr8(0x2DA + 0x2E), mr8(0x2DA + 0x30), mr8(0x2DA + 0x32), mr8(0x2DA + 0x34), mr8(0x2DA + 0x56), mr8(0x2DA + 0x57), mr8(0x2DA + 0xE), mr8(0x2DA + 0xF), mr8(0x2DA + 0x10), mr8(0x2DA + 0x11), mr8(0x2DA + 0x12), mr8(0x2DA + 0x36));
    if (getenv("DOW_UNITLOG")) { char b[1500]; int n = snprintf(b, sizeof b, "units vbl %u scroll %d hs %d:", f->vbl, f->scroll, f->hs); for (int i = 0; i < f->nunits && n < 1400; i++) n += snprintf(b + n, sizeof b - (size_t)n, " %s#%d(%d,%d+%d)@%X", unit_name[f->units[i].unit], f->units[i].order, f->units[i].px, f->units[i].py, f->units[i].fine, f->units[i].ident); host_log_file("%s", b); } Msg *m = msg_new(MSG_FRAME); m->frame = frame_ref(f); msgq_push(m); last_frame_vbl = f->vbl; have_frame = 1; }
static void on_reset_cb(void) { Msg *m = msg_new(MSG_RESET); msgq_push(m); }
static void on_sprite_cb(uint64_t k, Sprite *s) { (void)k; (void)s; }
static void on_drop_cb(uint64_t k) { (void)k; }

static State *good[3]; static uint32_t good_vbl[3]; static int ngood = 0; static uint32_t last_good = 0, last_recover = (uint32_t)-9999;
static void good_snapshot(int gameplay) {                                 /* periodic snapshots during the game (every ~3 s, 4 MB): to recover if the game gets corrupted */
    if (!gameplay) { ngood = 0; return; }
    if (tos.vbl - last_good < 150 && ngood) return;
    last_good = tos.vbl;
    if (ngood == 3) { State *o = good[0]; good[0] = good[1]; good[1] = good[2]; good[2] = o; good_vbl[0] = good_vbl[1]; good_vbl[1] = good_vbl[2]; ngood = 2; } else good[ngood] = state_new();
    tos_save_state(good[ngood]); good_vbl[ngood] = tos.vbl; ngood++; tos_ilog_trim(good_vbl[0]);
}
static int inject = 0;
static int do_step(void) {
    wpn_tick(); quick_tick();
    if ((jj[0] | jj[1]) & 0x80) fire_last = tos.vbl;
    if (getenv("DOW_CORRUPT") && tos.vbl == (uint32_t)atoi(getenv("DOW_CORRUPT"))) inject = 1;        /* recovery test */
    if (getenv("DOW_EFFECT")) { static int ef_done = 0; static uint32_t ef_v0 = 0; if (!ef_v0) ef_v0 = tos.vbl; int at = atoi(getenv("DOW_EFFECT")); if (!ef_done && (int)(tos.vbl - ef_v0) >= at) { ef_done = 1; tos_call_sub(0x41C04, (uint32_t)(getenv("DOW_EFFECT_N") ? atoi(getenv("DOW_EFFECT_N")) : 1)); host_log_file("efecto lanzado en vbl %u", tos.vbl); } }
    if (getenv("DOW_BOOM")) { static int bd = 0; static uint32_t b0 = 0; if (!b0) b0 = tos.vbl; int at, bx, by, bt; if (!bd && sscanf(getenv("DOW_BOOM"), "%d:%d:%d:%d", &at, &bx, &by, &bt) == 4 && (int)(tos.vbl - b0) >= at) { bd = 1; extern int tos_call_sub3(uint32_t, uint32_t, uint32_t, uint32_t); tos_call_sub3(0x6CA8, (uint32_t)bx, (uint32_t)by, (uint32_t)bt); host_log_file("explosion creada en vbl %u", tos.vbl); } }
    if (pb_fixed()) { vf_real = mr8(0x424F2); mw8(0x424F2, pb_done ? 0 : 0xFF); vf_on = 1; }       /* for the game's waits ("is the music playing?"): the mp3 of a song that ends by itself counts as the music itself */
    int r = tos_run_slice(2000000); if (inject) { inject = 0; r = -1; }
    if (vf_on) { vf_on = 0; mw8(0x424F2, vf_real); }
    if (r < 0 && ngood > 0 && tos.vbl - last_recover > 600) {              /* corrupted state: it goes back to the oldest good snapshot (about 7 s earlier) instead of leaving the game dead */
        { char p1[760], p2[760]; static int ncrash = 0; ncrash++; snprintf(p1, sizeof p1, "%s/crash%d_inicio.dows", save_dir, ncrash); snprintf(p2, sizeof p2, "%s/crash%d_entradas.txt", save_dir, ncrash);
#ifdef _WIN32
          CreateDirectoryA(save_dir, NULL);
#endif
          state_write_file(good[0], p1); tos_ilog_write(p2, good_vbl[0]); State *bad = state_new(); tos_save_state(bad); snprintf(p1, sizeof p1, "%s/crash%d_fin.dows", save_dir, ncrash); state_write_file(bad, p1); state_free(bad);
          host_log("corruption report saved in %s/crash%d_*  (previous snapshot, inputs and corrupted state)", save_dir, ncrash); }
        last_recover = tos.vbl; State *c = state_clone(good[0]); uint32_t bad_vbl = tos.vbl; restore_state(c); state_free(c); last_recover = tos.vbl + 0;
        host_log("recovery: the game was corrupted at VBL %u; going back to an earlier snapshot", bad_vbl); say_id("msg.recovered");
        r = 1;
    }
    if (r < 0) { Msg *m = msg_new(MSG_ERROR); snprintf(m->text, sizeof m->text, "Emulation error near %06X (vbl %u)", g_cpu.pc, tos.vbl); msgq_push(m); return -1; }
    tos_vbl();
    audio_chunk();
    if (money_on) for (int i = 0; i < 2; i++) { uint32_t a = i ? 0x3CE : 0x332; if (mr32(a) < 9000000u) mw32(a, 9999999u); }
    int gameplay = have_frame && tos.vbl - last_frame_vbl < 8;
    good_snapshot(gameplay);
    char kd[16]; screen_kind(gameplay, kd);
    if (want_song && !strcmp(kind_now, "players") && !song0) {              /* the intro was skipped without the title music starting: it plays from here (as in the original) */
        if (mr32(0x74F4) == 0x51ED01E8u && mr32(0x41BCC) == 0x48E7FFFEu && mr32(0x43368) == 0x47FAE1C0u) { want_song = 0; tos_call_sub(0x74F4, 0); }      /* $74F4 = wrapper: $1E8=0, jsr $41BCC (d0 = song), $1E8=ff */
    }
    if (strcmp(kd, kind_now) != 0) {
        if (!strcmp(kind_now, "map") && nmap_hist) { if (!pre_map) pre_map = state_new(); { uint8_t *rr = pre_map->ram; *pre_map = *map_hist[nmap_hist - 1]; pre_map->ram = rr; memcpy(rr, map_hist[nmap_hist - 1]->ram, RAMSZ); } have_premap = 1; nmap_hist = 0; }
        snprintf(kind_now, sizeof kind_now, "%s", kd);
        Msg *m = msg_new(MSG_SCR); snprintf(m->text, sizeof m->text, "%s", kd); msgq_push(m);
        want_song = !strcmp(kd, "players");
    }
    if (!strcmp(kd, "shop") && tos.vbl - last_snap_shop >= 50) { last_snap_shop = tos.vbl; snap_shop = take_state(snap_shop); have_shop = 1; }
    else if (!strcmp(kd, "map") && (!nmap_hist || (tos.vbl - last_snap_map >= 25 && tos.vbl - fire_last >= 50))) {
        last_snap_map = tos.vbl;
        if (nmap_hist >= 3) { State *old = map_hist[0]; memmove(map_hist, map_hist + 1, sizeof(State *) * 2); memmove(map_hist_vbl, map_hist_vbl + 1, sizeof(uint32_t) * 2); nmap_hist = 2; map_hist[nmap_hist] = old; }
        else map_hist[nmap_hist] = state_new();
        tos_save_state(map_hist[nmap_hist]); map_hist_vbl[nmap_hist] = tos.vbl; nmap_hist++;
        if (!have_premap) { if (!pre_map) pre_map = state_new(); { uint8_t *rr = pre_map->ram; *pre_map = *map_hist[0]; pre_map->ram = rr; memcpy(rr, map_hist[0]->ram, RAMSZ); } have_premap = 1; }
    }
    if (!gameplay) {
        static uint8_t idx[64000]; Zone zones[MAXZONES]; int nz; tos_screen_idx(idx, zones, &nz);
        int same = last_menu_nz == nz && !memcmp(idx, last_menu_idx, 64000) && !memcmp(zones, last_menu_zones, sizeof(Zone) * (size_t)nz);
        if (!same) {
            memcpy(last_menu_idx, idx, 64000); memcpy(last_menu_zones, zones, sizeof(Zone) * (size_t)nz); last_menu_nz = nz;
            Msg *m = msg_new(MSG_MENU); m->vbl = tos.vbl; m->idx = (uint8_t *)malloc(64000); memcpy(m->idx, idx, 64000); memcpy(m->zones, zones, sizeof(Zone) * (size_t)nz); m->nz = nz; msgq_push(m);
        }
    } else last_menu_nz = -1;
    return gameplay;
}

#ifdef _WIN32
static DWORD WINAPI emu_thread(LPVOID arg) {
    (void)arg; double tr = 0;
    timeBeginPeriod(1); SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);       /* the audio must not wait for the drawing thread to finish */
    double t_mode = 0; int mode_since = 0;
    t_rep = now_s();
    for (;;) {
        if (paused) { if (pump(tos.vbl)) break; audio_paused(); sleep_ms(10); continue; }
        double tv = tvf(); int tickmode = menu_clock;
        if ((tickmode && ticks <= 0) || (!tickmode && (double)tos.vbl >= tv + lead)) {
            WaitForSingleObject(ev_signal, 2);                              /* waiting for the clock: if a key press arrives, deliver it to the game NOW */
            Ev e; int got = 0;
            while (ev_pop(&e)) { got = 1; if (handle_event(&e)) goto out; }
            if (got) { if (pump(tos.vbl)) goto out; tos_deliver_keys(); }
            continue;
        }
        double t = now_s(); uint32_t v = tos.vbl;
        if (pump(v)) break;
        if (tickmode) { if (ticks > 0) ticks--; } else ticks = 0;
        int gp = do_step(); if (gp < 0) break; last_gp = gp;
        if (tickmode) { clk_t = now_s(); clk_v = (double)tos.vbl - lead; }                 /* the game clock follows the emulation when leaving tick mode */
        if (gp) { mode_since = 0; if (menu_clock || clk_r != game_hz) { set_rate(game_hz, 0.0); menu_clock = 0; } }                /* pace: 50 VBL/s during the game; at the monitor frequency outside it */
        else { mode_since++; if (mode_since > 25 && (!menu_clock || fabs(clk_r - menu_hz) > 0.01)) { set_rate(menu_hz, 0.0); menu_clock = 1; } }
        double dt = now_s() - t; busy += dt; steps++; if (dt > stepmax) stepmax = dt; if (dt > 0.020) slow++;
        if (gp && !tickmode) {                                              /* long stop: the clock is RE-ANCHORED instead of making up for the lost time (jerky scroll) */
            double behind = tvf() + lead - (double)tos.vbl;
            if (behind > ATRASO_MAX) { set_rate(clk_r, -behind); stalls++; }
        }
        if (now_s() - t_rep >= 2.0) {
            Msg *m = msg_new(MSG_STAT); m->stat[0] = busy / (steps ? steps : 1) * 1000; m->stat[1] = steps / 2.0; m->stat[2] = stepmax * 1000; m->stat[3] = slow; m->stat[4] = stalls; msgq_push(m);
            busy = 0; steps = 0; t_rep = now_s(); stepmax = 0; slow = 0;
        }
        (void)tr; (void)t_mode;
    }
out:
    if (getenv("DOW_TEST")) { FILETIME c_, e_, k_, u_; GetThreadTimes(GetCurrentThread(), &c_, &e_, &k_, &u_); host_log("emulation thread: CPU %.1f s (kernel %.1f s)", (((uint64_t)k_.dwHighDateTime << 32 | k_.dwLowDateTime) + ((uint64_t)u_.dwHighDateTime << 32 | u_.dwLowDateTime)) / 1e7, (((uint64_t)u_.dwHighDateTime << 32 | u_.dwLowDateTime)) / 1e7); }
    emu_running = 0; return 0;
}
#endif

int emu_start(const EmuConfig *c) {
    cfg = *c; lead = c->lead > 0 ? c->lead : 3.6;
    snprintf(save_dir, sizeof save_dir, "%s", c->save_dir);
    hex2bin(SHOP_PAL_HEX, SHOP_PAL, 32); hex2bin(MAP_PAL_HEX, MAP_PAL, 32);
    memset(down_at, 0xFF, sizeof down_at); for (int i = 0; i < 256; i++) down_at[i] = (uint32_t)-999;
    State *st = NULL;
    if (c->state_path) { st = state_new(); if (!state_read_file(st, c->state_path)) { host_log("Could not read the state %s", c->state_path); state_free(st); return 0; } }
    else if (!(st = boot_state_make(c->game_dir))) return 0;
    tos_init(c->game_dir); tos_load_state(st); state_free(st);
    music = music_new(c->music_dir); ym = ym_new(AUDIO_FS); { uint8_t r[16]; ym_get_regs(r, NULL); ym_set_state(ym, r); }
    fix_on = c->fix; apply_fix(); if (c->god) set_god(1); money_on = c->money; audio_on = c->sound; vol = c->vol; mp3_on = c->mp3; sfx_multi = c->multi; sfx_pan = c->pan; mixer_options(sfx_multi, sfx_pan);
    if (mp3_on) music_prefetch(music);
    install_hooks(); if (getenv("DOW_SONG")) { cur_song = atoi(getenv("DOW_SONG")); tos.song_now = cur_song; pb_reset(cur_song); }
    SceneCb scb = { on_frame_cb, on_sprite_cb, on_drop_cb, on_reset_cb }; scene_init(&scb); scene_install_hooks();
    clk_t = c->t_anchor; clk_v = (double)tos.vbl; clk_r = game_hz; menu_clock = 0; post_rate();
#ifdef _WIN32
    InitializeCriticalSection(&ev_cs); ev_signal = CreateEvent(NULL, FALSE, FALSE, NULL);
    emu_running = 1; th = CreateThread(NULL, 64u << 20, emu_thread, NULL, 0, NULL);
#endif
    return 1;
}
void emu_join(void) {
#ifdef _WIN32
    if (th) { WaitForSingleObject(th, 3000); th = NULL; }
#endif
}
const char *emu_kind(void) { return kind_now; }
