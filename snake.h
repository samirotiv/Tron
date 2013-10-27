/*
SNAKE LAYER
    Snake Structure and Routines
______________________________________________

FILE: snake.h

*/

//..################_________---GLOBAL SNAKE STRUCTURE---_________###############
#define UP -2
#define DOWN 2
#define LEFT -1
#define RIGHT 1

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
    struct point position[10000]; //Insurance

    int direction;
    int size;
    
    char marker;
};


extern struct snakestructure player;
extern struct snakestructure bot;



//..################_________---SNAKE FUNCTIONS---_________###############
void snakeInit();



//..################_________---SNAKE MACROS---_________###############
//TODO: TAKE CARE OF GOING OUT OF BOUNDARY

//Update direction of snake
#define snakeUpdateDirection(macro_snake, macro_direction)  if (macro_snake.direction + macro_direction != 0) {macro_snake.direction = macro_direction;}

//Elongate snake    (WARNING: DOES NOT CHECK IF INPUT IS OUT OF RANGE)
#define snakeElongate(m_snake) {                                                            \
                    engineAddCharFromPoint (m_snake.head, SNAKEBODY);                       \
                    MovePoint (m_snake.head, m_snake.direction % 2, m_snake.direction / 2); \
                                                                                            \
                    if (game.map[m_snake.head.x][m_snake.head.y] != 0) {                    \
                        m_snake.alive = 0;                                                  \
                        }                                                                   \
                                                                                            \
                    game.map[m_snake.head.x][m_snake.head.y] = m_snake.marker;             \
                    engineAddCharFromPoint (m_snake.head, SNAKEHEAD);                       \
                    m_snake.size++;                                                         \
                                                                                            \
                    EquatePoint (m_snake.position[m_snake.size - 1], m_snake.head);         \
                }
                
                