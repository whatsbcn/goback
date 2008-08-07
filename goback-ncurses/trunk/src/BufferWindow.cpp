#include "BufferWindow.h"
#include <WorkMode.h>
#include <DataSource.h>

#include <curses.h>

BufferWindow::BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) :
	_x(x), _y(y), _w(w), _h(h), _viewFirstSectionLine(0), _viewCol(0), _viewFirstSection(0), _numLines(0), _viewFirstLine(0),
	_cursorViewLine(0), _cursorViewCol(0), _df(NULL) {
}

void BufferWindow::setDataFormat(DataFormat *df) {
	DataSource *ds;
	WorkMode *wm;
	std::list<std::string> modes;
	_df = df;	
	for (int i = 0; i < _df->getNumberSections(); i++) {
		ds = _df->getSection(i);
		modes = ds->getWorkModes();
		//TODO: use the default workmode, not the first.
		wm = WorkMode::create(modes.front(), ds);
		_numLines += wm->getNumberLines();
	}
}

unsigned int BufferWindow::getViewPercentage() {
	if (_viewFirstLine + _h >= _numLines) {
		return 100;
	} else {
		return ((_viewFirstLine + _h) * 100) / _numLines;
	}
}

/*
 * Cursor
 */
unsigned int BufferWindow::getCursorLine() {
	return _viewFirstLine + _cursorViewLine;
}

unsigned int BufferWindow::getCursorCol() {
	return _viewCol + _cursorViewCol;
}

void BufferWindow::cursorMoveUp() {
	if (_cursorViewLine > 0) {
		// Move the cursor one line up on the screen
		_cursorViewLine--;
	} else {
		gotoLine(-1);
	}
}

