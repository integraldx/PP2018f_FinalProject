#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

#define SCREENWIDTH 50
#define SCREENHEIGHT 15

// Global variables for handling game status

char cards[4][4] = {0};
bool isCleared[4][4] = {0};

typedef enum Scene { MAINMENU, GAMEBOARD, PAUSE, VICTORY } Scene; 

bool gameEndFlag = false;

Scene scene = MAINMENU;

int xFocus = 0;
int yFocus = 0;

bool cardSelectionFlag = false;
int selectionXFocus = 0;
int selectionYFocus = 0;

bool displayingTrialFlag = false;

int leftPairs = 0;

int debugCommand[] = {KEY_UP, KEY_UP, KEY_DOWN, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_LEFT, KEY_RIGHT, 'b', 'a'};
int debugAccomplishCount = 0;
bool debugMode = false;


// Utility functions

void init();

void initializeGameBoard();

void card_alphabet(int row, int column, char ch);

void switchScene(Scene sc);

void end();

void display();

void display_bound();

void display_menu();

void display_cards();

void display_pause();

void display_victory();

void moveFocus(int input);

void handleSelection();

void examineTwoCards();

void pauseGame();


int main() {
	int input;
	init();

	display();

	while(true) {
		input = getch();
		if(!debugMode) {

			if(debugCommand[debugAccomplishCount] == input) {
				debugAccomplishCount++;
			}
			else {
				debugAccomplishCount = 0;
			}
			if(debugAccomplishCount >= (sizeof(debugCommand) / sizeof(int))) {
				debugMode = true;
			}
		}
		switch(input) {
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:
				if(scene == GAMEBOARD || scene == MAINMENU) {
					moveFocus(input);
				}
				break;
			case ' ':
				if(scene == GAMEBOARD || scene == MAINMENU) {
					handleSelection();
				}
				break;
			case 'q':
				if (scene == GAMEBOARD) {
					pauseGame();
				}
				else if (scene == PAUSE) {
					switchScene(MAINMENU);
				}
				break;
			case '\n':
				if (scene == VICTORY) {
					switchScene(MAINMENU);
				}
				else if (scene == PAUSE) {
					scene = GAMEBOARD;
				}
				break;
			case 'r':
				if (scene == PAUSE) {
					switchScene(GAMEBOARD);
				}
		}
		display();
		if(gameEndFlag) {
			break;
		}

	}

	end();
	return 0;
}

void init() {
	int i = 0;
	int j = 0;
	initscr();
	raw();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_CYAN);
	init_pair(3, COLOR_BLACK, COLOR_YELLOW);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	clear();

	refresh();
	display();
}


void card_alphabet(int row, int column, char ch) {
	
	cards[row][column] = ch;

}


void end() {
	endwin();
}


void display() {
	display_bound();
	switch(scene) {
		case MAINMENU:
			display_menu();
			break;
		case GAMEBOARD:
			display_cards();	
			break;
		case PAUSE:
			display_pause();
			break;
		case VICTORY:
			display_victory();
			break;
	}
}

void display_bound() {
	int i;
	const char* debug = "DEBUG MODE";
	mvaddch(0, 0, ACS_ULCORNER);
	for (i = 1; i < SCREENWIDTH - 1; i++) {
		mvaddch(0, i, ACS_HLINE);
	}
	mvaddch(0, SCREENWIDTH - 1, ACS_URCORNER);

	for (i = 1; i < SCREENHEIGHT - 1; i++) {
		mvaddch(i, 0, ACS_VLINE);
		mvaddch(i, SCREENWIDTH - 1, ACS_VLINE);
	}
	mvaddch(SCREENHEIGHT - 1, 0, ACS_LLCORNER);

	for (i = 1; i < SCREENWIDTH - 1; i++) {
		mvaddch(SCREENHEIGHT - 1, i, ACS_HLINE);
	}
	mvaddch(SCREENHEIGHT - 1, SCREENWIDTH - 1, ACS_LRCORNER);

	if(debugMode) {
		for(i = 0; i < 10; i++) {
			mvaddch(0, 5 + i, debug[i]);
		}
	}

}



