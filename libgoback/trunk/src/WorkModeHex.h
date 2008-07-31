#ifndef WORKMODEHEX_H
#define WORKMODEHEX_H

#include "WorkMode.h"

class WorkModeHexModule : public WorkModeModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Get the applicable format
	std::string getApplicableFormat() const;

	// WorkMode instantiation
	WorkMode *create(DataSource *ds) const;
};

class WorkModeHex : public WorkMode {
public:
	WorkModeHex(DataSource *ds);

	int getNumberLines();
	ViewLine getLine(int line);

protected:
	int _lineBytes;
};

#endif // WORKMODEHEX_H
