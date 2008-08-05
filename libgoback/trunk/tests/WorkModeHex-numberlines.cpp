#include "DataSource.h"
#include "WorkMode.h"

int main(int argc, char **argv) {
	DataSource *fds = DataSource::create("file");

	// Try to open the specified file
	if (!fds->open(argv[1])) {
		delete fds;
		return 0;
	}

	// Create the Hex WorkMode and get the number of lines
	WorkMode *wm = WorkMode::create("hex", fds);
	int lines = wm->getNumberLines();

	delete wm;
	delete fds;
	return lines;
}
