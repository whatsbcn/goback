#ifndef WORKMODEHEX_H
#define WORKMODEHEX_H

#include "WorkMode.h"

class WorkModeHex : public WorkMode {
public:
	WorkModeHex(DataSource *ds);

	int getNumberLines();
	ViewLine getLine(int line);

protected:
	int _lineBytes;

};

#endif // WORKMODEHEX_H
