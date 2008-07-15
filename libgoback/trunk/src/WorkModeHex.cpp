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
	char buff[11];

	// Read the line
	int bytes = _dataSource->readBytes(c, _lineBytes, line * _lineBytes);

	// Show the line number
	// TODO: Move it to the client? Maybe add a function to return the line
	// number to be shown for a given internal line number?
	sprintf(buff, "%08x: ", line);
	viewline.push_back(ViewBlock(buff, false));

	// Show the hexadecimal values
	for (int i = 0; i < bytes; i++) {
		if (i != 0) {
			// Add a space to separate from the previous hex block
			viewline.push_back(ViewBlock(" ", false));
		}

		// Prepare the hexadecimal representation
		sprintf(buff, "%02x", c[i]);
		viewline.push_back(ViewBlock(buff, true));
	}

	// Add the empty positions filling and separation spaces
	int gaps = (_lineBytes - bytes) * 3 + 2;
	viewline.push_back(ViewBlock(std::string(gaps, ' '), false));

	// ASCII representation
	buff[1] = 0;
	for (int i = 0; i < bytes; i++) {
		buff[0] = isprint(c[i]) ? c[i] : '.';
		viewline.push_back(ViewBlock(buff, true));
	}

	return viewline;
}
