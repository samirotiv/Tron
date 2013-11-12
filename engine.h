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
#define MAXSQUARES (SCREENHEIGHT * SCREENWIDTH)

#define WALL 'W'
#define SNAKEHEAD '@'
#define SNAKEBODY '#'


//ALL GAME COLORCODES
#define SNAKE1_COLORCODE 1
#define SNAKE2_COLORCODE 2
#define WALL_COLORCODE 3
#define OUTSIDE_COLORCODE 4
#define SNAKE1HEAD_COLORCODE 5
#define SNAKE2HEAD_COLORCODE 6

#define COLOR_GREY 8


//#####################________________-----UNIVERSAL GAME STRUCTURE------__________________####################

struct gamestructure {
    char state;
    char speed;
    int difficulty;
    int depth;
    char winner;
    
    char map[SCREENWIDTH][SCREENHEIGHT];
};

//Game States
#define SINGLEPLAYERGAME 1
#define TWOPLAYERGAME 2
#define PAUSED 3
#define MAIN_MENU 4
#define END_MENU 5


//TODO: CREATE A FUNCTION
#define GAMEDELAY 150000
#define MAXKEYBUFFERSIZE 3
#define ENGINETIMEOUTCOUNTER 30

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
