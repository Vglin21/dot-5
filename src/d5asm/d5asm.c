#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dot-5/cpu.h>

#define ASM_NAME "d5asm.exe"
#define MAX_LABEL_LEN 64
#define MAX_LINE_LEN 256
#define MAX_ENTRIES 248
#define MAX_LABELS 128

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
    char label[MAX_LABEL_LEN+1];
} Entry;

typedef struct {
    char str[MAX_LABEL_LEN+1];
    byte address;
} Label;

const Opcode opcodes[] = {
    {"INC", INC, 0,     0,     false},
    {"DEC", DEC, 0,     0,     false},
    {"LDA", 0,   LDA_I, LDA_Z, false},
    {"STA", 0,   0,     STA,   false},
    {"JMP", 0,   JMP,   JMP,   false},
    {"BEQ", 0,   BEQ,   BEQ,   true },
    {"BNE", 0,   BNE,   BNE,   true },
    {"ADD", 0,   ADD_I, ADD_Z, false},
    {"SUB", 0,   SUB_I, SUB_Z, false},
    {"ORA", 0,   ORA_I, ORA_Z, false},
    {"AND", 0,   AND_I, AND_Z, false}
};

Entry entries[MAX_ENTRIES];
byte entry_count = 0;

Label labels[MAX_LABELS];
byte label_count = 0;

byte bin[ROM_SIZE] = {0};
byte bin_count = 0;

char line[MAX_LINE_LEN] = {0};
word lpos = 0;

void to_big_letters(char *str) {
    for (dword c = 0; c < strlen(str); ++c)
        if ('a' <= str[c] && str[c] <= 'z') str[c] -= 0x20;
}

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
    byte shift = 0;
    qword rvalue = 0;

    while (hsize < 16 && is_hex(line[lpos]) && lpos < MAX_LINE_LEN)
        hex[hsize++] = line[lpos++];

    for (byte c = hsize - 1; c < hsize; --c) {
        char h = hex[c];

        if ('0' <= h && h <= '9') rvalue += (hex[c] - '0') << shift;
        else if ('a' <= h && h <= 'f') rvalue += (hex[c] - 'a' + 0xa) << shift;
        else rvalue += (h - 'A' + 0xa) << shift;

        shift += 4;
    }
    return (byte)rvalue;
}

byte get_int() {
    char num[33] = {0};
    for (byte c = 0; c < 32 && is_int(line[lpos]) && lpos < MAX_LINE_LEN; ++c) num[c] = line[lpos++];
    return (byte)atoi(num);
}

void skip_line() { while (line[lpos] != '\0' && line[lpos] != '\n' && lpos < MAX_LINE_LEN) ++lpos; }

void skip_space() { while (line[lpos] == ' ' && lpos < MAX_LINE_LEN) ++lpos; }

void get_name(Entry *entry) {
    memset(entry->name, 0, 4);
    for (byte c = 0; c < 3 && is_char(line[lpos]) && lpos < MAX_LINE_LEN; ++c) entry->name[c] = line[lpos++];
}

