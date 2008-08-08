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
	
    for (int i = 0; i < ds->getNumberSections(); i++) {
        dss = ds->getSection(i);
        modes = dss->getWorkModes();
        //TODO: use the default workmode, not the first.
        wm = WorkMode::create(modes.front(), ds);
        _numLines += wm->getNumberLines();
    }
	_currLine = 0;
	_currSectionLine = 0;
	_currSection = 0;
	_ds = ds;
}

unsigned int WorkModeGroup::getNumberLines() {
	return _numLines;
}
/**
 * Get the global line of the group, but caching the actual position to speedup normal operations
 * @param line The line to get
 * @return the line object string representation
 */
//TODO: don't ask every time for workmodes!
ViewLine WorkModeGroup::getLine(unsigned int line) {
	if (line < 0 || line > _numLines) {
		ViewLine line;
		return line;
	}
	int displacement = line - _currLine;
	DataSource *dss;
	WorkMode *wm;
	std::vector<std::string> modes;
	if (displacement == 0) {
		dss = _ds->getSection(_currSection);
		modes = dss->getWorkModes();
		wm = WorkMode::create(modes.front(), dss);
	/** If we are going to the nex line */
	} else if (displacement == 1) {
		dss = _ds->getSection(_currSection);
		std::vector<std::string> modes = dss->getWorkModes();
		wm = WorkMode::create(modes.front(), dss);
		/** If there are more lines in the section, jump to next line. */
		if (_currSectionLine < wm->getNumberLines() - 1) {
			_currLine++;
			_currSectionLine++;
		/** If there aren't more lines but sections, jump to the next. */
		} else if (_currSection < _ds->getNumberSections() - 1) {
			_currLine++;
			_currSection++;
			_currSectionLine = 0;
			dss = _ds->getSection(_currSection);
			modes = dss->getWorkModes();
			wm = WorkMode::create(modes.front(), dss);
		}
	/** If we are going to the prev line */
	} else if (displacement == -1) {
		/** If there was more lines in the section, jump to the prev line */
		if (_currSectionLine > 0) {
			_currLine--;
			_currSectionLine--;
		/** If there wasn't more lines but sections, jump to the prev. */
		} else if (_currSection > 0) {
			_currSection--;
			_currLine--;
			dss = _ds->getSection(_currSection);
			modes = dss->getWorkModes();
			wm = WorkMode::create(modes.front(), dss);
			_currSectionLine = wm->getNumberLines() - 1;
		}
	/** If we are going to the next page */
	} else if (displacement > 0) {
		/** If there are more lines in the section, jump to next line. */
		if ((_currLine + displacement < _numLines)) {// && (_currSectionLine + displacement <  wm->getNumberLines() - 1)) {
		//if ((_currLine + displacement < _numLines) && (_currSectionLine + displacement <  wm->getNumberLines() - 1)) {
			_currLine += displacement;
			_currSectionLine += displacement;
		/** If there aren't more lines but sections, jump to the next section. */
		} else if (_currSection < _ds->getNumberSections() - 1) {
			//TODO: change this, it will be a bucle to arrive at line
			_currSection++;
			_currLine += wm->getNumberLines() - _currSectionLine;
			_currSectionLine = 0;
			dss = _ds->getSection(_currSection);
			modes = dss->getWorkModes();
			wm = WorkMode::create(modes.front(), dss);
		}
	} else if (displacement < 0) {
		/** If there was more lines in the section, jump to the prev line */
		if (_currSectionLine > displacement) {
			_currLine -= displacement;
			_currSectionLine -= displacement;
		/** If there wasn't more lines but sections, jump to the prev. */
		} else if (_currSectionLine > 0) {
			//TODO: change this, it will be a bucle to arrive at line
			_currSection--;
			_currLine -= _currSectionLine;
			dss = _ds->getSection(_currSection);
			modes = dss->getWorkModes();
			wm = WorkMode::create(modes.front(), dss);
			_currSectionLine = wm->getNumberLines() - 1;
		}
	} else {
		ViewLine lines;
		return lines;
	}
	
		/*ViewLine lines;
		char buffer[33];
		sprintf(buffer,"%d",_currSectionLine);
		lines.push_back(ViewBlock(buffer, true));;
		return lines;*/
	return wm->getLine(_currSectionLine);
}
