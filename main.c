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


void SinglePlayerGame();
void TwoPlayerGame();

void ExitGame();




int main(){
//STRICTLY TEMPORARY - TESTING ONLY
    engineInit();
    TwoPlayerGame();

    ExitGame();
}




void TwoPlayerGame(){
    engineStartGameEnvironment();
    int c, new_direction;

    queue player1buffer, player2buffer;
    InitQueue(&player1buffer);
    InitQueue(&player2buffer);
    
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
				    //Pause
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
    menuEndGame();
}



void SinglePlayerGame(){
    engineStartGameEnvironment();
    int c;

    //TEMPORARY LINE:
    game.map[snake2.head.x][snake2.head.y] = 0;
    
    while((snake1.alive == 1) && (snake2.alive == 1)){
        usleep (GAMEDELAY);
        switch (c = getch()){
            case ERR:
				// If we get no input
				break;

            case KEY_UP:
                snakeUpdateDirection(snake1, UP);
                break;

            case KEY_LEFT:
                snakeUpdateDirection(snake1, LEFT);
                break;

            case KEY_DOWN:
                snakeUpdateDirection(snake1, DOWN);
                break;

            case KEY_RIGHT:
                snakeUpdateDirection(snake1, RIGHT);
                break;

            case 'q':	case 'Q':
                ExitGame();
                break;

            case 'p':	case 'P':
				//Pause
				break;

            default:
                break;
        }
        
        snakeElongate (snake1);
        //snakeElongate (snake2);
        refresh();
    }
    
    //POST GAME PROCESSING
    engineProcessGameWinner();

}


void ExitGame(){
    endwin();
    exit(0);
}
