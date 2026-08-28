#include <dot-5/display.h>
#include <dot-5/cpu.h>

int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) return 1;

    if (!mem_load_rom_from_file_w(argv[1])) return 1;

    display_turn_on("DOT-5", 640, 640, DISPLAY_TYPE_LCD);
    display_set_fps(11.97);

    display_set_signal_size(16, 16, 0, 0);

    word beam = 0;
    while (!display_should_close()) {
        while (display_is_frame_active()) {
            byte cycles = cpu_step();

            while (cycles--) {
                if (beam < 256) {
                    bool dot = false;
                    for (byte c = 1; c < 6; ++c) if (beam == mem_read(c)) {
                        dot = true;
                        break;
                    }
                    display_draw_pixel(dot ? 0x070707ff : 0xe7e7e7ff);

                    ++beam;
                } else if (beam == 256) {
                    mem_write(0, 1);
                    if (
                        display_is_key_pressed(DISPK_RIGHT) ||
                        display_is_key_pressed(DISPK_D)
                    ) mem_write(0, mem_read(0) | 2);
                    if (
                        display_is_key_pressed(DISPK_LEFT) ||
                        display_is_key_pressed(DISPK_A)
                    ) mem_write(0, mem_read(0) | 4);
                    if (
                        display_is_key_pressed(DISPK_DOWN) ||
                        display_is_key_pressed(DISPK_S)
                    ) mem_write(0, mem_read(0) | 8);
                    if (
                        display_is_key_pressed(DISPK_UP) ||
                        display_is_key_pressed(DISPK_W)
                    ) mem_write(0, mem_read(0) | 16);

                    ++beam;
                } else if ((++beam) == 320) beam = 0;
            }
        }

        display_update();
    }

    display_turn_off();

    return 0;
}