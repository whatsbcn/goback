#include "WorkModeElf.h"
#include "DataSource.h"

#include <cstdlib>
#include <cstring>
#include <elf.h>
#include <bfd.h>
#include <dis-asm.h>
#include <stdarg.h>

extern int disNone(FILE *stream, const char *format, ...);

struct ASM_INSN {
	char mnemonic[16];
	char src[32];
	char dest[32];
	char arg[32];
	char arg2[32];
	char arg3[32];
} curr_insn2;

// Application data to embedd into the disassemble_info structure
struct DisasmAppData2 {
	DataSource *ds;
	WorkModeElf *wm;
	int secvma;
	int secoff;
	//TODO: move curr_insn here?
};

/*
 * Disasm memory reader callback
 */
static int disReadMemory(bfd_vma memaddr, bfd_byte *myaddr, unsigned int len,
	struct disassemble_info *info) {

	DataSource *ds = ((DisasmAppData2 *)info->application_data)->ds;
	if (memaddr + len <= ds->size()) {
		ds->readBytes((char *)myaddr, len, memaddr);
		return 0;
	} else {
		return -1;
	}
}

/*
 * Address calculator callback
 */
void print_address_func2(bfd_vma addr, struct disassemble_info *info) {
	int res = ((DisasmAppData2 *)info->application_data)->secvma - ((DisasmAppData2 *)info->application_data)->secoff + addr;
	sprintf(curr_insn2.src, "%x", res);
}

// TODO: this function is the same as disasm...
int disPrintfWrapper2(FILE *stream, const char *format, ...) {
	// per accedir a tots els parametres
	va_list args;
	char *str;
	va_start(args, format);
	str = va_arg(args, char*);
	if (!curr_insn2.mnemonic[0]) {
		snprintf(curr_insn2.mnemonic, 16, format, str);
	}  else if (!curr_insn2.src[0]) {
		// remove '^0*'
		for (; *str == '0'; str++);
		snprintf(curr_insn2.src, 31, format, str);
	} else if (!curr_insn2.dest[0]) {
		snprintf(curr_insn2.dest, 31, format, str);
	} else if (!curr_insn2.arg[0]) {
		snprintf(curr_insn2.arg, 31, format, str);
	} else if (!curr_insn2.arg2[0]) {
		snprintf(curr_insn2.arg2, 31, format, str);
	} else {
		snprintf(curr_insn2.arg3, 31, format, str);
	}
	va_end(args);
	return(0);
}

// This fucntion is the same as disasm...
int WorkModeElf::disasmOp(int inst_offset, struct ASM_INSN *op, int sect_vma, int sect_offset) {
	// TODO: do the info initalization, for multi arch
	disassembler_ftype disassemble_fn;
	disassemble_info info;
	INIT_DISASSEMBLE_INFO(info, stdout, disPrintfWrapper2);
	info.flavour = bfd_target_unknown_flavour;
	info.arch = bfd_arch_i386;
	info.mach = bfd_mach_i386_i386;
	info.endian = BFD_ENDIAN_LITTLE;
	disassemble_fn = print_insn_i386_att;
	info.read_memory_func = disReadMemory;

	// Prepare the application data for the disasm
	DisasmAppData2 appData;
	appData.ds = _dataSource;
	appData.secvma = sect_vma;
	appData.secoff = sect_offset;
	appData.wm = this;
	info.application_data = &appData;
	info.print_address_func = print_address_func2;

	int size = 0;
	if (op != NULL) {
		// TODO: here will be a mutual exclusion
		memset(&curr_insn2, 0, sizeof(struct ASM_INSN));
		size = (*disassemble_fn)(inst_offset, &info);
		memcpy(op, &curr_insn2, sizeof(struct ASM_INSN));
	}
	// TODO: here will finish the mutual exclusion
	return size;
}

void WorkModeElf::indexSection(int offset, int size) {
	int bytes = 0;
	// TODO: differentiating among code and data sections
	disassembler_ftype disassemble_fn;
	disassemble_info info;
	INIT_DISASSEMBLE_INFO(info, stdout, disNone);
	info.flavour = bfd_target_unknown_flavour;
	info.arch = bfd_arch_i386;
	info.mach = bfd_mach_i386_i386;
	info.endian = BFD_ENDIAN_LITTLE;
	disassemble_fn = print_insn_i386_att;
	info.read_memory_func = disReadMemory;

	// Prepare the application data for the disasm
	DisasmAppData2 appData;
	appData.ds = _dataSource;
	appData.wm = this;
	info.application_data = &appData;

	while (bytes < size) {
		_linies.push_back(bytes + offset);
		bytes += (*disassemble_fn) (bytes + offset, &info);
	}
}

