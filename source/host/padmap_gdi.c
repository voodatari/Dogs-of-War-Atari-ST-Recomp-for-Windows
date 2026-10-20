/*
 * padmap_gdi.c - Windows (GDI) backend of the pad picture (see padmap_backend.h). It draws at 3x into a DIB and hands back RGBA; the pixels that keep the panel colour are transparent.
 */
#include "padmap_backend.h"
#ifdef _WIN32
#include <windows.h>
#include <math.h>

#define SS 3
#define BGC RGB(10, 12, 24)                                                         /* the colour of the menu panel: what is left with it is transparent */
#define CC(h) RGB(((h) >> 16) & 255, ((h) >> 8) & 255, (h) & 255)
static double ox, oy, cw, ch; static HDC dc; static HBITMAP bmp; static HGDIOBJ oldb; static uint8_t *bits;
#define PX(x) ((int)lround(((x) - ox) * SS))
#define PY(y) ((int)lround(((y) - oy) * SS))

void pmb_begin(double x0, double y0, double w, double h) {
    ox = x0; oy = y0; cw = w; ch = h; int W = (int)lround(w * SS), H = (int)lround(h * SS);
    dc = CreateCompatibleDC(NULL); BITMAPINFO bi; memset(&bi, 0, sizeof bi); bi.bmiHeader.biSize = sizeof bi.bmiHeader; bi.bmiHeader.biWidth = W; bi.bmiHeader.biHeight = -H; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32; bi.bmiHeader.biCompression = BI_RGB;
    bmp = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, (void **)&bits, NULL, 0); oldb = SelectObject(dc, bmp);
    HBRUSH br = CreateSolidBrush(BGC); RECT rc = { 0, 0, W, H }; FillRect(dc, &rc, br); DeleteObject(br);
    SetBkMode(dc, TRANSPARENT); SetTextAlign(dc, TA_LEFT | TA_TOP);
}
uint8_t *pmb_end(int *pw, int *ph) {
    int W = (int)lround(cw * SS), H = (int)lround(ch * SS); GdiFlush();
    uint8_t *rgba = (uint8_t *)malloc((size_t)W * H * 4);
    for (int i = 0; i < W * H; i++) { const uint8_t *s = bits + (size_t)i * 4; uint8_t *d = rgba + (size_t)i * 4; d[0] = s[2]; d[1] = s[1]; d[2] = s[0]; d[3] = (s[2] == 10 && s[1] == 12 && s[0] == 24) ? 0 : 255; }
    SelectObject(dc, oldb); DeleteObject(bmp); DeleteDC(dc); *pw = W; *ph = H; return rgba;
}

typedef struct { HGDIOBJ ob, op, nb, np; } Gs;
static Gs gs_begin(long fill, long stroke, double sw) {
    Gs g;
    g.nb = fill < 0 ? GetStockObject(NULL_BRUSH) : (HGDIOBJ)CreateSolidBrush(CC(fill));
    if (stroke < 0) g.np = GetStockObject(NULL_PEN);
    else { LOGBRUSH lb; lb.lbStyle = BS_SOLID; lb.lbColor = CC(stroke); lb.lbHatch = 0; DWORD w = (DWORD)lround(sw * SS); if (w < 1) w = 1; g.np = (HGDIOBJ)ExtCreatePen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_ROUND | PS_JOIN_ROUND, w, &lb, 0, NULL); }
    g.ob = SelectObject(dc, g.nb); g.op = SelectObject(dc, g.np); return g;
}
static void gs_end(Gs g) { SelectObject(dc, g.ob); SelectObject(dc, g.op); DeleteObject(g.nb); DeleteObject(g.np); }

