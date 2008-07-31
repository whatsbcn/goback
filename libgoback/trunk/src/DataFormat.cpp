#include "DataFormat.h"
#include "ModuleManager.h"

std::list<std::string> DataFormat::detect(DataSource *ds) {
	// List to return
	std::list<std::string> detectedFormats;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Test all the available DataFormats
	std::list<DataFormatModule *>::const_iterator df = dataFormats.begin();
	while (df != dataFormats.end()) {
		// Try this format
		if ((*df)->detect(ds)) {
			// Format detected, add it to the list
			detectedFormats.push_back((*df)->id());
		}
		df++;
	}

	return detectedFormats;
}

DataFormat *DataFormat::create(std::string id, DataSource *ds) {
	DataFormat *df = NULL;

	// Get the list of available DataFormat modules
	DataFormatModules dataFormats;
	dataFormats = ModuleManager::getInstance()->getDataFormats();

	// Find the DataFormat with the specified id
	std::list<DataFormatModule *>::const_iterator dfl = dataFormats.begin();
	while ((!df) && (dfl != dataFormats.end())) {
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
