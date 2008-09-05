#include "BufferWindow.h"
#include <WorkMode.h>
#include <DataSource.h>

#include <curses.h>

BufferWindow::BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) :
	_x(x), _y(y), _w(w), _h(h), _viewSectionLine(0), _viewCol(0), _viewSection(0), _numLines(0), _viewLine(0),
	_cursorViewLine(0), _cursorViewCol(0), _wm(NULL), _ds(NULL) {
}

BufferWindow::~BufferWindow() {
	delete _wm;
}

void BufferWindow::setDataSource(DataSource *ds) {
	std::vector<std::string> modes = ds->getWorkModes();
	if (modes.size() != 0) {
		_wm = WorkMode::create(modes.front(), ds);
		_ds = ds;
		_numLines = _wm->getNumberLines();
	} else {
		printw("I can't work with this file!\n");
	}
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
	} else if  (_viewLine > 0) {
		scrollUp(1);		
	}
}

void BufferWindow::cursorMoveDown() {
	if (_cursorViewLine + 1 < _h) {
		// Move the cursor one line down on the screen
		_cursorViewLine++;
	} else if (_viewLine + _h < _numLines) {
		scrollDown(1);		
		// I don't understand why I have to invalidate this line when I go down...
		wredrawln(stdscr, _h, 1);
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
	if  (_viewLine > 0) {
		if (_viewLine + _h >= _h * 2) {
			scrollUp(_h);		
		} else {
			scrollUp(_viewLine);
		}
	}
}

void BufferWindow::cursorPageDown() {
	if (_viewLine < _numLines) {
		if (_viewLine + (_h * 2) < _numLines) {
			scrollDown(_h);		
		} else {
			scrollDown(_numLines - _viewLine - _h);
		}
	}
}

void BufferWindow::showCursor() {
	move(_cursorViewLine, _cursorViewCol);
}

/**
 * Update all the window lines
 */
void BufferWindow::updateWindow() {
    for (int i = 0; i < _numLines && i < _h; i++) {
        updateWindowLine(i);
    }
}

/**
 *  Update a line in the window.
 *  @param numline the line number to update
 */
void BufferWindow::updateWindowLine(unsigned int numline) {
        ViewLine line = _wm->getLine(_viewLine + numline);
        move(_y + numline, _x);

        // Print the blocks
        ViewLine::iterator j = line.begin();
        while (j != line.end()) {
                printw("%s", j->_str.c_str());
                j++;
        }
        clrtoeol(); // TODO: just clear until the end of the window
}

/**
 * Scroll down count lines
 * @param count lines to scroll down
 */
void BufferWindow::scrollDown(unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		scrl(1);
		_viewLine++;
		updateWindowLine(_h - 1);
	}
}

/**
 * Scroll up count lines
 * @param count lines to scroll up
 */
void BufferWindow::scrollUp(unsigned int count) {
	for (unsigned int i = 0; i < count; i++) {
		scrl(-1);
		_viewLine--;
		updateWindowLine(0);
	}
}
