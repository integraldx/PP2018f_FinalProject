#include <stdio.h>
#include <ncurses.h>

char cards[4][4] = {0};


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

void init() {
	int i = 0;
	int j = 0;
	initscr();
	raw();
	keypad(stdscr, TRUE);
	curs_set(0);
	noecho();
	start_color();
	clear();

	mvaddch(0, 0, ACS_ULCORNER);
	mvaddch(49, 0, ACS_LLCORNER);
	mvaddch(0, 99, ACS_URCORNER);
	mvaddch(49, 99, ACS_LRCORNER);
	for (i = 1; i < 99; i++) {
		mvaddch(0, i, ACS_HLINE);
		mvaddch(49, i, ACS_HLINE);
	}
	for (i = 1; i < 49; i++) {
		mvaddch(i, 0, ACS_VLINE);
		mvaddch(i, 99, ACS_VLINE);
	}

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			cards[i][j] = 'A';
		}
	}


	refresh();
	display_cards();
}

void card_alphabet(int row, int column, char ch) {
	
	cards[row][column] = ch;

}

void end() {
	endwin();
}

int main() {
	init();

	
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			card_alphabet(i, j, 4 * i + j + 'A');
		}
	}

	display_cards();
	while(getch() != 'q') {
		display_cards();
	}


	end();
	return 0;
}
