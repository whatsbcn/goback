#include "DataSourceRange.h"
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

// Return a datasoruce with a limited range
DataSource *DataSource::createRange(int offset, int size, std::string format) {
	DataSourceRange *dsr = new DataSourceRange();
	dsr->setDataFormat(format);
	dsr->setRange(this, offset, size);
	return dsr;
}

std::list<std::string> DataSource::getWorkModes() {
	// TODO: Make it depend on the _dataFormat string
	std::list<std::string> modeslist;
	modeslist.push_back("hex");
	modeslist.push_back("disasm");

	return modeslist;
}

void DataSource::setDataFormat(std::string format) {
	_dataFormat = format;
}
