#ifndef BUFFERWINDOW_H
#define BUFFERWINDOW_H

#include <DataFormat.h>

class WorkMode;

class BufferWindow {
public:
	BufferWindow(unsigned int x, unsigned int y, unsigned int w, unsigned int h);
	void setDataFormat(DataFormat *df);

	unsigned int getViewPercentage();

	// Cursor
	unsigned int getCursorLine();
	unsigned int getCursorCol();
	void cursorMoveUp();
	void cursorMoveDown();
	void cursorMoveLeft();
	void cursorMoveRight();
	void cursorMoveBeginning();
	void cursorMoveEnd();
	void cursorPageUp();
	void cursorPageDown();
	//void cursorSetPosition();
	void showCursor();

	void updateWindow();

private:
	// Window position and size
	unsigned int _x, _y, _w, _h;

	// View position
	unsigned int _viewLine, _viewCol, _viewSection;

	// Cursor position
	unsigned int _cursorViewLine, _cursorViewCol;

	// The WorkMode to show
	//WorkMode *_wm;
	// The DataFormat to show
	DataFormat *_df;
	unsigned int _numLines;

	// Helper functions
	void updateWindowLine(unsigned int windowLine, unsigned int sectionLine, WorkMode *wm);

	/* TODO:
	 * Possible events:
	 * - View Position changed
	 * - Cursor Position changed
	 */
};

#endif // BUFFERWINDOW_H
