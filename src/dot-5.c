#include <dot-5/dot-5.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

struct {
    struct {
        bool fullscreen;
        dword width;
        dword height;
    } display;

    float emulation_speed;

    struct {
        dword background;
        dword pixel;
    } rendering;

    struct {
        dword up;
        dword left;
        dword down;
        dword right;
        dword exit;
        dword fullscreen;
    } input;
} config;

typedef struct {
    char *name;
    dword key;
} Key;

static Key keys[] = {
    {"KEY_A", DISPK_A},
    {"KEY_B", DISPK_B},
    {"KEY_C", DISPK_C},
    {"KEY_D", DISPK_D},
    {"KEY_E", DISPK_E},
    {"KEY_F", DISPK_F},
    {"KEY_G", DISPK_G},
    {"KEY_H", DISPK_H},
    {"KEY_I", DISPK_I},
    {"KEY_J", DISPK_J},
    {"KEY_K", DISPK_K},
    {"KEY_L", DISPK_L},
    {"KEY_M", DISPK_M},
    {"KEY_N", DISPK_N},
    {"KEY_O", DISPK_O},
    {"KEY_P", DISPK_P},
    {"KEY_Q", DISPK_Q},
    {"KEY_R", DISPK_R},
    {"KEY_S", DISPK_S},
    {"KEY_T", DISPK_T},
    {"KEY_U", DISPK_U},
    {"KEY_V", DISPK_V},
    {"KEY_W", DISPK_W},
    {"KEY_X", DISPK_X},
    {"KEY_Y", DISPK_Y},
    {"KEY_Z", DISPK_Z},

    {"KEY_RETURN", DISPK_RETURN},
    {"KEY_ESCAPE", DISPK_ESCAPE},
    {"KEY_BACKSPACE", DISPK_BACKSPACE},
    {"KEY_TAB", DISPK_TAB},
    {"KEY_SPACE", DISPK_SPACE},

    {"KEY_MINUS", DISPK_MINUS},
    {"KEY_EQUALS", DISPK_EQUALS},
    {"KEY_LBRACKET", DISPK_LBRACKET},
    {"KEY_RBRACKET", DISPK_RBRACKET},
    {"KEY_BACKSLASH", DISPK_BACKSLASH},

    {"KEY_SEMICOLON", DISPK_SEMICOLON},
    {"KEY_APOSTROPHE", DISPK_APOSTROPHE},
    {"KEY_GRAVE", DISPK_GRAVE},
    {"KEY_COMMA", DISPK_COMMA},
    {"KEY_PERIOD", DISPK_PERIOD},
    {"KEY_SLASH", DISPK_SLASH},

    {"KEY_CAPSLOCK", DISPK_CAPSLOCK},

    {"KEY_F1", DISPK_F1},
    {"KEY_F2", DISPK_F2},
    {"KEY_F3", DISPK_F3},
    {"KEY_F4", DISPK_F4},
    {"KEY_F5", DISPK_F5},
    {"KEY_F6", DISPK_F6},
    {"KEY_F7", DISPK_F7},
    {"KEY_F8", DISPK_F8},
    {"KEY_F9", DISPK_F9},
    {"KEY_F10", DISPK_F10},
    {"KEY_F11", DISPK_F11},
    {"KEY_F12", DISPK_F12},

    {"KEY_PRINTSCREEN", DISPK_PRINTSCREEN},
    {"KEY_SCROLLLOCK", DISPK_SCROLLLOCK},
    {"KEY_PAUSE", DISPK_PAUSE},
    {"KEY_INSERT", DISPK_INSERT},
    {"KEY_HOME", DISPK_HOME},
    {"KEY_PAGEUP", DISPK_PAGEUP},
    {"KEY_DELETE", DISPK_DELETE},
    {"KEY_END", DISPK_END},
    {"KEY_PAGEDOWN", DISPK_PAGEDOWN},
    
    {"KEY_RIGHT", DISPK_RIGHT},
    {"KEY_LEFT", DISPK_LEFT},
    {"KEY_DOWN", DISPK_DOWN},
    {"KEY_UP", DISPK_UP},

    {"KEY_NUMLOCK", DISPK_NUMLOCK},
    
    {"KEY_KP_DIVIDE", DISPK_KP_DIVIDE},
    {"KEY_KP_MULTIPLY", DISPK_KP_MULTIPLY},
    {"KEY_KP_MINUS", DISPK_KP_MINUS},
    {"KEY_KP_PLUS", DISPK_KP_PLUS},
    {"KEY_KP_ENTER", DISPK_KP_ENTER},
    {"KEY_KP_1", DISPK_KP_1},
    {"KEY_KP_2", DISPK_KP_2},
    {"KEY_KP_3", DISPK_KP_3},
    {"KEY_KP_4", DISPK_KP_4},
    {"KEY_KP_5", DISPK_KP_5},
    {"KEY_KP_6", DISPK_KP_6},
    {"KEY_KP_7", DISPK_KP_7},
    {"KEY_KP_8", DISPK_KP_8},
    {"KEY_KP_9", DISPK_KP_9},
    {"KEY_KP_0", DISPK_KP_0},
    {"KEY_KP_PERIOD", DISPK_KP_PERIOD},

    {"KEY_LCTRL", DISPK_LCTRL},
    {"KEY_LSHIFT", DISPK_LSHIFT},
    {"KEY_LALT", DISPK_LALT},
    {"KEY_RCTRL", DISPK_RCTRL},
    {"KEY_RSHIFT", DISPK_RSHIFT},
    {"KEY_RALT", DISPK_RALT}
};

