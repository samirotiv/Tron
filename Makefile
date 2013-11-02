# Makefile

all: engine.c engine.h snake.c snake.h menu.c menu.h main.c ai.h ai.c
	gcc -c engine.c -o engine.o -lncurses
	gcc -c snake.c -o snake.o -lncurses
	gcc -c menu.c -o menu.o -lncurses
	gcc -c ai.c -o ai.o -lncurses
	gcc main.c engine.o snake.o menu.o ai.o -o tron -lncurses -lpthread -lrt

clean:
	rm -rf *.o
	rm -rf tron
