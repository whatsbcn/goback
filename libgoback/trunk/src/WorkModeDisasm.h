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
	int disasmOp(int, int, struct ASM_INSN *);
	std::vector<int> _linies;
	char * _data;
};

#endif // WORKMODEDISASM_H
