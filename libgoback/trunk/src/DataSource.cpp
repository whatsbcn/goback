#include "DataSource.h"
#include "FileDataSource.h"

DataSource *DataSource::create(std::string name) {
	if (!name.compare("file")) {
		return new FileDataSource();
	}

	return NULL;
}

std::list<std::string> DataSource::getWorkModes() {
	std::list<std::string> modeslist;
	modeslist.push_back("hex");

	return modeslist;
}
