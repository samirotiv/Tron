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
int directions[] = {LEFT, UP, RIGHT, DOWN};

//Djikstra's Algorithm Variables
queue dijk_unvisited_x, dijk_unvisited_y;
int dijk_unvisited_x_arr[MAXSQUARES], dijk_unvisited_y_arr[MAXSQUARES];
int dijk_seq[4] = {0, 0, 1, -1}; 

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
        	
    memcpy (FG.map, game.map, (SCREENWIDTH * SCREENHEIGHT * sizeof(char)));
    memcpy (&(FG.bot), botsnakepointer, sizeof(struct snakestructure));
    memcpy (&(FG.usr), usrsnakepointer, sizeof(struct snakestructure));
    
    int movesarray[2][3];   //There are going to be 3 allowed directions
    j = 0;  //Incremented every time an allowed direction is encountered
    for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.bot, directions[i])) {
    		movesarray[0][j] = directions[i];
    		movesarray[1][j] = aiScore(FG, directions[i], 0);
    		j++;
    	}
    }

    //TESTING
    
    fprintf(fp, "AT OUTER MINIMAX:\n");
    for (i=0; i<3; i++){
        fprintf(fp, "\tDirection=%d\tScore = %d\n", movesarray[0][i], movesarray[1][i]);
    }
    fprintf(fp, "SELECTED = Direction=%d\tScore = %d\n", movesarray[0][aiMaxOf3(movesarray[1])], movesarray[1][aiMaxOf3(movesarray[1])]);
    

	

    return movesarray[0][aiMaxOf3(movesarray[1])];
}





/*Recursive function to find score of a direction*/
int aiScore( struct future FG, int direction, int depth){
	snakeUpdateDirection (FG.bot, direction);
	aiElongate (FG.bot);
	
	if (FG.bot.alive == 0) return -100000;	
	
	int i;
	int j = 0;
	int negativesubscores[3];
	for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.usr, directions[i])) {
    		negativesubscores[j] = 0 - aiSubScore(FG, directions[i], depth + 1);
    		j++;
    	}
    }


    //TESTING
    /*
    fprintf(fp, "AT AiSCORE Depth = %d:\n", depth);
    for (i=0; i<3; i++){
        fprintf(fp, "\tSubScore = %d\n", negativesubscores[i], negativesubscores[i]);
    }
    fprintf(fp, "SELECTED SubScore = %d\n", 0 - negativesubscores[aiMaxOf3(negativesubscores)]);*/
    
    return (0 - negativesubscores[aiMaxOf3(negativesubscores)]);
}




