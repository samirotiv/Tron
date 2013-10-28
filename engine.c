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
    
    /* SETTING I/O OPTIONS */
    raw();                          //Immediately record keystroke & don't interpret control characters
    noecho();                       //Disable Echo
    keypad(stdscr, TRUE);           //Enable input of control keys
    curs_set(0);                    //Disable cursor
    
    //STRICTLY TEMPORARY - TESTING ONLY
    //nodelay(stdscr, TRUE);          //Disable waiting for user input
    nodelay(stdscr, FALSE);          //Enable waiting for user input
    
    refresh();                      //First refresh of screen

}



void engineDrawWalls(){
    int y;
    for (y=0; y<SCREENHEIGHT; y++){
        engineAddChar (0, y, WALL);
        game.map[0][y] = WALL;
        
        engineAddChar (SCREENWIDTH - 1, y, WALL);
        game.map[SCREENWIDTH-1][y] = WALL;
    }

    int x;
    for (x=0; x<SCREENWIDTH; x++){
        engineAddChar (x, 0, WALL);
        game.map[x][0] = WALL;
        
        engineAddChar (x, SCREENHEIGHT - 1, WALL);
        game.map[x][SCREENHEIGHT-1] = WALL;
    }

    refresh();
    return;
}



void engineClearMap(){
    erase();
    refresh();    

    int i, j;
    for (i=0; i<SCREENWIDTH; i++){
        for (j=0; j<SCREENHEIGHT; j++){
            game.map[i][j] = 0;
        }
    }
}
        
    
    