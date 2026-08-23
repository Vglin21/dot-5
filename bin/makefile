flags = -s -O2 -flto
program = dot-5

all:
	gcc $(flags) -I ../include -o $(program) ../src/*.c -L ../lib -lSDL3

spa:
	gcc $(flags) -I ../include -o spa ../src/spa/spa.c