#ifndef _DOT_5_H_
#define _DOT_5_H_

#include <dot-5/display.h>
#include <dot-5/cpu.h>
#include <dot-5/config.h>

extern bool d5_load(const char *bin_filepath, const char *config_filepath);
extern bool d5_load_w(const wchar_t *bin_filepath, const wchar_t *config_filepath);
extern void d5_run();
extern void d5_exit();

#endif