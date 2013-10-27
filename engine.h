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
#define SNAKEBODY '*'


//#####################________________-----UNIVERSAL GAME STRUCTURE------__________________####################

struct gamestructure {
    char state;
    char speed;
    char difficulty;
    
    char map[SCREENWIDTH][SCREENHEIGHT];
};

extern struct gamestructure game;



//..################_________---FUNCTION DECLARATIONS---_________###############
void engineInit();
void engineDrawWalls();




//..################_________---ENGINE MACROS---_________###############
//Add character
#define engineAddChar(m_x, m_y, char) mvaddch(m_y, m_x, char)

//Add point
#define engineAddCharFromPoint(m_point, m_symbol) mvaddch(m_point.y, m_point.x, m_symbol)

#endif