# Makefile

all:
	gcc -c engine.c -o engine.o -lncurses
	gcc -c snake.c -o snake.o -lncurses
	gcc main.c engine.o snake.o -o tron -lncurses

clean:
	rm -rf *.o
	rm -rf tron