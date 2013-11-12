/*
TRON GAME - MAIN
    Game Control File
______________________________________________

FILE: main.c

*/
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>

#include "engine.h"
#include "snake.h"
#include "menu.h"
#include "ai.h"


void SinglePlayerGame();
void TwoPlayerGame();

void ExitGame();




int main(){
    engineInit();
    menuMainMenu();

    ExitGame();
}




/*
*********************************************************************
FUNCTION: TwoPlayerGame
    Runs a two player game of tron

*********************************************************************
*/
void TwoPlayerGame(){
    engineStartGameEnvironment();
    game.state = TWOPLAYERGAME;
    
    //Creating our own input buffer to ensure responsive input
    int c, new_direction;

    queue player1buffer, player2buffer;
    int buffer1[MAXKEYBUFFERSIZE], buffer2[MAXKEYBUFFERSIZE];
    InitQueue(&player1buffer, buffer1, MAXKEYBUFFERSIZE);
    InitQueue(&player2buffer, buffer2, MAXKEYBUFFERSIZE);
    
    player1buffer.lastenqueued = SNAKE1_STARTDIRECTION;
    player2buffer.lastenqueued = SNAKE2_STARTDIRECTION;
    
    while((snake1.alive == 1) && (snake2.alive == 1)){
        usleep (GAMEDELAY);
        while ((c = getch()) != ERR){
            switch (c){
                case KEY_UP:
                    if (player2buffer.lastenqueued != UP) enqueue (&player2buffer, UP);
                    break;

                case KEY_LEFT:
                    if (player2buffer.lastenqueued != LEFT) enqueue (&player2buffer, LEFT);
                    break;

                case KEY_DOWN:
                    if (player2buffer.lastenqueued != DOWN) enqueue (&player2buffer, DOWN);
                    break;

                case KEY_RIGHT:
                    if (player2buffer.lastenqueued != RIGHT) enqueue (&player2buffer, RIGHT);
                    break;

                case 'w': case 'W':
				    if (player1buffer.lastenqueued != UP) enqueue (&player1buffer, UP);
				    break;

			    case 'a': case 'A':
				    if (player1buffer.lastenqueued != LEFT) enqueue (&player1buffer, LEFT);
				    break;

			    case 's': case 'S':
				    if (player1buffer.lastenqueued != DOWN) enqueue (&player1buffer, DOWN);
				    break;

			    case 'd': case 'D':
				    if (player1buffer.lastenqueued != RIGHT) enqueue (&player1buffer, RIGHT);
				    break;

                case 'q':	case 'Q':
                    ExitGame();
                    break;

                case 'p':	case 'P':
                    menuPauseGame();
                    break;

                default:
                    break;
            }
        }

        if (player1buffer.size != 0) {
            new_direction = dequeue(&player1buffer);
            snakeUpdateDirection(snake1, new_direction);
        }
        if (player2buffer.size != 0) {
            new_direction = dequeue(&player2buffer);
            snakeUpdateDirection(snake2, new_direction);
        }
        
        snakeElongate (snake1);
        snakeElongate (snake2);
        refresh();
    }
    
   
    //POST GAME PROCESSING
    engineProcessGameWinner();
    menuPrintWinner();
    usleep(3000000);
    menuEndGame();
    return;
}


/*
*********************************************************************
FUNCTION: SinglePlayerGame
    Runs a single player game of tron against a bot

*********************************************************************
*/
void SinglePlayerGame(){
        
    engineStartGameEnvironment();
    game.state = SINGLEPLAYERGAME;
    
    //Creating our own input buffer to ensure responsive input.
    int c, new_direction;
    
    queue player1buffer;
    int buffer1[MAXKEYBUFFERSIZE];
    InitQueue(&player1buffer, buffer1, MAXKEYBUFFERSIZE);
    player1buffer.lastenqueued = SNAKE1_STARTDIRECTION;
    
    while((snake1.alive == 1) && (snake2.alive == 1)){
        engineSleepAndCallBot(&snake2, &snake1, GAMEDELAY);
        while ((c = getch()) != ERR){
            switch (c){
                case ERR:
                    // If we get no input
                    break;

                case KEY_UP:
                    if (player1buffer.lastenqueued != UP) enqueue (&player1buffer, UP);
                    break;

                case KEY_LEFT:
                    if (player1buffer.lastenqueued != LEFT) enqueue (&player1buffer, LEFT);
                    break;

                case KEY_DOWN:
                    if (player1buffer.lastenqueued != DOWN) enqueue (&player1buffer, DOWN);
                    break;

                case KEY_RIGHT:
                    if (player1buffer.lastenqueued != RIGHT) enqueue (&player1buffer, RIGHT);
                    break;

                case 'q':	case 'Q':
                    ExitGame();
                    break;

                case 'p':	case 'P':
                    menuPauseGame();
                    break;

                default:
                    break;
            }
        }
        
        snakeUpdateDirection(snake2, snake2.bot_newdirection);
        
        if (player1buffer.size != 0) {
            new_direction = dequeue(&player1buffer);
            snakeUpdateDirection(snake1, new_direction);
        }
        
        snakeElongate (snake1);
        snakeElongate (snake2);
        
        refresh();
    }
    
    //POST GAME PROCESSING
    engineProcessGameWinner();
    menuPrintWinner();
    
    usleep(3000000);
    menuEndGame();
    return;

}

/*
*********************************************************************
FUNCTION: ExitGame
    Restores the terminal prompt and exits the program

*********************************************************************
*/
void ExitGame(){
    endwin();
    exit(0);
}
