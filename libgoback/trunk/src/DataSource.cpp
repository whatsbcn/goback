#include "DataSource.h"

std::list<std::string> DataSource::getWorkModes() {
	std::list<std::string> modeslist;
	modeslist.push_back("hex");

	return modeslist;
}
