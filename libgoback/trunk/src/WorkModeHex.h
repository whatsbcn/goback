#include "WorkMode.h"

class WorkModeHex : public WorkMode {
public:
	int getNumberLines();
	ViewLine getLine(int line);
};
