/*
ARTIFICIAL INTELLIGENCE
    The game AI.
______________________________________________

FILE: ai.c

//KILL VERSION

*/

#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <limits.h>

#include "engine.h"
#include "snake.h"
#include "ai.h"


//Initializing Mutex & Condition
pthread_mutex_t processing = PTHREAD_MUTEX_INITIALIZER;
int doneflag;


//Global Misc. Variables
int directions[] = {LEFT, UP, RIGHT, DOWN};

//Dijkstra's Algorithm Variables
queue dijk_unvisited_x, dijk_unvisited_y;
int dijk_unvisited_x_arr[MAXSQUARES], dijk_unvisited_y_arr[MAXSQUARES];
int dijk_seq[4] = {0, 0, 1, -1}; 


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
    
    @param data: A pointer to an array of 2 snakes.
    
*********************************************************************
*/
void *aiProcessGame(void *data){    
    //Allow the thread to be killed at any time.
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

    //DO OPERATIONS HERE
    //. . . . . . . . . . 
    struct snakestructure* botsnakepointer = *((struct snakestructure**) data + 0);
    struct snakestructure* usrsnakepointer = *((struct snakestructure**) data + 1);
    
	botsnakepointer->bot_newdirection = aiMinimax(botsnakepointer, usrsnakepointer);
    
    //After Processing:
    //Ensure that a cancel doesn't take place here.
    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_cleanup_push(aiUnlockMutex, (void *) &processing);
    pthread_mutex_lock(&processing);
    
    doneflag = 1;
    
    pthread_cleanup_pop(1);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    
    return NULL;
}



/*
*********************************************************************
FUNCTION: aiUnlockMutex
    Wrapper non-returning function that accepts a void pointer
    and unlocks a mutex.

*********************************************************************
*/
void aiUnlockMutex( void *data){
    pthread_mutex_unlock((pthread_mutex_t *)data);
    return;
}


/*
*********************************************************************
FUNCTION: aiMinimax
    Returns the best direction to go using the minimax algorithm.

*********************************************************************
*/

int aiMinimax(struct snakestructure* botsnakepointer, struct snakestructure* usrsnakepointer){
	//Start MINIMAX
   	struct future FG; 
	int i, j;
        	
    memcpy (FG.map, game.map, (SCREENWIDTH * SCREENHEIGHT * sizeof(char)));
    memcpy (&(FG.bot), botsnakepointer, sizeof(struct snakestructure));
    memcpy (&(FG.usr), usrsnakepointer, sizeof(struct snakestructure));

    //The minimum score that the maximizing player is assured of.
    int alpha = INT_MIN;   //alpha
    
    //The maximum score that the minimizing player is assured of.
    int beta = INT_MAX;  //beta
    
    int movesarray[2][3];   //There are going to be 3 allowed directions
    j = 0;  //Incremented every time an allowed direction is encountered
    for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.bot, directions[i])) {
    		movesarray[0][j] = directions[i];
    		movesarray[1][j] = aiScore(FG, directions[i], 0, alpha, beta);
            if (alpha < movesarray[1][j]) alpha = movesarray[1][j];
    		j++;
    	}
    }
	

    return movesarray[0][aiMaxOf3(movesarray[1])];
}




/*
*********************************************************************
FUNCTION: aiScore
    Returns the minimum score the maximizing player (bot) is assured 
    of - for a certain move by the bot.

*********************************************************************
*/
int aiScore( struct future FG, int direction, int depth, int alpha, int beta){
    //Move the maximizing player (bot) in the specified direction
	snakeUpdateDirection (FG.bot, direction);
	aiElongate (FG.bot);
	
	if (FG.bot.alive == 0) return INT_MIN/3;	
	
    //Now find the minimum score he's assured of.
	int i;
	int j = 0;
	int subscores[3];
	for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.usr, directions[i])) {
    		subscores[j] = aiSubScore(FG, directions[i], depth + 1, alpha, beta);
            if (beta > subscores[j]) beta = subscores[j];
            if (alpha > beta) return INT_MIN;
    		j++;
    	}
    }
    
    return (subscores[aiMinOf3(subscores)]);
}




