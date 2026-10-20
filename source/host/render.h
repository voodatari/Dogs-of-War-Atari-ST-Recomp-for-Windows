/*
 * render.h - Composition of a screen frame from two logic frames.
 */
#ifndef RENDER_H
#define RENDER_H
#include "scene.h"

#define RS_MAX 4 /* maximum output scale */
#define SCR_W  320
#define SCR_H  200

typedef struct UEntry {
    int used;
    int unit;
    uint32_t ident;
    uint32_t pair0, pair1;
    float ua[2], ub[2], e[2];
} UEntry;
typedef struct UState {
    UEntry e[256];
} UState;

typedef struct RenderOpts {
    int S; /* output scale (3: 960x600) */
    int interpolate;
    int use_res; /* residual layer */
    int subpx;   /* sub-pixel position of the sprites */
    UState *st;  /* enemy continuity (NULL = none) */
    float st_decay;
    int st_enemy_only;
} RenderOpts;

/* RGB (200*S, 320*S, 3). f = 0 -> frame A exacto. */
void render(const Frame *A, const Frame *B, float f, const RenderOpts *o, uint8_t *out);
void pal_to_rgb(const uint8_t *pal32, uint8_t rgb[16][3]);
/* RGB of a fixed screen (200x320 indices + palette zones) at scale S */
void render_static(const uint8_t *idx, const Zone *zones, int nz, int S, uint8_t *out);
void render_clear_palmax(void); /* between missions */

/* graphics pack (gfxpack.c): replacement of a sprite at scale S: RGBA (h*S, w*S, 4) and offset (game px) from the
 * sprite's corner; NULL = none */
typedef struct PackSpr {
    const uint8_t *rgba;
    int w, h;
    int dx, dy;
} PackSpr;
extern int (*pack_sprite_cb)(Sprite *sp, int S, PackSpr *out);
/* HUD layers over `base` */
extern void (*pack_hud_cb)(uint8_t *base, const uint8_t *idx, const Zone *zones, int nz, int S);
/* texture of the horizontal stretch (32S x 32S x 3) */
extern int (*pack_texture_cb)(const uint8_t *tile32, int S, const uint8_t **rgb);
/* background of the whole level with tiles of the pack (vertical): RGB (rows_px x 192S x 3) for the palette pal32, or 0
 * if it is not ready yet; H = level height in game lines */
extern int (*pack_level_cb)(uint32_t id, const uint8_t *pal32, int S, const uint8_t **rgb, int *rows_px, int *H);
extern int pack_gen;
#endif
