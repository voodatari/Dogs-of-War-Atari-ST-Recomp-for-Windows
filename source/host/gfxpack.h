/*
 * gfxpack.h - Replacement graphics packs: RGBA PNG sheets that replace the game's graphics, identified by the hash of
 * their content.
 */
#ifndef GFXPACK_H
#define GFXPACK_H
#include "host.h"
#include "scene.h"

/* Reads assets/original/index.json; 0 if there is no dump (then there are no packs). `want` = requested pack ("" = the
 * first one found or the original). */
int gfxpack_init(const char *assets_root, int S, const char *want);
/* hooks the render.c calls (sprites, HUD, texture of the horizontal stretch) */
void gfxpack_attach(void);
const char *gfxpack_name(void); /* chosen pack ("original" if none) */
int gfxpack_replaced(void);     /* replaced elements of the chosen pack */
int gfxpack_list_count(void);   /* available packs (without "original") */
const char *gfxpack_list_name(int i);
void gfxpack_select(const char *name); /* chooses the pack and rereads the sheets */
void gfxpack_reload(void);             /* F9: rereads the sheets of the chosen pack */
int gfxpack_has_layers(void);
/* the pack changes floor tiles or objects (the whole level must be built) */
int gfxpack_has_tiles(void);
/* RGBA replacement (32S x 64S) of a floor tile, or NULL */
const uint8_t *gfxpack_ground_rgba(const uint8_t *tile32x64, int S);
/* RGBA replacement (16S x 16S) of an object tile, or NULL */
const uint8_t *gfxpack_object_rgba(const uint8_t *tile16x16, int S);
/* active level builders (threads): while there is one, what was replaced is not freed */
void gfxpack_builders(int delta);
/* hash of a sprite (graphics dump) */
void gfx_sprite_hash(const Sprite *sp, char *hex);
/* hash of a tile of indices (h x w) */
void gfx_array_hash(const uint8_t *a, int h, int w, char *hex);
/* layers of fixed screens (menus, map, shop, panels) over `out` (RGB 200S x 320S); ingame_only: only the mission panels
 */
void gfxpack_apply_layers(uint8_t *out, const uint8_t *idx, const Zone *zones, int nz, int S, int ingame_only);
#endif
