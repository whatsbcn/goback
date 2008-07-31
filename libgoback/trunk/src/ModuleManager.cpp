#include "ModuleManager.h"

// TODO: Find a better way to link the modules without including them here
#include "DataFormatElf.h"

ModuleManager *ModuleManager::_instance = NULL;

ModuleManager *ModuleManager::getInstance() {
	if (!_instance) {
		_instance = new ModuleManager();
	}

	return _instance;
}

ModuleManager::ModuleManager() {
	// Populate the lists of modules
	// TODO: This could be used to load plugins
	_dataFormats.push_back((DataFormatModule *)new DataFormatElfModule());
}

ModuleManager::~ModuleManager() {
	std::list<DataFormatModule *>::iterator df = _dataFormats.begin();
	while (df != _dataFormats.end()) {
		// Delete it
		delete *df;
	}
}

DataFormatModules ModuleManager::getDataFormats() const {
	return _dataFormats;
}
