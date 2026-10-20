/*
 * padmap.c - The picture of the gamepad of the Controls tab (portable core). It reproduces an illustration of a pad
 * with a labelled box for every control; every box shows what the button does NOW (the action assigned in the "Keys and
 * gamepad buttons" window, or the factory one), so the picture always matches the real controls. The text comes from
 * the language files (padmap.*). Nothing here is platform-specific: the drawing is done through the primitives of
 * padmap_backend.h. The picture is made once and kept until the assignments, the twin-stick option or the language
 * change (padmap_signature).
 */
#include "padmap.h"
#include "padmap_backend.h"
#include "input.h"
#include "lang.h"
#include <math.h>

/* size of the picture in its own units; it starts at VY0 (the title boxes of the original illustration are not drawn)
 */
#define VW   740
#define VY0  112
#define VH   450
#define NONE PMB_NONE

/* ---- the pad itself (coordinates of the illustration's group: translate(74 70) scale(0.8)) ---- */
#define GX(x) (74 + 0.8 * (x))
#define GY(y) (70 + 0.8 * (y))
/* a polygon: x, y, x, y... in picture units */
typedef struct {
    double v[512];
    int n;
} Pts;
static void addpt(Pts *p, double x, double y) {
    if (p->n < 254) {
        p->v[2 * p->n] = GX(x);
        p->v[2 * p->n + 1] = GY(y);
        p->n++;
    }
}
static void cubic(Pts *p, double x0, double y0, double x1, double y1, double x2, double y2, double x3, double y3) {
    for (int i = 1; i <= 16; i++) {
        double t = i / 16.0, u = 1 - t;
        addpt(p, u * u * u * x0 + 3 * u * u * t * x1 + 3 * u * t * t * x2 + t * t * t * x3,
              u * u * u * y0 + 3 * u * u * t * y1 + 3 * u * t * t * y2 + t * t * t * y3);
    }
}
static void quad(Pts *p, double x0, double y0, double x1, double y1, double x2, double y2) {
    for (int i = 1; i <= 12; i++) {
        double t = i / 12.0, u = 1 - t;
        addpt(p, u * u * x0 + 2 * u * t * x1 + t * t * x2, u * u * y0 + 2 * u * t * y1 + t * t * y2);
    }
}
static void gcircle(double cx, double cy, double r, long fill, long stroke, double sw) {
    pmb_circle(GX(cx), GY(cy), 0.8 * r, fill, stroke, 0.8 * sw);
}
static void grrect(double x, double y, double w, double h, double r, long fill) {
    pmb_rrect(GX(x), GY(y), 0.8 * w, 0.8 * h, 0.8 * r, fill, NONE, 0);
}

