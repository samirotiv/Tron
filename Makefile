# Makefile

all: engine.c engine.h snake.c snake.h menu.c menu.h main.c
	gcc -c engine.c -o engine.o -lncurses
	gcc -c snake.c -o snake.o -lncurses
	gcc -c menu.c -o menu.o -lncurses
	gcc main.c engine.o snake.o menu.o -o tron -lncurses

clean:
	rm -rf *.o
	rm -rf tron
