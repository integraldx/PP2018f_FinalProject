#include <stdio.h>
#include <ncurses.h>

int main() {
	initscr();
	clear();
	printw("Hello World!\n");
	refresh();

	while(getch() != 'q'){

	}


	endwin();
	return 0;
}
