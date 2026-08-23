#ifndef __MEMORY_MAP_H_
#define __MEMORY_MAP_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef __CUSTOM_TYPES_
#define __CUSTOM_TYPES_

typedef uint8_t byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;

#endif

extern byte mem_load_rom(byte *rom, byte size);
extern byte mem_load_rom_from_file(const char *filename);

extern byte mem_read(byte address);
extern void mem_write(byte address, byte value);

#endif