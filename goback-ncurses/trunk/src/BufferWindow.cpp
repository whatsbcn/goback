#include "BufferWindow.h"
#include <WorkMode.h>
#include <DataSource.h>

#include <curses.h>

BufferWindow::BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) :
	_x(x), _y(y), _w(w), _h(h), _viewLine(0), _viewCol(0), _viewSection(0),
	_cursorViewLine(0), _cursorViewCol(0), _df(NULL) {
}

void BufferWindow::setDataFormat(DataFormat *df) {
	_df = df;
}

unsigned int BufferWindow::getViewPercentage() {
	if (_viewLine + _h >= _numLines) {
		return 100;
	} else {
		return ((_viewLine + _h) * 100) / _numLines;
	}
}

/*
 * Cursor
 */
unsigned int BufferWindow::getCursorLine() {
	return _viewLine + _cursorViewLine;
}

unsigned int BufferWindow::getCursorCol() {
	return _viewCol + _cursorViewCol;
}

void BufferWindow::cursorMoveUp() {
	if (_cursorViewLine > 0) {
		// Move the cursor one line up on the screen
		_cursorViewLine--;
	} else {
		// Already on the top line, move the contents
		if (_viewLine > 0) {
			_viewLine--;
		}
	}
}

void BufferWindow::cursorMoveDown() {
	if (_cursorViewLine + 1 < _h) {
		// Move the cursor one line down on the screen
		_cursorViewLine++;
	} else {
		// Already on the bottom line, move the contents
		if (_viewLine + _h < _numLines) {
			_viewLine++;
			// I don't understand why I have to invalidate this line when I go down...
			wredrawln(stdscr, _h, 1);
		}
	}
}

void BufferWindow::cursorMoveRight() {
	if (_cursorViewCol + 1 < _w) {
		// Move the cursor one col right on the screen
		_cursorViewCol++;
	} 
}

void BufferWindow::cursorMoveLeft() {
	if (_cursorViewCol > 0) {
		// Move the cursor one col right on the screen
		_cursorViewCol--;
	} 
}

void BufferWindow::cursorMoveEnd() {
	// Move the cursor at the last col on the screen
	_cursorViewCol = _w - 1;
}

void BufferWindow::cursorMoveBeginning() {
	// Move the cursor at first column of the screen
	_cursorViewCol = 0;
}


void BufferWindow::cursorPageUp() {
	if (_viewLine < _h) {
		_viewLine = 0;
	} else {
		_viewLine -= _h;
	}
}

void BufferWindow::cursorPageDown() {
	if (_viewLine + _h * 2 > _numLines) {
		_viewLine = _numLines - _h;
		if (_viewLine < 0) {
			_viewLine = 0;
		}
	} else {
		_viewLine += _h;
	}
}

void BufferWindow::showCursor() {
	move(_cursorViewLine, _cursorViewCol);
}

void BufferWindow::updateWindow() {
	// Get the first view section
	DataSource *ds = _df->getSection(_viewSection);
	if (ds) {
		// Get the possible WorkModes
		std::list<std::string> strWorkModes = ds->getWorkModes();
		// Use the first WorkMode for this section
		WorkMode *wm = WorkMode::create(strWorkModes.front(), ds);

		for (int updatedLines = 0; updatedLines < _h; updatedLines += wm->getNumberLines()) {
			for (int i = 0; i < wm->getNumberLines() && i < _h; i++) {
				updateWindowLine(i, wm);
			} 
			// If there are more sections
			if (_df->getNumberSections() > _viewSection + 1) {
				_viewSection++;
				ds = _df->getSection(_viewSection);
				strWorkModes = ds->getWorkModes();
				wm = WorkMode::create(strWorkModes.front(), ds);
			} else {
				break;
			}
		}
	} else {
		printw("ds == NULL");
	}
}

// Update a line in the window.
void BufferWindow::updateWindowLine(unsigned int numline, WorkMode *wm) {

	ViewLine line = wm->getLine(_viewLine + numline);
	move(_y + numline, _x);

	// Print the blocks
	ViewLine::iterator j = line.begin();
	while (j != line.end()) {
		printw("%s", j->_str.c_str());
		clrtoeol(); // TODO: just clear until the end of the window
		j++;
	}
}
