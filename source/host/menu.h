/*
 * menu.h - On-screen options menu: drawn with GDI into an RGBA image that is overlaid on the shader output.
 */
#ifndef MENU_H
#define MENU_H
#include "host.h"

enum { MK_SEP, MK_BOOL, MK_CHOICE, MK_SLIDER, MK_ACTION, MK_TAB, MK_IMAGE };      /* MK_IMAGE: a picture (its label is the path of the PNG) that fills the rest of the list, not selectable; MK_TAB: starts a tab (its label is the name of the tab, hint = text shown while the tab bar is selected); the items that follow belong to it */
typedef struct MItem {
    int kind; char label[96]; const char *hint; int plain;
    int (*getb)(void); void (*setb)(int);                               /* bool */
    int (*nch)(void); const char *(*chtext)(int i); int (*getch)(void); void (*setch)(int i);        /* choice */
    double lo, hi, step; const char *fmt; double (*getd)(void); void (*setd)(double);                /* slider */
    void (*act)(void);                                                  /* action */
    const char *(*dynlabel)(void);                                      /* etiqueta dinamica (opcional) */
    const char *(*valtext)(void);                                       /* text on the right of an action (optional) */
    int (*enabled)(void);                                               /* NULL = always enabled; 0 = greyed out and unchangeable */
    const char *(*valhint)(int i);                                      /* choice: explanation of the value i, shown below the general one, separated by a line (optional) */
} MItem;

typedef struct Menu Menu;
Menu *menu_new(const char *title);
void menu_add(Menu *m, const MItem *it);
void menu_last_valtext(Menu *m, const char *(*v)(void));              /* value text of the last added item */
void menu_last_enabled(Menu *m, int (*e)(void));
void menu_last_valhint(Menu *m, const char *(*v)(int i));           /* explanation of each value of the last added choice */                    /* enabled condition of the last added item */
int  menu_is_open(Menu *m);
void menu_show(Menu *m);
void menu_hide(Menu *m);
void menu_nav(Menu *m, const char *dir);          /* "up" "down" "left" "right", "tabprev" "tabnext" */
void menu_activate(Menu *m);
void menu_status(Menu *m, const char *text);
int  menu_signature(Menu *m, int w, int h, char *out, size_t n);   /* string that changes when the menu must be repainted */
uint8_t *menu_draw(Menu *m, int w, int h);        /* RGBA (malloc) w*h */
#endif
