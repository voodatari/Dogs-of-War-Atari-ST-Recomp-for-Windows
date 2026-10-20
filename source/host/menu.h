/*
 * menu.h - On-screen options menu: drawn with GDI into an RGBA image that is overlaid on the shader output.
 */
#ifndef MENU_H
#define MENU_H
#include "host.h"

/* MK_IMAGE: a picture (its label is the path of the PNG) that fills the rest of the list, not selectable; MK_TAB:
 * starts a tab (its label is the name of the tab, hint = text shown while the tab bar is selected); the items that
 * follow belong to it */
enum { MK_SEP, MK_BOOL, MK_CHOICE, MK_SLIDER, MK_ACTION, MK_TAB, MK_IMAGE };
typedef struct MItem {
    /* path: the PNG of an MK_IMAGE item (it does not fit in label: paths can be long) */
    int kind;
    char label[96];
    const char *hint;
    int plain;
    const char *path;
    /* bool */
    int (*getb)(void);
    void (*setb)(int);
    /* choice */
    int (*nch)(void);
    const char *(*chtext)(int i);
    int (*getch)(void);
    void (*setch)(int i);
    /* slider */
    double lo, hi, step;
    const char *fmt;
    double (*getd)(void);
    void (*setd)(double);
    void (*act)(void); /* action */
    /* etiqueta dinamica (opcional) */
    const char *(*dynlabel)(void);
    /* text on the right of an action (optional) */
    const char *(*valtext)(void);
    /* NULL = always enabled; 0 = greyed out and unchangeable */
    int (*enabled)(void);
    /* choice: explanation of the value i, shown below the general one, separated by a line (optional) */
    const char *(*valhint)(int i);
} MItem;

typedef struct Menu Menu;
Menu *menu_new(const char *title);
void menu_add(Menu *m, const MItem *it);
/* value text of the last added item */
void menu_last_valtext(Menu *m, const char *(*v)(void));
void menu_last_enabled(Menu *m, int (*e)(void));
void menu_last_valhint(Menu *m, const char *(*v)(int i));
/* explanation of each value of the last added choice */ /* enabled condition of the last added item */
int menu_is_open(Menu *m);
void menu_show(Menu *m);
void menu_hide(Menu *m);
/* "up" "down" "left" "right", "tabprev" "tabnext" */
void menu_nav(Menu *m, const char *dir);
void menu_activate(Menu *m);
void menu_status(Menu *m, const char *text);
/* string that changes when the menu must be repainted */
int menu_signature(Menu *m, int w, int h, char *out, size_t n);
uint8_t *menu_draw(Menu *m, int w, int h); /* RGBA (malloc) w*h */
#endif