void get_value(Entry *entry) {
    if (line[lpos] == '$') {
        ++lpos;
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
    to_big_letters(entry->name);
    skip_space();

    if (line[lpos] == '\0' || line[lpos] == '\n') {
        entry->has_arg = false;

        ++bin_count;
        ++entry_count;
    } else {
        if (line[lpos] == '#') {
            entry->imm = true;
            ++lpos;
            if (!(line[lpos] == '$' || is_int(line[lpos]) || is_char(line[lpos]))) return;
        }
    
        if (line[lpos] == '$' || is_int(line[lpos])) get_value(entry);
        else if (is_char(line[lpos])) {
            byte c = 0;
            while ((is_char(line[lpos]) || is_int(line[lpos])) && c < MAX_LABEL_LEN) entry->label[c++] = line[lpos++];
            entry->label[c] = '\0';
            entry->has_arg = true;
        }
    
        bin_count += 2;
        ++entry_count;
    }
}

void get_label() {
    Label *label = &labels[label_count];
    byte c = 0;
    while ((is_char(line[lpos]) || is_int(line[lpos])) && c < MAX_LABEL_LEN) label->str[c++] = line[lpos++];
    label->str[c] = '\0';

    skip_space();

    if (line[lpos] != '\n' && line[lpos] != '\0') {
        if (line[lpos] == '=') {
            ++lpos;
            skip_space();
        }

        if (line[lpos] == '$') {
            ++lpos;
            label->address = get_hex();
        } else if (is_int(line[lpos])) label->address = get_int();
    } else label->address = bin_count + ROM_ENTRY_POINT;

    ++label_count;
}

void to_bin(Entry entry) {
    for (byte c = 0; c < (sizeof(opcodes) / sizeof(Opcode)) && bin_count < ROM_SIZE; ++c) {
        if (!strcmp(entry.name, opcodes[c].name)) {
            Opcode opcode = opcodes[c];
            if (entry.has_arg) {
                if (entry.label[0] != '\0') {
                    for (byte i = 0; i < label_count; ++i) {
                        if (!strcmp(entry.label, labels[i].str)) {
                            if (opcode.branch) entry.arg = labels[i].address - (bin_count + ROM_ENTRY_POINT) - 2;
                            else entry.arg = labels[i].address;
                            break;
                        } 
                    }
                }

                if (entry.imm) bin[bin_count++] = opcode.imm;
                else bin[bin_count++] = opcode.zp;

                if (bin_count < ROM_SIZE) bin[bin_count++] = entry.arg;
            } else bin[bin_count++] = opcode.no_arg;
            break;
        }
    }
}

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[]) {
    wchar_t *src_file = NULL;
    wchar_t *out_file = L"output.d5";
#else
int main(int argc, char *argv[]) {
    char *src_file = NULL;
    char *out_file = "output.d5";
#endif

    bool error = false;

    for (byte c = 1; c < argc; ++c) {
#ifdef _WIN32
        if (!wcscmp(argv[c], L"-h") || !wcscmp(argv[c], L"--help")) {
#else
        if (!strcmp(argv[c], "-h") || !strcmp(argv[c], "--help")) {
#endif
            printf(
                "Usage: "ASM_NAME" [flags] file\n"
                "Options:\n"
                "  -h --help - Display this message.\n"
                "  -o <file> - Place the output into <file>.\n"
            );
            return 0;
#ifdef _WIN32
        } else if (!wcscmp(argv[c], L"-o")) {
#else
        } else if (!strcmp(argv[c], "-o")) {
#endif
            if (++c >= argc) {
                printf(ASM_NAME": error: missing filename after \"-o\"\n");
                error = true;
            } else out_file = argv[c];
        } else if (src_file != NULL) {
#ifdef _WIN32
            printf(ASM_NAME": warning: more than one file are given, any file after \"%ls\" will be skipped\n", src_file);
#else
            printf(ASM_NAME": warning: more than one file are given, any file after \"%s\" will be skipped\n", src_file);
#endif
        } else src_file = argv[c];
    }

    FILE *file;
    if (!src_file) {
        printf(ASM_NAME": error: no input file\n");
        error = true;
    } else {
#ifdef _WIN32
        if (!(file = _wfopen(src_file, L"r"))) {
            printf(ASM_NAME": error: couldn't open \"%ls\"\n", src_file);
#else
        if (!(file = fopen(src_file, "r"))) {
            printf(ASM_NAME": error: couldn't open \"%s\"\n", src_file);
#endif
            error = true;
        }
    }

    if (error) {
        printf(ASM_NAME": assembly terminated\n");
        if (file) fclose(file);
        return 1;
    }
    
    while (fgets(line, 256, file)) {
        bool nl = true;
        lpos = 0;
        
        while (line[lpos] != '\n' && line[lpos] != '\0') {
            char ch = line[lpos];

            if (is_char(ch)) {
                if (nl) get_label();
                else get_entry();
            } else if (ch == ';') skip_line();
            else ++lpos;

            nl = false;
        }
    }

    fclose(file);

    bin_count = 0;
    for (byte c = 0; c < entry_count; ++c) to_bin(entries[c]);

#ifdef _WIN32
    if (!(file = _wfopen(out_file, L"wb"))) return 1;
#else
    if (!(file = fopen(out_file, "wb"))) return 1;
#endif
    
    fwrite(bin, 1, ROM_SIZE, file);
    printf(ASM_NAME": %d bytes written\n", bin_count);

    fclose(file);

    return 0;
}