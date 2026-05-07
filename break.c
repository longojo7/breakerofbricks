// hellow world ncurses test

#include <ncurses.h>

// definitions for constants
#define G_HEIGHT 24
#define G_WIDTH 41
#define S_WIDTH 20
#define PADDLE_Y G_HEIGHT-2
#define BALL_Y G_HEIGHT-3
#define PADDLE "<=====>"
#define BALL   "o"
#define BRICK  "####"
#define BRICK_WIDTH 5
#define PADDLE_WIDTH 7

WINDOW* create_game_win(int start_y, int start_x) {
	WINDOW* game_win = newwin(G_HEIGHT, G_WIDTH, start_y, start_x);
	box(game_win, 0, 0);
	wrefresh(game_win);
	return game_win;
}

WINDOW* create_score_win(int start_y, int start_x) {
	WINDOW* score_win = newwin(G_HEIGHT, S_WIDTH, start_y, start_x);
	box(score_win, 0, 0);
	wrefresh(score_win);
	return score_win;
}

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

	// start adding some colors
	start_color();
	// darkened red a little bit
	init_color(COLOR_RED, 700, 0, 0);
	// 1st is paddle color, then ball color, then brick color
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	init_pair(3, COLOR_RED, COLOR_RED);

	// set the start x and start y to center the two windows
	int space = 1;
	int startx = (COLS - (G_WIDTH + S_WIDTH + space)) / 2;
	int starty = (LINES - G_HEIGHT) / 2;
	
	// create the game and score window
	WINDOW* game_win = create_game_win(starty, startx);
	WINDOW* score_win = create_score_win(starty, startx + G_WIDTH + space);

	// initialize coordinates and deltas
	int paddle_x = (G_WIDTH - 2) / 2 - 2;
	int ball_x = (G_WIDTH - 2) / 2 + 1;
	int ball_y = BALL_Y;
	int ball_dx = 1;
	int ball_dy = -1;
	int game_start = 0;

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
			if (!game_start) ball_x--;
		}
		if (ch == KEY_RIGHT && paddle_x < 33) {
			paddle_x++;
			if (!game_start) ball_x++;
		}

		// add a play button
		if (ch == 'p') game_start = 1;
		if (game_start) {
			// update the ball position
			ball_x += ball_dx;
			ball_y += ball_dy;

			// check if the ball is bouncing off the wall
			if (ball_x <= 1 || ball_x >= G_WIDTH - 2) ball_dx = -ball_dx;
			// set to 0 because it looks better with ball hitting the top border
			if (ball_y <= 0) ball_dy = -ball_dy;

			// check if the ball has bounced off the paddle,give one col past paddle width for wider collision
			if (ball_x >= paddle_x - 1 && ball_x <= paddle_x + PADDLE_WIDTH && ball_y == PADDLE_Y - 1) {
				ball_dy = -ball_dy;

				// ball moves left on left side of paddle, right on right side of paddle
				if (ball_x < paddle_x + 2) {
					ball_dx = -1; 
				} else if (ball_x > paddle_x + 4) {
					ball_dx = 1;
				}
			}
		}

		// erase both windows
		werase(game_win);
		werase(score_win);

		// redraw borders for each window
		box(game_win, 0, 0);
		box(score_win, 0, 0);

		// draw the paddle on the screen
		wattron(game_win, COLOR_PAIR(1));
		mvwprintw(game_win, PADDLE_Y, paddle_x, PADDLE);
		wattroff(game_win, COLOR_PAIR(1));

		// draw the ball on the screen
		wattron(game_win, COLOR_PAIR(2));
		mvwprintw(game_win, ball_y, ball_x, BALL);
		wattroff(game_win, COLOR_PAIR(2));

		// draw the a couple bricks near the top of the window
		wattron(game_win, COLOR_PAIR(3));
		// use a for loop to draw a row of brick
		int brick_start = 1;
		for (int i = 0; i < 5; i += 2) {
			for (int j = 0; j < 8; j++) {
				mvwprintw(game_win, 3 + i, brick_start + (BRICK_WIDTH * j), BRICK);
			}
		}
		wattroff(game_win, COLOR_PAIR(3));

		// push game window
		wrefresh(game_win);

		// draw a basic side panel
	        mvwprintw(score_win, 3, 6, "Score: 0");
		mvwprintw(score_win, 4, 6, "Lives: 3");
		mvwprintw(score_win, 5, 6, "Level: 1");
		wrefresh(score_win);

		// 60 fps
		napms(80);
	}

	// end curses mode
	endwin();

	return 0;
}
