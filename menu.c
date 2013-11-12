#include <curses.h>
#include "menu.h"
#include "engine.h"

enum gamemodes { ONE_PLAYER, TWO_PLAYERS, BOT_VS_BOT};
enum Colors { BLACK_WHITE = 1, CYAN_BLACK, BLUE_BLACK,
              WHITE_BLACK, GREEN_BLACK, RED_BLACK  };
enum gamestates { MAIN_MENU, GAME, PAUSED, END_MENU };             

int gamemode;

void menuMainMenu(){
    int current_speed = 1;
    char speed_options[9] = {'1', '2', '3', '4', '5', '6', '7', '8', '9'};

	int option_row_pos = MENUTEXTSTARTX + 17;

	int wait;
	int i;
	int j;

	while(1){   //Testing returning to main menu
    	wait = TRUE;
	    clear ();
    	game.state = MAIN_MENU;
    	if (has_colors () == TRUE)
	    {
	    	int bg_color;

    		start_color ();
    		if (use_default_colors () == ERR)
    			bg_color = COLOR_BLACK;
    		else
    			bg_color = -1;

    		// Start support for colors (Name, Foreground, Background)
    		init_pair (GREEN_BLACK, COLOR_GREEN, bg_color);
    		init_pair (CYAN_BLACK,  COLOR_CYAN,  bg_color);
    		init_pair (WHITE_BLACK, COLOR_WHITE, bg_color);
    		init_pair (RED_BLACK,   COLOR_RED,   bg_color);
    		init_pair (BLUE_BLACK,  COLOR_BLUE,  bg_color);
    		init_pair (BLACK_WHITE, COLOR_BLACK, bg_color);
    	}

    	while (wait == TRUE)
    	{
    		// Draw the borders
    		menuStartAtrribute (COLOR_PAIR (WHITE_BLACK));
    		for (i = 0; i < SCREENWIDTH; i++)
    		{
    			engineAddChar(i, 0, MENUBORDER);
    			engineAddChar(i ,SCREENHEIGHT - 1, MENUBORDER);
    		}
    		for (i = 0; i < SCREENHEIGHT; i++)
    		{
    			engineAddChar (0, i, MENUBORDER);
    			engineAddChar (SCREENWIDTH - 1, i, MENUBORDER);
    		}
    		menuStartAtrribute (COLOR_PAIR (BLUE_BLACK));
    		engineAddStr(MENUBOXSTARTX, MENUBOXSTARTY, " ___________________________________________________ ");
    		
    		for(i=MENUBOXSTARTY+1 ; i<=MENUBOXSTARTY+11 ; i++){
    		    engineAddStr(MENUBOXSTARTX, i, "|                                                   |");
    		} 
    		    
    		engineAddStr(MENUBOXSTARTX, MENUBOXSTARTY+12, "|___________________________________________________|");
    
    		engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY, "Press <enter> or <space> to start game");
    
    		engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+1, "Press <q> to quit game");
    
    		// Draw the game mode
    		engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+3, "Game Mode:");
    		if (gamemode == ONE_PLAYER)
    		{
    			menuStartAtrribute (COLOR_PAIR (WHITE_BLACK));
    			engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+4, "One Player");
    
    			menuStartAtrribute (COLOR_PAIR (BLUE_BLACK));
    			engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+5, "Two Players");
    		}
    		else
    		{
    		    menuStartAtrribute (COLOR_PAIR (BLUE_BLACK));
    		    engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+4, "One Player");
    		    
    		    menuStartAtrribute (COLOR_PAIR (WHITE_BLACK));
    		    engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+5, "Two Players");
    		}
    		    
    
    		// Draw the level numbers
    		menuStartAtrribute (COLOR_PAIR (BLUE_BLACK));
    		engineAddStr(MENUTEXTSTARTX, MENUTEXTSTARTY+7, "Starting speed:");
    
    		// Draw the options with the right colors
    		for (i = 0, j = 0; i < 9; i++)
    		{
    		    if (i == (current_speed-1))
    				menuStartAtrribute (COLOR_PAIR (WHITE_BLACK));
    			else
    				menuStartAtrribute (COLOR_PAIR (BLUE_BLACK));
    				
    			mvprintw(MENUTEXTSTARTY+7, option_row_pos+j, "%c", speed_options[i]);	
    			j += 2;
    		}
    
    		
    		engineAddStr(MENUBOXSTARTX+1, MENUBOXSTARTY+11, "Use --help for guidelines");
    
    		wait = menuGetInput(&current_speed);
        	refresh ();
    	}
    
    	game.difficulty = current_speed;
    	game.state = GAME;
    	//Turns off all attributes
    	standend();
    	
    	
    	while (gamemode == ONE_PLAYER && game.state == GAME)    SinglePlayerGame();
    	//else if (gamemode == BOT_VS_BOT) BotAgainstBot();
    	while (gamemode == TWO_PLAYERS && game.state == GAME)     TwoPlayerGame();
    }
}




