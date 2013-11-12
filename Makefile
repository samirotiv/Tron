# Makefile

all: engine.c engine.h snake.c snake.h menu.c menu.h ai.h ai.c main.c
	gcc -c -g engine.c -o engine.o
	gcc -c -g snake.c -o snake.o
	gcc -c -g menu.c -o menu.o
	gcc -c -g ai.c -o ai.o
	gcc -g main.c engine.o snake.o menu.o ai.o -o tron -lncurses -lpthread

clean:
	rm -rf *.o
	rm -rf tron
