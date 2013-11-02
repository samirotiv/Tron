#include <curses.h>
#include "menu.h"
#include "engine.h"

void menuEndGame()
{	
	int highlight = 1;
	int choice = 0;
	int c;

	engineStartMenuEnvironment();	//set delay for menu
			
	menuEndPrint(highlight);
	while(1)
	{	c = getch();
		switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = ENDCHOICES;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == ENDCHOICES)
					highlight = 1;
				else 
					++highlight;
				break;
			case 10:
				choice = highlight;
				break;
			default:
				refresh();
				break;
		}
		menuEndPrint(highlight);
		if(choice != 0)	/* User did a choice come out of the infinite loop */
			break;
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
