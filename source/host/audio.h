/*
 * audio.h - Sound: YM2149 synthesizer, sample mixer (multichannel + panning), MP3 music and output to the device.
 */
#ifndef AUDIO_H
#define AUDIO_H
#include "host.h"

#define AUDIO_FS 44100

/* ym.c */
typedef struct YM YM;
YM  *ym_new(int fs);
void ym_set_state(YM *y, const uint8_t *regs16);
int  ym_samples_for(YM *y, double seconds);
void ym_render(YM *y, int n, const YmEvent *ev, int nev, float *out /* n mono */);
const float *ym_dac16(void);
void ym_set_channel_gains(YM *y, const float *g /* 3 */);      /* mixer gain of channels A, B, C (chip music or chip effect) */
void ym_set_sample_gain(YM *y, double g);      /* volume of the sampled effects (sample channel) */

/* mixer.c: the game's 4-bit samples (timer A, 4915.2 Hz) mixed separately */
void mixer_reset(void);
void mixer_options(int multi, int pan);
void mixer_set_gain(double g);                 /* volume of the sampled effects */
void mixer_start(int sample_no, int ch /* 0 = A, 1 = B */, double ph, float pan);   /* the game starts a sample (hook of $E6EC / $E756) */
void mixer_render(int n, float *stereo /* n x 2, added to the output */);
void mixer_drop_cache(void);
int  mixer_active(void);

/* mp3.c */
typedef struct Music Music;
Music *music_new(const char *folder);
const int16_t *music_get(Music *m, int song, int *frames);   /* NULL if there is no file, it is not ready or it is unreadable */
int    music_pending(Music *m, int song);
void   music_prefetch(Music *m);
void   music_set_dir(Music *m, const char *folder);
int    music_loop_points(Music *m, int song, int *start, int *back);   /* 1 if the song has loop points (frames of the decoded audio, from its .txt): when the playback reaches `start` it jumps back to `back` */                       /* switch to another music pack (subfolder of music); discards what was decoded from the previous one */
typedef struct Looper { int pos; } Looper;
void   looper_take(Looper *l, const int16_t *a, int frames, int n, int16_t *out /* n x 2 */);
void   looper_take_lp(Looper *l, const int16_t *a, int frames, int start, int back, int n, int16_t *out /* n x 2 */);      /* like looper_take, but the song loops between `back` and `start` */
int    looper_take_once(Looper *l, const int16_t *a, int frames, int n, int16_t *out /* n x 2 */);       /* a single pass (fills with silence at the end); returns 1 once it has finished */
int    music_loops(int song);                                              /* 0 = game song that ends by itself (bonus, end of level, game over...): its mp3 plays once, whole */

/* audioout.c */
typedef struct AudioOut AudioOut;
AudioOut *audio_open(int fs, double target_ms);
int audio_set_lowlat(AudioOut *a, int on);        /* 1 = low-latency WASAPI output (if it cannot be opened, winmm stays and 0 is returned) */
double audio_device_ms(AudioOut *a);              /* delay added by the device (player queue) in ms, approximate */
void audio_push(AudioOut *a, const int16_t *stereo, int frames);
void audio_set_target(AudioOut *a, double ms);
double audio_level_ms(AudioOut *a);
void audio_close(AudioOut *a);
void audio_stats(AudioOut *a, int *underruns, int *drops, long *pushed);
#endif
