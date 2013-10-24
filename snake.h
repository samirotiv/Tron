/*
SNAKE LAYER
    Snake Structure and Routines
______________________________________________

FILE: snake.h

*/

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