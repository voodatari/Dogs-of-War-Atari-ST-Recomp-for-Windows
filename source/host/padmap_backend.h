/*
 * padmap_backend.h - The few drawing primitives padmap.c needs. padmap.c (the picture: geometry, texts, layout) is
 * plain portable C; everything platform-specific lives in an implementation of this interface: padmap_gdi.c (Windows,
 * GDI). To port the program to another system write another backend (for example a small scanline rasterizer +
 * stb_truetype with a bundled font) and nothing else changes.
 *
 * Coordinates and sizes are in the picture's own units (the canvas is 740 x 450 units starting at y = 112); the backend
 * decides the resolution it draws at (the GDI one draws at 3x so the edges come out smooth when the menu scales the
 * picture down). Colours are 0xRRGGBB, or PMB_NONE for "not painted". Texts are the program's 8-bit strings (as
 * returned by tr()).
 */
#ifndef PADMAP_BACKEND_H
#define PADMAP_BACKEND_H
#include "host.h"

#define PMB_NONE (-1L)

/* a new canvas covering that area of units, transparent */
void pmb_begin(double x0, double y0, double w, double h);
/* the finished canvas as RGBA (malloc'd, transparent where nothing was drawn); *pw x *ph pixels */
uint8_t *pmb_end(int *pw, int *ph);

void pmb_circle(double cx, double cy, double r, long fill, long stroke, double sw);
void pmb_rrect(double x, double y, double w, double h, double r, long fill, long stroke, double sw);
/* closed; xy = x0, y0, x1, y1, ... */
void pmb_polygon(const double *xy, int n, long fill, long stroke, double sw);
/* open, round ends and joints */
void pmb_polyline(const double *xy, int n, long stroke, double sw);
/* fills the polygon with a vertical gradient between the top and the bottom of its box */
void pmb_polygon_gradient(const double *xy, int n, long top, long bottom);

typedef void *PmFont;
/* bold: 0 normal, 1 semibold, 2 extra bold; size in units (em height) */
PmFont pmb_font(double size, int bold);
void pmb_font_free(PmFont f);
/* width in units */
double pmb_text_w(PmFont f, const char *s);
/* left edge (or centre if centered) at x, vertically centred on ycenter */
void pmb_text(PmFont f, const char *s, double x, double ycenter, long col, int centered);
#endif
