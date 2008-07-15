#include "BufferWindow.h"

#include <DataSource.h>
#include <WorkMode.h>

#include <curses.h>
#include <iostream>

// Command mode like vim
bool cmdmode = false;
// Insert mode like vim
bool insertmode = false;


/*
 * Initialize ncurses environtment.
 */
void init_ncurses() {
	initscr();
	if (has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	keypad(stdscr, TRUE);
	//(void) nonl();      /* tell curses not to do NL->CR/NL on output */
	cbreak();
	noecho();
	//nodelay(stdscr, TRUE);
	//idlok(stdscr, TRUE);    /* allow use of insert/delete line */
	//intrflush(stdscr, FALSE);
	start_color();
	if (use_default_colors() == OK)
		init_pair(1, COLOR_RED, -1);

	attron(COLOR_PAIR(1));
}

/*
 * Destroy ncurses environtment.
 */
void close_ncurses() {
	attroff(COLOR_PAIR(1));
	endwin();
}

/*
 * Exec a command.
 */
bool exec_command(char *str) {
	return true;
}

/*
 * Print the file position percentage.
 */
void print_footer(BufferWindow *win) {
	//move(LINES-1, 0);
	//clrtoeol();
	char str[256];

	// Print cmdline if enabled
	if (cmdmode) {
		move(LINES - 1, 0);
		printw(":");
		clrtoeol();
		echo();
		refresh();
		getstr(str);
		exec_command(str);
		noecho();
		cmdmode = false;
		move(LINES - 1, 0);
		clrtoeol();
	} else {
		//move(LINES-1, 0);
		//clrtoeol();
	}

	// Show the cursor position
	//move(LINES-1, COLS - 18);
	mvprintw(LINES - 1, COLS - 18, "%d,0", win->getCursorLine() + 1, win->getCursorCol() + 1);
	clrtoeol();

	// Show the file position percentage
	//move(LINES-1, COLS - 4);
	mvprintw(LINES - 1, COLS - 4, "%d%%", win->getViewPercentage());
	clrtoeol();
	move(LINES - 1, 0);
}

/*
 * Print main view.
 */
void print_view(BufferWindow *win) {
	// Update the current window
	win->updateWindow();

	// Print file position percentage
	print_footer(win);

	// Show th cursor
	win->showCursor();

	refresh();
}

int main(int argc, char *argv[]) {
	int key;

	if (argc != 2){
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
		exit(-1);
	}

	DataSource *ds = DataSource::create("file");
	if (!ds->open(argv[1])) {
		perror("open");
		delete ds;
		return 1;
	}

	WorkMode *wm = WorkMode::create("elf", ds);
	//WorkMode *wm = WorkMode::create("disasm", ds);
	//WorkMode *wm = WorkMode::create("hex", ds);

	init_ncurses();

	// Initialize the buffer window
	BufferWindow win(0, 0, COLS, LINES - 1);
	win.setWorkMode(wm);

	print_view(&win);
	for (;;) {
		key = getch();
		switch (key) {
		case KEY_DOWN:
			win.cursorMoveDown();
			break;
		case KEY_UP:
			win.cursorMoveUp();
			break;
		case KEY_LEFT:
			//win.cursorMoveLeft();
			break;
		case KEY_RIGHT:
			//win.cursorMoveRight();
			break;
		case KEY_NPAGE:
			win.cursorPageDown();
			break;
		case KEY_PPAGE:
			win.cursorPageUp();
			break;
		case ':':
			cmdmode = true;
			break;
		case 'q':
			goto exit;
			break;
		}
		print_view(&win);
	}

	exit:
	close_ncurses();
	ds->close();
	return 0;
}
