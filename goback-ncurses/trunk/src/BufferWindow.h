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
	void gotoLine(int distance);

	void updateWindow();

private:
	// Window position and size
	unsigned int _x, _y, _w, _h;

	/**
 	* View position
 	* _viewLine: the global line numer. This is the first line in the view.
 	* _viewSectionLine: the relative line number in the section. This is the first line in the view.
 	* _viewCol: the first column in the view.
 	* _viewSection: the first section in the view.
 	*/
	unsigned int _viewLine, _viewSectionLine, _viewCol, _viewSection;

	// Cursor position
	unsigned int _cursorViewLine, _cursorViewCol;

	// The WorkMode to show
	//WorkMode *_wm;
	// The DataFormat to show
	DataFormat *_df;
	unsigned int _numLines;

	// Helper functions
	void updateWindowLine(unsigned int windowLine, unsigned int sectionLine, WorkMode *wm, DataSource *ds);

	/* TODO:
	 * Possible events:
	 * - View Position changed
	 * - Cursor Position changed
	 */
};

#endif // BUFFERWINDOW_H
