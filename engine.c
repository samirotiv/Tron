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
	
	//Initialize colors
	start_color();
	

	if ((current_width < SCREENWIDTH) || (current_height < SCREENHEIGHT) || (SCREENHEIGHT < 24) || (SCREENWIDTH < 80)){
        endwin();
        printf ("Your console screen is smaller than %d x %d\nPlease resize your window and try again\nPlease note, the set SCREENWIDTH and SCREENHEIGHT in engine.h must be greater than 24x80\n", SCREENWIDTH, SCREENHEIGHT);
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



/*
*********************************************************************
FUNCTION: engineStartGameEnvironment
    Sets the input options and sets the game environment (drawing
    walls, etc.)

*********************************************************************
*/
void engineStartGameEnvironment(){
    nodelay(stdscr, TRUE);
    engineClearMap();
    engineDrawWalls();
    snakeInit();
    game.depth = game.difficulty;
    //init_color(COLOR_GREY, 100, 100, 100);
    
    //Initialize game colours
    init_pair(SNAKE1_COLORCODE, COLOR_BLUE, COLOR_BLUE);
    init_pair(SNAKE2_COLORCODE, COLOR_RED, COLOR_RED);
    init_pair(SNAKE1HEAD_COLORCODE, COLOR_BLACK, COLOR_BLACK);
    init_pair(SNAKE2HEAD_COLORCODE, COLOR_BLACK, COLOR_BLACK);
    init_pair(WALL_COLORCODE, COLOR_BLACK, COLOR_BLACK);
    init_pair(OUTSIDE_COLORCODE, COLOR_BLACK, COLOR_WHITE);

    refresh();
    flushinp();

    return;
}

/*
*********************************************************************
FUNCTION: engineStartMenuEnvironment
    Sets the input options and sets the menu environment.

*********************************************************************
*/
void engineStartMenuEnvironment(){
    nodelay(stdscr, FALSE);

    erase();
    refresh();
    flushinp();

    return;
}



/*
*********************************************************************
FUNCTION: engineDrawWalls
    Draws game walls and updates internal game map

*********************************************************************
*/
void engineDrawWalls(){
    //Clear screen
    attrset(COLOR_PAIR(OUTSIDE_COLORCODE));
    erase();
    bkgd(COLOR_PAIR(OUTSIDE_COLORCODE));
    
    attrset(COLOR_PAIR(WALL_COLORCODE));
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


/*
*********************************************************************
FUNCTION: engineClearMap
    Clears the internal game map

*********************************************************************
*/
void engineClearMap(){
    int i, j;
    for (i=0; i<SCREENWIDTH; i++){
        for (j=0; j<SCREENHEIGHT; j++){
            game.map[i][j] = 0;
        }
    }
}

/*
*********************************************************************
FUNCTION: engineProcessGameWinner
    Processes the game and sets the winner or checks if it's a draw

*********************************************************************
*/
void engineProcessGameWinner(){
    //Handle the special case of head-to-head collisions
    if ((snake1.head.x == snake2.head.x) && (snake1.head.y == snake2.head.y)) snake1.alive = snake2.alive = 0;

    //Now declare the winner
    if (snake1.alive != 0) game.winner = 1;
    else if (snake2.alive != 0) game.winner = 2;
    else game.winner = 0;
}






/*
*********************************************************************
FUNCTION: engineSleepAndCallBot
    Spawns a seperate thread amd executes the bot function in this
    thread and kills it when the game timer elapses

*********************************************************************
*/
void engineSleepAndCallBot(struct snakestructure* botsnakepointer, struct snakestructure* usrsnakepointer, long int usleeptime){
    struct timespec timeout;
    pthread_t aithread;
    int error = 0;
    void *exitstatus;
    
    //Dynamic depth
    static int counter;
    
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
    
    if (doneflag == 1) {
        if (counter > 0) counter--;
        else if (game.depth < game.difficulty) game.depth++;
    }
    if (doneflag == 0) {
        counter = ENGINETIMEOUTCOUNTER;
        game.depth--;
    }
    
    return;
}


    
 


//..######-----BASIC QUEUE LIBRARY: FOR RESPONSIVE INPUT IN 2 PLAYER GAMES-----######
void InitQueue (queue* q, int* array, int arraysize){
    q->size = q->front = q->behindback = 0;
    q->maxqueuesize = arraysize;
    if (array != NULL){
        q->data = array;
    }
    else{
        q->data = (int*) malloc ((q->maxqueuesize + 1) * sizeof(int));
    }
}

void enqueue (queue* q, int val){
    if ((q->size == 0) || (q->behindback != q->front)){
        q->size++;
        q->data[q->behindback] = val;
        q->lastenqueued = val;
        q->behindback = (q->behindback + 1) % q->maxqueuesize;
    }
}

int dequeue (queue* q){
    if (q->size > 0){
        q->size--;
        int returnvalue = q->data[q->front];
        q->front = (q->front + 1) % q->maxqueuesize;
        return returnvalue;
    }
    else return 0;
}


void FreeQueue (queue* q){
    free(q->data);
}
