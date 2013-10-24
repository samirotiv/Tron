/*
TRON GAME
    Game structures and routines
______________________________________________

FILE: snake.h

*/

#define MAXSCREENHEIGHT 100
#define MAXSCREENWIDTH 100


//#####################________________-----UNIVERSAL GAME STRUCTURE------__________________####################

struct gamestructure {
    char state;
    char speed;
    char difficulty;
    
    char map[MAXSCREENHEIGHT][MAXSCREENWIDTH];
    
    char borders;
}

extern struct gamestructure game;
