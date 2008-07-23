#include "WorkMode.h"
#include "WorkModeHex.h"
#include "WorkModeDisasm.h"
#include "WorkModeElf.h"

WorkMode::WorkMode(DataSource *ds) : _dataSource(ds) {
}

WorkMode *WorkMode::create(std::string name, DataSource *ds) {
	WorkMode *wm = NULL;

	// Create the requested WorkMode
	//TODO: use a list
	if (!name.compare("hex")) {
		wm = new WorkModeHex(ds);
	} else if (!name.compare("disasm")) {
		wm = new WorkModeDisasm(ds);
	} else if (!name.compare("elf")) {
		wm = new WorkModeElf(ds);
	}

	return wm;
}
