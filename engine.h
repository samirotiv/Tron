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
//Max squares = SCRHEIGHT * SCRWIDTH
#define MAXSQUARES 2000

#define WALL 'W'
#define SNAKEHEAD '@'
#define SNAKEBODY '#'


//#####################________________-----UNIVERSAL GAME STRUCTURE------__________________####################

struct gamestructure {
    char state;
    char speed;
    char difficulty;
    char winner;
    
    char map[SCREENWIDTH][SCREENHEIGHT];
};

#define GAMEDELAY 200000
#define MAXKEYBUFFERSIZE 3

#define PLAYER1 1
#define PLAYER2 2
#define DRAW 0

extern struct gamestructure game;



//..################_________---FUNCTION DECLARATIONS---_________###############
void engineInit();
void engineDrawWalls();
void engineClearMap();
void engineStartGameEnvironment();
void engineStartMenuEnvironment();
void engineProcessGameWinner();
//void engineSleepAndCallBot(struct snakestructure*, long int);



//..################_________---ENGINE MACROS---_________###############
//Writing onto screen
#define engineAddChar(m_x, m_y, m_char)  mvaddch  (m_y, m_x, m_char)
#define engineAddStr(m_x, m_y, m_str)    mvaddstr (m_y, m_x, m_str)
#define enginePrintF(m_x, m_y, ...)      mvprintw (m_y, m_x, __VA_ARGS__)

//Add point
#define engineAddCharFromPoint(m_point, m_symbol) mvaddch(m_point.y, m_point.x, m_symbol)



//.###___Queue Library___###
#define MAXQUEUESIZE 10


typedef struct queue_ {
    int* data;
    int maxqueuesize;
    int front;
    int behindback;
    int lastenqueued;
    int size;
} queue;

void InitQueue (queue* q, int* array, int arraysize);
void enqueue (queue* q, int val);
int dequeue (queue* q);
void FreeQueue (queue* q);



#endif
