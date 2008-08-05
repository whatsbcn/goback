#include "DataSourceRange.h"
#include "DataSourceFile.h"
#include "ModuleManager.h"

DataSource *DataSource::create(std::string name, std::string format) {
	DataSource *ds = NULL;

	// Create the requested DataSource
	// TODO: use a list
	if (!name.compare("file")) {
		ds = new DataSourceFile();
	}

	if (ds) {
		// Set the DataSource format
		ds->setDataFormat(format);
	}

	return ds;
}

// Return a DataSource with a limited range
DataSource *DataSource::createRange(std::string name, unsigned int offset, unsigned int size, unsigned int address, std::string format) {
	DataSourceRange *dsr = new DataSourceRange();
	dsr->setName(name);
	dsr->setAddress(address);
	dsr->setDataFormat(format);
	dsr->setRange(this, offset, size);
	return dsr;
}

std::list<std::string> DataSource::getWorkModes() {
	std::list<std::string> modeslist;

	// Get the list of available WorkMode modules
	WorkModeModules workModes;
	workModes = ModuleManager::getInstance()->getWorkModes();

	// Find the WorkModes applicable to the current format
	WorkModeModules::const_iterator wmm = workModes.begin();
	while (wmm != workModes.end()) {
		// Test whether it's a valid format for this module
		// TODO: Test with complete words
		if (_dataFormat == (*wmm)->getApplicableFormat().substr(0, _dataFormat.length())) {
			// Add the WorkMode id to the list
			modeslist.push_back((*wmm)->id());
		}
		wmm++;
	}

	return modeslist;
}

void DataSource::setDataFormat(std::string format) {
	_dataFormat = format;
}

std::string DataSource::getName() {
	return _name;
}

unsigned int DataSource::getAddress() {
	return _address;
}

void DataSource::setName(std::string name) {
	_name = name;
}

void DataSource::setAddress(unsigned int address) {
	_address = address;
}
