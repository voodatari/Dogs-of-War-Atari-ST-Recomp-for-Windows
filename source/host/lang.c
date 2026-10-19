/*
 * lang.c - Language files (XML) of the menus. See lang.h.
 *
 * Format:
 *   <?xml version="1.0" encoding="UTF-8"?>
 *   <language name="English" code="en">
 *     <string id="menu.fullscreen">Fullscreen (F11)</string>
 *     ...
 *   </language>
 * Entities &amp; &lt; &gt; &quot; &apos; and &#N; (N = a number) are understood; the text is trimmed; comments <!-- --> are skipped.
 * The files are UTF-8; the menus are drawn with the ANSI code page, so characters outside it (and only those) are replaced by '?'.
 */
#include "lang.h"
#include <stdarg.h>
#ifdef _WIN32
#include <windows.h>
#endif

#define MAXLANG 16
typedef struct Entry { char *id, *text; } Entry;
typedef struct Table { char name[64], code[16]; Entry *e; int n; } Table;
static Table tabs[MAXLANG]; static char paths[MAXLANG][700]; static int ntabs = 0, loaded[MAXLANG];
static int cur = 0, fallback = -1;

static int cmp_entry(const void *a, const void *b) { return strcmp(((const Entry *)a)->id, ((const Entry *)b)->id); }

/* UTF-8 -> ANSI (code page of the system) */
static char *utf8_to_ansi(const char *s) {
#ifdef _WIN32
    int wn = MultiByteToWideChar(CP_UTF8, 0, s, -1, NULL, 0); if (wn <= 0) return _strdup(s);
    wchar_t *w = (wchar_t *)malloc((size_t)wn * sizeof(wchar_t)); MultiByteToWideChar(CP_UTF8, 0, s, -1, w, wn);
    int an = WideCharToMultiByte(CP_ACP, 0, w, -1, NULL, 0, "?", NULL); char *a = (char *)malloc((size_t)an + 1);
    WideCharToMultiByte(CP_ACP, 0, w, -1, a, an, "?", NULL); free(w); return a;
#else
    return strdup(s);
#endif
}
static void decode_entities(char *s) {
    char *o = s;
    for (char *p = s; *p; ) {
        if (*p != '&') { *o++ = *p++; continue; }
        if (!strncmp(p, "&amp;", 5)) { *o++ = '&'; p += 5; } else if (!strncmp(p, "&lt;", 4)) { *o++ = '<'; p += 4; } else if (!strncmp(p, "&gt;", 4)) { *o++ = '>'; p += 4; }
        else if (!strncmp(p, "&quot;", 6)) { *o++ = '"'; p += 6; } else if (!strncmp(p, "&apos;", 6)) { *o++ = '\''; p += 6; }
        else if (p[1] == '#') { int v = atoi(p + 2); char *e = strchr(p, ';'); if (e && v > 0 && v < 128) { *o++ = (char)v; p = e + 1; } else *o++ = *p++; }
        else *o++ = *p++;
    }
    *o = 0;
}
static char *attr(const char *tag, const char *name, char *out, size_t n) {            /* value of name="..." inside a tag */
    char key[40]; snprintf(key, sizeof key, "%s=\"", name); const char *p = strstr(tag, key); out[0] = 0; if (!p) return out;
    p += strlen(key); const char *e = strchr(p, '"'); if (!e) return out; size_t k = (size_t)(e - p); if (k >= n) k = n - 1; memcpy(out, p, k); out[k] = 0; return out;
}
static int load_file(const char *path, Table *t) {
    size_t n = 0; uint8_t *d = read_file(path, &n); if (!d) return 0;
    char *s = (char *)malloc(n + 1); memcpy(s, d, n); s[n] = 0; free(d);
    char *p = s; if (!strncmp(p, "\xEF\xBB\xBF", 3)) p += 3;
    int cap = 256; t->e = (Entry *)malloc(sizeof(Entry) * (size_t)cap); t->n = 0; t->name[0] = t->code[0] = 0;
    while ((p = strchr(p, '<')) != NULL) {
        if (!strncmp(p, "<!--", 4)) { char *e = strstr(p, "-->"); if (!e) break; p = e + 3; continue; }
        if (!strncmp(p, "<?", 2)) { char *e = strstr(p, "?>"); if (!e) break; p = e + 2; continue; }
        if (!strncmp(p, "<language", 9)) {
            char *e = strchr(p, '>'); if (!e) break; char tag[400]; size_t k = (size_t)(e - p); if (k >= sizeof tag) k = sizeof tag - 1; memcpy(tag, p, k); tag[k] = 0;
            char nm[200]; attr(tag, "name", nm, sizeof nm); decode_entities(nm); char *a = utf8_to_ansi(nm); snprintf(t->name, sizeof t->name, "%s", a); free(a); attr(tag, "code", t->code, sizeof t->code); p = e + 1; continue;
        }
        if (!strncmp(p, "<string", 7) && (p[7] == ' ' || p[7] == '\t' || p[7] == '\n' || p[7] == '\r')) {
            char *e = strchr(p, '>'); if (!e) break; char tag[400]; size_t k = (size_t)(e - p); if (k >= sizeof tag) k = sizeof tag - 1; memcpy(tag, p, k); tag[k] = 0;
            char id[160]; attr(tag, "id", id, sizeof id); char *body = e + 1; char *end = strstr(body, "</string>"); if (!end) break;
            if (e[-1] != '/' && id[0]) {
                size_t bl = (size_t)(end - body); char *txt = (char *)malloc(bl + 1); memcpy(txt, body, bl); txt[bl] = 0;
                char *a = txt; while (*a == ' ' || *a == '\t' || *a == '\r' || *a == '\n') a++; char *z = a + strlen(a); while (z > a && (z[-1] == ' ' || z[-1] == '\t' || z[-1] == '\r' || z[-1] == '\n')) z--; *z = 0;
                decode_entities(a); char *ansi = utf8_to_ansi(a); free(txt);
                if (t->n == cap) { cap *= 2; t->e = (Entry *)realloc(t->e, sizeof(Entry) * (size_t)cap); }
                t->e[t->n].id = _strdup(id); t->e[t->n].text = ansi; t->n++;
            }
            p = end + 9; continue;
        }
        p++;
    }
    free(s); qsort(t->e, (size_t)t->n, sizeof(Entry), cmp_entry);
    return t->code[0] != 0;
}
static int ensure(int i) { if (loaded[i]) return 1; if (load_file(paths[i], &tabs[i])) { loaded[i] = 1; return 1; } return 0; }

