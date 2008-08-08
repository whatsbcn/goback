#ifndef WORKMODEGROUP_H
#define WORKMODEGROUP_H

#include "WorkMode.h"

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

	unsigned int getNumberLines();
	ViewLine getLine(unsigned int line);

protected:
	DataSource *_ds;
	unsigned int _numLines;
	unsigned int _currLine;
	unsigned int _currSectionLine;
	unsigned int _currSection;
};

#endif // WORKMODEGROUP_H
