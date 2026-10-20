/*
 * menu.c - On-screen options menu, drawn with GDI into an RGBA image.
 */
#include "menu.h"
#include "lang.h"
#include "padmap.h"
#ifdef _WIN32
#include <windows.h>
#endif

#define MAXITEMS 128
struct Menu {
    MItem it[MAXITEMS]; int n; char title[80]; int cur, top, open; char status[200]; double status_t;
    int ntabs, tab, tabfocus;                                         /* tabs: MK_TAB items; tab = current one; tabfocus = 1 while the tab bar (not an item) is selected */
};

/* labels may start with spaces (indentation): they are kept in front of the translation of the rest */
static const char *tr_label(const char *id, char *buf, size_t n) { int k = 0; while (id[k] == ' ') k++; snprintf(buf, n, "%.*s%s", k, "                ", tr(id + k)); return buf; }
Menu *menu_new(const char *title) { Menu *m = (Menu *)calloc(1, sizeof(Menu)); snprintf(m->title, sizeof m->title, "%s", title); return m; }
void menu_add(Menu *m, const MItem *it) { if (m->n < MAXITEMS) { m->it[m->n++] = *it; if (it->kind == MK_TAB) m->ntabs++; } }
int menu_is_open(Menu *m) { return m->open; }
/* items of the current tab: [a, b); without tabs, all of them */
static void range_(Menu *m, int *a, int *b) {
    if (!m->ntabs) { *a = 0; *b = m->n; return; }
    int t = -1, s0 = 0; *b = m->n;
    for (int i = 0; i < m->n; i++) if (m->it[i].kind == MK_TAB) { t++; if (t == m->tab) s0 = i + 1; else if (t == m->tab + 1) { *b = i; break; } }
    *a = s0;
}
#define NOSEL(k) ((k) == MK_SEP || (k) == MK_IMAGE)              /* items the cursor skips */
/* pictures of MK_IMAGE items: loaded once */
typedef struct { char path[520]; uint8_t *rgba; int w, h, tried; unsigned sig; } ImgCache;
static ImgCache imgc[4];
static ImgCache *get_image(const char *path) {
    if (!strcmp(path, "@padmap")) {                                       /* the picture of the pad is drawn by the program (padmap.c) and made again when what it shows changes */
        static ImgCache pm; unsigned sg = padmap_signature();
        if (!pm.tried || pm.sig != sg) { free(pm.rgba); pm.rgba = padmap_render(&pm.w, &pm.h); pm.sig = sg; pm.tried = 1; }
        return &pm;
    }
    for (int i = 0; i < 4; i++) if (imgc[i].tried && !strcmp(imgc[i].path, path)) return &imgc[i];
    for (int i = 0; i < 4; i++) if (!imgc[i].tried) { snprintf(imgc[i].path, sizeof imgc[i].path, "%s", path); imgc[i].tried = 1; imgc[i].rgba = png_read_rgba(path, &imgc[i].w, &imgc[i].h); return &imgc[i]; }
    return NULL;
}
/* The picture of the current language: "<name>_<code>.<ext>" (controller_es.png in Spanish) if it exists, else the given one (controller.png, the English one) */
static const char *localized_path(const char *path, char *out, size_t n) {
    const char *code = lang_current(); const char *dot = strrchr(path, '.');
    if (!path[0] || !dot || !strcmp(code, "en")) return path;
    snprintf(out, n, "%.*s_%s%s", (int)(dot - path), path, code, dot);
    return file_exists(out) ? out : path;
}
/* draws the picture centred in the box (x, y, w, h), scaled to fit, over the panel colour (10, 12, 24) */
static void blit_image(uint8_t *bits, int W, int H, const char *path, int x0, int y0, int bw, int bh) {
    ImgCache *c = get_image(path); if (!c || !c->rgba || bw < 8 || bh < 8) return;
    double sc = (double)bw / c->w; if ((double)bh / c->h < sc) sc = (double)bh / c->h; int dw = (int)(c->w * sc), dh = (int)(c->h * sc); if (dw < 1 || dh < 1) return;
    int ox = x0 + (bw - dw) / 2, oy = y0 + (bh - dh) / 2;
    for (int y = 0; y < dh; y++) {
        int yy = oy + y; if (yy < 0 || yy >= H) continue;
        double sy0 = y / sc, sy1 = (y + 1) / sc; int iy0 = (int)sy0, iy1 = (int)ceil(sy1); if (iy1 > c->h) iy1 = c->h;
        for (int x = 0; x < dw; x++) {
            int xx = ox + x; if (xx < 0 || xx >= W) continue;
            double sx0 = x / sc, sx1 = (x + 1) / sc; int ix0 = (int)sx0, ix1 = (int)ceil(sx1); if (ix1 > c->w) ix1 = c->w;
            double r = 0, g = 0, b = 0, a = 0, wsum = 0;                                         /* box average of the source pixels that fall in this one */
            for (int sy = iy0; sy < iy1; sy++) for (int sx = ix0; sx < ix1; sx++) { const uint8_t *q = c->rgba + ((size_t)sy * c->w + sx) * 4; double al = q[3] / 255.0; r += q[0] * al; g += q[1] * al; b += q[2] * al; a += al; wsum += 1; }
            if (wsum <= 0) continue; double al = a / wsum; if (al <= 0) continue;
            double R = r / a, G = g / a, B = b / a;
            uint8_t *d = bits + ((size_t)yy * W + xx) * 4;
            d[0] = (uint8_t)(B * al + 24 * (1 - al) + 0.5); d[1] = (uint8_t)(G * al + 12 * (1 - al) + 0.5); d[2] = (uint8_t)(R * al + 10 * (1 - al) + 0.5);
            if (d[0] == 0 && d[1] == 0 && d[2] == 0) d[0] = d[1] = d[2] = 1;                  /* the last pass makes pure black translucent and the panel colour (10, 12, 24) the panel's: the picture must not use them */
            else if (d[2] == 10 && d[1] == 12 && d[0] == 24) d[0] = 25;
        }
    }
}
static int first_sel(Menu *m) { int a, b; range_(m, &a, &b); for (int i = a; i < b; i++) if (NOSEL(m->it[i].kind) == 0) return i; return a; }
static int last_sel(Menu *m) { int a, b; range_(m, &a, &b); for (int i = b - 1; i >= a; i--) if (NOSEL(m->it[i].kind) == 0) return i; return a; }
static const MItem *tab_item(Menu *m, int t) { int k = -1; for (int i = 0; i < m->n; i++) if (m->it[i].kind == MK_TAB && ++k == t) return &m->it[i]; return NULL; }
static void set_tab(Menu *m, int t) { m->tab = (t % m->ntabs + m->ntabs) % m->ntabs; m->cur = first_sel(m); m->top = 0; }
void menu_show(Menu *m) {
    m->open = 1;
    if (m->ntabs) { m->tabfocus = 0; if (m->tab >= m->ntabs) m->tab = 0; int a, b; range_(m, &a, &b); if (m->cur < a || m->cur >= b || NOSEL(m->it[m->cur].kind)) { m->cur = first_sel(m); m->top = 0; } }     /* it reopens on the tab and option that were in use */
    else for (int i = 0; i < m->n; i++) { if (NOSEL(m->it[m->cur].kind) == 0) break; m->cur = (m->cur + 1) % m->n; }
}
void menu_hide(Menu *m) { m->open = 0; }
void menu_status(Menu *m, const char *t) { snprintf(m->status, sizeof m->status, "%s", t); m->status_t = now_s(); }
static void move_(Menu *m, int d) {
    int a, b; range_(m, &a, &b); int len = b - a; if (len <= 0) return;
    for (int i = 0; i < len; i++) { m->cur = a + ((m->cur - a + d) % len + len) % len; if (NOSEL(m->it[m->cur].kind) == 0) break; }
}

