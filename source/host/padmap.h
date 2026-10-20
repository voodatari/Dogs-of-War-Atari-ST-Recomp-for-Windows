/*
 * padmap.h - The picture of the gamepad shown in the Controls tab, drawn by the program from the current controls (so
 * it follows the redefined buttons, the twin-stick option and the language).
 */
#ifndef PADMAP_H
#define PADMAP_H
#include "host.h"

/* changes whenever something the picture shows changes (button assignments, twin-stick, language) */
unsigned padmap_signature(void);
/* the picture: RGBA, malloc'd, supersampled; transparent where nothing is drawn */
uint8_t *padmap_render(int *w, int *h);
#endif