static void draw_pad(void) {
    Pts p;
    /* bumpers */
    p.n = 0;
    addpt(&p, 205, 222);
    quad(&p, 205, 222, 228, 186, 302, 188);
    addpt(&p, 306, 204);
    quad(&p, 306, 204, 246, 204, 216, 230);
    pmb_polygon(p.v, p.n, 0x2a2f37, 0x454c56, 1.6);
    p.n = 0;
    addpt(&p, 535, 222);
    quad(&p, 535, 222, 512, 186, 438, 188);
    addpt(&p, 434, 204);
    quad(&p, 434, 204, 494, 204, 524, 230);
    pmb_polygon(p.v, p.n, 0x2a2f37, 0x454c56, 1.6);
    /* body: gradient fill, then outline */
    p.n = 0;
    addpt(&p, 230, 205);
    cubic(&p, 230, 205, 290, 190, 330, 200, 370, 200);
    cubic(&p, 370, 200, 410, 200, 450, 190, 510, 205);
    cubic(&p, 510, 205, 560, 215, 585, 260, 600, 330);
    cubic(&p, 600, 330, 615, 400, 625, 470, 590, 495);
    cubic(&p, 590, 495, 560, 515, 525, 495, 505, 465);
    cubic(&p, 505, 465, 485, 435, 460, 420, 430, 420);
    addpt(&p, 310, 420);
    cubic(&p, 310, 420, 280, 420, 255, 435, 235, 465);
    cubic(&p, 235, 465, 215, 495, 180, 515, 150, 495);
    cubic(&p, 150, 495, 115, 470, 125, 400, 140, 330);
    cubic(&p, 140, 330, 155, 260, 180, 215, 230, 205);
    pmb_polygon_gradient(p.v, p.n, 0x30353d, 0x121418);
    pmb_polygon(p.v, p.n, NONE, 0x3d444e, 2.4);
    /* guide button */
    gcircle(370, 245, 19, 0x1b1f25, 0xa7b1bd, 3);
    gcircle(330, 300, 10, 0x0d0f12, 0x4a525d, 2);
    gcircle(410, 300, 10, 0x0d0f12, 0x4a525d, 2);
    pmb_rrect(GX(360), GY(320), 0.8 * 20, 0.8 * 11, 0.8 * 5.5, 0x0d0f12, 0x4a525d, 1.6);
    /* sticks */
    gcircle(240, 290, 35, 0x0b0d10, NONE, 0);
    gcircle(240, 290, 25, 0x2a2f37, 0x4a525d, 2);
    gcircle(240, 290, 15, 0x1c2026, NONE, 0);
    gcircle(450, 380, 35, 0x0b0d10, NONE, 0);
    gcircle(450, 380, 25, 0x2a2f37, 0x4a525d, 2);
    gcircle(450, 380, 15, 0x1c2026, NONE, 0);
    /* D-pad */
    gcircle(300, 375, 32, 0x0b0d10, NONE, 0);
    grrect(288, 351, 24, 48, 4, 0x2a2f37);
    grrect(276, 363, 48, 24, 4, 0x2a2f37);
    static const struct {
        double x, y;
        long col;
        const char *s;
    } fb[4] = {
        {500, 268, 0xf2c230, "Y"}, {468, 300, 0x3b95f2, "X"}, {532, 300, 0xec4a44, "B"}, {500, 332, 0x5fd146, "A"}};
    PmFont f = pmb_font(19 * 0.8, 2);
    for (int i = 0; i < 4; i++) {
        gcircle(fb[i].x, fb[i].y, 15, 0x0d0f12, fb[i].col, 2.5);
        pmb_text(f, fb[i].s, GX(fb[i].x), GY(fb[i].y), fb[i].col, 1);
    }
    pmb_font_free(f);
}

