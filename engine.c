/*
GAME ENGINE
    Handles the graphical engine functions.
______________________________________________

FILE: engine.c

*/

#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "engine.h"
#include "snake.h"
#include "ai.h"

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

    erase();
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


void engineProcessGameWinner(){
    //Handle the special case of head-to-head collisions
    if ((snake1.head.x == snake2.head.x) && (snake1.head.y == snake2.head.y)) snake1.alive = snake2.alive = 0;

    //Now declare the winner
    if (snake1.alive != 0) game.winner = 1;
    else if (snake2.alive != 0) game.winner = 2;
    else game.winner = 0;
}







void engineSleepAndCallBot(struct snakestructure* botsnakepointer, struct snakestructure* usrsnakepointer, long int usleeptime){
    struct timespec timeout;
    pthread_t aithread;
    int error = 0;
    void *exitstatus;
    
    //Set initial direction
    botsnakepointer->bot_newdirection = botsnakepointer->direction;
    
    //Format data
    struct snakestructure* snakes[] = {botsnakepointer, usrsnakepointer};
    
    //Start the aiProcessGame thread.
    doneflag = 0;
    pthread_create(&aithread, NULL, aiProcessGame, snakes);
    
    timeout.tv_sec = usleeptime / 1000000;
    timeout.tv_nsec = (usleeptime % 1000000) * 1000;
    nanosleep(&timeout, NULL);
    
    
    error = pthread_cancel(aithread);    
    error = pthread_join(aithread, &exitstatus);
    
    if (doneflag == 1) {enginePrintF (5, SCREENHEIGHT + 1, "THREAD FINISHED SUCCESSFULLY");}
    if (doneflag == 0) {enginePrintF (5, SCREENHEIGHT + 1, "THREAD ABORTED, IT SEEMS    ");}
    
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
