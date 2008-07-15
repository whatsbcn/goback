#include "StatusBar.h"

#include <curses.h>

StatusBar::StatusBar() {
}

void StatusBar::update(BufferWindow *win) {
	// Show the cursor position
	mvprintw(LINES - 1, COLS - 18, "%d,0", win->getCursorLine() + 1, win->getCursorCol() + 1);
	clrtoeol();

	// Show the file position percentage
	mvprintw(LINES - 1, COLS - 4, "%d%%", win->getViewPercentage());
	clrtoeol();
}

std::string StatusBar::getCommand() {
	char str[256];
	move(LINES - 1, 0);
	printw(":");
	clrtoeol();
	echo();
	refresh();
	getstr(str);
	noecho();
	move(LINES - 1, 0);
	clrtoeol();
	return str;
}
