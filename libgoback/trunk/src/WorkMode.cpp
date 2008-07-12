#include "WorkMode.h"
#include "WorkModeHex.h"
#include "WorkModeDisasm.h"

WorkMode::WorkMode(DataSource *ds) : _dataSource(ds) {
}

WorkMode *WorkMode::create(std::string name, DataSource *ds) {
	if (!name.compare("hex")) {
		return new WorkModeHex(ds);
	} else if (!name.compare("disasm")) {
		return new WorkModeDisasm(ds);
	}

	return NULL;
}
