#include "DataFormat.h"
#include "DataFormatElf.h"

DataFormatManager::DataFormatManager() {
	_dataFormats.push_back((DataFormatModule *)new DataFormatElfModule());
}

DataFormatManager::~DataFormatManager() {
	std::list<DataFormatModule *>::iterator df = _dataFormats.begin();
	while (df != _dataFormats.end()) {
		// Delete it
		delete *df;
	}
}

std::list<std::string> DataFormatManager::detect(DataSource *ds) const {
	// List to return
	std::list<std::string> detectedFormats;

	std::list<DataFormatModule *>::const_iterator df = _dataFormats.begin();
	while (df != _dataFormats.end()) {
		// Try this format
		if ((*df)->detect(ds)) {
			// Format detected, add it to the list
			detectedFormats.push_back((*df)->id());
		}
		df++;
	}

	return detectedFormats;
}

DataFormat *DataFormatManager::create(std::string id, DataSource *ds) const {
	DataFormat *df = NULL;

	std::list<DataFormatModule *>::const_iterator dfl = _dataFormats.begin();
	while ((!df) && (dfl != _dataFormats.end())) {
		// Search the format id
		if (id == (*dfl)->id()) {
			// Create the DataFormat instance
			df = (*dfl)->create(ds);
		}
		df++;
	}

	if (df) {
		// Load the format
		if (!df->load()) {
			// Couldn't load correctly
			delete df;
			df = NULL;
		}
	}

	return df;
}

DataFormat::DataFormat(DataSource *ds) :
	_dataSource(ds) {
}

unsigned int DataFormat::getNumberSections() {
	return _formatSections.size();
}

DataSource *DataFormat::getSection(unsigned int section) {
	//return _formatSections[section];
	return NULL;
}
