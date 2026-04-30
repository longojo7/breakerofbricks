// hellow world ncurses test

#include <ncurses.h>
#define PADDLE "<=====>"
#define BALL   "o"
#define BRICK  "####"
#define BRICK_WIDTH 5

int main() {
	// start curses mode
	initscr();
	// send keypress immediately without needing enter (keep cntrl c for quitting)
	cbreak();
	// want to enable function key reading for UI
	keypad(stdscr, TRUE);
	// remove printing keypress to the screen
	noecho();
	// hide the cursor
	curs_set(0);

	// check if the terminal supports colors
	if (has_colors() == FALSE) {
		printw("Your terminal does not support color.\n");
		printw("Press any key to exit.\n");
		refresh();
		getch();
		endwin();
		return 1;
	}

//	printw("TYPE ANY CHARACTER\n");
//	int ch = getch();
//	if (ch == KEY_F(1)) {
//		printw("F1 key pressed\n");
//	} else {
//		printw("The key pressed is ");
//		attron(A_BOLD);
//		printw("%c", ch);
//		attroff(A_BOLD);
//	}


	// start adding some colors
	start_color();
	// 1st is paddle color, then ball color, then brick color
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_RED);

	// draw a border around the screen
	box(stdscr, 0, 0);

	// set the paddle x outside the loop
	int paddle_x = 17;
	int ball_x = 20;

	// getch() won't block the game loop so game will
	// keep running when player does not press anything
	nodelay(stdscr, TRUE);

	// start the game while loop
	while(1) {
		int ch = getch();
		// quit if q is typed
		if (ch == 'q') break;
		
		// be able to move paddle to the left and right
		if (ch == KEY_LEFT && paddle_x > 1) {
			paddle_x--;
			ball_x--;
		}
		if (ch == KEY_RIGHT && paddle_x < 33) {
			paddle_x++;
			ball_x++;
		}

		// erase the screen to wipe internal buffer only`
		erase();
		box(stdscr, 0, 0);

		// draw the paddle on the screen
		attron(COLOR_PAIR(1));
		mvprintw(20, paddle_x, PADDLE);
		attroff(COLOR_PAIR(1));

		// draw the ball on the screen
		attron(COLOR_PAIR(2));
		mvprintw(19, ball_x, BALL);
		attroff(COLOR_PAIR(2));

		// draw the a couple bricks near the top of the window
		attron(COLOR_PAIR(3));
		// use a for loop to draw a row of brick
		int brick_start = 1;
		for (int i = 0; i < 5; i += 2) {
			for (int j = 0; j < 8; j++) {
				mvprintw(3 + i, brick_start + (BRICK_WIDTH * j), BRICK);
			}
		}
		attroff(COLOR_PAIR(3));

		// draw a basic side panel
	        mvprintw(3, 45, "Score: 0");
		mvprintw(4, 45, "Lives: 3");
		mvprintw(5, 45, "Level: 1");

		// push everything to the real screen
		refresh();
		// 60 fps
		napms(16);
	}



	// end curses mode
	endwin();

	return 0;
}
