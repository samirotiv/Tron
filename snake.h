/*
SNAKE LAYER
    Snake Structure and Routines
______________________________________________

FILE: snake.h

*/

//..################_________---GLOBAL SNAKE STRUCTURE---_________###############
enum directions { UP = 0, LEFT, DOWN, RIGHT };

struct point {
    int x;
    int y;
}

struct snakestructure {
    char alive;
    
    struct point head;
    struct point position[10000]; //Insurance

    char direction;
    int size;
}

extern struct snakestructure player;
//extern struct snakestructure bot;




//..################_________---SNAKE MACROS---_________###############