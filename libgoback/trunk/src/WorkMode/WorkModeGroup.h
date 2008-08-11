#ifndef WORKMODEGROUP_H
#define WORKMODEGROUP_H

#include "WorkMode.h"
#include <vector>

class WorkModeGroupModule : public WorkModeModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Get the applicable format
	std::string getApplicableFormat() const;

	// WorkMode instantiation
	WorkMode *create(DataSource *ds) const;
};

class WorkModeGroup : public WorkMode {
public:
	WorkModeGroup(DataSource *ds);
	~WorkModeGroup();

	unsigned int getNumberLines();
	ViewLine getLine(unsigned int line);

protected:
	DataSource *_ds;
	unsigned int _numLines;
	std::vector<int> _sectionLastLine;
	std::vector<WorkMode *> _sectionWorkMode;

	unsigned int section;
	unsigned int getSection(unsigned int line);
	unsigned int getSectionLastLine(unsigned int line, unsigned int section);
};

#endif // WORKMODEGROUP_H
