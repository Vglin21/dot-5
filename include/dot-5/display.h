#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef _CUSTOM_TYPES_
#define _CUSTOM_TYPES_

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

#endif

typedef enum {
    DISPLAY_TYPE_CRT,
    DISPLAY_TYPE_LCD
} DisplayType;

typedef enum {
    DISPK_A = 4,
    DISPK_B,
    DISPK_C,
    DISPK_D,
    DISPK_E,
    DISPK_F,
    DISPK_G,
    DISPK_H,
    DISPK_I,
    DISPK_J,
    DISPK_K,
    DISPK_L,
    DISPK_M,
    DISPK_N,
    DISPK_O,
    DISPK_P,
    DISPK_Q,
    DISPK_R,
    DISPK_S,
    DISPK_T,
    DISPK_U,
    DISPK_V,
    DISPK_W,
    DISPK_X,
    DISPK_Y,
    DISPK_Z,

    DISPK_RETURN = 40,
    DISPK_ESCAPE,
    DISPK_SPACE = 44,
    
    DISPK_RIGHT = 79,
    DISPK_LEFT,
    DISPK_DOWN,
    DISPK_UP,

    DISPK_LCTRL = 224,
    DISPK_LSHIFT,
    DISPK_LALT,
    DISPK_RCTRL = 228,
    DISPK_RSHIFT,
    DISPK_RALT
} DisplayKey;

extern bool display_turn_on(const char *title, dword width, dword height, DisplayType type);
extern void display_turn_off();
extern void display_close();

extern bool display_should_close();
extern bool display_is_key_pressed(DisplayKey key);
extern bool display_is_frame_active();

extern bool display_set_signal_size(word width, word height, word hblank, word vblank);
extern void display_set_fps(double fps);
extern void display_set_aspect_ratio(double aspect_ratio);
extern void display_set_window_size(dword width, dword height);

extern void display_draw_pixel(dword color);
extern void display_draw_pixels(dword color, size_t count);

extern void display_update();

#endif