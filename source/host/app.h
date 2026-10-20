/*
 * app.h - State and services shared by the parts of the program that run in the window thread.
 *
 * The program is made of three kinds of code:
 *   - the game itself (../generated, ../runtime), run by the emulation thread (emu.c);
 *   - the "host" libraries that give it a machine to run on (tos.c, scene.c, ym.c, mixer.c ...) and show its output
 *     (present.c, render.c, video.c, audioout.c);
 *   - this application layer, which owns the window and the options and ties everything together:
 *
 *       main.c          start-up, the loop that draws one image per monitor refresh, shutdown
 *       app.c           the shared state below, loading / saving options.json, small calculations on it
 *       game_input.c    keyboard, gamepads and window events of every frame
 *       fastinput.c     optional thread that reads the inputs 1000 times per second
 *       menu_ui.c       opening / closing the options menu, redefining keys and buttons
 *       menu_options.c  what the options menu contains and what each option does
 *       musicpacks.c    the music packs found in the music folder
 *       devtest.c       scripted input for automatic tests (only with DOW_TEST=1)
 *
 * Everything declared here is owned by app.c unless a comment says otherwise.
 */
#ifndef APP_H
#define APP_H
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

/* Emulation margin, in VBL (50 per second), kept between what is emulated and what is shown. */
#define LEAD_DEFAULT 3.6

/* ---- the objects of the running program ---- */
extern Presenter *presenter;     /* builds every image from what the emulation published */
extern AudioOut *audio_out;      /* the sound device (NULL when there is none) */
extern Menu *menu_options;       /* the options menu (tabs) */
extern Menu *menu_controls;      /* the window to redefine keys and pad buttons */
extern Menu *menu_cur;           /* the one of them that is on screen (or was last) */
extern volatile int app_running; /* cleared to leave the program (window closed or "Quit") */

/* ---- display ---- */
extern int render_scale;         /* the game image is made at 320*render_scale x 200*render_scale pixels */
extern int monitor_hz;           /* refresh rate of the monitor, limited to 50..165 */
extern int fullscreen;           /* 1 while in fullscreen */
extern int overlay_on;           /* the image of the menu is being shown over the game */
extern char menu_drawn_sig[600]; /* signature of what the menu image shows; empty = draw it again */

/* ---- options chosen by the user (saved in options.json by save_options) ---- */
extern int sound_on, mp3_on, multichannel_fx, fx_pan;
extern double master_volume;
extern double part_volume[4];        /* 0 MP3 music, 1 chip music, 2 sampled effects, 3 chip effects */
extern int script_fix;               /* the enemy-script fix is on */
extern int god_mode, infinite_money; /* testing options of the menu */
extern int save_slot;                /* 1..3: slot used by "Save game" / "Load game" */
extern int fast_input;               /* input read by its own thread at 1 kHz */
extern int low_latency_audio;        /* WASAPI event-driven shared mode */
extern double audio_trim_ms;         /* fine adjustment of the audio delay */
extern int match_refresh;            /* without interpolation: game speed follows the screen refresh */
extern double lead;                  /* emulation margin in use = lead_with_interp or lead_without_interp */
extern double lead_with_interp, lead_without_interp;
extern int gfx_pack_chosen; /* the user chose a graphics pack (so it is saved) */
extern char music_pack[80]; /* folder of the music pack in use (inside music_root; "" = files directly there) */

/* ---- folders and files (all next to the executable unless the environment says otherwise) ---- */
extern char app_dir_path[600];        /* folder of dowport.exe */
extern char options_path[700];        /* options.json: your options, written when the menu closes */
extern char defaults_path[700];       /* default.json: factory options */
extern char legacy_options_path[700]; /* opciones.json: options file of earlier versions */
extern char game_data_dir[600];       /* data: contains FILES/DOW, the original game file, and languages/ */
extern char assets_root[700];         /* assets: graphics dump and graphics packs */
extern char save_dir[700];            /* saves: the three save slots */
extern char music_root[700];          /* music: the music packs */
extern char pack_from_cmdline[64];    /* graphics pack requested with --pack (or DOW_GRAFICO) */

/* ---- app.c ---- */
double env_d(const char *name, double def);   /* a number from the environment */
int find_game_dir(void);                      /* looks for data/FILES/DOW; 0 if it is not there */
void set_paths(void);                         /* fills the folders above from the executable's folder */
void load_options(int fix_cli, int no_sound); /* reads options.json (or the older files) into the variables above */
void save_options(void);                      /* writes them to options.json (not during automatic tests) */
void apply_video_cfg(void);                   /* video options (shader, filter, aspect...) from the loaded options */
double aud_target_ms(void);                   /* target delay of the audio buffer, in ms */
double game_rate(void);                       /* game VBL per second: 50, or the monitor's divided by k */
void apply_lead(void);                        /* sends the margin / game speed / audio target to their owners */

/* ---- menu_ui.c: showing the menu and redefining controls ---- */
extern volatile int menu_open_flag; /* 1 while the menu is open (the fast-input thread reads it) */
extern int rebind_kind;             /* redefining controls: 0 nothing, 1 waiting for a key, 2 for a pad button */
extern int rebind_action;           /* the action being redefined */
extern int rebind_prev_ok;          /* pad buttons already held when the wait started are ignored */
extern int rebind_wait_release;     /* wait for every pad button to be released after assigning one */
extern int resend_input;            /* set to resend the whole joystick / aim / hold state to the emulation */
void open_menu(void);
void close_menu(void);
void menu_back(void); /* Esc / B: from the controls window back to the menu, from the menu it closes */
void start_rebind(int kind, int action);
const char *ctl_text(int kind, int action); /* text of the key / button assigned to an action (or "press a key") */

/* ---- menu_options.c ---- */
void menu_build(int have_audio, int have_gl, int have_pack); /* creates menu_options and menu_controls */

/* ---- musicpacks.c: the subfolders of music/ that contain audio files ---- */
void scan_packs(void);
int pack_count(void);
const char *pack_name(int i);                         /* "" = the audio files that lie directly in music/ */
int pack_find(const char *name);                      /* index of a pack by name (case-insensitive), or -1 */
void pack_dir(const char *name, char *out, size_t n); /* full path of a pack */
void pack_resolve(
    const char *want); /* sets music_pack to a pack that exists: the wanted one, else "arrange", else the first */

/* ---- game_input.c: one frame of keyboard, window and pad handling ---- */
extern uint8_t key_held[512];    /* keys that must be ignored until they are released (they were used in the menu) */
extern int pad_fire_blocked[2];  /* the fire button used to accept in the menu does not fire until released */
extern volatile int input_ready; /* the window loop has started (the fast-input thread waits for it) */
extern int sent_hold[2], sent_aim[2];              /* last hold / aim values sent to the emulation (-1 = send again) */
int pad_aim_code(const PadRead *pr, int n, int i); /* value of the EV_AIM event for pad i */
void input_set_kbd_state(int bits);                /* joystick bits from the keyboard (used by the test scripts) */
int input_frame(double now); /* handles the window events and the pads; returns 1 if the menu is open */

/* ---- fastinput.c ---- */
int fast_input_active(void); /* the option is on (and it is not an automatic test) */
void fast_input_start(void); /* starts the thread */

/* ---- devtest.c ---- */
void devtest_frame(double elapsed); /* applies the scripted input of the test variables, if any */
#endif
