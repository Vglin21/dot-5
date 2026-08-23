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
    DISPK_RIGHT = 79,
    DISPK_LEFT,
    DISPK_DOWN,
    DISPK_UP
} DisplayKey;

extern bool display_turn_on(const char *title, dword width, dword height, DisplayType type);
extern void display_turn_off();

extern bool display_should_close();
extern bool display_is_key_pressed(DisplayKey key);
extern bool display_is_frame_active();

extern bool display_set_signal_size(word width, word height, word hblank, word vblank);
extern void display_set_fps(double fps);

extern void display_draw_pixel(dword color);
extern void display_draw_pixels(dword color, size_t count);

extern void display_update();

#endif