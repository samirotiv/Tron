/*
SNAKE LAYER
    Snake Routines
______________________________________________

FILE: snake.c

*/

#include "snake.h"
#include "engine.h"

struct snakestructure snake1;
struct snakestructure snake2;

void snakeInit(){
    snake1.size = snake1.alive = snake2.size = snake2.alive = 1;

    SetPoint (snake1.head, SNAKE1_STARTX, SNAKE1_STARTY);
    SetPoint (snake2.head, SNAKE2_STARTX, SNAKE2_STARTY);
    
    EquatePoint (snake1.position[0], snake1.head);
    EquatePoint (snake2.position[0], snake2.head);

    snake1.marker = SNAKE1_MARKER;
    snake2.marker = SNAKE2_MARKER;

    game.map[snake1.head.x][snake1.head.y] = snake1.marker;
    game.map[snake2.head.x][snake2.head.y] = snake2.marker;

    snake1.direction = SNAKE1_DIRECTION;
    snake2.direction = SNAKE2_DIRECTION;
    
    engineAddCharFromPoint (snake1.head, SNAKEHEAD);
    engineAddCharFromPoint (snake2.head, SNAKEHEAD);
}