#include <dot-5/config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char key[CFG_MAX_KEY_LEN+1];
    char value[CFG_MAX_VALUE_LEN+1];
} Value;

static Value values[CFG_MAX_VALUES] = {0};
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
    if (value_count >= CFG_MAX_VALUES) return false;

    Value *value = &values[value_count];

    for (int i = 0; pos < size && (is_char(src[pos]) || is_num(src[pos])) && i < CFG_MAX_KEY_LEN; ++i)
        value->key[i] = src[pos++];
    skip_space();

    if (src[pos] != '=') return false;
    ++pos;
    skip_space();

    if (src[pos] == '\n' || src[pos] == '\r') return false;
    
    for (int i = 0; pos < size && src[pos] != ' ' && src[pos] != '\n' && src[pos] != '\r' && i < CFG_MAX_VALUE_LEN; ++i)
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

char *cfg_get_value(const char *key) {
    for (size_t i = 0; i < value_count; ++i) {
        if (!strcmp(values[i].key, key)) return values[i].value;
    }
    return NULL;
}
void cfg_set_value(const char *key, const char *value) {
    for (size_t i = 0; i < value_count; ++i) {
        if (!strcmp(values[i].key, key)) {
            strncpy(values[i].value, value, CFG_MAX_VALUE_LEN);
            return;
        }
    }
    
    if (value_count < CFG_MAX_VALUES) {
        strncpy(values[value_count].key, key, CFG_MAX_KEY_LEN);
        strncpy(values[value_count++].value, value, CFG_MAX_VALUE_LEN);
    }
}