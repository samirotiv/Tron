/*
SNAKE LAYER
    Snake Structure and Routines
______________________________________________

FILE: snake.h

*/

#ifndef SNAKE_DEFINED
#define SNAKE_DEFINED

#include "engine.h"

//..################_________---GLOBAL SNAKE STRUCTURE---_________###############
#define UP -2
#define DOWN 2
#define LEFT -1
#define RIGHT 1

#define SNAKE1_STARTX 4
#define SNAKE2_STARTX (SCREENWIDTH - 5)
#define SNAKE1_STARTY (SCREENHEIGHT/2)
#define SNAKE2_STARTY (SCREENHEIGHT/2)
#define SNAKE1_STARTDIRECTION RIGHT
#define SNAKE2_STARTDIRECTION LEFT
#define SNAKE1_MARKER 1
#define SNAKE2_MARKER 2

//Maxsnakesize = Half of number of squares
#define MAXSNAKESIZE (MAXSQUARES / 2)

struct point {
    int x;
    int y;
};

#define SetPoint(m_point, m_x, m_y) m_point.x = m_x; m_point.y = m_y
#define MovePoint(m_point, m_x, m_y) m_point.x += m_x; m_point.y += m_y
#define EquatePoint(m_point1, m_point2) m_point1.x = m_point2.x; m_point1.y = m_point2.y

struct snakestructure {
    char alive;
    
    struct point head;

    int direction;
    int size;
    
    //Use this in the internal game map
    char marker;
    char colorcode;
    char headcolorcode;
    
    //For BOT
    int bot_newdirection;
};


extern struct snakestructure snake1;
extern struct snakestructure snake2;



//..################_________---SNAKE FUNCTIONS---_________###############
void snakeInit();



//..################_________---SNAKE MACROS---_________###############
#define snakeIsDirectionAllowed(macro_snake, macro_direction) (macro_snake.direction + macro_direction != 0)

//Update direction of snake
#define snakeUpdateDirection(macro_snake, macro_direction)  if (macro_snake.direction + macro_direction != 0) {macro_snake.direction = macro_direction;}

//Elongate snake    (WARNING: DOES NOT CHECK IF INPUT IS OUT OF RANGE)
#define snakeElongate(m_snake) {                                                            \
                    attrset(COLOR_PAIR(m_snake.colorcode));                                 \
                    engineAddCharFromPoint (m_snake.head, SNAKEBODY);                       \
                    MovePoint (m_snake.head, m_snake.direction % 2, m_snake.direction / 2); \
                                                                                            \
                    if (game.map[m_snake.head.x][m_snake.head.y] != 0) {                    \
                        m_snake.alive = 0;                                                  \
                        }                                                                   \
                                                                                            \
                    game.map[m_snake.head.x][m_snake.head.y] = m_snake.marker;             \
                    attrset(COLOR_PAIR(m_snake.headcolorcode));                             \
                    engineAddCharFromPoint (m_snake.head, SNAKEHEAD);                       \
                    m_snake.size++;                                                         \
                                                                                            \
                }
                

#endif        