/*
*********************************************************************
FUNCTION: aiSubScore
    Returns the maximum score the minimizing player (human) is 
    assured of - for a specified move of the player

*********************************************************************
*/
int aiSubScore( struct future FG, int direction, int depth, int alpha, int beta){
    //Move the minimizing player (human) in the specified direction
	snakeUpdateDirection (FG.usr, direction);
	aiElongate (FG.usr);
	if (FG.usr.alive == 0) return INT_MAX/3;
    
	if (depth >= game.depth) return aiVoronoi(&FG);
	
    //Now find the minimum score he's assured of.
	int i;
    int j = 0;
	int scores[3];
	for (i=0; i<4; i++){
    	if (snakeIsDirectionAllowed (FG.bot, directions[i])) {
    		scores[j] = aiScore(FG, directions[i], depth, alpha, beta);
            if (alpha < scores[j]) alpha = scores[j];
            if (beta < alpha) return INT_MAX;
            
    		j++;
    	}
    }

    return scores[aiMaxOf3(scores)];
}



/*
*********************************************************************
FUNCTION: aiVoronoi
    Calculates a score for the current game with the idea of a 
    Voronoi diagram.
    
    In brief, a Voronoi diagram is a diagram showing the regions of
    a map that are closest to a given player.
    
    The bot tries to maximise the number of squares that are closer
    to it, and minimise the regions closer to the player.
    
    It also tries to minimise the sum of distances of all squares
    from it, and maximize the sum of distances of all squares from
    the player.
    
    In case the player is going to get cut off from the bot, it 
    checks the sizes of their respective components.
    
    (Dijkstra's algorithm is used here to calculate distances and
    sizes of components)

*********************************************************************
*/
int aiVoronoi(struct future* FGptr){  
    int botcomponent, usrcomponent, result = 0;
    
    //Set the snakes heads as free space - so as to calculate distance to them.
    FGptr->map[FGptr->usr.head.x][FGptr->usr.head.y] = FGptr->map[FGptr->bot.head.x][FGptr->bot.head.y] = 0;
    
    //Populate distances and get component sizes
    botcomponent = aiDijkstra(FGptr->map, FGptr->botdistancemap, FGptr->bot.head.x, FGptr->bot.head.y);
    usrcomponent = aiDijkstra(FGptr->map, FGptr->usrdistancemap, FGptr->usr.head.x, FGptr->usr.head.y);
    
    //Reward or punish, depending on component sizes - if the user & bot lie in different components.
    if (FGptr->botdistancemap[FGptr->usr.head.x][FGptr->usr.head.y] == -1) result = (botcomponent - usrcomponent) * 10000;
            
    int x, y;
    for (y=0; y<SCREENHEIGHT; y++){
        for (x=0; x<SCREENWIDTH; x++){
            //Reward or punish based on distances
            if (FGptr->botdistancemap[x][y] == -1) result -= 50;
            if (FGptr->usrdistancemap[x][y] == -1) result += 50;
            if ((FGptr->usrdistancemap[x][y] > 0) && (FGptr->botdistancemap[x][y] > 0)){
                if (FGptr->usrdistancemap[x][y] > FGptr->botdistancemap[x][y]) result += 10;
                if (FGptr->usrdistancemap[x][y] < FGptr->botdistancemap[x][y]) result -= 10;
                result += (FGptr->usrdistancemap[x][y]) / 30;
                result -= (FGptr->botdistancemap[x][y]) / 30;
            }
        }
    }
    
    return result;
}




/*
*********************************************************************
FUNCTION: aiVoronoi
    For a map of dimension SCREENWIDTH x SCREENHEIGHT, with walls as 
    borders ONLY.
    
    This is a fast implementation of Dijkstra's algorithm, using 
    queues. This is easily possible because of the 'square-grid'
    nature of the map.
    
    Returns the size of the component.

*********************************************************************
*/
int aiDijkstra(char map[SCREENWIDTH][SCREENHEIGHT], int distance[SCREENWIDTH][SCREENHEIGHT], int start_x, int start_y){
    //Note that -1 is a distance of infinity
    int x, y;
    int i, j, k;
    int componentsize = 0;
    
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

       
    while(dijk_unvisited_x.size > 0){
        //Pick a vertex    
        x = dequeue (&dijk_unvisited_x);
        y = dequeue (&dijk_unvisited_y);
        componentsize++;
        

        //Update its neighbours
        for (k=0; k < 4; k++){
            i = dijk_seq[k];
            j = dijk_seq[3-k];
            if ((map[x+i][y+j] == 0) && (distance[x+i][y+j] == -1)){
                //When all edges have weight 1, it's impossible that an unvisited neighbour's
                //shortest distance will be <= the current vertex's distance.
                //So we don't need to compare it.
                //And this is why the first element in the queue is also the one with the shortest distance.
                distance[x+i][y+j] = distance[x][y] + 1;
                enqueue (&dijk_unvisited_x, x+i);
                enqueue (&dijk_unvisited_y, y+j);
            }
        }
    }
        
    return componentsize;
}
