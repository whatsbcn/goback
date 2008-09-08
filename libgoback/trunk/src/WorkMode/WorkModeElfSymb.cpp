#include "WorkModeElfSymb.h"
#include "DataSource.h"

#include <cstring>
#include <elf.h>

// WorkModeElfSymbModule

std::string WorkModeElfSymbModule::id() const {
	return "elfsymb";
}

std::string WorkModeElfSymbModule::name() const {
	return "Show Elf Symbols";
}

std::string WorkModeElfSymbModule::getApplicableFormat() const {
	return "elfSymb";
}

WorkMode *WorkModeElfSymbModule::create(DataSource *ds) const {
	return new WorkModeElfSymb(ds);
}

//TODO: check arch
WorkModeElfSymb::WorkModeElfSymb(DataSource *ds) : WorkMode(ds) {
	unsigned int count = _dataSource->size()/sizeof(Elf32_Dyn);
	Elf32_Dyn section;
	for (int i = 0; i < count; i++) {
		_dataSource->readBytes((char *)&section, sizeof(Elf32_Dyn), i * sizeof(Elf32_Dyn));
		switch (section.d_tag) {
		case DT_HASH:
			printf("DT_HASH\n");
			break;
		case DT_SYMTAB:
			printf("DT_SYMTAB\n");
			break;
		case DT_STRTAB:
			printf("DT_STRTAB\n");
			break;
		default:
			printf("default\n");
		}
	}
	/*
 * Load symbols from an ELF file. It tries to locate them on section header and on program header.
 * The _section structure has to be initialized.
 */
	//for each section, find if the section header that contains the symbols exist, if not, tries to use the program header.
}

unsigned int WorkModeElfSymb::getNumberLines() {
	return 0;
}

ViewLine WorkModeElfSymb::getLine(unsigned int line) {
	ViewLine viewline;
/*
	snprintf(pos, 32, "%s:%08x", sectionName.c_str(), _linies.at(line) + sectionAddress);

	// Put the file position of this opcode
	viewline.push_back(ViewBlock(pos, false));
	viewline.push_back(ViewBlock("    ", false));
*/
	return viewline;
}
