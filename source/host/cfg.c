/*
 * cfg.c - Options in JSON (options.json next to the program): minimal reader (flat objects and the nested "params" object) and writer.
 */
#include "cfg.h"
#include <ctype.h>

typedef struct { char key[48]; int type; double num; char str[200]; } Ent;   /* type: 0 num, 1 bool, 2 string, 3 null */
static Ent ents[96]; static int nents = 0;

static const char *skip(const char *p) { while (*p && isspace((unsigned char)*p)) p++; return p; }
static const char *parse_str(const char *p, char *out, size_t n) {
    size_t k = 0; if (*p != '"') return NULL; p++;
    while (*p && *p != '"') { if (*p == 0x5C && p[1]) { p++; if (*p == 'n') { if (k + 1 < n) out[k++] = 10; } else if (*p == 'u' && p[1] && p[2] && p[3] && p[4]) { p += 4; if (k + 1 < n) out[k++] = '?'; } else if (k + 1 < n) out[k++] = *p; p++; } else { if (k + 1 < n) out[k++] = *p; p++; } }
    out[k] = 0; return *p == '"' ? p + 1 : NULL;
}
static const char *parse_obj(const char *p, const char *prefix) {
    p = skip(p); if (*p != '{') return NULL; p++;
    for (;;) {
        p = skip(p); if (*p == '}') return p + 1; if (*p == ',') { p++; continue; }
        char key[48]; p = parse_str(p, key, sizeof key); if (!p) return NULL; p = skip(p); if (*p != ':') return NULL; p = skip(p + 1);
        char full[96]; snprintf(full, sizeof full, "%s%s", prefix, key);
        if (*p == '{') { char np[96]; snprintf(np, sizeof np, "%s.", full); p = parse_obj(p, np); if (!p) return NULL; continue; }
        Ent *e = nents < 96 ? &ents[nents++] : NULL; if (!e) return NULL; memset(e, 0, sizeof *e); snprintf(e->key, sizeof e->key, "%s", full);
        if (*p == '"') { e->type = 2; p = parse_str(p, e->str, sizeof e->str); if (!p) return NULL; }
        else if (!strncmp(p, "true", 4)) { e->type = 1; e->num = 1; p += 4; }
        else if (!strncmp(p, "false", 5)) { e->type = 1; e->num = 0; p += 5; }
        else if (!strncmp(p, "null", 4)) { e->type = 3; p += 4; }
        else { char *end; e->num = strtod(p, &end); if (end == p) return NULL; p = end; }
    }
}
int cfg_load(const char *path) {
    nents = 0; size_t n; uint8_t *d = read_file(path, &n); if (!d) return 0;
    const char *p = (const char *)d; if (n >= 3 && d[0] == 0xEF && d[1] == 0xBB && d[2] == 0xBF) p += 3;
    int ok = parse_obj(p, "") != NULL; free(d); return ok;
}
static Ent *find(const char *key) { for (int i = 0; i < nents; i++) if (!strcmp(ents[i].key, key)) return &ents[i]; return NULL; }
int cfg_has(const char *key) { Ent *e = find(key); return e && e->type != 3; }
double cfg_num(const char *key, double def) { Ent *e = find(key); return e && (e->type == 0 || e->type == 1) ? e->num : def; }
int cfg_bool(const char *key, int def) { Ent *e = find(key); return e && (e->type == 0 || e->type == 1) ? e->num != 0 : def; }
const char *cfg_str(const char *key, const char *def) { Ent *e = find(key); return e && e->type == 2 ? e->str : def; }
void cfg_set_num(const char *key, double v) { Ent *e = find(key); if (!e && nents < 96) { e = &ents[nents++]; memset(e, 0, sizeof *e); snprintf(e->key, sizeof e->key, "%s", key); } if (e) { e->type = 0; e->num = v; } }
void cfg_set_bool(const char *key, int v) { cfg_set_num(key, v); Ent *e = find(key); if (e) e->type = 1; }
void cfg_set_str(const char *key, const char *v) { Ent *e = find(key); if (!e && nents < 96) { e = &ents[nents++]; memset(e, 0, sizeof *e); snprintf(e->key, sizeof e->key, "%s", key); } if (e) { e->type = 2; snprintf(e->str, sizeof e->str, "%s", v); } }
void cfg_clear(void) { nents = 0; }
void cfg_remove(const char *key) { for (int i = 0; i < nents; i++) if (!strcmp(ents[i].key, key)) { ents[i] = ents[--nents]; return; } }

static void put_str(FILE *f, const char *s) { fputc('"', f); for (; *s; s++) { if (*s == '"' || *s == 0x5C) fputc(0x5C, f); fputc(*s, f); } fputc('"', f); }
int cfg_save(const char *path) {
    char tmp[700]; snprintf(tmp, sizeof tmp, "%s.tmp", path); FILE *f = fopen(tmp, "wb"); if (!f) return 0;
    fprintf(f, "{\n"); int first = 1;
    for (int pass = 0; pass < 2; pass++) {                                       /* first the flat keys, then "params" */
        if (pass == 1) { int any = 0; for (int i = 0; i < nents; i++) if (!strncmp(ents[i].key, "params.", 7)) any = 1; if (!any) break; fprintf(f, "%s \"params\": {\n", first ? "" : ",\n"); first = 0; int fp = 1;
            for (int i = 0; i < nents; i++) if (!strncmp(ents[i].key, "params.", 7)) { fprintf(f, "%s  \"%s\": %g", fp ? "" : ",\n", ents[i].key + 7, ents[i].num); fp = 0; } fprintf(f, "\n }"); continue; }
        for (int i = 0; i < nents; i++) {
            Ent *e = &ents[i]; if (!strncmp(e->key, "params.", 7)) continue;
            fprintf(f, "%s \"%s\": ", first ? "" : ",\n", e->key); first = 0;
            if (e->type == 1) fputs(e->num != 0 ? "true" : "false", f); else if (e->type == 2) put_str(f, e->str); else if (e->type == 3) fputs("null", f); else fprintf(f, "%g", e->num);
        }
    }
    fprintf(f, "\n}\n"); fclose(f);
#ifdef _WIN32
    extern int MoveFileExA(const char *, const char *, unsigned long); return MoveFileExA(tmp, path, 1) != 0;
#else
    return rename(tmp, path) == 0;
#endif
}
