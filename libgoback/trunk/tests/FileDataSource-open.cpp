#include "DataSource.h"

int main(int argc, char **argv) {
	DataSource *fds = DataSource::create("file");

	// Try to open the specified file
	if (fds->open(argv[1])) {
		delete fds;
		return 0;
	}

	delete fds;
	return 1;
}
