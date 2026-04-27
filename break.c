// hellow world ncurses test

#include <ncurses.h>

int main() {
	// start curses mode
	initscr();

	// print hello world 
	printw("Hello world !!");

	// print it on the real screen
	refresh();

	printw("\nTEST TO SEE IF PRINT");

	// wait for user input
	getch();

	// end curses mode
	endwin();

	return 0;
}
