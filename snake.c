/*
SNAKE LAYER
    Snake Routines
______________________________________________

FILE: snake.c

*/

#include "snake.h"
#include "engine.h"
#include "curses.h"

struct snakestructure snake1;
struct snakestructure snake2;


/*
*********************************************************************
FUNCTION: snakeInit
    Initialises the positions and colors and details of the snakes

*********************************************************************
*/
void snakeInit(){
    snake1.size = snake1.alive = snake2.size = snake2.alive = 1;

    SetPoint (snake1.head, SNAKE1_STARTX, SNAKE1_STARTY);
    SetPoint (snake2.head, SNAKE2_STARTX, SNAKE2_STARTY);

    snake1.marker = SNAKE1_MARKER;
    snake2.marker = SNAKE2_MARKER;
    
    snake1.colorcode = SNAKE1_COLORCODE;
    snake2.colorcode = SNAKE2_COLORCODE;
    snake1.headcolorcode = SNAKE1HEAD_COLORCODE;
    snake2.headcolorcode = SNAKE2HEAD_COLORCODE;

    game.map[snake1.head.x][snake1.head.y] = snake1.marker;
    game.map[snake2.head.x][snake2.head.y] = snake2.marker;

    snake1.direction = snake1.bot_newdirection = SNAKE1_STARTDIRECTION;
    snake2.direction = snake2.bot_newdirection = SNAKE2_STARTDIRECTION;
    
    engineAddCharFromPoint (snake1.head, SNAKEHEAD);
    engineAddCharFromPoint (snake2.head, SNAKEHEAD);
}
