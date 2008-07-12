#ifndef WORKMODEDISASM_H
#define WORKMODEDISASM_H

#include "WorkMode.h"
#include <vector>

class WorkModeDisasm : public WorkMode {
public:
	WorkModeDisasm(DataSource *ds);

	int getNumberLines();
	ViewLine getLine(int line);

protected:
	//int disnone(FILE *, const char *, ...);
	std::vector<int> _linies;

};

#endif // WORKMODEDISASM_H
