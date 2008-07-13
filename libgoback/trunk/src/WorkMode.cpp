#include "WorkMode.h"
#include "WorkModeHex.h"
#include "WorkModeDisasm.h"
#include "WorkModeElf.h"

WorkMode::WorkMode(DataSource *ds) : _dataSource(ds) {
}

WorkMode *WorkMode::create(std::string name, DataSource *ds) {
	if (!name.compare("hex")) {
		return new WorkModeHex(ds);
	} else if (!name.compare("disasm")) {
		return new WorkModeDisasm(ds);
	} else if (!name.compare("elf")) {
		return new WorkModeElf(ds);
	}

	return NULL;
}