static bool fullscreen = false;
static word beam = 0;

static dword get_key(char *name) {
    for (size_t i = 0; i < sizeof(keys) / sizeof(Key); ++i) {
        if (!strcmp(keys[i].name, name)) return keys[i].key;
    }
    return 0;
}

static bool is_hex(char hex) {
    return ('0' <= hex && hex <= '9') || ('a' <= hex && hex <= 'f') || ('A' <= hex && hex <= 'F');
}

static qword hex_to_int(char *hex) {
    if (!hex) return 0;

    int8_t pos = strlen(hex);
    byte shift = 0;
    qword value = 0;

    while ((--pos) >= 0) {
        char hc = hex[pos];
        if ('0' <= hc && hc <= '9') value += (hc - '0') << shift;
        else if ('a' <= hc && hc <= 'f') value += (hc - 'a' + 0xa) << shift;
        else if ('A' <= hc && hc <= 'F') value += (hc - 'A' + 0xa) << shift;
        else return 0;

        shift += 4;
    }

    return value;
}

static void configure() {
    char *v;
    config.display.fullscreen = (v = cfg_get_value("fullscreen")) ? !strcmp(v, "true") : false;
    config.display.width      = (v = cfg_get_value("window_width")) ? atoi(v) : 480;
    config.display.height     = (v = cfg_get_value("window_height")) ? atoi(v) : 480;

    config.emulation_speed = (v = cfg_get_value("emulation_speed")) ? atof(v) : 1.0;

    config.rendering.background = (v = cfg_get_value("background_color")) ? (hex_to_int(v) << 8) + 0xff : 0xe7e7e7ff;
    config.rendering.pixel      = (v = cfg_get_value("pixel_color")) ? (hex_to_int(v) << 8) + 0xff : 0x070707ff;

    config.input.up         = (v = cfg_get_value("input_up")) ? get_key(v) : DISPK_UP;
    config.input.left       = (v = cfg_get_value("input_left")) ? get_key(v) : DISPK_LEFT;
    config.input.down       = (v = cfg_get_value("input_down")) ? get_key(v) : DISPK_DOWN;
    config.input.right      = (v = cfg_get_value("input_right")) ? get_key(v) : DISPK_RIGHT;
    config.input.exit       = (v = cfg_get_value("close_window")) ? get_key(v) : DISPK_ESCAPE;
    config.input.fullscreen = (v = cfg_get_value("toggle_fullscreen")) ? get_key(v) : DISPK_F;

}

static void d5_frame() {
    while (display_is_frame_active()) {
#ifndef __EMSCRIPTEN__
        if (display_is_key_pressed(config.input.exit)) display_close();
#endif

        if (display_is_key_pressed(config.input.fullscreen)) {
            if (!fullscreen) display_set_fullscreen(!display_is_fullscreen());
            fullscreen = true;
        } else fullscreen = false;

        byte cycles = cpu_step();

        while (cycles--) {
            if (beam < 256) {
                bool dot = false;
                for (byte c = 1; c < 6; ++c) if (beam == mem_read(c)) {
                    dot = true;
                    break;
                }
                display_draw_pixel(dot ? config.rendering.pixel : config.rendering.background);

                ++beam;
            } else if (beam == 256) {
                mem_write(0, 1);
                if (display_is_key_pressed(config.input.right)) mem_write(0, mem_read(0) | 2);
                if (display_is_key_pressed(config.input.left))  mem_write(0, mem_read(0) | 4);
                if (display_is_key_pressed(config.input.down))  mem_write(0, mem_read(0) | 8);
                if (display_is_key_pressed(config.input.up))    mem_write(0, mem_read(0) | 16);

                ++beam;
            } else if ((++beam) == 320) beam = 0;
        }
    }

    display_update();

#ifdef __EMSCRIPTEN__
    if (display_should_close()) emscripten_cancel_main_loop();
#endif
}

bool d5_load(const char *bin_filepath, const char *config_filepath) {
    if (bin_filepath) {
        if (!mem_load_rom_from_file(bin_filepath)) return false;
    } else return false;
    
    cfg_load(config_filepath);
    configure();
    
    return true;
}
#ifdef _WIN32
bool d5_load_w(const wchar_t *bin_filepath, const wchar_t *config_filepath) {
    if (bin_filepath) {
        if (!mem_load_rom_from_file_w(bin_filepath)) return false;
    } else return false;
    
    cfg_load_w(config_filepath);
    configure();

    return true;
}
#endif

void d5_run() {
    display_turn_on("DOT-5", config.display.width, config.display.height, DISPLAY_TYPE_LCD);
    display_set_fullscreen(config.display.fullscreen);
    
    display_set_fps(11.97 * config.emulation_speed);
    
    display_set_signal_size(16, 16, 0, 0);
    
    fullscreen = false;
    beam = 0;
    
#ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(d5_frame, 0, 1);
#else
    while (!display_should_close()) d5_frame();
#endif

    display_turn_off();
}

void d5_configure(const char *key, const char *value) {
    cfg_set_value(key, value);
    configure();
}