#include "DataSource.h"
#include "../src/WorkModeHex.h"

int main(int argc, char **argv) {
	DataSource *fds = DataSource::create("file");

	// Try to open the specified file
	if (!fds->open(argv[1])) {
		delete fds;
		return 0;
	}

	// Create the Hex WorkMode and get the number of lines
	WorkModeHex wm(fds);
	int lines = wm.getNumberLines();

	delete fds;
	return lines;
}
