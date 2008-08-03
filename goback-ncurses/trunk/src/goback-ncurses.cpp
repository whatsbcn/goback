#include "StatusBar.h"

#include <DataSource.h>
#include <WorkMode.h>
#include <DataFormat.h>

#include <cstdlib>
#include <curses.h>
#include <iostream>

StatusBar statusBar;

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
bool exec_command(std::string cmd) {
	return true;
}

/*
 * Print main view.
 */
void print_view(BufferWindow *win) {
	// Update the current window
	win->updateWindow();

	// Print file position percentage
	statusBar.update(win);

	// Show th cursor
	win->showCursor();

	refresh();
}

int main(int argc, char *argv[]) {
	int key, numsections;

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

	DataFormat *df = DataFormat::create("elf", ds);
	if (!df) {
		printf("Can't initialize dataformat\n");
		exit(1);
	}
	numsections = df->getNumberSections();

	//WorkMode *wm = WorkMode::create("elf", ds);
	//WorkMode *wm = WorkMode::create("disasm", ds);
	//WorkMode *wm = WorkMode::create("hex", ds);

	//printf("numsections %d\n", numsections);
	//exit(0);
	init_ncurses();

	// Initialize the buffer window
	BufferWindow win(0, 0, COLS, LINES - 1);
	win.setDataFormat(df);

	print_view(&win);
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
			win.cursorMoveLeft();
			break;
		case KEY_RIGHT:
			win.cursorMoveRight();
			break;
		case KEY_NPAGE:
			win.cursorPageDown();
			break;
		case KEY_PPAGE:
			win.cursorPageUp();
			break;
		case KEY_HOME:
			win.cursorMoveBeginning();
			break;
		case KEY_END:
			win.cursorMoveEnd();
			break;
		case ':':
			exec_command(statusBar.getCommand());
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