void display_menu() {
	int i;
	char* game = "GAME START";
	char* quit = "QUIT";

	for (i = 0; i < 10; i++) {
		if(yFocus == 0) {
			attron(COLOR_PAIR(2));
		}
		mvaddch(6, 6 + i, game[i]);
		attroff(COLOR_PAIR(2));
	}

	for (i = 0; i < 4; i++) {
		if (yFocus == 1) {
			attron(COLOR_PAIR(2));
		}
		mvaddch(8, 6 + i, quit[i]);
		attroff(COLOR_PAIR(2));
	}
	
}


void display_cards() {
	int i = 0;
	int j = 0;
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			if (i == yFocus && j == xFocus) {
				attron(COLOR_PAIR(2));
			}
			else if (cardSelectionFlag && i == selectionYFocus && j == selectionXFocus) {
				attron(COLOR_PAIR(3));
			}


			if(!isCleared[i][j]) {
				mvaddch(3 * i + 1, 5 * j + 1, ACS_ULCORNER);
				mvaddch(3 * i + 1, 5 * j + 2, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 3, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 4, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 5, ACS_URCORNER);
				mvaddch(3 * i + 2, 5 * j + 1, ACS_VLINE);
				mvaddch(3 * i + 2, 5 * j + 2, ' ');
				if(debugMode || ((cardSelectionFlag || displayingTrialFlag)&& i == selectionYFocus && j == selectionXFocus) || (displayingTrialFlag && i == yFocus && j == xFocus)) {
					mvaddch(3 * i + 2, 5 * j + 3, cards[i][j]);
				}
				else {
					mvaddch(3 * i + 2, 5 * j + 3, ' ');
				}
				mvaddch(3 * i + 2, 5 * j + 4, ' ');
				mvaddch(3 * i + 2, 5 * j + 5, ACS_VLINE);
				mvaddch(3 * i + 3, 5 * j + 1, ACS_LLCORNER);
				mvaddch(3 * i + 3, 5 * j + 2, ACS_HLINE);
				mvaddch(3 * i + 3, 5 * j + 3, ACS_HLINE);
				mvaddch(3 * i + 3, 5 * j + 4, ACS_HLINE);
				mvaddch(3 * i + 3, 5 * j + 5, ACS_LRCORNER);
			}
			else {
				mvaddch(3 * i + 1, 5 * j + 1, ' ');
				mvaddch(3 * i + 1, 5 * j + 2, ' ');
				mvaddch(3 * i + 1, 5 * j + 3, ' ');
				mvaddch(3 * i + 1, 5 * j + 4, ' ');
				mvaddch(3 * i + 1, 5 * j + 5, ' ');
				mvaddch(3 * i + 2, 5 * j + 1, ' ');
				mvaddch(3 * i + 2, 5 * j + 2, ' ');
				if(debugMode || ((cardSelectionFlag || displayingTrialFlag)&& i == selectionYFocus && j == selectionXFocus) || (displayingTrialFlag && i == yFocus && j == xFocus)) {
					mvaddch(3 * i + 2, 5 * j + 3, cards[i][j]);
				}
				else {
					mvaddch(3 * i + 2, 5 * j + 3, ' ');
				}
				mvaddch(3 * i + 2, 5 * j + 4, ' ');
				mvaddch(3 * i + 2, 5 * j + 5, ' ');
				mvaddch(3 * i + 3, 5 * j + 1, ' ');
				mvaddch(3 * i + 3, 5 * j + 2, ' ');
				mvaddch(3 * i + 3, 5 * j + 3, ' ');
				mvaddch(3 * i + 3, 5 * j + 4, ' ');
				mvaddch(3 * i + 3, 5 * j + 5, ' ');
			}
			attroff(COLOR_PAIR(2));
			attroff(COLOR_PAIR(3));
		}
	}

	refresh();
}


void display_victory() {
	int i = 0;
	char vict[] = "VICTORY!";
	char ret[] = "Press enter to return to main menu.";

	for (i = 0; i < 8; i++) {
		mvaddch(6, 6 + i, vict[i]);
	}

	for (i = 0; i < (sizeof(ret) / sizeof(char)) - 1; i++) {
		mvaddch(8, 6 + i, ret[i]);
	}
}

