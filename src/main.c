#include <dot-5/dot-5.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
#include <direct.h>
#elif __linux
#include <sys/stat.h>
#endif

#ifndef __EMSCRIPTEN__
char cfg[] = "# Display\n"
"fullscreen = false\n"
"window_width = 480\n"
"window_height = 480\n"
"\n"
"# Emulation\n"
"emulation_speed = 1.0\n"
"\n"
"# Rendering\n"
"background_color = e7e7e7\n"
"pixel_color = 070707\n"
"\n"
"# Input\n"
"input_up = KEY_UP\n"
"input_left = KEY_LEFT\n"
"input_down = KEY_DOWN\n"
"input_right = KEY_RIGHT\n"
"\n"
"# Hotkeys\n"
"close_window = KEY_ESCAPE\n"
"toggle_fullscreen = KEY_F";
#endif

#ifdef __EMSCRIPTEN__
int main(int argc, char *argv[]) {
    if (!d5_load("rom.d5", "dot-5.cfg")) return 1;

    d5_run();

    return 0;
}
#else

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) return 1;

    wchar_t *bin_file = NULL;
#else
int main(int argc, char *argv[]) {
    if (argc < 2) return 1;

    char *bin_file = NULL;
#endif

    for (byte c = 1; c < argc; ++c) {
#ifdef _WIN32
        if (!wcscmp(argv[c], L"-f") || !wcscmp(argv[c], L"--fullscreen"))
#else
        if (!strcmp(argv[c], "-f") || !strcmp(argv[c], "--fullscreen"))
#endif
            d5_configure("fullscreen", "true");
        else if (bin_file == NULL) bin_file = argv[c];
    }

    if (!bin_file) return 1;

#ifdef _WIN32
    wchar_t cfg_file[512];
    
    FILE *file;
    if (file = _wfopen(L"dot-5.cfg", L"r")) {
        wcscpy(cfg_file, L"dot-5.cfg");
        fclose(file);
    } else {
        wchar_t *appdata = _wgetenv(L"APPDATA");

        if (appdata) {
            swprintf(cfg_file, 512, L"%ls\\DOT-5", appdata);
            _wmkdir(cfg_file);
            
            swprintf(cfg_file, 512, L"%ls\\dot-5.cfg", cfg_file);
        } else wcscpy(cfg_file, L"dot-5.cfg");

        if (!(file = _wfopen(cfg_file, L"r"))) {
            if (!(file = _wfopen(cfg_file, L"w"))) return 1;
            
            fwrite(cfg, 1, strlen(cfg), file);
    
            fclose(file);
        }
    }
    
    if (!d5_load_w(bin_file, cfg_file)) return 1;
#else
    char cfg_file[512];
    
    FILE *file;
    if (file = fopen("dot-5.cfg", "r")) {
        strcpy(cfg_file, "dot-5.cfg");
        fclose(file);
    } else {
        char *home = getenv("HOME");

        if (home) {
            char dir[512];
            
            snprintf(dir, 512, "%s/.config/DOT-5", home);
            mkdir(dir, 0755);
            
            snprintf(cfg_file, 512, "%s/dot-5.cfg", dir);
        } else strcpy(cfg_file, "dot-5.cfg");

        if (!(file = fopen(cfg_file, "r"))) {
            if (!(file = fopen(cfg_file, "w"))) return 1;
            
            fwrite(cfg, 1, strlen(cfg), file);
    
            fclose(file);
        }
    }

    if (!d5_load(bin_file, cfg_file)) return 1;
#endif

    d5_run();

    return 0;
}

#endif