void pmb_circle(double cx, double cy, double r, long fill, long stroke, double sw) { Gs g = gs_begin(fill, stroke, sw); Ellipse(dc, PX(cx - r), PY(cy - r), PX(cx + r), PY(cy + r)); gs_end(g); }
void pmb_rrect(double x, double y, double w, double h, double r, long fill, long stroke, double sw) { Gs g = gs_begin(fill, stroke, sw); RoundRect(dc, PX(x), PY(y), PX(x + w), PY(y + h), (int)lround(2 * r * SS), (int)lround(2 * r * SS)); gs_end(g); }
static int to_points(const double *xy, int n, POINT *p) { for (int i = 0; i < n; i++) { p[i].x = PX(xy[2 * i]); p[i].y = PY(xy[2 * i + 1]); } return n; }
void pmb_polygon(const double *xy, int n, long fill, long stroke, double sw) {
    POINT p[260]; if (n > 256) n = 256; to_points(xy, n, p); Gs g = gs_begin(fill, stroke, sw);
    if (fill < 0) { p[n] = p[0]; Polyline(dc, p, n + 1); } else Polygon(dc, p, n);
    gs_end(g);
}
void pmb_polyline(const double *xy, int n, long stroke, double sw) { POINT p[256]; if (n > 256) n = 256; to_points(xy, n, p); Gs g = gs_begin(PMB_NONE, stroke, sw); Polyline(dc, p, n); gs_end(g); }
void pmb_polygon_gradient(const double *xy, int n, long top, long bottom) {
    POINT p[256]; if (n > 256) n = 256; to_points(xy, n, p);
    int y0 = p[0].y, y1 = p[0].y, x0 = p[0].x, x1 = p[0].x; for (int i = 1; i < n; i++) { if (p[i].y < y0) y0 = p[i].y; if (p[i].y > y1) y1 = p[i].y; if (p[i].x < x0) x0 = p[i].x; if (p[i].x > x1) x1 = p[i].x; }
    HRGN rg = CreatePolygonRgn(p, n, WINDING); SelectClipRgn(dc, rg);
    for (int y = y0; y <= y1; y++) {
        double t = (double)(y - y0) / (y1 - y0 > 0 ? y1 - y0 : 1); int c[3];
        for (int k = 0; k < 3; k++) { int a = (top >> (16 - 8 * k)) & 255, b = (bottom >> (16 - 8 * k)) & 255; c[k] = (int)(a + (b - a) * t); }
        HBRUSH br = CreateSolidBrush(RGB(c[0], c[1], c[2])); RECT rc = { x0, y, x1 + 1, y + 1 }; FillRect(dc, &rc, br); DeleteObject(br);
    }
    SelectClipRgn(dc, NULL); DeleteObject(rg);
}

static char face[40];
PmFont pmb_font(double size, int bold) {
    if (!face[0]) {                                                         /* a condensed face if the system has one (the boxes are narrow) */
        static const char *cand[] = { "Bahnschrift SemiCondensed", "Arial Narrow", "Segoe UI" };
        HDC d = GetDC(NULL);
        for (int i = 0; i < 3 && !face[0]; i++) {
            HFONT f = CreateFontA(-20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, cand[i]);
            HGDIOBJ o = SelectObject(d, f); char got[64] = ""; GetTextFaceA(d, sizeof got, got); SelectObject(d, o); DeleteObject(f);
            if (!_stricmp(got, cand[i])) snprintf(face, sizeof face, "%s", cand[i]);
        }
        ReleaseDC(NULL, d); if (!face[0]) snprintf(face, sizeof face, "Segoe UI");
    }
    return (PmFont)CreateFontA(-(int)lround(size * SS), 0, 0, 0, bold >= 2 ? FW_EXTRABOLD : bold == 1 ? FW_SEMIBOLD : FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
}
void pmb_font_free(PmFont f) { DeleteObject((HGDIOBJ)f); }
double pmb_text_w(PmFont f, const char *s) { HGDIOBJ o = SelectObject(dc, (HFONT)f); SIZE sz; GetTextExtentPoint32A(dc, s, (int)strlen(s), &sz); SelectObject(dc, o); return (double)sz.cx / SS; }
void pmb_text(PmFont f, const char *s, double x, double ycenter, long col, int centered) {
    HGDIOBJ o = SelectObject(dc, (HFONT)f); SIZE sz; GetTextExtentPoint32A(dc, s, (int)strlen(s), &sz); SetTextColor(dc, CC(col));
    TextOutA(dc, PX(x) - (centered ? sz.cx / 2 : 0), PY(ycenter) - sz.cy / 2, s, (int)strlen(s)); SelectObject(dc, o);
}
#endif
