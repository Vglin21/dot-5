flags = -s -O2 -flto
program = dot-5

all:
	windres ../src/dot-5.rc -O coff -o dot-5.res
	gcc $(flags) -I ../include -o $(program) ../src/*.c dot-5.res -L ../lib -lSDL3 -mwindows
	del dot-5.res

debug:
	windres ../src/dot-5.rc -O coff -o dot-5.res
	gcc $(flags) -I ../include -o $(program) ../src/*.c -L ../lib -lSDL3
	del dot-5.res

spa:
	gcc $(flags) -I ../include -o spa ../src/spa/spa.c