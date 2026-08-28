#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dot-5/cpu.h>

typedef struct {
    char name[4];
    byte no_arg;
    byte imm;
    byte zp;
    bool branch;
} Opcode;

typedef struct {
    char name[4];
    byte arg;
    bool has_arg;
    bool imm;
    char label[65];
} Entry;

typedef struct {
    char str[65];
    byte address;
} Label;

const Opcode opcodes[] = {
    {"inc", INC, 0,     0,     false},
    {"dec", DEC, 0,     0,     false},
    {"lda", 0,   LDA_I, LDA_Z, false},
    {"sta", 0,   0,     STA,   false},
    {"jmp", 0,   JMP,   JMP,   false},
    {"beq", 0,   BEQ,   BEQ,   true },
    {"bne", 0,   BNE,   BNE,   true },
    {"add", 0,   ADD_I, ADD_Z, false},
    {"sub", 0,   SUB_I, SUB_Z, false},
    {"ora", 0,   ORA_I, ORA_Z, false},
    {"and", 0,   AND_I, AND_Z, false}
};

Entry entries[248];
byte entry_count = 0;

Label labels[124];
byte label_count = 0;

char *src = NULL;
dword pos = 0;
dword size = 0;
char ch = 0;
char label[4] = {0};
byte bin[248] = {0};
byte bc = 0;
bool nl = false;

bool is_hex(char hex) {
    return ('0' <= hex && hex <= '9') || ('a' <= hex && hex <= 'f') || ('A' <= hex && hex <= 'F');
}

bool is_int(char num) {
    return '0' <= num && num <= '9';
}

bool is_char(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z');
}

byte get_hex() {
    char hex[17] = {0};
    byte hsize = 0;
    qword shift = 0;
    byte rvalue = 0;

    while (hsize < 16 && is_hex(src[pos]) && pos < size)
        hex[hsize++] = src[pos++];

    for (byte c = hsize - 1; c < hsize; --c) {
        char h = hex[c];
        if ('0' <= h && h <= '9') rvalue += (hex[c] - '0') << shift;
        else if ('a' <= h && h <= 'f') rvalue += (hex[c] - 'a' + 0xa) << shift;
        else rvalue += (h - 'A' + 0xa) << shift;
        shift += 4;
    }
    return rvalue;
}

byte get_int() {
    char num[33] = {0};
    for (byte c = 0; c < 32 && is_int(src[pos]) && pos < size; ++c) num[c] = src[pos++];
    return (byte)atoi(num);
}

void skip_line() { while (src[pos] != '\n' && pos < size) ++pos; }

void skip_space() { while (src[pos] == ' ' && pos < size) ++pos; }

void get_name(Entry *entry) {
    memset(entry->name, 0, 4);
    for (byte c = 0; c < 3 && 'a' <= src[pos] && src[pos] <= 'z' && pos < size; ++c) entry->name[c] = src[pos++];
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

    get_name(entry);
    skip_space();
    entry->has_arg = false;

    if (src[pos] == '#') {
        entry->imm = true;
        ++pos;
        skip_space();
    }

    if (src[pos] == '$' || is_int(src[pos])) get_value(entry);
    else if (is_char(src[pos])) {
        byte c = 0;
        while ((is_char(src[pos]) || is_int(src[pos])) && c < 64) entry->label[c++] = src[pos++];
        entry->label[c] = '\0';
        entry->has_arg = true;
    }

    if (entry->has_arg) bc += 2;
    else ++bc;

    ++entry_count;
}

void get_label() {
    Label *label = &labels[label_count];
    byte c = 0;
    while ((is_char(src[pos]) || is_int(src[pos])) && c < 64) label->str[c++] = src[pos++];
    label->str[c] = '\0';

    skip_space();

    if (src[pos] != '\n') {
        if (src[pos] == '=') {
            ++pos;
            skip_space();
        }

        if (src[pos] == '$') {
            ++pos;
            label->address = get_hex();
        } else if (is_int(src[pos])) label->address = get_int();
    } else label->address = bc + 0x8;

    ++label_count;
}

void to_bin(Entry entry) {
    for (byte c = 0; c < (sizeof(opcodes) / sizeof(Opcode)) && bc < 248; ++c) {
        if (!strcmp(entry.name, opcodes[c].name)) {
            Opcode opcode = opcodes[c];
            if (entry.has_arg) {
                if (entry.label[0] != '\0') {
                    for (byte i = 0; i < label_count; ++i) {
                        if (!strcmp(entry.label, labels[i].str)) {
                            if (opcode.branch) entry.arg = labels[i].address - (bc + 0x8) - 2;
                            else entry.arg = labels[i].address;
                            break;
                        } 
                    }
                }

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

int wmain(int argc, wchar_t *argv[]) {
    if (argc < 2) {
        printf("d5asm.exe: error: no input file\n");
        return 1;
    }

    if (!wcscmp(argv[1], L"--h") || !wcscmp(argv[1], L"--help")) {
        printf(
            "Usage: d5asm.exe [source-file] [output-file-name](optional)\n"
        );
        return 0;
    }
    
    FILE *file;
    if (!(file = _wfopen(argv[1], L"r"))) {
        printf("d5asm.exe: error: %hs: no such file or directory\n", argv[1]);
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
    
    nl = true;

    while (pos < size) {
        ch = src[pos];
        if (is_char(ch)) {
            if (nl) get_label();
            else get_entry();
        } else if (ch == ';') skip_line();
        else if (ch == '\n') {
            nl = true;
            ++pos;
        } else {
            ++pos;
            nl = false;
        }
    }

    free(src);

    bc = 0;
    for (byte c = 0; c < entry_count; ++c) to_bin(entries[c]);

    if (!(file = _wfopen(argc < 3 ? L"output.d5" : argv[2], L"wb"))) return 1;
    
    fwrite(bin, 1, 248, file);
    printf("d5asm.exe: %d bytes written\n", bc);

    fclose(file);

    return 0;
}