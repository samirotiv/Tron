#ifndef MENU_DEFINED
#define MENU_DEFINED

#define MENUBORDER '*'
#define MENUBOXWIDTH 50
#define MENUBOXHEIGHT 10
#define MENUBOXSTARTX (SCREENWIDTH - MENUBOXWIDTH )/2
#define MENUBOXSTARTY 10
#define MENUTEXTSTARTX (LOGOSTARTX + 22)
#define MENUTEXTSTARTY (LOGOSTARTY + 12 )
#define GAMEMODESTARTX (LOGOSTARTX+2)
#define GAMEMODESTARTY (LOGOSTARTY + 12)

#define ENDBOXWIDTH 30
#define ENDBOXHEIGHT 10
#define ENDCHOICES 2
#define ENDBOXSTARTX (SCREENWIDTH - ENDBOXWIDTH)/2
#define ENDBOXSTARTY (SCREENHEIGHT - ENDBOXHEIGHT)/2 -1
#define ENDBOXENDX (ENDBOXSTARTX + ENDBOXWIDTH -1)
#define ENDBOXENDY (ENDBOXSTARTY + ENDBOXHEIGHT)

#define RESTART 1
#define QUITTOMENU 2

#define LOGOSTARTX ((SCREENWIDTH - 70)/2)
#define LOGOSTARTY 2

void menuMainMenu();
int menuGetInput( int*, int*);
void menuStartAtrribute (int );

void menuEndPrint(int highlight);
void menuEndGame();

void menuPauseGame();
void menuPrintWinner();

#endif
