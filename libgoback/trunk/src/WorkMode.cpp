#include "WorkMode.h"
#include "WorkModeHex.h"

WorkMode::WorkMode(DataSource *ds) : _dataSource(ds) {
}

WorkMode *WorkMode::create(std::string name, DataSource *ds) {
	if (!name.compare("hex")) {
		return new WorkModeHex(ds);
	}

	return NULL;
}
