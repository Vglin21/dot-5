#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <stdbool.h>
#include <wchar.h>

#define CFG_MAX_KEY_LEN 64
#define CFG_MAX_VALUE_LEN 64
#define CFG_MAX_VALUES 128

extern bool cfg_load(const char *filename);
#ifdef _WIN32
extern bool cfg_load_w(const wchar_t *filename);
#endif

extern char *cfg_get_value(const char *key);
extern void cfg_set_value(const char *key, const char *value);

#endif