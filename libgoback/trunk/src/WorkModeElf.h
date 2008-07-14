#ifndef WORKMODEELF_H
#define WORKMODEELF_H

#include "WorkMode.h"
#include <vector>

class WorkModeElf : public WorkMode {
public:
	WorkModeElf(DataSource *ds);

	int getNumberLines();
	ViewLine getLine(int line);

protected:
	struct intString {
		std::string name;
		int offset;
		int addr;
	};
	void indexSection(int, int);
	int disasmOp(int, struct ASM_INSN *, int, int);
	struct intString *findSection(int);
	std::vector<int> _linies;
	std::vector<intString> _sections;
};

#endif // WORKMODEELF_H
