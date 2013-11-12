#ifndef MENU_DEFINED
#define MENU_DEFINED

#define MENUBORDER '*'
#define MENUBOXWIDTH 50
#define MENUBOXHEIGHT 10
#define MENUBOXSTARTX (SCREENWIDTH - MENUBOXWIDTH )/2
#define MENUBOXSTARTY 10
#define MENUTEXTSTARTX MENUBOXSTARTX + 5
#define MENUTEXTSTARTY 12


#define ENDBOXWIDTH 30
#define ENDBOXHEIGHT 10
#define ENDCHOICES 2
#define ENDBOXSTARTX (SCREENWIDTH - ENDBOXWIDTH)/2
#define ENDBOXSTARTY (SCREENHEIGHT - ENDBOXHEIGHT)/2 -1
#define ENDBOXENDX (ENDBOXSTARTX + ENDBOXWIDTH -1)
#define ENDBOXENDY (ENDBOXSTARTY + ENDBOXHEIGHT)

#define RESTART 1
#define QUITTOMENU 2
void menuMainMenu();
int menuGetInput(int* );
void menuStartAtrribute (int );

void menuEndPrint(int highlight);
void menuEndGame();

void menuPauseGame();

#endif
