#include "WorkModeGroup.h"
#include "DataSource.h"

#include <stdio.h>
#include <stdlib.h>
// WorkModeGroupModule

std::string WorkModeGroupModule::id() const {
	return "group";
}

std::string WorkModeGroupModule::name() const {
	return "Grouped section view";
}

std::string WorkModeGroupModule::getApplicableFormat() const {
	return "elf";
}

WorkMode *WorkModeGroupModule::create(DataSource *ds) const {
	return new WorkModeGroup(ds);
}

// WorkModeGroup

WorkModeGroup::WorkModeGroup(DataSource *ds) : WorkMode(ds) {
	DataSource *dss;
	WorkMode *wm;
	std::vector<std::string> modes;
	
	_numLines = 0;
    for (int i = 0; i < ds->getNumberSections(); i++) {
        dss = ds->getSection(i);
        modes = dss->getWorkModes();
        //TODO: use the default workmode, not the first.
        wm = WorkMode::create(modes.front(), dss);
        _numLines += wm->getNumberLines();
		_sectionLastLine.push_back(_numLines);
    }
	_ds = ds;
}

unsigned int WorkModeGroup::getNumberLines() {
	return _numLines;
}

/**
 * Get the mapping section of a global line
 * @param line The blogal line to get
 */
unsigned int WorkModeGroup::getSection(unsigned int line) {
	int i = 0;
	while (line > _sectionLastLine[i] - 1 && i < _sectionLastLine.size() - 1) {
		i++;
	}
	return i;
}

/**
 * Get the section line of a global line
 * @param line The blogal line to get
 */
unsigned int WorkModeGroup::getSectionLastLine(unsigned int line, unsigned int section) {
	if (section == 0) return line;
	return line - _sectionLastLine[section - 1];
}

/**
 * Get the global line of the group.
 * @param line The line to get
 * @return the line object string representation
 */
//TODO: don't ask every time for workmodes.
//TODO: cache created workmode to reuse.
ViewLine WorkModeGroup::getLine(unsigned int line) {
	DataSource *dss;
	WorkMode *wm;
	std::vector<std::string> modes;
	unsigned int section = getSection(line);
	dss = _ds->getSection(section);
	modes = dss->getWorkModes();
	wm = WorkMode::create(modes.front(), dss);

	return wm->getLine(getSectionLastLine(line, section));
}