/**	Gets the input for the main menu.
 */
int menuGetInput(int* speed_cur_option)
{
	nodelay (stdscr, FALSE);

	int input = getch();
	switch (input)
	{
	case 10:	case ' ':
		return FALSE;
		break;

	case 'q':	case 'Q':
		ExitGame();
		break;

	case KEY_UP: 
		if (gamemode == TWO_PLAYERS)
			gamemode = ONE_PLAYER;
		break;

	case KEY_DOWN:
		if (gamemode == ONE_PLAYER)
			gamemode = TWO_PLAYERS;
		break;

	case KEY_LEFT:
		if (*speed_cur_option > 1)
			(*speed_cur_option)--;
		break;

	case KEY_RIGHT:
		if (*speed_cur_option < 9)
			(*speed_cur_option)++;
		break;

	case '1': case '2': case '3': case '4': case '5':
	case '6': case '7': case '8': case '9':
		*speed_cur_option = (input - '0'); // ASCII table value
		break;

	default:
		break;
	}

	return TRUE;
}

/** Wrapper to the attron() function, in case the current terminal
 *  doesn't support colors.
 */
void menuStartAtrribute (int attr)
{
	if (has_colors () == TRUE)
	{
		attron (attr);
	}
}




void menuEndGame()
{	
	int highlight = 1;
	int choice = 0;
	int c;

	engineStartMenuEnvironment();	//set delay for menu
	game.state = END_MENU;		
	
	menuEndPrint(highlight);
	while(1)
	{	
	    c = getch();
		switch(c)
		{	
		    case KEY_UP:
				if(highlight == QUITTOMENU)
					highlight = RESTART;
				break;
			case KEY_DOWN:
				if(highlight == RESTART)
					highlight = QUITTOMENU;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				refresh();
				//break;
		}
		menuEndPrint(highlight);
		if(choice == 1){	    /* Player chose to restart*/
		    game.state = GAME;
			//Turn off all attributes
			standend();
			break;
	    }
	    else if(choice == 2){
	        game.state = MAIN_MENU;
	        //Turn off all attributes
			standend();
	        break;
	    }
	}	

	refresh();
	endwin();
	return;
}

/*
 * Function to print end game menu
 * @param the choice to be highlighted
 */
void menuEndPrint(int highlight)
{
	int x, y, i;
	char *choices[] = { 
			"RESTART",
			"MAIN MENU",
		  };
	int size_choices[] = { 7, 9};
	
	engineAddStr( ENDBOXSTARTX, ENDBOXSTARTY," ____________________________ ");
	for ( i=1; i<= ENDBOXHEIGHT ; i++){
		engineAddChar( ENDBOXSTARTX, ENDBOXSTARTY + i,'|');
		engineAddChar( ENDBOXENDX, ENDBOXSTARTY + i,'|');
	}
	engineAddStr(ENDBOXSTARTX, ENDBOXENDY, "|____________________________|");
	
	for(i = 0; i < ENDCHOICES; ++i)
	{	
		x=(( ENDBOXWIDTH - size_choices[i])/2 + ENDBOXSTARTX);
		y= ENDBOXSTARTY+(ENDBOXHEIGHT-ENDCHOICES)/2 +i;
		if(highlight == i + 1)
		{	attron( A_STANDOUT); 
			enginePrintF( x, y, "%s", choices[i]);
			attroff( A_STANDOUT);
		}
		else
			enginePrintF( x, y, "%s", choices[i]);
	}
	attron( A_BOLD);
	if (game.winner !=0 )
		enginePrintF( (ENDBOXWIDTH - 16)/2 + ENDBOXSTARTX, ENDBOXENDY-1,"PLAYER %d WINS!!!", game.winner);
	else
		enginePrintF( (ENDBOXWIDTH - 9)/2 + ENDBOXSTARTX, ENDBOXENDY-1,"DRAW GAME");
	attroff( A_BOLD);
	refresh();
}