void menu_last_valtext(Menu *m, const char *(*v)(void)) { if (m->n > 0) m->it[m->n - 1].valtext = v; }
void menu_last_enabled(Menu *m, int (*e)(void)) { if (m->n > 0) m->it[m->n - 1].enabled = e; }
void menu_last_valhint(Menu *m, const char *(*v)(int i)) { if (m->n > 0) m->it[m->n - 1].valhint = v; }
void menu_nav(Menu *m, const char *dir) {
    if (m->ntabs) {
        if (!strcmp(dir, "tabprev")) { set_tab(m, m->tab - 1); return; }
        if (!strcmp(dir, "tabnext")) { set_tab(m, m->tab + 1); return; }
        if (m->tabfocus) {                                                 /* the tab bar is selected: left / right change tab, down goes into it */
            if (!strcmp(dir, "left")) set_tab(m, m->tab - 1); else if (!strcmp(dir, "right")) set_tab(m, m->tab + 1);
            else if (!strcmp(dir, "down")) { m->tabfocus = 0; m->cur = first_sel(m); } else if (!strcmp(dir, "up")) { m->tabfocus = 0; m->cur = last_sel(m); }
            return;
        }
        if (!strcmp(dir, "up") && m->cur == first_sel(m)) { m->tabfocus = 1; return; }                   /* up from the first option: the tab bar */
    }
    MItem *it = &m->it[m->cur];
    if (!strcmp(dir, "up")) { move_(m, -1); return; }
    if (!strcmp(dir, "down")) { move_(m, 1); return; }
    int step = !strcmp(dir, "left") ? -1 : 1;
    if (it->enabled && !it->enabled()) return;
    if (it->kind == MK_BOOL) it->setb(!it->getb());
    else if (it->kind == MK_CHOICE) { int n = it->nch(), c = it->getch(); if (n > 0) it->setch(((c + step) % n + n) % n); }
    else if (it->kind == MK_SLIDER) { double v = it->getd() + step * (it->step > 0 ? it->step : 0.1); if (v < it->lo) v = it->lo; if (v > it->hi) v = it->hi; it->setd(floor(v * 10000 + 0.5) / 10000); }
}
void menu_activate(Menu *m) {
    if (m->ntabs && m->tabfocus) { m->tabfocus = 0; m->cur = first_sel(m); return; }
    MItem *it = &m->it[m->cur];
    if (it->enabled && !it->enabled()) return;
    if (it->kind == MK_ACTION) it->act();
    else if (it->kind == MK_BOOL || it->kind == MK_CHOICE || it->kind == MK_SLIDER) menu_nav(m, "right");
}

