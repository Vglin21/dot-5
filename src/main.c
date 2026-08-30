#include <dot-5/dot-5.h>
#include <stdio.h>
#ifdef _WIN32
#include <direct.h>

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

int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) return 1;

    wchar_t cfg_path[512];
    wchar_t *appdata = _wgetenv(L"APPDATA");
    
    if (appdata) {
        swprintf(cfg_path, 512, L"%ls\\DOT-5", appdata);
        
        _wmkdir(cfg_path);
        
        swprintf(cfg_path, 512, L"%ls\\config.cfg", cfg_path);
    } else wcscpy(cfg_path, L"config.cfg");

    FILE *file;
    if (!(file = _wfopen(cfg_path, L"r"))) {
        if (!(file = _wfopen(cfg_path, L"w"))) return 1;
        
        fwrite(cfg, 1, strlen(cfg), file);

        fclose(file);
    }

    if (!d5_load_w(argv[1], cfg_path)) return 1;
#else
int main(int argc, char *argv[]) {
    if (!d5_load("rom.d5", "config.cfg")) {
        printf("d5_load failed - rom.d5 or config.cfg not found/loadable\n");
        return 1;
    }
#endif

    d5_run();

    return 0;
}