#include "StatusBar.h"

#include <DataSource.h>
#include <WorkMode.h>

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
	/** To wait only some time in getch() calls */
	//nodelay(stdscr, TRUE);
	//idlok(stdscr, TRUE);    /* allow use of insert/delete line */
	//intrflush(stdscr, FALSE);
	
	/** Scroll */
	scrollok(stdscr, TRUE);
	setscrreg(0, LINES - 2);

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

/**
 * Print static content
 * @param win current window
 */
void printStaticView(BufferWindow *win) {
	// Print file position percentage
	statusBar.update(win);

	// Show th cursor
	win->showCursor();

	refresh();
}

void printContentView(BufferWindow *win) {
	// Update the current window
	win->updateWindow();
	
	/** Print static content */
	printStaticView(win);
}

int main(int argc, char *argv[]) {
	int key, numsections;

	if (argc != 2){
		std::cout << "Usage: " << argv[0] << " file" << std::endl;
		exit(-1);
	}
	
	/** Create datasource with a file */
	DataSource *ds = DataSource::create("file");
	if (!ds->open(argv[1])) {
		perror("open");
		delete ds;
		return 1;
	}
	
	/** Detect file formats */
	std::vector<std::string> formats = ds->detectFormat();
	if (formats.size() == 0) {
		printf("Can't detect fileformat, open as hex!\n");
		formats.push_back("");
	}
	
	/** Select file format */
	if (!ds->setDataFormat(formats.front())) {
		printf("Can't initialize dataformat!\n");
		exit(1);
	}

	init_ncurses();

	// Initialize the buffer window
	BufferWindow win(0, 0, COLS, LINES - 1);
	win.setDataSource(ds);

	printContentView(&win);
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
	//	printContentView(&win);
		printStaticView(&win);
	}

	exit:
	close_ncurses();
	ds->close();
	delete ds;
	return 0;
}