void BufferWindow::cursorMoveDown() {
	if (_cursorViewLine + 1 < _h) {
		// Move the cursor one line down on the screen
		_cursorViewLine++;
	} else {
		gotoLine(1);
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
	gotoLine(-_h);
}

void BufferWindow::cursorPageDown() {
	gotoLine(+_h);
}

void BufferWindow::showCursor() {
	move(_cursorViewLine, _cursorViewCol);
}

void BufferWindow::gotoLine(int displacement) {
	int newline = _viewFirstLine + displacement;
		DataSource *ds = _df->getSection(_viewFirstSection);
		std::list<std::string> modes = ds->getWorkModes();
		WorkMode *wm = WorkMode::create(modes.front(), ds);
		/** If we are going to the nex line */
		if (displacement == 1) {
			/** If there are more lines in the section, jump to next line. */
			if ((_viewFirstLine + _h < _numLines) && (_viewFirstSectionLine < wm->getNumberLines() - 1)) {
				_viewFirstLine++;
                _viewFirstSectionLine++;
				//scrollLines(_h - 1, _viewFirstSectionLine, wm, 1, 1);
				//TODO: eleminate this, and use scrolling
				updateWindow();
			/** If there aren't more lines but sections, jump to the next. */
			} else if (_viewFirstSection < _df->getNumberSections() - 1) {
				_viewFirstSectionLine = 0;
				_viewFirstSection++;		
				_viewFirstLine++;
				//scrollLines(_h - 1, _viewFirstSectionLine, wm, 1, 1);
				//TODO: eleminate this, and use scrolling
				updateWindow();
			} 
		/** If we are going to the prev line */
		} else if (displacement == -1) {
			/** If there was more lines in the section, jump to the prev line */
			if (_viewFirstSectionLine > 0) {
				_viewFirstLine--;
				_viewFirstSectionLine--;
				scrollLines(0, _viewFirstSectionLine, wm, 1, -1);
			/** If there wasn't more lines but sections, jump to the prev. */
			} else if (_viewFirstSection > 0) {
				_viewFirstSection--;		
				_viewFirstLine--;
				ds = _df->getSection(_viewFirstSection);
				modes = ds->getWorkModes();
				wm = WorkMode::create(modes.front(), ds);
				_viewFirstSectionLine = wm->getNumberLines() - 1;
				scrollLines(0, _viewFirstSectionLine, wm, 1, -1);
			}
		/** If we are going to the next page */
		} else if (displacement == _h ) {
			/** If there are more lines in the section, jump to next line. */
			if ((_viewFirstLine + (_h * 2) < _numLines) && (_viewFirstSectionLine + _h < wm->getNumberLines() - 1)) {
				_viewFirstLine += _h;
                _viewFirstSectionLine += _h;
				//TODO: eleminate this, and use scrolling
				updateWindow();
			/** If there aren't more lines but sections, jump to the next section. */
			} else if (_viewFirstSection < _df->getNumberSections() - 1) {
				_viewFirstSection++;		
				_viewFirstLine += wm->getNumberLines() - _viewFirstSectionLine;
				_viewFirstSectionLine = 0;
				//TODO: eleminate this, and use scrolling
				updateWindow();
			} 
		} else if (displacement == -_h) {
			/** If there was more lines in the section, jump to the prev line */
			if (_viewFirstSectionLine > _h) {
				_viewFirstLine -= _h;
				_viewFirstSectionLine -= _h;
				scrollLines(0, _viewFirstSectionLine, wm, _h, -1);
			/** If there wasn't more lines but sections, jump to the prev. */
			} else if (_viewFirstSection > 0) {
				_viewFirstSection--;		
				_viewFirstLine -= _viewFirstSectionLine;
				scrollLines(0, 0, wm, _viewFirstSectionLine, -1);
				ds = _df->getSection(_viewFirstSection);
				modes = ds->getWorkModes();
				wm = WorkMode::create(modes.front(), ds);
				_viewFirstSectionLine = wm->getNumberLines() - 1;
				scrollLines(0, _viewFirstSectionLine, wm, 1, -1);
			} else {
				scrollLines(0, 0, wm, _viewFirstSectionLine, -1);
				_viewFirstLine = 0;
				_viewFirstSectionLine = 0;
				_viewFirstSection = 0;
			}
	} else {
		printf("displacement unknown");
	}
}

//TODO: use the window scrolling to improve performance
void BufferWindow::updateWindow() {
	// Get the first view section
	DataSource *ds = _df->getSection(_viewFirstSection);
	if (ds) {
		std::list<std::string> modes = ds->getWorkModes();
		WorkMode *wm = WorkMode::create(modes.front(), ds);
		
		int startSection = _viewFirstSection;
		int i = 0, j = _viewFirstSectionLine;

		while (i < _h) {
			// It it is the last printable line from a section
			if (j >= wm->getNumberLines() && j != 0) {
				// If there are more sections, jump to the next
				if (_df->getNumberSections() > startSection + 1) {
					startSection++;
					ds = _df->getSection(startSection);
					modes = ds->getWorkModes();
					wm = WorkMode::create(modes.front(), ds);
					j = 0;
				}
			}
			updateWindowLine(i, j, wm);
			i++;
			j++;
		} 
			// If there are more sections
	} else {
		printw("There is no sections in the file :S");
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
	} else {
		move(_y + windowLine, _x);
		printw("Trying to acces out of range: sectionLine:%d numberLines:%d", sectionLine, wm->getNumberLines());
	}
	clrtoeol(); // TODO: just clear until the end of the window
}

/**
 * Update count lines
 */
void BufferWindow::scrollLines(unsigned int windowLine, unsigned int sectionLine, WorkMode *wm, unsigned int count, int down) {
	if (down > 0) {
		for (int i = 0; i < count; i++) {
			scrl(down);
			updateWindowLine(windowLine, sectionLine + i, wm);
		}
	} else {
		for (int i = 0; i < count; i++) {
			scrl(down);
			updateWindowLine(windowLine, sectionLine + count - 1 - i, wm);
		}
	}
}
