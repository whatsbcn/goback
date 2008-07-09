#include "WorkModeHex.h"

WorkModeHex::WorkModeHex(DataSource *ds) : WorkMode(ds) {
}

int WorkModeHex::getNumberLines() {
	return 1;
}

ViewLine WorkModeHex::getLine(int line) {
	ViewLine viewline;
	viewline.push_back(ViewBlock("asd", false));

	return viewline;
}
