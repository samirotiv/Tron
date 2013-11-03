/*
ARTIFICIAL INTELLIGENCE
    The game AI.
______________________________________________

FILE: ai.c

//KILL VERSION

*/

#define botsnake (*botsnakepointer)

#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "engine.h"
#include "snake.h"



//Initializing Mutex & Condition
pthread_mutex_t processing = PTHREAD_MUTEX_INITIALIZER;
int doneflag;
int newdirection;



/*
*********************************************************************
FUNCTION: aiProcessGame
    Processes the game (and updates the global variable for new 
    direction.
    
    EXERCISE CAUTION: This thread might be killed at any time.
    To create a block of code that executes completely, change cancel
    type to DEFERRED and ensure that no statement is a CANCELLATION
    POINT.
    
    (Refer to pthread man pages for a list of CANCELLATION POINTS)
    
*********************************************************************
*/
void *aiProcessGame(void *data){    
    //Allow the thread to be killed at any time.
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    
    
    //DO OPERATIONS HERE
    //. . . . . . . . . .
    struct snakestructure* botsnakepointer = (struct snakestructure*) data;
    //sleep(2);
    int dirarr[] = {UP, DOWN, LEFT, RIGHT};
    int i;
    for (i=0; i<4; i++){
        if (game.map[botsnake.head.x + (dirarr[i] % 2)][botsnake.head.y + (dirarr[i] / 2)] == 0){
            enginePrintF (5, SCREENHEIGHT + 2, "COAST IS CLEAR");
            botsnake.bot_newdirection = dirarr[i];
            break;
        }
    }
    

    
    //After Processing:
    //Ensure that a cancel doesn't take place here.
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_cleanup_push(pthread_mutex_unlock, (void *) &processing);
    pthread_mutex_lock(&processing);
    
    doneflag = 1;
    
    pthread_cleanup_pop(1);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    return NULL;
}




void aiProcessAndSleep(struct snakestructure* botsnakepointer, long int usleeptime){
    struct timespec timeout;
    pthread_t aithread;
    int error = 0;
    void *exitstatus;
    
    //Set initial direction
    botsnake.bot_newdirection = botsnake.direction;
    
    //Start the aiProcessGame thread.
    doneflag = 0;
    pthread_create(&aithread, NULL, aiProcessGame, botsnakepointer);
    
    timeout.tv_sec = usleeptime / 1000000;
    timeout.tv_nsec = (usleeptime % 1000000) * 1000;
    nanosleep(&timeout, NULL);
    
    
    error = pthread_cancel(aithread);    
    error = pthread_join(aithread, &exitstatus);
    
    if (doneflag == 1) {enginePrintF (5, SCREENHEIGHT + 1, "THREAD FINISHED SUCCESSFULLY");}
    if (doneflag == 0) {enginePrintF (5, SCREENHEIGHT + 1, "THREAD ABORTED, IT SEEMS    ");}
    
}