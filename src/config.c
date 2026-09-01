#include <dot-5/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[65];
    char value[65];
} Value;

static Value values[128] = {0};
static size_t value_count = 0;

static char *src = NULL;
static long size = 0;
static size_t pos = 0;

bool is_char(char ch) {
    return ('a' <= ch && ch <= 'z') || ('A' <= ch && ch <= 'Z') || ch == '_';
}

bool is_num(char num) {
    return ('0' <= num && num <= '9');
}

void skip_space() { while (pos < size && src[pos] == ' ') ++pos; }
void skip_line() { while (pos < size && src[pos] != '\n' && src[pos] != '\r') ++pos; }

bool read_value() {
    Value *value = &values[value_count];

    for (int i = 0; pos < size && (is_char(src[pos]) || is_num(src[pos])) && i < 64; ++i)
        value->name[i] = src[pos++];
    skip_space();

    if (src[pos] != '=') return false;
    ++pos;
    skip_space();

    if (src[pos] == '\n' || src[pos] == '\r') return false;
    
    for (int i = 0; pos < size && src[pos] != ' ' && src[pos] != '\n' && src[pos] != '\r' && i < 64; ++i)
        value->value[i] = src[pos++];
    ++value_count;

    return true;
}

bool read_values() {
    while (pos < size) {
        char ch = src[pos];
        if (is_char(ch)) {
            if (!read_value()) return false;
        } else if (ch == '#') skip_line();
        else ++pos;
    }

    return true;
}

bool cfg_load(const char *filename) {
    if (!filename) return false;
    
    FILE *file;
    if (!(file = fopen(filename, "r"))) return false;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (!(src = (char*)malloc(size))) {
        fclose(file);
        return false;
    }
    memset(src, 0, size);

    fread(src, 1, size, file);

    fclose(file);

    bool result = read_values();
    free(src);

    return result;
}
#ifdef _WIN32
bool cfg_load_w(const wchar_t *filename) {
    if (!filename) return false;

    FILE *file;
    if (!(file = _wfopen(filename, L"r"))) return false;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (!(src = (char*)malloc(size))) {
        fclose(file);
        return false;
    }
    memset(src, 0, size);

    fread(src, 1, size, file);

    fclose(file);

    bool result = read_values();
    free(src);

    return result;
}
#endif

char *cfg_get_value(const char *name) {
    for (size_t i = 0; i < value_count; ++i) {
        if (!strcmp(values[i].name, name)) return values[i].value;
    }
    return NULL;
}