WorkModeElf::WorkModeElf(DataSource *ds) : WorkMode(ds) {
	// TODO: get elf arch

	// Load elf headers.
	Elf32_Ehdr elfHeader;
	_dataSource->readBytes((char *)&elfHeader, sizeof(Elf32_Ehdr), 0);

	// Check if it is a elf file.
	if (memcmp(elfHeader.e_ident, "\x7f\x45\x4c\x46", 4)) {
		// TODO: It should be an exception.
		exit(0);
	}

	// Load section headers
	Elf32_Shdr *sectionHeaders = new Elf32_Shdr[elfHeader.e_shnum];
	_dataSource->readBytes((char *)sectionHeaders, sizeof(Elf32_Shdr) * elfHeader.e_shnum, elfHeader.e_shoff);

	char *stringTable = new char[sectionHeaders[elfHeader.e_shstrndx].sh_size];
	_dataSource->readBytes(stringTable, sectionHeaders[elfHeader.e_shstrndx].sh_size, sectionHeaders[elfHeader.e_shstrndx].sh_offset);

	struct intString section;
	// Index all sections
	for (int i = 0; i < elfHeader.e_shnum; i++) {
		// Save this section
		section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.offset = sectionHeaders[i].sh_offset;
		section.addr = sectionHeaders[i].sh_addr;
		_sections.push_back(section);

		// printf("name %s offset %d addr %x, type: %d flags: %d\n",section.name.c_str(),section.offset,section.addr, sectionHeaders[i].sh_type, sectionHeaders[i].sh_flags);
		// Index section
		indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}

	// Free the temporary arrays
	delete[] sectionHeaders;
	delete[] stringTable;
}

unsigned int WorkModeElf::getNumberLines() {
	return _linies.size();
}

struct WorkModeElf::intString *WorkModeElf::findSection(int offset) {
	int i;
	for (i = _sections.size(); i >= 0; i--) {
		if (_sections.at(i - 1).offset <= offset) {
			return &_sections.at(i - 1);
		}
	}
	return &_sections.at(i - 1);
}

ViewLine WorkModeElf::getLine(unsigned int line) {
	ViewLine viewline;
	char hexbyte[3];
	struct ASM_INSN op;
	char pos[32];

	struct WorkModeElf::intString * section = findSection(_linies.at(line));

	snprintf(pos, sizeof(pos), "%s:%08x:", section->name.c_str(), section->addr + _linies.at(line) - section->offset);

	// Put the file position of this opcode
	viewline.push_back(ViewBlock(pos, false));
	viewline.push_back(ViewBlock("    ", false));

	// Disasm
	int size = disasmOp(_linies.at(line), &op, section->addr, section->offset);

	// Put hex representation
	for (int i = 0; i < 8; i++) {
		if (i < size) {
			// Read the next byte
			_dataSource->readBytes(hexbyte, 1, _linies.at(line) + i);
			sprintf(hexbyte, "%02hhx", *hexbyte);
			// Put the file position of this opcode
			viewline.push_back(ViewBlock(hexbyte, true));
			viewline.push_back(ViewBlock(" ", false));
		} else {
			viewline.push_back(ViewBlock("   ", false));
		}
	}
	viewline.push_back(ViewBlock("    ", false));

	// Put disasm
	viewline.push_back(ViewBlock(op.mnemonic, true));
	if (op.src[0]) {
		viewline.push_back(ViewBlock(op.src, true));
		if (op.dest[0]) {
			viewline.push_back(ViewBlock(op.dest, true));
			if (op.arg[0]) {
				viewline.push_back(ViewBlock(op.arg, true));
				if (op.arg2[0]) {
					viewline.push_back(ViewBlock(op.arg2, true));
					if (op.arg3[0]) {
						viewline.push_back(ViewBlock(op.arg3, true));
					}
				}
			}
		}
	}
	return viewline;
}
