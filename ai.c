/*
ARTIFICIAL INTELLIGENCE
    The game AI.
______________________________________________

FILE: ai.c

*/

#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

#include "engine.h"
#include "snake.h"



//Initializing Mutex & Condition
pthread_mutex_t processing = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t donemutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t done = PTHREAD_COND_INITIALIZER;
int doneflag;




/*
*********************************************************************
FUNCTION: aiProcessGame
    Processes the game (and updates the global variable for new 
    direction.
    
    EXERCISE CAUTION: This thread might be killed at any time.
    
*********************************************************************
*/
void *aiProcessGame(void *data){
    int oldcanceltype;
    
    //Allow the thread to be killed at any time.
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldcanceltype);
    
    
    
    //DO OPERATIONS HERE
    //. . . . . . . . . .
    //struct snakestructure* botsnake = (struct snakestructure*) data;

/*FOR TESTING*/
        usleep(500000);
        enginePrintF (5, SCREENHEIGHT + 5, "Hello World :)");
        refresh();
        usleep(600000);
        enginePrintF (5, SCREENHEIGHT + 11, "Should not show :)");
        refresh();
    
    
    
    //AFTER PROCESSING:
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldcanceltype);
    pthread_cleanup_push(pthread_mutex_unlock, (void *) &processing);
    pthread_mutex_lock(&processing);
    
    doneflag = 1;
    pthread_cond_signal(&done);
    
    pthread_cleanup_pop(1);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldcanceltype);
    
    return NULL;
}




void aiProcessWithTimeout(struct snakestructure* botsnake, long int usleep){
    struct timespec abs_time;
    pthread_t aithread;
    int error = 0;
    
    //Lock the processing mutex
    pthread_mutex_lock(&processing);
    
    //Get the absolute time
    clock_gettime(CLOCK_REALTIME, &abs_time);
    abs_time.tv_sec += usleep / 1000000;
    abs_time.tv_nsec += (usleep % 1000000) * 1000;
    
    
    //Start the aiProcessGame thread.
    doneflag = 0;
    pthread_create(&aithread, NULL, aiProcessGame, botsnake);
    
    //Handle spurious returns
    //while (error == 0 || doneflag == 1) 
    error = pthread_cond_timedwait(&done, &processing, &abs_time);
    
    //Handling after timeout
//FOR TESTING
    //if (error == ETIMEDOUT) {pthread_cancel(aithread); enginePrintF (5, SCREENHEIGHT + 8, "TIMEOUT");}
    if (error == ETIMEDOUT) pthread_cancel(aithread);
    else if (error == 0) pthread_mutex_unlock(&processing);
    
}