void display_pause() {
	int i = 0;
	attron(COLOR_PAIR(4));
	char pause[] =        "==Game Paused==";
	char instruction1[] = "Resume  'Enter'";
	char instruction2[] = "Restart   'r'  ";
	char instruction3[] = "Quit      'q'  ";

	for (i = 0; i < (sizeof(pause) / sizeof(char)) - 1; i++) {
		mvaddch(6, 3 + i, pause[i]);
	}

	for (i = 0; i < (sizeof(instruction1) / sizeof(char)) - 1; i++) {
		mvaddch(7, 3 + i, instruction1[i]);
	}

	for (i = 0; i < (sizeof(instruction2) / sizeof(char)) - 1; i++) {
		mvaddch(8, 3 + i, instruction2[i]);
	}

	for (i = 0; i < (sizeof(instruction3) / sizeof(char)) - 1; i++) {
		mvaddch(9, 3 + i, instruction3[i]);
	}
	attroff(COLOR_PAIR(4));
}
void moveFocus(int input) {
	displayingTrialFlag = false;
	switch(input) {
		case KEY_UP:
			if (yFocus > 0) {
				yFocus--;
			}
			break;
		case KEY_DOWN:
			if (yFocus < 100) {
				yFocus++;
			}
			break;
		case KEY_LEFT:
			if (xFocus > 0) {
				xFocus--;
			}
			break;
		case KEY_RIGHT:
			if (xFocus < 100) {
				xFocus++;
			}
			break;
	}

	if (scene == MAINMENU) {
		if (yFocus > 1) {
			yFocus = 1;
		}
		xFocus = 0;
	}
	else if (scene == GAMEBOARD) {
		if (yFocus > 3) {
			yFocus = 3;
		}
		if (xFocus > 3) {
			xFocus = 3;
		}
	}
}


void handleSelection() {
	switch(scene) {
		case MAINMENU:
			if(yFocus == 0) {
				switchScene(GAMEBOARD);
			}
			else if (yFocus == 1) {
				gameEndFlag = true;
			}
			break;
		case GAMEBOARD:
			if(!isCleared[yFocus][xFocus]){
				if(cardSelectionFlag) {
					examineTwoCards();
					cardSelectionFlag = false;
				}
				else {
					cardSelectionFlag = true;
					selectionXFocus = xFocus;
					selectionYFocus = yFocus;
				}
			}
			break;
	}
}

void examineTwoCards() {
	if(cards[yFocus][xFocus] == cards[selectionYFocus][selectionXFocus] && (yFocus != selectionYFocus || xFocus != selectionXFocus)) {
		isCleared[yFocus][xFocus] = 1;
		isCleared[selectionYFocus][selectionXFocus] = 1;
		leftPairs--;
	}
	displayingTrialFlag = true;

		

	if (leftPairs == 0) {
		scene = VICTORY;
	}
	clear();
}

void switchScene(Scene sc) {
	clear();
	switch(sc) {
		case GAMEBOARD:
			initializeGameBoard();
			scene = GAMEBOARD;
			break;
		case MAINMENU:
			xFocus = 0;
			yFocus = 0;
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					cards[i][j] = 0;
					isCleared[i][j] = 0;
				}
			}
			scene = MAINMENU;
			cardSelectionFlag = false;
			break;
	}

	return;
}



void initializeGameBoard() {
	int i = 0;
	int x = 0;
	int y = 0;
	char alphabets[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	srand(time(NULL));

	xFocus = 0;
	yFocus = 0;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			cards[i][j] = 0;
			isCleared[i][j] = 0;
		}
	}
	cardSelectionFlag = false;


	for(i = 0; i < 8; i++) {
		while (true) {
			x = rand() % 4;
			y = rand() % 4;

			if(cards[y][x] == 0) {
				cards[y][x] = alphabets[i];
				break;
			}

		}
		while (true) {
			x = rand() % 4;
			y = rand() % 4;

			if(cards[y][x] == 0) {
				cards[y][x] = alphabets[i];
				break;
			}
		}

	}
	leftPairs = 8;
}

void pauseGame() {
	scene = PAUSE;
}


