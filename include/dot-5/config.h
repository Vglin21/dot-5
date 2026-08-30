#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdbool.h>
#include <wchar.h>

extern bool cfg_load(const char *filename);
#ifdef _WIN32
extern bool cfg_load_w(const wchar_t *filename);
#endif
extern char *cfg_get_value(const char *name);

#endif