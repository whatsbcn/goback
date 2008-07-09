#include "WorkModeHex.h"
#include "DataSource.h"

WorkModeHex::WorkModeHex(DataSource *ds) : WorkMode(ds) {
	_lineBytes = 8;
}

int WorkModeHex::getNumberLines() {
	return _dataSource->size() / _lineBytes;
}

ViewLine WorkModeHex::getLine(int line) {
	ViewLine viewline;

	// Show the hexadecimal values
	for (int i = 0; i < _lineBytes; i++) {
		viewline.push_back(ViewBlock("XX", true));
		viewline.push_back(ViewBlock(" ", false));
	}

	// Separation space
	viewline.push_back(ViewBlock(" ", false));

	// ASCII representation
	for (int i = 0; i < _lineBytes; i++) {
		viewline.push_back(ViewBlock("a", true));
	}

	return viewline;
}
