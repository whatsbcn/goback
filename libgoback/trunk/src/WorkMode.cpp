#include "ModuleManager.h"
#include "WorkMode.h"

WorkMode::WorkMode(DataSource *ds) : _dataSource(ds) {
}

WorkMode *WorkMode::create(std::string id, DataSource *ds) {
	WorkMode *wm = NULL;

	// Get the list of available WorkMode modules
	WorkModeModules workModes;
	workModes = ModuleManager::getInstance()->getWorkModes();

	// Find the WorkMode with the specified id
	WorkModeModules::const_iterator wmm = workModes.begin();
	while ((!wm) && (wmm != workModes.end())) {
		// Search the format id
		if (id == (*wmm)->id()) {
			// Create the DataFormat instance
			wm = (*wmm)->create(ds);
		}
		wmm++;
	}

	return wm;
}
