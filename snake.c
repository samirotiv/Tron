/*
SNAKE LAYER
    Snake Routines
______________________________________________

FILE: snake.c

*/

#include "snake.h"
#include "engine.h"

struct snakestructure player;
struct snakestructure bot;

void snakeInit(){
//STRICTLY TEMPORARY
    player.size = 1;
    player.alive = 1;
    SetPoint (player.head, 5, 12);
    EquatePoint (player.position[0], player.head);
    player.marker = 'H';
    game.map[player.head.x][player.head.y] = player.marker;
    player.direction = RIGHT;
    
    engineAddCharFromPoint (player.head, SNAKEHEAD);
}