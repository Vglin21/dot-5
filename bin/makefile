flags = -s -O2 -flto
program = dot-5

all: dot-5 d5asm

dot-5:
	windres ../src/dot-5.rc -O coff -o dot-5.res
	gcc $(flags) -I ../include -o $(program) ../src/*.c dot-5.res -L ../lib -lSDL3 -mwindows
	del dot-5.res

dot-5-debug:
	windres ../src/dot-5.rc -O coff -o dot-5.res
	gcc $(flags) -I ../include -o $(program) ../src/*.c -L ../lib -lSDL3
	del dot-5.res

d5asm:
	gcc $(flags) -I ../include -o d5asm ../src/d5asm/d5asm.c