static void value_text(const MItem *it, char *out, size_t n) {
    out[0] = 0;
    if (it->kind == MK_BOOL) snprintf(out, n, "%s", tr(it->getb() ? "val.on" : "val.off"));
    else if (it->kind == MK_CHOICE) snprintf(out, n, "%s", it->nch() > 0 ? tr(it->chtext(it->getch())) : "");
    else if (it->kind == MK_ACTION && it->valtext) snprintf(out, n, "%s", tr(it->valtext()));
    else if (it->kind == MK_SLIDER) { double v = it->getd(); if (it->fmt) snprintf(out, n, it->fmt, v); else snprintf(out, n, "%d %%", (int)lrint(v * 100)); }
}

int menu_signature(Menu *m, int w, int h, char *out, size_t n) {
    size_t k = 0; k += (size_t)snprintf(out + k, n - k, "%d|%d|%d|%d|%d|", m->cur, w, h, m->tab, m->tabfocus);
    if (now_s() - m->status_t < 5.0) k += (size_t)snprintf(out + k, n - k, "%s|", m->status);
    k += (size_t)snprintf(out + k, n - k, "%u|", padmap_signature());                       /* the picture of the pad depends on the controls and the language */
    for (int i = 0; i < m->n && k + 100 < n; i++) if (NOSEL(m->it[i].kind) == 0) { char v[160]; value_text(&m->it[i], v, sizeof v); k += (size_t)snprintf(out + k, n - k, "%s;", v); }
    return (int)k;
}

