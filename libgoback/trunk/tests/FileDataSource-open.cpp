#include "FileDataSource.h"

int main(int argc, char **argv) {
	FileDataSource fds;

	// Try to open the specified file
	if (fds.open(argv[1])) {
		return 0;
	}

	return 1;
}
