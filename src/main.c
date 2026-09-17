#include <dot-5/dot-5.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifndef __EMSCRIPTEN__
char cfg[] = "# Display\n"
"fullscreen = false\n"
"window_width = 480\n"
"window_height = 480\n"
"resizable = true\n"
"\n"
"# Emulation\n"
"emulation_speed = 1.0\n"
"\n"
"# Rendering\n"
"background_color = e7e7e7\n"
"pixel_color = 070707\n"
"aspect_ratio = 1.0\n"
"\n"
"# Input (keyboard)\n"
"input_key_up = KEY_UP\n"
"input_key_left = KEY_LEFT\n"
"input_key_down = KEY_DOWN\n"
"input_key_right = KEY_RIGHT\n"
"\n"
"# Input (gamepad)\n"
"input_gamepad_up = GAMEPAD_UP\n"
"input_gamepad_left = GAMEPAD_LEFT\n"
"input_gamepad_down = GAMEPAD_DOWN\n"
"input_gamepad_right = GAMEPAD_RIGHT\n"
"\n"
"# Hotkeys\n"
"close_window = KEY_ESCAPE\n"
"toggle_fullscreen = KEY_F";
#endif

#ifdef _WIN32
#include <direct.h>

#define char wchar_t
#define s(a) L##a
#define get_cfg_path _wgetenv(L"LOCALAPPDATA")
#define sfmt "%ls\\"

#define main wmain
#define strcmp(a, b) wcscmp(a, b)
#define strcpy(a, b) wcscpy(a, b)
#define fopen(a, b) _wfopen(a, b)
#define mkdir(a) _wmkdir(a)
#define snprintf(a, b, c, d) swprintf(a, b, c, d)
#define d5_load(a, b) d5_load_w(a, b)

#elif defined(__linux)
#include <sys/stat.h>

#define s(a) a
#define get_cfg_path getenv("HOME")
#define sfmt "%s/"

#define snprintf(a, b, c, d) snprintf(a, b, c, d)
#define mkdir(a) mkdir(a, 0700)

#endif

#ifdef __EMSCRIPTEN__
int main(int argc, char *argv[]) {
    if (!d5_load("rom.d5", "dot-5.cfg")) return 1;

    d5_run();

    return 0;
}
#else
int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    char *bin_file = NULL;
    char cfg_file[512] = {0};
    FILE *file;

    for (byte c = 1; c < argc; ++c) {
        if (!strcmp(argv[c], s("-f")) || !strcmp(argv[c], s("--fullscreen")))
            d5_configure("fullscreen", "true");
        else if (!strcmp(argv[c], s("-c")) || !strcmp(argv[c], s("--config"))) {
            if (++c < argc) {
                if (file = fopen(argv[c], s("r"))) {
                    strcpy(cfg_file, argv[c]);
                    fclose(file);
                }
            } else return 1;
        } else if (bin_file == NULL) bin_file = argv[c];
    }

    if (!bin_file) return 1;
    
    if (!cfg_file[0]) {
        if (file = fopen(s("dot-5.cfg"), s("r"))) {
            strcpy(cfg_file, s("dot-5.cfg"));
            fclose(file);
        } else {
            char *cfg_path = get_cfg_path;
    
            if (cfg_path) {
#ifdef _WIN32
                snprintf(cfg_file, 512, s(""sfmt"DOT-5"), cfg_path);
#else
                snprintf(cfg_file, 512, s(""sfmt".config/DOT-5"), cfg_path);
#endif
                mkdir(cfg_file);
                
                snprintf(cfg_file, 512, s(""sfmt"dot-5.cfg"), cfg_file);
            } else strcpy(cfg_file, s("dot-5.cfg"));
    
            if (!(file = fopen(cfg_file, s("r")))) {
                if (!(file = fopen(cfg_file, s("w")))) return 1;
                
                fwrite(cfg, 1, strlen(cfg), file);
        
                fclose(file);
            }
        }
    }
    
    if (!d5_load(bin_file, cfg_file)) return 1;

    d5_run();

    return 0;
}
#endif