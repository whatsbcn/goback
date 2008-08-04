#include "BufferWindow.h"
#include <WorkMode.h>
#include <DataSource.h>

#include <curses.h>

BufferWindow::BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h) :
	_x(x), _y(y), _w(w), _h(h), _viewSectionLine(0), _viewCol(0), _viewSection(0), _numLines(0), _viewLine(0),
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
	int newline = _viewLine + displacement;
		DataSource *ds = _df->getSection(_viewSection);
		std::list<std::string> modes = ds->getWorkModes();
		WorkMode *wm = WorkMode::create(modes.front(), ds);
		/** If we are going to the nex line */
		if (displacement == 1) {
			/** If there are more lines in the section, jump to next line. */
			if ((_viewLine + _h < _numLines) && (_viewSectionLine < wm->getNumberLines())) {
				_viewLine++;
                _viewSectionLine++;
			/** If there aren't more lines but sections, jump to the next. */
			} else if (_viewSection < _df->getNumberSections() - 1) {
				_viewSectionLine = 1;
				_viewSection++;		
				_viewLine++;
			} 
		/** If we are going to the prev line */
		} else if (displacement == -1) {
			/** If there was more lines in the section, jump to the prev line */
			if (_viewSectionLine > 0) {
				_viewLine--;
				_viewSectionLine--;
			/** If there wasn't more lines but sections, jump to the prev. */
			} else if (_viewSection > 0) {
				_viewSection--;		
				_viewLine--;
				ds = _df->getSection(_viewSection);
				modes = ds->getWorkModes();
				wm = WorkMode::create(modes.front(), ds);
				_viewSectionLine = wm->getNumberLines() - 1;
			}
		/** If there AvPag and there are at least _h lines or more sections */
	//	}// else if (displacement == _h && ((_viewSectionLine + _h) < wm->getNumberLines()) || (_viewSection < _df->getNumberSections())) {
			/** If we are at the end of the section */
		/*	if ((_viewSectionLine + _h) >= wm->getNumberLines()) {
	printf("if");
				_viewLine += wm->getNumberLines() - _viewSectionLine;
				_viewSectionLine = 0;
				_viewSection++;		
		*/	/** If we have sufficient lines in the same section */
		/*	} else {
	printf("else");
				_viewLine += _h;
				_viewSectionLine += _h;
			}		
		//TODO: fix it!
		} else if (displacement == -_h) {
			if (_viewSectionLine - _h < 0 && _viewSection != 0) {
				_viewSection--;		
				ds = _df->getSection(_viewSection);
				modes = ds->getWorkModes();
				wm = WorkMode::create(modes.front(), ds);
				_viewLine -= _viewSectionLine;
				_viewSectionLine = wm->getNumberLines() - 1;
			} else {
				_viewLine -= _h;
				_viewSectionLine -= _h;
			}		*/
	//	}
	} else {
		printf("displacement unknown");
	}
}

//TODO: use the window scrolling to improve performance
void BufferWindow::updateWindow() {
	// Get the first view section
	DataSource *ds = _df->getSection(_viewSection);
	if (ds) {
		std::list<std::string> modes = ds->getWorkModes();
		WorkMode *wm = WorkMode::create(modes.front(), ds);
		
		int startLine = _viewSectionLine;
		int startSection = _viewSection;
		int i = 0, j = _viewSectionLine;

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
					startLine = 0;
				}
			}
			updateWindowLine(i, j, wm, ds);
			i++;
			j++;
		} 
			// If there are more sections
	} else {
		printw("There is no sections in the file :S");
	}
}

// Update a line in the window.
void BufferWindow::updateWindowLine(unsigned int windowLine, unsigned int sectionLine, WorkMode *wm, DataSource *ds) {
	if (sectionLine < wm->getNumberLines()) {
	ViewLine line = wm->getLine(sectionLine, ds->getName(), ds->getAddress());
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