/*Recursive function to find subscore of a direction*/
int aiSubScore( struct future FG, int direction, int depth){
	snakeUpdateDirection (FG.usr, direction);
	aiElongate (FG.usr);
	if (FG.usr.alive == 0) return 100000;	
	if (depth >= DIFFICULTY) {
        int score = aiVoronoi(&FG);
	    //TESTING ONLY
        //TESTING
        
        fprintf(fp, "AT AiSUBSCORE Depth = %d: (DIFFICULTY HIT)\n", depth);
        fprintf(fp, "VORONOI Score = %d\n", score);
        
        //TEMPORARY - might be changed
	    return score;
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
    
    
    //TESTING
    /*
    fprintf(fp, "AT AiSUBSCORE Depth = %d:\n", depth);
    for (i=0; i<3; i++){
        fprintf(fp, "\tScore = %d\n", scores[i], scores[i]);
    }
    fprintf(fp, "SELECTED Score = %d\n", scores[aiMaxOf3(scores)]);*/

    return scores[aiMaxOf3(scores)];
}





int aiVoronoi(struct future* FGptr){  
    int x, y;
    aiDijkstra(FGptr->map, FGptr->botdistancemap, FGptr->bot.head.x, FGptr->bot.head.y);
    aiDijkstra(FGptr->map, FGptr->usrdistancemap, FGptr->usr.head.x, FGptr->usr.head.y);
    
    int result = 0;
    int sign;
    
    for (y=0; y<SCREENHEIGHT; y++){
        for (x=0; x<SCREENWIDTH; x++){
            //FGptr->botdistancemap[x][y]
            //FGptr->usrdistancemap[x][y]
            if (FGptr->botdistancemap[x][y] != FGptr->usrdistancemap[x][y]) {
                sign = (2 * ((FGptr->botdistancemap[x][y] > 0) && (FGptr->usrdistancemap[x][y] > 0))) - 1;
                result += sign * (2 * (FGptr->botdistancemap[x][y] < FGptr->usrdistancemap[x][y])) - 1;
            }
        }
    }
    
    
    //FOR TESTING
    /*
    fprintf(fp, "AIVORONOI CALLED\n");    
    for (y=0; y<SCREENHEIGHT; y++){
        for (x=0; x<SCREENWIDTH; x++){
            //FGptr->botdistancemap[x][y]
            //FGptr->usrdistancemap[x][y]
            if (FGptr->botdistancemap[x][y] != FGptr->usrdistancemap[x][y]) {
                sign = (2 * ((FGptr->botdistancemap[x][y] > 0) && (FGptr->usrdistancemap[x][y] > 0))) - 1;
                fprintf(fp, " %3d ", sign * (2 * (FGptr->botdistancemap[x][y] < FGptr->usrdistancemap[x][y])) - 1);
            }
            else fprintf(fp, " %3d ", 0);
        }
        fprintf(fp, "\n\n");
    }
    fprintf(fp, "\n\n\n\n\n\n\n\n");
    */
    
    //Temporarily just returning - might save this somewhere.
    return result;
}



//For a map of dimension SCREENWIDTH x SCREENHEIGHT, with walls as borders.
void aiDijkstra(char map[SCREENWIDTH][SCREENHEIGHT], int distance[SCREENWIDTH][SCREENHEIGHT], int start_x, int start_y){
    //Note that -1 is a distance of infinity
    int x, y;
    int i, j, k;
    
    //Initialize distance to infinity
    for (x=0; x<SCREENWIDTH; x++){
        for (y=0; y<SCREENHEIGHT; y++){
            distance[x][y] = -1;
        }
    }

    //(Dijkstra declartions made global to reuse variables)
    //Clearing queue
    InitQueue (&dijk_unvisited_x, dijk_unvisited_x_arr, MAXSQUARES);
    InitQueue (&dijk_unvisited_y, dijk_unvisited_y_arr, MAXSQUARES);
    
    enqueue (&dijk_unvisited_x, start_x);
    enqueue (&dijk_unvisited_y, start_y);
    distance[start_x][start_y] = 0;
    char temp = map[start_x][start_y];
    map[start_x][start_y] = 0;

       
    while(dijk_unvisited_x.size > 0){
        //Pick a vertex    
        x = dequeue (&dijk_unvisited_x);
        y = dequeue (&dijk_unvisited_y);
        

        //Update its neighbours
        for (k=0; k < 4; k++){
            i = dijk_seq[k];
            j = dijk_seq[3-k];
            if ((map[x+i][y+j] == 0) && (distance[x+i][y+j] == -1)){
                //When all edges have weight 1, it's impossible that an unvisited neighbour's
                //shortest distance will be <= the current vertex's distance.
                //So we don't need to compare it.
                distance[x+i][y+j] = distance[x][y] + 1;
                enqueue (&dijk_unvisited_x, x+i);
                enqueue (&dijk_unvisited_y, y+j);
            }
        }
    }
    
    map[start_x][start_y] = temp;
    
    //FOR TESTING
    /*
    for (y=0; y<SCREENHEIGHT; y++){
        for (x=0; x<SCREENWIDTH; x++){
            fprintf(fp, "%c(%3d)  ", map[x][y], distance[x][y]);
        }
        fprintf(fp, "\n\n");
    }
    fprintf(fp, "\n\n\n\n\n\n\n");
    */
    
    return;
}
