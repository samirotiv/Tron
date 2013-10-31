/*
GAME ENGINE
    Handles the graphical engine functions.
______________________________________________

FILE: engine.c

*/

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include "engine.h"

struct gamestructure game;


/*
###############################__________---MAJOR FUNCTIONS---__________###################################
*/

/*
# FUNCTION engineInit
#     - Initialize NCURSES
#     - Set I/O Options
#     - Ensure screen size
*/
void engineInit(){
    (void) initscr();               //Initializing NCURSES

    int current_height, current_width;
	// Gets the current width and height of the terminal
	getmaxyx (stdscr, current_height, current_width);

	if ((current_width < SCREENWIDTH) || (current_height < SCREENHEIGHT)){
        endwin();
        printf ("Your console screen is smaller than 80x24\nPlease resize your window and try again\n\n");
        exit(0);
    }
    
    /* SETTING UNIVERSAL I/O OPTIONS */
    raw();                          //Immediately record keystroke & don't interpret control characters
    noecho();                       //Disable Echo
    keypad(stdscr, TRUE);           //Enable input of control keys
    curs_set(0);                    //Disable cursor
    
    //KEY DELAY: PROGRAM STATE DEPENDENT
    nodelay(stdscr, TRUE);          //Disable waiting for user input
    
    refresh();                      //First refresh of screen

}




void engineStartGameEnvironment(){
    nodelay(stdscr, TRUE);
    engineClearMap();
    engineDrawWalls();
    snakeInit();

    refresh();
    flushinp();

    return;
}


void engineStartMenuEnvironment(){
    nodelay(stdscr, FALSE);

    refresh();
    flushinp();

    return;
}




void engineDrawWalls(){
    //Clear screen
    erase();

    //Draw vertical walls
    int y;
    for (y=0; y<SCREENHEIGHT; y++){
        engineAddChar (0, y, WALL);
        game.map[0][y] = WALL;
        
        engineAddChar (SCREENWIDTH - 1, y, WALL);
        game.map[SCREENWIDTH-1][y] = WALL;
    }

    //Draw horizontal walls
    int x;
    for (x=0; x<SCREENWIDTH; x++){
        engineAddChar (x, 0, WALL);
        game.map[x][0] = WALL;
        
        engineAddChar (x, SCREENHEIGHT - 1, WALL);
        game.map[x][SCREENHEIGHT-1] = WALL;
    }

    return;
}



void engineClearMap(){
    int i, j;
    for (i=0; i<SCREENWIDTH; i++){
        for (j=0; j<SCREENHEIGHT; j++){
            game.map[i][j] = 0;
        }
    }
}
        
    
 


//..######-----BASIC QUEUE LIBRARY: FOR RESPONSIVE INPUT IN 2 PLAYER GAMES-----######
void InitQueue (queue* q){
    q->size = q->front = q->behindback = 0;
}

void enqueue (queue* q, int val){
    if ((q->size == 0) || (q->behindback != q->front)){
        q->size++;
        q->data[q->behindback] = val;
        q->lastenqueued = val;
        q->behindback = (q->behindback + 1) % MAXQUEUESIZE;
    }
}

int dequeue (queue* q){
    if (q->size > 0){
        q->size--;
        int returnvalue = q->data[q->front];
        q->front = (q->front + 1) % MAXQUEUESIZE;
        return returnvalue;
    }
    else return 0;
}
