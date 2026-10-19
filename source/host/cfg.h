/* cfg.h - options in JSON (options.json) */
#ifndef CFG_H
#define CFG_H
#include "host.h"
int  cfg_load(const char *path);
int  cfg_save(const char *path);
void cfg_clear(void);
int  cfg_has(const char *key);
double cfg_num(const char *key, double def);
int  cfg_bool(const char *key, int def);
const char *cfg_str(const char *key, const char *def);
void cfg_set_num(const char *key, double v);
void cfg_set_bool(const char *key, int v);
void cfg_set_str(const char *key, const char *v);
void cfg_remove(const char *key);
#endif
