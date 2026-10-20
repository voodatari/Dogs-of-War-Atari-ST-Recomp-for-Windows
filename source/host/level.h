/*
 * level.h - Map of the vertical-scroll sections: decoded the first time a section is seen so the whole background can
 * be drawn with tiles from a graphics pack (64x32 floor and 16x16 objects).
 */
#ifndef LEVEL_H
#define LEVEL_H
#include "host.h"

/* 64 px block: floor and objects (tile, position code) */
typedef struct LevBlk {
    int ground;
    int nobj;
    int *objt, *objc;
} LevBlk;
typedef struct Level {
    uint32_t id;
    int sec;
    /* rows of the map (without the prefix of 8 uniform floor rows) */
    int nrows;
    LevBlk *blk; /* (nrows + 8) x 3: row 0 is the BOTTOM one */
    /* baldosas usadas: suelo 32 x 64 indices, objeto 16 x 16 indices (0 = transparente) */
    uint8_t *gtile[128];
    uint8_t *otile[256];
    int pos_ok[64], pos_x[64], pos_y[64]; /* position code -> (x, y) inside the block */
    int H;                                /* height in lines: 32 * (rows + 8) */
} Level;

/* NULL if not there (the registry keeps the latest ones) */
Level *level_find(uint32_t id);
void level_new_segment(void);
/* RGB (H*S x 192*S x 3) of the level with the tiles of the chosen graphics pack; NULL until it is built (separate
 * thread) */
const uint8_t *level_rgb(uint32_t id, const uint8_t *pal32, int S, int *rows_px);
/* graphics dump: a whole section as RGB (malloc) */
uint8_t *level_dump_rgb(int sec, const uint8_t *pal32, int *rows_px, int *gmax);
#endif
