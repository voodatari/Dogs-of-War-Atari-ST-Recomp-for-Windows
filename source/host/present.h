/*
 * present.h - Presenter: receives logic frames from the emulator and composes, at the monitor's rate,
 * the interpolated image with enemy-motion filters, scroll stabilization and fixed screens (menus, map, shop).
 */
#ifndef PRESENT_H
#define PRESENT_H
#include "render.h"

typedef struct PFlags { int sfix, subpx, hold, extrap, xfilter, yfilter, cont, res, interp; } PFlags;
extern PFlags pflags;

/* messages from the emulator to the presenter */
enum { MSG_RATE, MSG_FRAME, MSG_RESET, MSG_MENU, MSG_SCR, MSG_TEXT, MSG_STAT, MSG_ERROR, MSG_LEVEL, MSG_HUD };
typedef struct Msg {
    int type;
    double t0, vbl0, rate;                         /* MSG_RATE */
    Frame *frame;                                  /* MSG_FRAME */
    uint32_t vbl; uint8_t *idx; Zone zones[MAXZONES]; int nz;     /* MSG_MENU */
    char text[256];                                /* MSG_SCR / MSG_TEXT / MSG_ERROR */
    double stat[6];                                /* MSG_STAT */
    void *ptr;
} Msg;
Msg *msg_new(int type);
void msg_free(Msg *m);
void msgq_push(Msg *m);                            /* emulator -> presenter (drops the oldest ones if a non-critical type fills up) */
Msg *msgq_pop(void);
void msgq_clear(void);
int  msgq_count(void);

typedef struct Presenter Presenter;
Presenter *present_new(int S);
void present_drain(Presenter *p);                  /* handles the pending messages */
/* RGB image (200*S, 320*S, 3) for the instant `at` (seconds of the monotonic clock); *is_menu = 1 if a fixed screen is shown */
const uint8_t *present_frame(Presenter *p, double at, int *is_menu);
int  present_wait_menu(Presenter *p, double timeout);
double present_tv(Presenter *p, double at);        /* game VBL corresponding to the instant `at` */
const char *present_screen_kind(Presenter *p);     /* "game" "shop" "map" "players" "other" */
int  present_last_vbl(Presenter *p);
int  present_umode_h(Presenter *p);
void present_set_ahead(Presenter *p, double a);
int  present_take_text(Presenter *p, char *out, int n, double *t);   /* last notice from the emulator (game saved...) */
const char *present_error(Presenter *p);
void present_stat(Presenter *p, double *out6, int *have);
Frame *present_latest(Presenter *p);               /* ultimo frame logico (referenciado) */
void present_menu_cur(Presenter *p, const uint8_t **idx, const Zone **z, int *nz);
void present_set_scale(Presenter *p, int S);
int  present_scale(Presenter *p);
void present_pack_changed(Presenter *p);           /* the saved frames must rebuild their caches */
#endif
