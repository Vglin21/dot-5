#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dot-5/cpu.h>

typedef struct {
    char label[4];
    byte no_arg;
    byte imm;
    byte zp;
} Opcode;

typedef struct {
    char label[4];
    byte arg;
    bool has_arg;
    bool imm;
} Entry;

Opcode opcodes[] = {
    {"inc", INC, 0,     0    },
    {"dec", DEC, 0,     0    },
    {"lda", 0,   LDA_I, LDA_Z},
    {"sta", 0,   0,     STA  },
    {"jmp", 0,   JMP,   JMP  },
    {"beq", 0,   BEQ,   BEQ  },
    {"bne", 0,   BNE,   BNE  },
    {"add", 0,   ADD_I, ADD_Z},
    {"sub", 0,   SUB_I, SUB_Z},
    {"ora", 0,   ORA_I, ORA_Z},
    {"and", 0,   AND_I, AND_Z}
};

Entry entries[248];
byte entry_count = 0;

char *src = NULL;
dword pos = 0;
dword size = 0;
char ch = 0;
char label[4] = {0};
byte bin[248] = {0};
byte bc = 0;

bool is_hex(char hex) {
    return ('0' <= hex && hex <= '9') || ('a' <= hex && hex <= 'f');
}

bool is_int(char num) {
    return '0' <= num && num <= '9';
}

byte get_hex() {
    char hex[17] = {0};
    byte hsize = 0;
    for (; hsize < 16 && is_hex(src[pos]) && pos < size; ++hsize) hex[hsize] = src[pos++];
    qword a = 1;
    byte value = 0;
    for (byte c = hsize - 1; c < hsize; --c) {
        if ('0' <= hex[c] && hex[c] <= '9') value += (hex[c] - '0') * a;
        else value += (hex[c] - 'a' + 0xa) * a;
        a <<= 4;
    }
    return value;
}

byte get_int() {
    char num[33] = {0};
    for (byte c = 0; c < 32 && is_int(src[pos]) && pos < size; ++c) num[c] = src[pos++];
    return (byte)atoi(num);
}

void skip_space() { while (src[pos] == ' ') ++pos; }

void get_label(Entry *entry) {
    memset(entry->label, 0, 4);
    for (byte c = 0; c < 3 && 'a' <= src[pos] && src[pos] <= 'z' && pos < size; ++c) entry->label[c] = src[pos++];
}

void get_value(Entry *entry) {
    if (src[pos] == '$') {
        ++pos;
        entry->arg = get_hex();
        entry->has_arg = true;
    } else {
        entry->arg = get_int();
        entry->has_arg = true;
    }
}

void get_entry() {
    Entry *entry = &entries[entry_count];
    get_label(entry);
    skip_space();
    if (src[pos] == '#') {
        entry->imm = true;
        ++pos;
        get_value(entry);
    } else if (src[pos] == '$' || ('0' <= src[pos] && src[pos] <= '9')) get_value(entry);
    else entry->has_arg = false;
    ++entry_count;
}

void to_bin(Entry entry) {
    for (byte c = 0; c < (sizeof(opcodes) / sizeof(Opcode)) && bc < 248; ++c) {
        if (!strcmp(entry.label, opcodes[c].label)) {
            Opcode opcode = opcodes[c];
            if (entry.has_arg) {
                if (entry.imm) {
                    bin[bc++] = opcode.imm;
                    if (bc >= 248) break;
                    bin[bc++] = entry.arg;
                } else {
                    bin[bc++] = opcode.zp;
                    if (bc >= 248) break;
                    bin[bc++] = entry.arg;
                }
            } else bin[bc++] = opcode.no_arg;
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("d5asm.exe: error: no input file\n");
        return 1;
    }

    if (!strcmp(argv[1], "--h") || !strcmp(argv[1], "--help")) {
        printf(
            "Usage: d5asm.exe [source-file] [output-file-name](optional)\n"
        );
        return 0;
    }
    
    FILE *file;
    if (!(file = fopen(argv[1], "r"))) {
        printf("d5asm.exe: error: %s: no such file or directory\n", argv[1]);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (!(src = (char*)malloc(size))) {
        printf("d5asm.exe: error: an error occured\n");
        fclose(file);
        return 1;
    }

    size = fread(src, 1, size, file);

    fclose(file);
    
    while (pos < size) {
        ch = src[pos];
        if ('a' <= ch && ch <= 'z') get_entry();
        else ++pos;
    }

    free(src);

    for (byte c = 0; c < entry_count; ++c) to_bin(entries[c]);

    if (!(file = fopen(argc < 3 ? "output.d5" : argv[2], "wb"))) return 1;
    
    fwrite(bin, 1, 248, file);
    printf("d5asm.exe: %d bytes written\n", bc);

    fclose(file);

    return 0;
}