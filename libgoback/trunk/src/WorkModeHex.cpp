#include "WorkModeHex.h"
#include "DataSource.h"

WorkModeHex::WorkModeHex(DataSource *ds) : WorkMode(ds) {
	_lineBytes = 8;
}

int WorkModeHex::getNumberLines() {
	int bytes = _dataSource->size();
	int numLines = bytes / _lineBytes;
	if (bytes % _lineBytes) {
		numLines++;
	}
	return numLines;
}

ViewLine WorkModeHex::getLine(int line) {
	ViewLine viewline;
	char c[_lineBytes];
	char buff[9];

	// Read the line
	int bytes = _dataSource->readBytes(c, _lineBytes, line * _lineBytes);

	sprintf(buff, "%08x:", line);
	viewline.push_back(ViewBlock(buff, true));
	viewline.push_back(ViewBlock(" ", false));

	// Show the hexadecimal values
	for (int i = 0; i < bytes; i++) {
		sprintf(buff, "%02x", c[i]);
		buff[2] = '\0';
		viewline.push_back(ViewBlock(buff, true));
		viewline.push_back(ViewBlock(" ", false));
	}
	// Fill the gaps with spaces
	int gaps = _lineBytes - bytes;
	for (int i = 0; i < gaps; i++) {
		viewline.push_back(ViewBlock("  ", false));
		viewline.push_back(ViewBlock(" ", false));
	}

	// Separation space
	viewline.push_back(ViewBlock(" ", false));

	// ASCII representation
	for (int i = 0; i < bytes; i++) {
		sprintf(buff, "%c", isprint(c[i]) ? c[i] : '.');
		buff[1] = '\0';
		viewline.push_back(ViewBlock(buff, true));
	}

	// Fill the gaps with spaces
	for (int i = 0; i < gaps; i++) {
		viewline.push_back(ViewBlock(" ", false));
	}

	return viewline;
}
