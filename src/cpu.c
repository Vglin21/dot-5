#include <dot-5/cpu.h>

static byte pc = 0;
static byte zero = 0;
static byte a = 0;

void cpu_reset() {
    pc = 0;
    zero = 0;
    a = 0;
}
byte cpu_step() {
    switch (mem_read(pc++)) {
        case INC:
            zero = (++a) == 0;
            return 2;
        case DEC:
            zero = (--a) == 0;
            return 2;
        case LDA_I:
            zero = (a = mem_read(pc++)) == 0;
            return 2;
        case LDA_Z:
            zero = (a = mem_read(mem_read(pc++))) == 0;
            return 3;
        case STA:
            mem_write(mem_read(pc++), a);
            return 3;
        case JMP:
            pc = mem_read(pc++);
            return 2;
        case BEQ:
            if (zero) {
                pc += (int8_t)mem_read(pc++);
                return 3;
            } else {
                pc++;
                return 2;
            }
        case BNE:
            if (!zero) {
                pc += (int8_t)mem_read(pc++);
                return 3;
            } else {
                pc++;
                return 2;
            }
        case ADD_I:
            zero = (a += mem_read(pc++)) == 0;
            return 2;
        case ADD_Z:
            zero = (a += mem_read(mem_read(pc++))) == 0;
            return 3;
        case SUB_I:
            zero = (a -= mem_read(pc++)) == 0;
            return 2;
        case SUB_Z:
            zero = (a -= mem_read(mem_read(pc++))) == 0;
            return 3;
        case AND_I:
            zero = (a &= mem_read(pc++)) == 0;
            return 2;
        case AND_Z:
            zero = (a &= mem_read(mem_read(pc++))) == 0;
            return 3;
        case ORA_I:
            zero = (a |= mem_read(pc++)) == 0;
            return 2;
        case ORA_Z:
            zero = (a |= mem_read(mem_read(pc++))) == 0;
            return 3;
    }
    return 2;
}