#include <stdio.h>
#include <ncurses.h>

char cards[4][4] = {0};

typedef enum Scene { MAINMENU, GAMEBOARD } Scene; 

Scene scene = MAINMENU;

int xFocus = 0;
int yFocus = 0;

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
	clear();

	refresh();
	display();
}

void card_alphabet(int row, int column, char ch) {
	
	cards[row][column] = ch;

}

void switchScene() {
	clear();
	switch(scene) {
		case MAINMENU:
			scene = GAMEBOARD;
			break;
		case GAMEBOARD:
			scene = MAINMENU;
			break;
	}
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




int main() {
	int input;
	init();

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			card_alphabet(i, j, 4 * i + j + 'A');
		}
	}

	display();
	while((input = getch()) != 'q') {
		switch(input) {
			case 's':
				switchScene();
				break;
			case KEY_UP:
			case KEY_DOWN:
			case KEY_LEFT:
			case KEY_RIGHT:
				moveFocus(input);
				break;
		}


		display();
	}


	end();
	return 0;
}