/* ---- the labelled boxes ---- */
enum { I_PILL, I_BTN, I_RING, I_SYS };
enum { F_NONE, F_LSTICK, F_DPAD, F_RSTICK };
enum { G_NONE, G_START, G_BACK, G_PLUS };
typedef struct {
    double x, y, w, h;
    long col;
    int icon;
    const char *sym;
    double ix, iy, ir;
    long symcol;
    double tx;
    int mask, fixed, glyph;
    const char *btn;
    double lx[3], ly[3];
    int nl;
    double dot;
    /* box, colour, icon (position, size, symbol colour), text column, what it shows (button / fixed control), leader
     * line (up to 3 points) and the size of its end dot */
} Slot;
static const Slot SL[] = {
    {178,      120,   126,   44,     0xa46cf2, I_PILL, "LB",       188,        132, 26,
     0xffffff, 221.7, XB_LB, F_NONE, G_NONE,   NULL,   {241, 262}, {166, 228}, 2,   4},
    {436,      120,   126,   44,     0xa46cf2, I_PILL, "RB",       446,        132, 26,
     0xffffff, 480.4, XB_RB, F_NONE, G_NONE,   NULL,   {499, 478}, {166, 228}, 2,   4},
    {14,       162,  146,   42,     0xf2c230, I_PILL, "LT",       24,         173, 26,
     0x1a1405, 56.5, PB_LT, F_NONE, G_NONE,   NULL,   {162, 238}, {183, 240}, 2,   4},
    {580,      152,   152,   48,     0xf2c230, I_PILL, "RT",       590,        166, 26,
     0x1a1405, 624.5, PB_RT, F_NONE, G_NONE,   NULL,   {578, 502}, {176, 240}, 2,   4},
    {14,       264, 146, 54,       0x3b95f2, I_RING, "L",        36,         291, 10.5,
     0x3b95f2, 56,  0,   F_LSTICK, G_NONE,   NULL,   {162, 266}, {291, 302}, 2,   4.5},
    {14,       362, 166, 48,     0x2ccbe4, I_SYS, "",         36,         386, 12,
     0x2ccbe4, 56,  0,   F_DPAD, G_PLUS,   NULL,  {182, 314}, {386, 370}, 2,   4.5},
    {198, 512,     140,    48,     0xc9d3de,          I_SYS,      "",         220, 536, 12, 0xe9eef4,
     240, XB_BACK, F_NONE, G_BACK, "padmap.btn.back", {334, 338}, {510, 319}, 2,   4},
    {398, 512,      156,    48,      0xc9d3de,           I_SYS,      "",         420, 536, 12, 0xe9eef4,
     440, XB_START, F_NONE, G_START, "padmap.btn.start", {404, 402}, {510, 319}, 2,   4},
    {580,      214, 152,  32,     0xf2c230, I_BTN, "Y",        601,        230, 11,
     0x1a1405, 620, XB_Y, F_NONE, G_NONE,   NULL,  {578, 485}, {230, 278}, 2,   3.5},
    {580,      256, 152,  72,     0xec4a44, I_BTN, "B",        601,        292, 11,
     0xffffff, 620, XB_B, F_NONE, G_NONE,   NULL,  {578, 512}, {292, 307}, 2,   3.5},
    {580,      340, 152,  32,     0x5fd146, I_BTN, "A",        601,        356, 11,
     0x0a1a06, 620, XB_A, F_NONE, G_NONE,   NULL,  {578, 487}, {356, 338}, 2,   3.5},
    {580,  384,    152,    58,   0x3b95f2,        I_BTN,           "X", 601, 413, 11, 0xffffff, 620,
     XB_X, F_NONE, G_NONE, NULL, {578, 462, 452}, {413, 352, 322}, 3,   3.5},
    {580,      454, 152, 76,       0xb866f2, I_RING, "R",        602,        492, 10.5,
     0xb866f2, 622, 0,   F_RSTICK, G_NONE,   NULL,   {578, 434}, {492, 374}, 2,   4.5},
};
#define NSL ((int)(sizeof SL / sizeof *SL))
/* order of PBA_* */
static const char *ACT[PBA_COUNT] = {"fire", "fire2", "hold", "wnext", "wprev", "rocket", "grenade", "menu", "exit"};

/* the text if the language has it, else "" */
static const char *opt(const char *id) {
    const char *t = tr(id);
    return strcmp(t, id) ? t : "";
}
static const char *act_text(int a, const char *suffix, char *buf, size_t n) {
    snprintf(buf, n, "padmap.act.%s%s", ACT[a], suffix);
    return opt(buf);
}

