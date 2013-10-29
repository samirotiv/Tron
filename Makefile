# Makefile

all: engine.c engine.h snake.c snake.h main.c
	gcc -c engine.c -o engine.o -lncurses
	gcc -c snake.c -o snake.o -lncurses
	gcc main.c engine.o snake.o -o tron -lncurses

clean:
	rm -rf *.o
	rm -rf tron