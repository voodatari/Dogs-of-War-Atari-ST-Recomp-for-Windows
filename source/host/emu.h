/*
 * emu.h - Emulation thread: keeps the 50 VBL/s clock, handles input, generates audio, publishes frames and fixed
 * screens, and keeps the machine snapshots (map, shop, saved games).
 */
#ifndef EMU_H
#define EMU_H
#include "present.h"
#include "audio.h"

enum {
    EV_QUIT,
    EV_HZ,
    EV_LEAD,
    EV_TICK,
    EV_GOD,
    EV_MONEY,
    EV_FIX,
    EV_SOUND,
    EV_VOL,
    EV_PAUSE,
    EV_MCH,
    EV_PAN,
    EV_MP3,
    EV_JOY,
    EV_JOY0,
    EV_KEY,
    EV_EXIT,
    /* EV_AIM: a = player (0, 1), b = right-stick direction (J_* bits) | 0x80 if it also holds fire; EV_VOLX: a = 0 MP3,
       1 chip music, 2 sampled effects, 3 chip effects; b = volume */
    EV_GOSHOP,
    EV_SAVE,
    EV_LOAD,
    EV_RESTART,
    EV_WPN,
    EV_QUICK,
    EV_VOLX,
    EV_GAMEHZ,
    EV_HOLD,
    EV_MUSICDIR,
    EV_AIM
};

typedef struct EmuConfig {
    const char *game_dir, *state_path, *music_dir, *save_dir;
    AudioOut *audio;
    double lead; /* VBLs the emulation runs ahead of the clock */
    int fix, god, money, sound, mp3, multi, pan;
    double vol;
    double t_anchor;
} EmuConfig;

/* starts the thread; 0 if the state could not be loaded */
int emu_start(const EmuConfig *cfg);
void emu_post(int type, double a, double b); /* event from the window thread */
/* changes the folder of the mp3 files (music pack) while the game is running */
void emu_set_music_dir(const char *path);
void emu_join(void);
/* current screen type: game shop map players other */
const char *emu_kind(void);
extern volatile int emu_running;
#endif
