#ifndef WORKMODEELFSYMB_H
#define WORKMODEELFSYMB_H

#include "WorkMode.h"

#include <vector>

class WorkModeElfSymbModule : public WorkModeModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Get the applicable format
	std::string getApplicableFormat() const;

	// WorkMode instantiation
	WorkMode *create(DataSource *ds) const;
};

class WorkModeElfSymb : public WorkMode {
public:
	WorkModeElfSymb(DataSource *ds);

	unsigned int getNumberLines();
	ViewLine getLine(unsigned int line);

};

#endif // WORKMODEELFSYMB_H
