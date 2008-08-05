#include "DataFormat.h"
#include "ModuleManager.h"

std::list<std::string> DataFormat::detect(DataSource *ds) {
	// List to return
	std::list<std::string> detectedFormats;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Test all the available DataFormats
	DataFormatModules::const_iterator dfm = dataFormats.begin();
	while (dfm != dataFormats.end()) {
		// Try this format
		if ((*dfm)->detect(ds)) {
			// Format detected, add it to the list
			detectedFormats.push_back((*dfm)->id());
		}
		dfm++;
	}

	return detectedFormats;
}

DataFormat *DataFormat::create(std::string id, DataSource *ds) {

	DataFormat *df = NULL;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Find the DataFormat with the specified id
	DataFormatModules::const_iterator dfm = dataFormats.begin();
	while ((!df) && (dfm != dataFormats.end())) {
		// Search the format id
		if (id == (*dfm)->id()) {
			// Create the DataFormat instance
			df = (*dfm)->create(ds);
		}
		dfm++;
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
	// Check if the section exists
	if (_formatSections.size() > section) {
		return _formatSections[section];
	}
	return NULL;
}
