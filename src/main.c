#include <dot-5/display.h>
#include <dot-5/cpu.h>
#include <stdio.h>

byte rom[] = {
    LDA_I, 0x7f,
    STA, 0x3,

    LDA_Z, 0x0,
    AND_I, 0x8,
    BEQ, 0x6,
    LDA_Z, 0x1,
    ADD_I, 0x10,
    STA, 0x1,

    LDA_Z, 0x0,
    AND_I, 0x10,
    BEQ, 0x6,
    LDA_Z, 0x1,
    SUB_I, 0x10,
    STA, 0x1,
    
    LDA_Z, 0x1,
    ADD_I, 0x10,
    STA, 0x2,

    LDA_Z, 0x6,
    BNE, 0x9,
    LDA_Z, 0x5,
    DEC,
    STA, 0x5,
    LDA_I, 0x0,
    BEQ, 0x5,
    LDA_Z, 0x5,
    INC,
    STA, 0x5,

    LDA_Z, 0x7,
    BNE, 0xa,
    LDA_Z, 0x5,
    SUB_I, 0x10,
    STA, 0x5,
    LDA_I, 0x0,
    BEQ, 0x6,
    LDA_Z, 0x5,
    ADD_I, 0x10,
    STA, 0x5,

    LDA_Z, 0x5,
    AND_I, 0xf0,
    BNE, 0x6,
    LDA_I, 0x1,
    STA, 0x7,
    BNE, 0x8,
    SUB_I, 0xf0,
    BNE, 0x4,
    LDA_I, 0x0,
    STA, 0x7,

    LDA_Z, 0x5,
    AND_I, 0xf,
    BNE, 0x12,
    LDA_Z, 0x6,
    BEQ, 0x20,
    LDA_Z, 0x7,
    BEQ, 0x1c,
    LDA_Z, 0x5,
    SUB_I, 0x10,
    STA, 0x5,
    LDA_I, 0x0,
    BNE, 0x12,
    SUB_I, 0xf,
    BNE, 0xe,
    LDA_Z, 0x6,
    BNE, 0xa,
    LDA_Z, 0x7,
    BNE, 0x6,
    LDA_Z, 0x5,
    ADD_I, 0x10,
    STA, 0x5,

    LDA_Z, 0x5,
    AND_I, 0xf0,
    STA, 0x0,

    LDA_Z, 0x5,
    DEC,
    SUB_Z, 0x1,
    BNE, 0x7,
    LDA_I, 0x1,
    STA, 0x6,
    DEC,
    STA, 0x7,

    LDA_Z, 0x5,
    DEC,
    SUB_Z, 0x2,
    BNE, 0x6,
    LDA_I, 0x1,
    STA, 0x6,
    STA, 0x7,

    LDA_Z, 0x5,
    INC,
    SUB_Z, 0x3,
    BNE, 0x6,
    LDA_I, 0x0,
    STA, 0x6,
    STA, 0x7,

    LDA_Z, 0x5,
    INC,
    SUB_Z, 0x4,
    BNE, 0x7,
    LDA_I, 0x0,
    STA, 0x6,
    INC,
    STA, 0x7,

    LDA_Z, 0x3,
    AND_I, 0xf0,
    SUB_Z, 0x0,
    BNE, 0xa,
    LDA_Z, 0x7,
    BNE, 0x6,
    LDA_Z, 0x3,
    SUB_I, 0x10,
    STA, 0x3,

    LDA_Z, 0x4,
    AND_I, 0xf0,
    SUB_Z, 0x0,
    BNE, 0xa,
    LDA_Z, 0x7,
    BEQ, 0x6,
    LDA_Z, 0x3,
    ADD_I, 0x10,
    STA, 0x3,

    LDA_Z, 0x3,
    ADD_I, 0x10,
    STA, 0x4,

    LDA_I, 0x0,
    STA, 0x0,
    LDA_Z, 0x0,
    BEQ, 0xfc,
    JMP, 0xc,
};

int main() {
    display_turn_on("DOT-5", 640, 640, DISPLAY_TYPE_LCD);
    display_set_fps(11.97);

    display_set_signal_size(16, 16, 0, 0);

    mem_load_rom(rom, sizeof(rom));

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
                    if (display_is_key_pressed(DISPK_RIGHT)) mem_write(0, mem_read(0) | 2);
                    if (display_is_key_pressed(DISPK_LEFT))  mem_write(0, mem_read(0) | 4);
                    if (display_is_key_pressed(DISPK_DOWN))  mem_write(0, mem_read(0) | 8);
                    if (display_is_key_pressed(DISPK_UP))    mem_write(0, mem_read(0) | 16);

                    ++beam;
                } else if ((++beam) == 320) beam = 0;
            }

            printf(
                "-------------------------\n"
                "|00|01|02|03|04|05|06|07|\n"
                "-------------------------\n"
                "|%.2x|%.2x|%.2x|%.2x|%.2x|%.2x|%.2x|%.2x|\n"
                "-------------------------\n",
                mem_read(0),
                mem_read(1),
                mem_read(2),
                mem_read(3),
                mem_read(4),
                mem_read(5),
                mem_read(6),
                mem_read(7)
            );
        }

        display_update();
    }

    display_turn_off();

    FILE *file = fopen("output.rom", "w");

    fwrite(rom, 1, sizeof(rom), file);

    fclose(file);

    return 0;
}