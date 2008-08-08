#include "DataSourceRange.h"
#include "DataSourceFile.h"
#include "ModuleManager.h"

DataSource *DataSource::create(std::string name) {
	DataSource *ds = NULL;

	// Create the requested DataSource
	// TODO: use a list
	if (!name.compare("file")) {
		ds = new DataSourceFile();
	}

	return ds;
}

DataSource::~DataSource() {
	clearFormat();
}

void DataSource::clearFormat() {
	// Delete the DataSources allocated for the sections
	std::vector<DataSource *>::iterator i = _formatSections.begin();
	while (i != _formatSections.end()) {
		// Delete the current DataSource
		delete *i;

		i++;
	}

	// Remove the pointers
	_formatSections.clear();
}

std::vector<std::string> DataSource::detectFormat() {
	// List to return
	std::vector<std::string> detectedFormats;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Test all the available DataFormats
	DataFormatModules::const_iterator dfm = dataFormats.begin();
	while (dfm != dataFormats.end()) {
		// Try this format
		if ((*dfm)->detect(this)) {
			// Format detected, add it to the list
			detectedFormats.push_back((*dfm)->id());
		}
		dfm++;
	}

	return detectedFormats;
}

bool DataSource::setDataFormat(std::string format) {
	DataFormat *df = NULL;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Find the DataFormat with the specified id
	DataFormatModules::const_iterator dfm = dataFormats.begin();
	while ((!df) && (dfm != dataFormats.end())) {
		// Search the format id
		if (format == (*dfm)->id()) {
			// Create the DataFormat instance
			df = (*dfm)->create(this);
		}
		dfm++;
	}

	// Prepare the return value
	bool valid = false;

	if (df) {
		// We found the format, try to load the sections

		// Use temporary sections in case it can't be loaded correctly
		std::vector<DataSource *> newsections;

		// Found the format, try to load it
		if (df->load(newsections)) {
			// Loaded correctly, save the format name
			valid = true;
			_dataFormat = format;

			// Save the new sections, removing the old ones
			clearFormat();
			_formatSections = newsections;
		}

		// Delete the DataFormat, it already did its job
		delete df;
	} else {
		// Couldn't find the format. Supposing it will be implemented
		// in the future and accepting it as is, but without sections.
		valid = true;
		_dataFormat = format;
		clearFormat();
	}

	return valid;
}

unsigned int DataSource::getNumberSections() {
	return _formatSections.size();
}

DataSource *DataSource::getSection(unsigned int section) {
	// Check if the section exists
	if (_formatSections.size() > section) {
		return _formatSections[section];
	}
	return NULL;
}

std::vector<std::string> DataSource::getWorkModes() {
	std::vector<std::string> modeslist;

	// Get the list of available WorkMode modules
	WorkModeModules workModes;
	workModes = ModuleManager::getInstance()->getWorkModes();

	// Find the WorkModes applicable to the current format
	WorkModeModules::const_iterator wmm = workModes.begin();
	while (wmm != workModes.end()) {
		// Test whether it's a valid format for this module
		// TODO: Test with complete words
		//printf("DataSource(%s)->getWorkModes: %s, trying: %s\n", _name.c_str(), _dataFormat.c_str(), (*wmm)->getApplicableFormat().substr(0, _dataFormat.length()).c_str());
		if (_dataFormat.substr(0, (*wmm)->getApplicableFormat().length()) == (*wmm)->getApplicableFormat()) {
			// Add the WorkMode id to the list
			modeslist.push_back((*wmm)->id());
			//printf("Adding :%s\n",(*wmm)->id().c_str());
		}
		wmm++;
	}

	return modeslist;
}

// Return a DataSource with a limited range
DataSource *DataSource::createRange(std::string name, unsigned int offset, unsigned int size, unsigned int address) {
	DataSourceRange *dsr = new DataSourceRange();
	dsr->setName(name);
	dsr->setAddress(address);
	dsr->setRange(this, offset, size);
	return dsr;
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