/* title, description and second description (L3 / R3 click) of a box; *dim = the control does nothing now */
static void content(const Slot *s, char *title, char *desc, char *desc2, int *dim) {
    char id[64];
    title[0] = desc[0] = desc2[0] = 0;
    *dim = 0;
    if (s->fixed == F_NONE) {
        int a = pad_action_of(s->mask);
        /* Start / Back: the button name, then what it does */
        if (s->btn) {
            snprintf(title, 200, "%s", tr(s->btn));
            snprintf(desc, 200, "%s", a >= 0 ? act_text(a, "", id, sizeof id) : tr("padmap.unassigned"));
        } else if (a < 0)
            snprintf(title, 200, "%s", tr("padmap.unassigned"));
        else {
            snprintf(title, 200, "%s", act_text(a, "", id, sizeof id));
            snprintf(desc, 200, "%s", act_text(a, ".desc", id, sizeof id));
        }
        return;
    }
    int mask = s->fixed == F_LSTICK ? XB_L3 : XB_R3;
    snprintf(title, 200, "%s",
             tr(s->fixed == F_LSTICK ? "padmap.lstick"
                : s->fixed == F_DPAD ? "padmap.dpad"
                                     : "padmap.rstick"));
    if (s->fixed == F_RSTICK && !pad_dual_stick) {
        snprintf(desc, 200, "%s", tr("padmap.rstick.off"));
        *dim = 1;
    } else
        snprintf(desc, 200, "%s",
                 tr(s->fixed == F_LSTICK ? "padmap.lstick.desc"
                    : s->fixed == F_DPAD ? "padmap.dpad.desc"
                                         : "padmap.rstick.desc"));
    if (s->fixed != F_DPAD) {
        int a = pad_action_of(mask);
        if (a >= 0)
            trf(desc2, 200, "padmap.click", act_text(a, "", id, sizeof id));
    }
}

/* at most maxl lines of `width` units with font f */
static int wrap(PmFont f, const char *text, double width, char lines[][200], int maxl) {
    char buf[512];
    snprintf(buf, sizeof buf, "%s", text);
    int nl = 0;
    char cur[200] = "";
    /* words separated by spaces */
    for (char *w = buf, *nx; *w; w = nx) {
        while (*w == ' ')
            w++;
        if (!*w)
            break;
        nx = w;
        while (*nx && *nx != ' ')
            nx++;
        if (*nx)
            *nx++ = 0;
        char t[260];
        snprintf(t, sizeof t, "%s%s%s", cur, cur[0] ? " " : "", w);
        if (pmb_text_w(f, t) <= width || !cur[0])
            snprintf(cur, sizeof cur, "%s", t);
        else {
            if (nl < maxl)
                snprintf(lines[nl++], 200, "%s", cur);
            snprintf(cur, sizeof cur, "%s", w);
        }
    }
    if (cur[0] && nl < maxl)
        snprintf(lines[nl++], 200, "%s", cur);
    return nl;
}

static void glyph(int gl, double cx, double cy, long col) {
    if (gl == G_START)
        for (int i = -1; i <= 1; i++) {
            double l[4] = {cx - 5.5, cy + 4.2 * i, cx + 5.5, cy + 4.2 * i};
            pmb_polyline(l, 2, col, 2.1);
        }
    else if (gl == G_PLUS) {
        double a[4] = {cx - 6.5, cy, cx + 6.5, cy}, b[4] = {cx, cy - 6.5, cx, cy + 6.5};
        pmb_polyline(a, 2, col, 3.4);
        pmb_polyline(b, 2, col, 3.4);
    } else if (gl == G_BACK) {
        pmb_rrect(cx - 7, cy - 5, 9, 9, 1, NONE, col, 1.8);
        pmb_rrect(cx - 2, cy - 1, 9, 9, 1, NONE, col, 1.8);
    }
}

