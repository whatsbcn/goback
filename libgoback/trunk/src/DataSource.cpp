#include "DataSource.h"
#include "FileDataSource.h"

DataSource *DataSource::create(std::string name, std::string format) {
	DataSource *ds = NULL;

	// Create the requested DataSource
	// TODO: use a list
	if (!name.compare("file")) {
		ds = new FileDataSource();
	}

	if (ds) {
		// Set the DataSource format
		ds->setDataFormat(format);
	}

	return ds;
}

std::list<std::string> DataSource::getWorkModes() {
	// TODO: Make it depend on the _dataFormat string
	std::list<std::string> modeslist;
	modeslist.push_back("hex");
	modeslist.push_back("disasm");

	return modeslist;
}
