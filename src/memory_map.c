#include <dot-5/memory_map.h>
#include <stdio.h>

static byte memory[0x100] = {0};

byte mem_load_rom(byte *rom, byte size) {
    byte s = size > ROM_SIZE ? ROM_SIZE : size;
    for (byte c = 0; c < s; ++c) memory[c + ROM_ENTRY_POINT] = rom[c];
    return s;
}
byte mem_load_rom_from_file(const char *filename) {
    FILE *file;
    if (!(file = fopen(filename, "rb"))) return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size > ROM_SIZE) size = ROM_SIZE;
    fread(&memory[ROM_ENTRY_POINT], 1, size, file);

    fclose(file);

    return size;
}
#ifdef _WIN32
byte mem_load_rom_from_file_w(const wchar_t *filename) {
    FILE *file;
    if (!(file = _wfopen(filename, L"rb"))) return 0;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (size > ROM_SIZE) size = ROM_SIZE;
    fread(&memory[ROM_ENTRY_POINT], 1, size, file);

    fclose(file);

    return size;
}
#endif

byte mem_read(byte address) { return memory[address]; }
void mem_write(byte address, byte value) { if (address < RAM_SIZE) memory[address] = value; }