void lang_init(const char *dir) {
    ntabs = 0; cur = 0; fallback = -1;
#ifdef _WIN32
    char pat[760]; snprintf(pat, sizeof pat, "%s/*.xml", dir); WIN32_FIND_DATAA fd; HANDLE h = FindFirstFileA(pat, &fd);
    if (h != INVALID_HANDLE_VALUE) {
        do {
            if (ntabs >= MAXLANG) break; snprintf(paths[ntabs], sizeof paths[ntabs], "%s/%s", dir, fd.cFileName);
            memset(&tabs[ntabs], 0, sizeof(Table)); loaded[ntabs] = 0;
            if (load_file(paths[ntabs], &tabs[ntabs])) { loaded[ntabs] = 1; ntabs++; }
        } while (FindNextFileA(h, &fd));
        FindClose(h);
    }
#endif
    for (int i = 0; i < ntabs; i++) if (!_stricmp(tabs[i].code, "en")) fallback = i;
    if (fallback < 0 && ntabs > 0) fallback = 0;
    if (fallback >= 0) cur = fallback;
    if (fallback < 0) host_log("Languages: no language file found in %s (the menus will show their text ids)", dir);
}
int lang_count(void) { return ntabs; }
const char *lang_name(int i) { return i >= 0 && i < ntabs ? tabs[i].name : ""; }
const char *lang_code(int i) { return i >= 0 && i < ntabs ? tabs[i].code : ""; }
int lang_set(const char *code) { for (int i = 0; i < ntabs; i++) if (!_stricmp(tabs[i].code, code)) { cur = i; return 1; } return 0; }
const char *lang_current(void) { return cur >= 0 && cur < ntabs ? tabs[cur].code : "en"; }
int lang_index(void) { return cur; }
static const char *find(int t, const char *id) {
    if (t < 0 || t >= ntabs) return NULL;
    int lo = 0, hi = tabs[t].n - 1; while (lo <= hi) { int m = (lo + hi) / 2; int c = strcmp(id, tabs[t].e[m].id); if (!c) return tabs[t].e[m].text; if (c < 0) hi = m - 1; else lo = m + 1; }
    return NULL;
}
const char *tr(const char *id) {
    if (!id) return "";
    const char *r = find(cur, id); if (!r) r = find(fallback, id); return r ? r : id;
}
void trf(char *out, size_t n, const char *id, ...) { va_list ap; va_start(ap, id); vsnprintf(out, n, tr(id), ap); va_end(ap); }
