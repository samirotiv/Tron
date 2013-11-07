/*
ARTIFICIAL INTELLIGENCE
    The game AI.
______________________________________________

FILE: ai.c

//KILL VERSION

*/

#define botsnake (*botsnakepointer)
#define usrsnake (*usrsnakepointer)

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <string.h>

#include "engine.h"
#include "snake.h"
#include "ai.h"


//Initializing Mutex & Condition
pthread_mutex_t processing = PTHREAD_MUTEX_INITIALIZER;
int doneflag;

//Global Misc. Variables
int directions[] = {LEFT, UP, DOWN, RIGHT};

//TESTING ONLY
int test=1;
FILE* fp;



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
    struct snakestructure** snakes = (struct snakestructure**) data;
    struct snakestructure* botsnakepointer = (struct snakestructure*) snakes[0];
    struct snakestructure* usrsnakepointer = (struct snakestructure*) snakes[1];
    //Use 'botsnake' to dereference botsnakepointer.
    
    
	botsnake.bot_newdirection = aiMinimax(botsnakepointer, usrsnakepointer);
    
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


//Returns the best direction to go
//Use 'botsnake' to dereference botsnakepointer.
int aiMinimax(struct snakestructure* botsnakepointer, struct snakestructure* usrsnakepointer){
	//Start MINIMAX
   	struct future FG; 
	int i, j;
	
	/*leftscore = 2;
	straightscore = 10;
	rightscore = 9;*/
        	
    memcpy (FG.map, game.map, (SCREENWIDTH * SCREENHEIGHT * sizeof(char)));
    memcpy (&(FG.bot), botsnakepointer, sizeof(struct snakestructure));
    memcpy (&(FG.usr), usrsnakepointer, sizeof(struct snakestructure));
    
    int movesarray[2][3];
    j = 0;
    for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.bot, directions[i])) {
    		movesarray[0][j] = directions[i];
    		movesarray[1][j] = aiScore(FG, directions[i], 0);
    		j++;
    	}
    }

    fprintf(fp, "AT OUTER MINIMAX:\n");
    for (i=0; i<3; i++){
        fprintf(fp, "\tDirection=%d\tScore = %d\n", movesarray[0][i], movesarray[1][i]);
    }
    fprintf(fp, "SELECTED = Direction=%d\tScore = %d\n", movesarray[0][aiMaxOf3(movesarray[1])], movesarray[1][aiMaxOf3(movesarray[1])]);
	return movesarray[0][aiMaxOf3(movesarray[1])];
}






/*
 *Function to find the maximum of three integers
 *Returns random if integers are equal
 */
int aiMaxOf3 (int arr[]){
	int i,max;
	max=arr[0];
	if (arr[1]>=max) max=arr[1];
    if (arr[2]>max)	max=arr[2];
	if (max==arr[0])	return 0;
	if (max==arr[1])	return 1;
	if (max==arr[2])	return 2;
}





/*Recursive function to find score of a direction*/
int aiScore( struct future FG, int direction, int depth){
	snakeUpdateDirection (FG.bot, direction);
	aiElongate (FG.bot);
	
	if (FG.bot.alive == 0) return -1000;	
	
	int i;
	int j = 0;
	int negativesubscores[3];
	for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.usr, directions[i])) {
    		negativesubscores[i] = 0 - aiSubScore(FG, directions[i], depth + 1);
    		j++;
    	}
    }
    
    return (0 - negativesubscores[aiMaxOf3(negativesubscores)]);
}




/*Recursive function to find subscore of a direction*/
int aiSubScore( struct future FG, int direction, int depth){
	snakeUpdateDirection (FG.usr, direction);
	aiElongate (FG.usr);
	if (FG.usr.alive == 0) return 1000;	
	if (depth >= DIFFICULTY) {
	    //TESTING ONLY
	    //enginePrintF (5, SCREENHEIGHT + 7 + test, "              ");
	    //test = 1 - test;
        //enginePrintF (5, SCREENHEIGHT + 7 + test, "HIT DIFFICULTY");
        //refresh();
        
	    return 1;
    }
	
	int i;
    int j = 0;
	int scores[3];
	for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.bot, directions[i])) {
    		scores[j] = aiScore(FG, directions[i], depth);
    		j++;
    	}
    }
    
    return scores[aiMaxOf3(scores)];
}