static void draw_slot(const Slot *s) {
    char title[200], desc[200], desc2[200];
    int dim;
    content(s, title, desc, desc2, &dim);
    long col = dim ? 0x5b6573 : s->col, tcol = dim ? 0x7d8794 : 0xeef3f8, dcol = dim ? 0x6b7480 : 0xb9c6d4;
    pmb_rrect(s->x, s->y, s->w, s->h, 10, 0x121a27, col, 2);
    if (s->icon == I_PILL) {
        pmb_rrect(s->ix, s->iy, s->ir, 20, 6, col, NONE, 0);
        PmFont f = pmb_font(15, 2);
        pmb_text(f, s->sym, s->ix + s->ir / 2, s->iy + 10.5, dim ? 0xc8cdd4 : s->symcol, 1);
        pmb_font_free(f);
    } else if (s->icon == I_BTN) {
        pmb_circle(s->ix, s->iy, s->ir, col, NONE, 0);
        PmFont f = pmb_font(14, 2);
        pmb_text(f, s->sym, s->ix, s->iy + 1, s->symcol, 1);
        pmb_font_free(f);
    } else if (s->icon == I_RING) {
        pmb_circle(s->ix, s->iy, s->ir, NONE, col, 3);
        PmFont f = pmb_font(13, 2);
        pmb_text(f, s->sym, s->ix, s->iy + 1, col, 1);
        pmb_font_free(f);
    } else {
        pmb_circle(s->ix, s->iy, s->ir, s->glyph == G_PLUS ? col : 0xe9eef4, NONE, 0);
        glyph(s->glyph, s->ix, s->iy, 0x08111b);
    }
    /* text: the largest size at which the three paragraphs (name, description, second description) fit in the box */
    double avail = s->x + s->w - 8 - s->tx, sc, total = 0;
    char lt[3][200], ld[4][200], l2[3][200];
    int nt = 0, nd = 0, n2 = 0;
    PmFont ft = NULL, fd = NULL;
    for (sc = 1.0; sc >= 0.5; sc -= 0.05) {
        if (ft)
            pmb_font_free(ft);
        if (fd)
            pmb_font_free(fd);
        ft = pmb_font(16 * sc, 1);
        fd = pmb_font(12.5 * sc, 0);
        nt = title[0] ? wrap(ft, title, avail, lt, 3) : 0;
        nd = desc[0] ? wrap(fd, desc, avail, ld, 4) : 0;
        n2 = desc2[0] ? wrap(fd, desc2, avail, l2, 3) : 0;
        total = nt * 16.8 * sc + (nd + n2) * 13.3 * sc + (nt && (nd + n2) ? 2 * sc : 0);
        if (total <= s->h - 8)
            break;
    }
    double y = s->y + s->h / 2 - total / 2, step = 16.8 * sc;
    for (int i = 0; i < nt; i++) {
        pmb_text(ft, lt[i], s->tx, y + step / 2, tcol, 0);
        y += step;
    }
    if (nt && (nd + n2))
        y += 2 * sc;
    step = 13.3 * sc;
    for (int i = 0; i < nd + n2; i++) {
        pmb_text(fd, i < nd ? ld[i] : l2[i - nd], s->tx, y + step / 2, i < nd ? dcol : 0xffe08c, 0);
        y += step;
    }
    pmb_font_free(ft);
    pmb_font_free(fd);
}

static int slot_dim(const Slot *s) {
    return s->fixed == F_RSTICK && !pad_dual_stick;
}

unsigned padmap_signature(void) {
    unsigned h = 2166136261u;
    for (int a = 0; a < PBA_COUNT; a++)
        h = (h ^ (unsigned)pad_bind[a]) * 16777619u;
    h = (h ^ (unsigned)pad_dual_stick) * 16777619u;
    h = (h ^ (unsigned)lang_index()) * 16777619u;
    return h;
}

uint8_t *padmap_render(int *w, int *h) {
    pmb_begin(0, VY0, VW, VH);
    draw_pad();
    /* leader lines: a dark edge first, then the colour */
    for (int pass = 0; pass < 2; pass++)
        for (int i = 0; i < NSL; i++) {
            double xy[6];
            for (int k = 0; k < SL[i].nl; k++) {
                xy[2 * k] = SL[i].lx[k];
                xy[2 * k + 1] = SL[i].ly[k];
            }
            pmb_polyline(xy, SL[i].nl,
                         pass == 0          ? 0x05090e
                         : slot_dim(&SL[i]) ? 0x5b6573
                                            : SL[i].col,
                         pass == 0 ? 6 : 2.5);
        }
    for (int i = 0; i < NSL; i++) {
        int k = SL[i].nl - 1;
        pmb_circle(SL[i].lx[k], SL[i].ly[k], SL[i].dot, slot_dim(&SL[i]) ? 0x5b6573 : SL[i].col, 0x05090e, 1.5);
    }
    for (int i = 0; i < NSL; i++)
        draw_slot(&SL[i]);
    return pmb_end(w, h);
}
