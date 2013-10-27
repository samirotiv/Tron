/*
TRON GAME - MAIN
    Game Control File
______________________________________________

FILE: main.c

*/
#include <curses.h>

#include "engine.h"
#include "snake.h"


int main(){
//STRICTLY TEMPORARY - TESTING ONLY
    engineInit();
    engineDrawWalls();
    snakeInit();
    refresh();
    int c;
    
    while(player.alive == 1){
        switch (c = getch())
                {
                case ERR:
                    // If we get no input
                    break;

                case KEY_UP:
                    snakeUpdateDirection(player, UP);
                    break;

                case KEY_LEFT:
                    snakeUpdateDirection(player, LEFT);
                    break;

                case KEY_DOWN:
                    snakeUpdateDirection(player, DOWN);
                    break;

                case KEY_RIGHT:
                    snakeUpdateDirection(player, RIGHT);
                    break;

                case 'q':	case 'Q':
                    endwin();
                    exit(0);
                    break;

                default:
                    break;
                }
        snakeElongate (player);
        refresh();
    }
    endwin();
}