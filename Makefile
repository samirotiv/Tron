# Makefile

all: engine.c engine.h snake.c snake.h menu.c menu.h ai.h ai.c main.c
	gcc -c -g engine.c -o engine.o -lncurses
	gcc -c -g snake.c -o snake.o -lncurses
	gcc -c -g menu.c -o menu.o -lncurses
	gcc -c -g ai.c -o ai.o -lncurses -lpthread
	gcc -g main.c engine.o snake.o menu.o ai.o -o tron -lncurses -lpthread -lrt

clean:
	rm -rf *.o
	rm -rf tron
