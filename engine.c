/*
GAME ENGINE
    Handles the graphical engine functions.
______________________________________________

FILE: engine.c

*/

#include <curses.h>


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

/*TODO: SCREEN SIZE CHECKING*/
    
    /* SETTING I/O OPTIONS */
    raw();                          //Immediately record keystroke & don't interpret control characters
    noecho();                       //Disable Echo
    keypad(stdscr, TRUE);           //Enable input of control keys
    curs_set(0);                    //Disable cursor
    nodelay(stdscr, TRUE);          //Disable waiting for user input
    
    refresh();                      //First refresh of screen

}
