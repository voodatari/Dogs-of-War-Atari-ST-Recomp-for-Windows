/*
 * video.h - Windows window + OpenGL: scaling, aspect, built-in or external CRT shaders (single-pass RetroArch) and an
 * overlay layer for the menu.
 */
#ifndef VIDEO_H
#define VIDEO_H
#include "host.h"

#define NPARAM 6
typedef struct VidParams {
    float p_scan, p_mask, p_curv, p_bright, p_vig, p_bloom;
} VidParams;
typedef struct VidCfg {
    /* None, Scanlines, Soft CRT, Full CRT, Custom */
    char preset[24];
    int filter; /* 0 Sharp, 1 Smooth, 2 Sharp bilinear */
    /* 0 Original 4:3, 1 Square pixels 16:10, 2 Stretch, 3 Integer */
    int aspect;
    char external[64]; /* name of the external shader ("" = none) */
    VidParams p;
    /* TEST: what the shaders start from. 0 the image as composed (320S x 200S); 1 / 2: always 320x200 (1 box average of
     * the SxS blocks, 2 centre sample) and TextureSize = the output size */
    int src_mode;
} VidCfg;
extern VidCfg vcfg;
extern const char *VID_FILTERS[3], *VID_ASPECTS[4], *VID_PRESETS[4], *VID_SOURCES[3];

/* window + context; 0 on failure */
int video_open(int w, int h, int src_w, int src_h, const char *shaders_dir);
void video_close(void);
/* 1 = no OpenGL: scaling with GDI, no shaders */
int video_is_software(void);
void video_set_preset(const char *name);
void video_use_external(const char *name); /* "" = quitar */
int video_external_count(void);
const char *video_external_name(int i);
void video_upload(const uint8_t *rgb);                 /* (src_h, src_w, 3) */
void video_overlay(const uint8_t *rgba, int w, int h); /* NULL removes it */
void video_rect(int *x, int *y, int *w, int *h);       /* rectangle of the image inside the window */
void video_draw(void);                                 /* draws and presents (with vsync) */
void video_size(int *w, int *h);
void video_set_fullscreen(int on);
int video_is_fullscreen(void);
void video_title(const char *t);
/* PNG of what is in the window (call before video_draw's swap: it is read from the back buffer) */
int video_screenshot(const char *path);
const char *video_take_error(void);
int video_refresh_hz(void);
void *video_hwnd(void);

/* window events */
enum { VE_QUIT, VE_KEY, VE_CHAR, VE_FOCUS, VE_RESIZE };
typedef struct VEvent {
    int type;
    int vk;
    int down;
    int mods;
    int rep;
} VEvent;
/* 1 if there is an event; pumps the Windows message queue */
int video_poll(VEvent *e);
#endif