#ifdef _WIN32
static HFONT mkfont(int h, int bold) { return CreateFontA(-h, 0, 0, 0, bold ? FW_BOLD : FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Segoe UI"); }
static int text_w(HDC dc, const char *s) { SIZE sz; GetTextExtentPoint32A(dc, s, (int)strlen(s), &sz); return sz.cx; }
static void text_at(HDC dc, int x, int y, const char *s, COLORREF c) { SetTextColor(dc, c); TextOutA(dc, x, y, s, (int)strlen(s)); }

/* wraps `text` into lines of at most `width` pixels */
static int wrap(HDC dc, const char *text, int width, char lines[][200], int maxl) {
    char buf[1024]; snprintf(buf, sizeof buf, "%s", text); int nl = 0; char cur[200] = ""; char *save = NULL;
    for (char *w = strtok_s(buf, " ", &save); w; w = strtok_s(NULL, " ", &save)) {
        char t[260]; snprintf(t, sizeof t, "%s%s%s", cur, cur[0] ? " " : "", w);
        if (text_w(dc, t) <= width || !cur[0]) snprintf(cur, sizeof cur, "%s", t);
        else { if (nl < maxl) snprintf(lines[nl++], 200, "%s", cur); snprintf(cur, sizeof cur, "%s", w); }
    }
    if (cur[0] && nl < maxl) snprintf(lines[nl++], 200, "%s", cur);
    return nl;
}

uint8_t *menu_draw(Menu *m, int W, int H) {
    int fs = H / 26; if (fs < 14) fs = 14;
    int fh = (int)((fs + 6) * 0.80), fhb = (int)((fs + 6) * 1.35 * 0.80), fhs = (int)((fs - 2) * 0.80 + 0.5); if (fhs < 10) fhs = 10;
    HDC dc = CreateCompatibleDC(NULL); BITMAPINFO bi; memset(&bi, 0, sizeof bi); bi.bmiHeader.biSize = sizeof bi.bmiHeader; bi.bmiHeader.biWidth = W; bi.bmiHeader.biHeight = -H; bi.bmiHeader.biPlanes = 1; bi.bmiHeader.biBitCount = 32; bi.bmiHeader.biCompression = BI_RGB;
    uint8_t *bits = NULL; HBITMAP bmp = CreateDIBSection(dc, &bi, DIB_RGB_COLORS, (void **)&bits, NULL, 0); HGDIOBJ oldb = SelectObject(dc, bmp);
    memset(bits, 0, (size_t)W * H * 4);
    HFONT f = mkfont(fh, 0), fb = mkfont(fhb, 1), fsm = mkfont(fhs, 0); SetBkMode(dc, TRANSPARENT);
    SelectObject(dc, f); TEXTMETRICA tm; GetTextMetricsA(dc, &tm); int lh = tm.tmHeight + 5;
    SelectObject(dc, fb); GetTextMetricsA(dc, &tm); int hb = tm.tmHeight; SelectObject(dc, f);
    int pw = (int)(W * 0.72), hint_lines = 4, has_img = 0, hint_h = 0, hpct = 82;          /* hint_h: height of the explanation box; hpct: its font, in % of the options' */
    { int a0, b0; range_(m, &a0, &b0); for (int i = a0; i < b0; i++) if (m->it[i].kind == MK_IMAGE) has_img = 1;
      if (has_img) {                                                       /* a tab with a picture: the explanation box is only as tall as the longest explanation of the tab needs, and the picture gets the rest */
          hpct = 68; HFONT fA = mkfont(fh * hpct / 100, 0); HGDIOBJ o = SelectObject(dc, fA); TEXTMETRICA t2; GetTextMetricsA(dc, &t2); int lhA2 = t2.tmHeight + 4, need = 1; static char tl[12][200];
          for (int i = a0; i <= b0; i++) { const char *hid = i < b0 ? m->it[i].hint : tab_item(m, m->tab)->hint; if (!hid) continue; int k = wrap(dc, tr(hid), pw - 44, tl, 12); if (k > need) need = k; }
          SelectObject(dc, o); DeleteObject(fA); hint_h = need * lhA2 + 12; if (hint_h > 4 * lh) hint_h = 4 * lh; } }
    if (!has_img) hint_h = hint_lines * lh;
    int ph_max = (int)(H * 0.94), tabh = m->ntabs ? lh + 10 : 0;                /* the explanation box is 4 lines tall; if the text does not fit the font is reduced */
    int foot_h = fhs * 2 + 16;                                       /* footer strip with the navigation instructions */
    int rows = (ph_max - hb - 24 - tabh - (hint_h + foot_h + 8)) / lh; if (rows < 5) rows = 5;
    int lo, n; range_(m, &lo, &n); if (m->top < lo || m->top >= n) m->top = lo;
    if (!m->tabfocus) { if (m->cur < m->top) m->top = m->cur; if (m->cur >= m->top + rows) m->top = m->cur - rows + 1; }
    if (m->top > lo && NOSEL(m->it[m->top - 1].kind) && m->cur - m->top < rows - 1) m->top--;
    int ph_nat = hb + 18 + tabh + rows * lh + hint_h + foot_h + 14, ph = (has_img && ph_nat < ph_max) ? ph_max : ph_nat, px = (W - pw) / 2, py = (H - ph) / 2;          /* with a picture the panel takes all the height and the picture the extra */
    int by0 = py + ph - hint_h - foot_h - 6;                              /* top of the explanation box (its separator line is 4 pixels above) */
    HBRUSH br = CreateSolidBrush(RGB(10, 12, 24)); HPEN pen = CreatePen(PS_SOLID, 2, RGB(230, 190, 70)); SelectObject(dc, br); SelectObject(dc, pen);
    RoundRect(dc, px, py, px + pw, py + ph, 24, 24);
    SelectObject(dc, fb); { int tw = text_w(dc, tr(m->title)); text_at(dc, px + (pw - tw) / 2, py + 8, tr(m->title), RGB(255, 215, 90)); }
    SelectObject(dc, f);
    if (m->ntabs) {                                                       /* tab bar */
        int tw[16], tot = 0, gap = 14, nt = m->ntabs < 16 ? m->ntabs : 16; SelectObject(dc, f);
        for (int t = 0; t < nt; t++) { tw[t] = text_w(dc, tr(tab_item(m, t)->label)) + 26; tot += tw[t] + (t ? gap : 0); }
        int x = px + (pw - tot) / 2, ty = py + 10 + hb;
        for (int t = 0; t < nt; t++) {
            int cur_t = t == m->tab;
            if (cur_t) { HBRUSH sb = CreateSolidBrush(m->tabfocus ? RGB(90, 105, 190) : RGB(55, 62, 118)); HPEN np = (HPEN)GetStockObject(NULL_PEN); HGDIOBJ op = SelectObject(dc, np), ob = SelectObject(dc, sb); RoundRect(dc, x, ty, x + tw[t], ty + lh + 2, 12, 12); SelectObject(dc, ob); SelectObject(dc, op); DeleteObject(sb); }
            text_at(dc, x + 13, ty + 2, tr(tab_item(m, t)->label), cur_t ? (m->tabfocus ? RGB(255, 255, 255) : RGB(255, 225, 120)) : RGB(150, 158, 190));
            x += tw[t] + gap;
        }
        HPEN lp = CreatePen(PS_SOLID, 1, RGB(90, 100, 150)); HGDIOBJ op = SelectObject(dc, lp); MoveToEx(dc, px + 16, ty + lh + 6, NULL); LineTo(dc, px + pw - 16, ty + lh + 6); SelectObject(dc, op); DeleteObject(lp);
    }
    int y = py + 14 + hb + tabh;
    for (int i = m->top; i < n && i < m->top + rows; i++) {
        MItem *it = &m->it[i]; int sel = i == m->cur && !m->tabfocus; char label[128];
        tr_label(it->dynlabel ? it->dynlabel() : it->label, label, sizeof label);
        if (it->kind == MK_IMAGE) { { char lp[560]; blit_image(bits, W, H, localized_path(it->path ? it->path : "", lp, sizeof lp), px + 20, y, pw - 40, by0 - 12 - y); break; } }
        if (it->kind == MK_SEP && !label[0]) { y += lh; continue; }
        if (it->kind == MK_SEP) {
            if (it->plain) text_at(dc, px + 30, y, label, RGB(215, 215, 230));
            else { char t[160]; snprintf(t, sizeof t, "-- %s --", label); text_at(dc, px + (pw - text_w(dc, t)) / 2, y, t, RGB(120, 190, 255)); }
            y += lh; continue;
        }
        if (sel) { HBRUSH sb = CreateSolidBrush(RGB(60, 70, 130)); HPEN np = (HPEN)GetStockObject(NULL_PEN); HGDIOBJ op = SelectObject(dc, np), ob = SelectObject(dc, sb); RoundRect(dc, px + 10, y - 2, px + pw - 10, y - 2 + lh, 12, 12); SelectObject(dc, ob); SelectObject(dc, op); DeleteObject(sb); }
        int off = it->enabled && !it->enabled();
        text_at(dc, px + 30, y, label, off ? RGB(105, 108, 125) : sel ? RGB(255, 255, 255) : RGB(205, 205, 215));
        char vt[200]; value_text(it, vt, sizeof vt);
        if (vt[0]) { char t[240]; if (sel && it->kind != MK_ACTION) snprintf(t, sizeof t, "<  %s  >", vt); else snprintf(t, sizeof t, "%s", vt); text_at(dc, px + pw - 30 - text_w(dc, t), y, t, off ? RGB(105, 108, 125) : sel ? RGB(255, 230, 120) : RGB(220, 200, 120)); }
        y += lh;
    }
    if (m->top > lo) text_at(dc, px + pw - 18, py + 10, "^", RGB(200, 200, 200));
    if (m->top + rows < n) text_at(dc, px + pw - 18, py + ph - hint_h - foot_h - 26, "v", RGB(200, 200, 200));
    int by = by0;
    { HPEN lp = CreatePen(PS_SOLID, 1, RGB(90, 100, 150)); HGDIOBJ op = SelectObject(dc, lp); MoveToEx(dc, px + 16, by - 4, NULL); LineTo(dc, px + pw - 16, by - 4); SelectObject(dc, op); DeleteObject(lp); }
    int k0 = 0;
    if (m->status[0] && now_s() - m->status_t < 5.0) { text_at(dc, px + 22, by, m->status, RGB(255, 225, 90)); k0 = 1; }
    static char lines[12][200], lines2[12][200]; const char *htxt = (m->ntabs && m->tabfocus) ? (tab_item(m, m->tab)->hint ? tr(tab_item(m, m->tab)->hint) : "") : (m->it[m->cur].hint ? tr(m->it[m->cur].hint) : "");
    const char *vtxt = NULL; { const MItem *ci = &m->it[m->cur]; if (!(m->ntabs && m->tabfocus) && ci->kind == MK_CHOICE && ci->valhint && ci->nch && ci->nch() > 0) vtxt = tr(ci->valhint(ci->getch())); if (vtxt && !vtxt[0]) vtxt = NULL; }
    int nl = 0, nl2 = 0, gap = 0, lhA = lh, lhB = lh; HFONT fh2 = NULL, fhA = mkfont(fh * hpct / 100, 0);       /* the general text always has the same font (smaller than the options); only the text of the chosen value shrinks to fit */
    SelectObject(dc, fhA); GetTextMetricsA(dc, &tm); lhA = tm.tmHeight + 4; nl = wrap(dc, htxt, pw - 44, lines, 12);
    if (!vtxt) lhB = lhA;
    for (int pct = hpct; vtxt && pct >= 40; pct -= 4) {                       /* the largest font with which the text of the value fits in what is left of the box */
        if (fh2) DeleteObject(fh2);
        fh2 = mkfont(fh * pct / 100, 0); SelectObject(dc, fh2); GetTextMetricsA(dc, &tm); lhB = tm.tmHeight + 5 * pct / 100; gap = lhB / 2 + 2;
        nl2 = wrap(dc, vtxt, pw - 44, lines2, 12);
        if (k0 * lh + nl * lhA + gap + nl2 * lhB <= hint_h) break;
    }
    SelectObject(dc, fhA); for (int k = 0; k < nl; k++) text_at(dc, px + 22, by + k0 * lh + k * lhA, lines[k], RGB(190, 200, 235));
    if (vtxt) {                                                              /* line, and the explanation of the chosen value */
        int ly = by + k0 * lh + nl * lhA + gap / 2; HPEN lp = CreatePen(PS_SOLID, 1, RGB(90, 100, 150)); HGDIOBJ op = SelectObject(dc, lp); MoveToEx(dc, px + 22, ly, NULL); LineTo(dc, px + pw - 22, ly); SelectObject(dc, op); DeleteObject(lp);
        SelectObject(dc, fh2); for (int k = 0; k < nl2; k++) text_at(dc, px + 22, by + k0 * lh + nl * lhA + gap + k * lhB, lines2[k], RGB(255, 225, 140));
    }
    SelectObject(dc, f); if (fh2) DeleteObject(fh2); DeleteObject(fhA);
    { const char *ft = tr(m->ntabs ? "footer.tabs" : "footer.plain");
      HFONT ff = NULL; int fhh = fhs; for (;;) { if (ff) DeleteObject(ff); ff = mkfont(fhh, 0); SelectObject(dc, ff); if (text_w(dc, ft) <= pw - 30 || fhh <= 8) break; fhh--; }     /* the footer font shrinks until the text fits in the panel */
      text_at(dc, px + (pw - text_w(dc, ft)) / 2, py + ph - fhh - 30, ft, RGB(130, 140, 170)); DeleteObject(ff); }
    GdiFlush();
    uint8_t *rgba = (uint8_t *)malloc((size_t)W * H * 4);
    for (int yy = 0; yy < H; yy++) for (int xx = 0; xx < W; xx++) {
        const uint8_t *s = bits + ((size_t)yy * W + xx) * 4; uint8_t *d = rgba + ((size_t)yy * W + xx) * 4; uint8_t b = s[0], g = s[1], r = s[2];
        int inside = xx >= px && xx < px + pw && yy >= py && yy < py + ph;
        uint8_t a = (r == 0 && g == 0 && b == 0) ? 120 : (r == 10 && g == 12 && b == 24 && inside) ? 238 : 255;
        d[0] = r; d[1] = g; d[2] = b; d[3] = a;
    }
    SelectObject(dc, oldb); DeleteObject(bmp); DeleteObject(f); DeleteObject(fb); DeleteObject(fsm); DeleteObject(br); DeleteObject(pen); DeleteDC(dc);
    return rgba;
}
#endif
