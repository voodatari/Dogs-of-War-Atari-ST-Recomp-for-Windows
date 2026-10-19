/*
 * scene.h - Scene capture: what the game draws in every logic frame, split into background + HUD + units.
 *
 * Each unit (player, enemy, bullet, explosion, vehicle...) is recognized when its drawing routine starts (hook) and its sprite comes from an ATLAS
 * keyed by (type, frame). The atlas is filled by decoding the graphics tables in the game's RAM (sprites.c) and each
 * hit is validated against what the game really draws; if it does not match, the sprite is measured by the difference between before and after the drawing.
 */
#ifndef SCENE_H
#define SCENE_H
#include "host.h"

enum { U_ENEMY, U_PLAYER, U_BULLET, U_BULLET2, U_OBJ5E32, U_OBJ5F1A, U_FX, U_VEH6, U_VEH4, U_VEH, U_TURRET, U_BOOM, U_NUNITS };
extern const char *unit_name[U_NUNITS];

typedef struct Sprite {
    int w, h;                                    /* size of the cropped opaque drawing */
    uint8_t *img, *mask;                         /* color indices (0..15) and mask (0/1), h x w */
    int ax, ay;                                  /* sprite corner relative to the position (x, y) of the game structure */
    int partial;                                 /* clipped by a screen edge or measured by difference: it is not the complete drawing */
    int ref;                                     /* referencias (atlas, frames, presentador) */
    char phash[16];                              /* content hash in hex (see gfxpack.c), valid if hashed */
    int hashed;
} Sprite;
Sprite *sprite_new(int w, int h);
Sprite *sprite_ref(Sprite *s);
void    sprite_unref(Sprite *s);

typedef struct UnitRec {
    int unit; uint64_t key;                      /* type and frame ((frame) or (frame, variant)) */
    uint32_t ident;                              /* address of the game structure */
    int px, py;                                  /* position according to the game (the one the atlas uses) */
    int fine;                                    /* fine part of x (explosions and mines: the game draws them aligned to 16 px; the real x is px + fine) */
    int order; int edge;
    Sprite *spr;
    /* presenter: raw, smoothed and world position */
    float raw[2], fpos[2], gpos[2], cw[2], pos[2];
    int kf0, kf1;                                /* fields of the key (frame, variant) to identify animation changes */
} UnitRec;

#define MAX_UNITS 128
typedef struct Frame {
    uint32_t vbl, draw;
    int scroll;                                  /* vertical: phase of the ring */
    int mode_h; int hs;                          /* horizontal: fase acumulada */
    int a280, sec; uint32_t lvl;                 /* vertical: position in the level, section, level id (0 = none) */
    uint8_t *ring_idx;                           /* 256 x 192 (vertical) o NULL */
    uint8_t tile[32 * 32];                       /* repeated texture of the horizontal stretch */
    uint8_t *hud;                                /* 200 x 320: complete game screen in indices */
    Zone zones[MAXZONES]; int nzones;
    UnitRec units[MAX_UNITS]; int nunits;
    uint8_t *res_mask, *res_idx;                 /* residue: pixels the reconstruction does not explain (or NULL) */
    int res_px;
    int reset;                                   /* first frame after a new segment */
    int bg_ok;                                   /* the background of this frame was captured */
    int ref;
    /* presenter: world offset */
    float Ux, Uy;
    /* render precalculations (see render.c) */
    void *cache;
} Frame;
Frame *frame_new(void);
Frame *frame_ref(Frame *f);
void   frame_unref(Frame *f);

/* ---- atlas ---- */
typedef struct Atlas Atlas;
extern Atlas *g_atlas;
Sprite *atlas_get(Atlas *a, uint64_t key);
void    atlas_put(Atlas *a, uint64_t key, Sprite *s);
void    atlas_drop(Atlas *a, uint64_t key);

/* ---- grabador ---- */
typedef struct SceneCb {
    void (*on_frame)(Frame *f);                  /* frame logico completo */
    void (*on_sprite)(uint64_t key, Sprite *s);  /* new atlas sprite (for the presenter) */
    void (*on_drop)(uint64_t key);               /* discarded atlas entry */
    void (*on_reset)(void);                      /* new segment (new mission) */
} SceneCb;
void scene_init(const SceneCb *cb);
void scene_install_hooks(void);                  /* registers the scene hooks in hooks.c */
void scene_new_segment(void);
void scene_clear_frames(void);
void scene_dump_atlas(void);
extern uint32_t scene_last_vbl;
extern int scene_nframes_since;                  /* frames published since the last reset */

/* ---- sprites of the graphics tables in RAM (sprites.c) ---- */
Sprite *rom_sprite(int unit, int key0, int key1);   /* NULL if it cannot be decoded */

/* ST format utilities */
void planes_to_idx(const uint8_t *raw, int rows, int groups, uint8_t *out);     /* (rows x groups*16) indices from interleaved planes */
#endif
