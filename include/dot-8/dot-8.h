#ifndef _DOT_8_H_
#define _DOT_8_H_

#include <dot-8/display.h>
#include <dot-8/cpu.h>
#include <dot-8/config.h>

extern bool d8_load(const char *bin_filepath, const char *config_filepath);
#ifdef _WIN32
extern bool d8_load_w(const wchar_t *bin_filepath, const wchar_t *config_filepath);
#endif

extern void d8_run();

extern void d8_configure(const char *key, const char *value);

#endif