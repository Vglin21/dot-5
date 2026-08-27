flags = -s -O2 -flto
program = dot-5

all: dot-5 d5asm

dot-5:
	windres src/dot-5.rc -O coff -o build/dot-5.res
	gcc $(flags) -I include -o bin/$(program) src/*.c build/dot-5.res -L lib -lSDL3 -mwindows

dot-5-debug:
	windres src/dot-5.rc -O coff -o build/dot-5.res
	gcc $(flags) -I include -o bin/$(program) src/*.c -L lib -lSDL3

d5asm:
	gcc $(flags) -I include -o bin/d5asm src/d5asm/d5asm.c