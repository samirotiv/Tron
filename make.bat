del *.o
del tron.exe
gcc -c -g engine.c -o engine.o
gcc -c -g snake.c -o snake.o
gcc -c -g menu.c -o menu.o
gcc -c -g ai.c -o ai.o
gcc -g main.c engine.o snake.o menu.o ai.o -o tron -lpdcurses -lpthread
