#include <stdio.h>
#include <stdbool.h>
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

char cards[4][4] = {0};

typedef enum Scene { MAINMENU, GAMEBOARD } Scene; 

bool gameEndFlag = false;

Scene scene = MAINMENU;

int xFocus = 0;
int yFocus = 0;

bool cardSelectionFlag = false;
int selectionXFocus = 0;
int selectionYFocus = 0;

void init();

void initializeGameBoard();

void card_alphabet(int row, int column, char ch);

void switchScene();

void end();

void display();

void display_menu();

void display_cards();

void moveFocus(int input);

void handleSelection();

void examineTwoCards();

int main() {
	int input;
	init();

	display();

	while(true) {
		input = getch();
		printf("\a");
		switch(input) {
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:
				moveFocus(input);
				break;
			case ' ':
				handleSelection();
				break;
			case 'q':
				if (scene == GAMEBOARD) {
					switchScene();
				}
				break;
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
	switch(scene) {
		case MAINMENU:
			display_menu();
			break;
		case GAMEBOARD:
			display_cards();	
			break;
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


			if(cards[i][j] != 0) {
				mvaddch(3 * i + 1, 5 * j + 1, ACS_ULCORNER);
				mvaddch(3 * i + 1, 5 * j + 2, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 3, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 4, ACS_HLINE);
				mvaddch(3 * i + 1, 5 * j + 5, ACS_URCORNER);
				mvaddch(3 * i + 2, 5 * j + 1, ACS_VLINE);
				mvaddch(3 * i + 2, 5 * j + 2, ' ');
				mvaddch(3 * i + 2, 5 * j + 3, cards[i][j]);
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
				mvaddch(3 * i + 2, 5 * j + 3, ' ');
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


void moveFocus(int input) {
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
				switchScene();
			}
			else if (yFocus == 1) {
				gameEndFlag = true;
			}
			break;
		case GAMEBOARD:
			if(cardSelectionFlag) {
				examineTwoCards();
				cardSelectionFlag = false;
			}
			else {
				cardSelectionFlag = true;
				selectionXFocus = xFocus;
				selectionYFocus = yFocus;
			}
			break;
	}
}

void examineTwoCards() {
	if(cards[yFocus][xFocus] == cards[selectionYFocus][selectionXFocus] && (yFocus != selectionYFocus || xFocus != selectionXFocus)) {
		cards[yFocus][xFocus] = 0;
		cards[selectionYFocus][selectionXFocus] = 0;
	}
	clear();
}

void switchScene() {
	clear();
	xFocus = 0;
	yFocus = 0;
	switch(scene) {
		case MAINMENU:
			initializeGameBoard();
			scene = GAMEBOARD;
			break;
		case GAMEBOARD:
			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					cards[i][j] = 0;
				}
			}
			scene = MAINMENU;
			cardSelectionFlag = false;
			break;
	}
	display();
}

void initializeGameBoard() {
	int i = 0;
	int x = 0;
	int y = 0;
	char alphabets[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
	srand(time(NULL));
	
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
}


	

