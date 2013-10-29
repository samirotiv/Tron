/*
GAME ENGINE
    Handles the internal representation of the game & the graphical engine.
______________________________________________

FILE: engine.h

*/

#ifndef ENGINE_DEFINED
#define ENGINE_DEFINED


#define SCREENHEIGHT 24
#define SCREENWIDTH 80

#define WALL 'W'
#define SNAKEHEAD '@'
#define SNAKEBODY '#'


//#####################________________-----UNIVERSAL GAME STRUCTURE------__________________####################

struct gamestructure {
    char state;
    char speed;
    char difficulty;
    
    char map[SCREENWIDTH][SCREENHEIGHT];
};

#define GAMEDELAY 200000

extern struct gamestructure game;



//..################_________---FUNCTION DECLARATIONS---_________###############
void engineInit();
void engineDrawWalls();
void engineClearMap();
void engineStartGameEnvironment();



//..################_________---ENGINE MACROS---_________###############
//Add character
#define engineAddChar(m_x, m_y, char) mvaddch(m_y, m_x, char)

//Add point
#define engineAddCharFromPoint(m_point, m_symbol) mvaddch(m_point.y, m_point.x, m_symbol)

#endif





//.###___Queue Library___###
#define MAXQUEUESIZE 10

typedef struct queue_ {
    int data[MAXQUEUESIZE];
    int head;
    int tail;
    int size;
} queue;

void InitQueue (queue* q);
void enqueue (queue* q, int val);
int dequeue (queue* q);