#include "WorkModeHex.h"
#include "DataSource.h"

// WorkModeHexModule

std::string WorkModeHexModule::id() const {
	return "hex";
}

std::string WorkModeHexModule::name() const {
	return "Hexadecimal view";
}

std::string WorkModeHexModule::getApplicableFormat() const {
	return "";
}

WorkMode *WorkModeHexModule::create(DataSource *ds) const {
	return new WorkModeHex(ds);
}

// WorkModeHex

WorkModeHex::WorkModeHex(DataSource *ds) : WorkMode(ds) {
	_lineBytes = 8;
}

unsigned int WorkModeHex::getNumberLines() {
	int bytes = _dataSource->size();
	int numLines = bytes / _lineBytes;
	if (bytes % _lineBytes) {
		numLines++;
	}
	return numLines;
}

ViewLine WorkModeHex::getLine(unsigned int line) {
	ViewLine viewline;
	char *c = new char[_lineBytes];
	char buff[11];

	// Get the name of the section
	Value *val = _dataSource->getProperty("SectionName");
	std::string sectionName = "";
	if (val) {
		sectionName = val->getString();
	}

	// Get the address of the section
	val = _dataSource->getProperty("SectionAddress");
	long int sectionAddress = 0;
	if (val) {
		sectionAddress = val->getInt();
	}

	// Show the line number
	// TODO: Move it to the client? Maybe add a function to return the line
	// number to be shown for a given internal line number?
	sprintf(buff, "%s:%08x: ", sectionName.c_str(), line + sectionAddress);
	viewline.push_back(ViewBlock(buff, false));

	// Read the line
	int bytes = _dataSource->readBytes(c, _lineBytes, line * _lineBytes);

	// Show the hexadecimal values
	for (int i = 0; i < bytes; i++) {
		if (i != 0) {
			// Add a space to separate from the previous hex block
			viewline.push_back(ViewBlock(" ", false));
		}

		// Prepare the hexadecimal representation
		sprintf(buff, "%02hhx", c[i]);
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

	// Free the temporary buffer
	delete[] c;

	return viewline;
}
