#include "BufferWindow.h"
#include <WorkMode.h>
#include <DataSource.h>

#include <curses.h>

BufferWindow::BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) :
	_x(x), _y(y), _w(w), _h(h), _viewLine(0), _viewCol(0), _viewSection(0), _numLines(0),
	_cursorViewLine(0), _cursorViewCol(0), _df(NULL) {
}

void BufferWindow::setDataFormat(DataFormat *df) {
	DataSource *ds;
	WorkMode *wm;
	_df = df;	
	for (int i = 0; i < _df->getNumberSections(); i++) {
		ds = _df->getSection(i);
		//TODO: this cannot be done only with the hex workmode!!, with this, the numlines are not exact
		wm = WorkMode::create("hex", ds);
		_numLines += wm->getNumberLines();
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
	} else {
		// Already on the top line, move the contents
		if (_viewLine > 0  || _viewSection > 0) {
			if (_viewLine == 0 && _viewSection > 0) {
            	_viewSection--;
				//TODO: Fix it! how?
            	DataSource *ds = _df->getSection(_viewSection);
            	WorkMode *wm = WorkMode::create("hex", ds);
           		_viewLine = wm->getNumberLines() - 1;
			} else {
				_viewLine--;
			}
		}
	}
}

void BufferWindow::cursorMoveDown() {
	if (_cursorViewLine + 1 < _h) {
		// Move the cursor one line down on the screen
		_cursorViewLine++;
	} else {
		// Already on the bottom line, move the contents
		if ((_viewLine + _h < _numLines) || (_df->getNumberSections() > _viewSection)) {
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

//TODO: Fix, it doesn't work
void BufferWindow::cursorPageUp() {
	if (_viewLine < _h && _viewSection == 0) {
		_viewLine = 0;
	} else {
		_viewLine -= _h;
	}
}

//TODO: Fix, it doesn't work
void BufferWindow::cursorPageDown() {
	if (_viewLine + _h * 2 > _numLines) {
		//TODO: Fix it! how?
        DataSource *ds = _df->getSection(_viewSection);
        WorkMode *wm = WorkMode::create("hex", ds);
		if (_viewLine < 0 && _viewSection >= _df->getNumberSections()) {
			_viewLine = wm->getNumberLines() - 1;
		}
	}
	_viewLine += _h;
}

void BufferWindow::showCursor() {
	move(_cursorViewLine, _cursorViewCol);
}

void BufferWindow::updateWindow() {
// Get the first view section
	DataSource *ds = _df->getSection(_viewSection);
	int startLine = _viewLine;
	int startSection = _viewSection;
	int i = 0, j = _viewLine;
	if (ds) {
		// Get the possible WorkModes
		//std::list<std::string> strWorkModes = ds->getWorkModes();
		// Use the first WorkMode for this section
		//TODO: Fix it! how?
		WorkMode *wm = WorkMode::create("hex", ds);
		
		if (_viewLine >= wm->getNumberLines()) {
			_viewLine = 0;
			_viewSection++;
			ds = _df->getSection(_viewSection);
			//TODO: Fix it! how?
			wm = WorkMode::create("hex", ds);
		}

		while (i < _h) {
			// It it is the last printable line from a section
			if (j >= wm->getNumberLines() && j != 0) {
				// If there are more sections, jump to the next
				if (_df->getNumberSections() > startSection + 1) {
					startSection++;
					ds = _df->getSection(startSection);
					//strWorkModes = ds->getWorkModes();
					//TODO: Fix it! how?
					wm = WorkMode::create("hex", ds);
					j = 0;
					startLine = 0;
				} else {
					move(_y + i, _x);
					printw("End of sections");
					clrtoeol(); 
				}
			} else {
				updateWindowLine(i, j, wm);
				i++;
				j++;
			}
		} 
			// If there are more sections
	} else {
		printw("ds == NULL");
	}
}

// Update a line in the window.
void BufferWindow::updateWindowLine(unsigned int windowLine, unsigned int sectionLine, WorkMode *wm) {
	if (sectionLine < wm->getNumberLines()) {
	ViewLine line = wm->getLine(sectionLine);
	move(_y + windowLine, _x);

	// Print the blocks
	ViewLine::iterator j = line.begin();
	while (j != line.end()) {
		printw("%s", j->_str.c_str());
		j++;
	}
	}
	clrtoeol(); // TODO: just clear until the end of the window
}
