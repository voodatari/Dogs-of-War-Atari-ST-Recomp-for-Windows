/*
 * lang.h - Language of the menus: the texts are read from data/languages/<language>.xml.
 *
 * Every text of the menus has an id (for example "menu.fullscreen" and "menu.fullscreen.hint"); the XML files have one <string id="...">text</string> per id.
 * tr(id) returns the text in the current language, or in English if that file lacks it, or the id itself (so a literal that is not an id passes through unchanged).
 */
#ifndef LANG_H
#define LANG_H
#include "host.h"

void        lang_init(const char *dir);                 /* scans dir for *.xml and loads the English one (the fallback); the current language stays English until lang_set */
int         lang_count(void);                           /* languages found */
const char *lang_name(int i);                           /* name to show ("English", "Español") */
const char *lang_code(int i);                           /* code ("en", "es") */
int         lang_set(const char *code);                 /* 1 if that language exists (and is now the current one) */
const char *lang_current(void);                         /* code of the current language */
int         lang_index(void);                           /* its index */
const char *tr(const char *id);                         /* the text of id; never NULL; valid for the life of the program */
void        trf(char *out, size_t n, const char *id, ...);   /* printf with the translated text as the format */
#endif
