#include "WorkModeDisasm.h"
#include "DataSource.h"

#include <cstring>
#include <dis-asm.h>
#include <stdarg.h>

// WorkModeDisasmModule

std::string WorkModeDisasmModule::id() const {
	return "disasm";
}

std::string WorkModeDisasmModule::name() const {
	return "Code disassembling";
}

std::string WorkModeDisasmModule::getApplicableFormat() const {
	return "code";
}

WorkMode *WorkModeDisasmModule::create(DataSource *ds) const {
	return new WorkModeDisasm(ds);
}

// WorkModeDisasm

struct ASM_INSN {
	char mnemonic[16];
	char src[32];
	char dest[32];
	char arg[32];
	char arg2[32];
	char arg3[32];
} curr_insn;

// Application data to embedd into the disassemble_info structure
struct DisasmAppData {
	DataSource *ds;
	WorkModeDisasm *wm;
	//TODO: move curr_insn here?
};

/*
 * Disasm memory reader callback
 */
static int disReadMemory(bfd_vma memaddr, bfd_byte *myaddr, unsigned int len,
	struct disassemble_info *info) {

	DataSource *ds = ((DisasmAppData *)info->application_data)->ds;
	if (memaddr + len <= ds->size()) {
		ds->readBytes((char *)myaddr, len, memaddr);
		return 0;
	} else {
		return -1;
	}
}

/*
 * fprintf wrapper for do nothing.
 */
int disNone(FILE *stream, const char *format, ...) {
	return 0;
}

/* 
 * fprintf wrapper for opcodes.
 */
int disPrintfWrapper(FILE *stream, const char *format, ...) {
	// per accedir a tots els parametres
	va_list args;
	char *str;
	va_start(args, format);
	str = va_arg(args, char*);
	if (!curr_insn.mnemonic[0]) {
		snprintf(curr_insn.mnemonic, 16, format, str);
	} else if (!curr_insn.src[0]) {
		// remove '^0*'
		for (; *str == '0'; str++);
		snprintf(curr_insn.src, 31, format, str);
	} else if (!curr_insn.dest[0]) {
		snprintf(curr_insn.dest, 31, format, str);
	/*if (strncmp(curr_insn.dest, "DN", 2) == 0)
		curr_insn.dest[0] = 0;*/
	} else if (!curr_insn.arg[0]) {
		snprintf(curr_insn.arg, 31, format, str);
	} else if (!curr_insn.arg2[0]) {
		snprintf(curr_insn.arg2, 31, format, str);
	} else {
		snprintf(curr_insn.arg3, 31, format, str);
	}
	va_end(args);
	return 0;
}

int WorkModeDisasm::disasmOp(int offset, struct ASM_INSN *op) {
	// TODO: do the info initalization, for multi arch
	disassembler_ftype disassemble_fn;
	disassemble_info info;
	INIT_DISASSEMBLE_INFO(info, stdout, disPrintfWrapper);
	info.flavour = bfd_target_unknown_flavour;
	info.arch = bfd_arch_i386;
	info.mach = bfd_mach_i386_i386;
	info.endian = BFD_ENDIAN_LITTLE;
	disassemble_fn = print_insn_i386_att;
	info.read_memory_func = disReadMemory;

	// Prepare the application data for the disasm
	DisasmAppData appData;
	appData.ds = _dataSource;
	appData.wm = this;
	info.application_data = &appData;

	int size = 0;
	if (op != NULL) {
	// TODO: here will be a mutual exclusion
		memset(&curr_insn, 0, sizeof(struct ASM_INSN));
		size = (*disassemble_fn)(offset, &info);
		memcpy(op, &curr_insn, sizeof(struct ASM_INSN));
	}
	// TODO: here will finish the mutual exclusion
	return size;
}

WorkModeDisasm::WorkModeDisasm(DataSource *ds) : WorkMode(ds) {
	disassembler_ftype disassemble_fn;
	disassemble_info info;

	INIT_DISASSEMBLE_INFO(info, stdout, disNone);

	info.flavour = bfd_target_unknown_flavour;
	info.arch = bfd_arch_i386;
	info.mach = bfd_mach_i386_i386;
	info.endian = BFD_ENDIAN_LITTLE;
	disassemble_fn = print_insn_i386;
	info.read_memory_func = disReadMemory;

	// Prepare the application data for the disasm
	DisasmAppData appData;
	appData.ds = _dataSource;
	appData.wm = this;
	info.application_data = &appData;

	int bytes = 0;
	int size = _dataSource->size();
	// index opcodes
	while (bytes < size) {
		_linies.push_back(bytes);
		bytes += (*disassemble_fn) (0 + bytes, &info);
	}
}

unsigned int WorkModeDisasm::getNumberLines() {
	return _linies.size();
}

ViewLine WorkModeDisasm::getLine(unsigned int line) {
	ViewLine viewline;
	char hexbyte[3];
	struct ASM_INSN op;
	char pos[32];

	// Get the name of the section
	Value *val = _dataSource->getProperty("SectionName");
	std::string sectionName = "";
	if (val) {
		sectionName = val->getString();
	}

	// Get the address of the section
	val = _dataSource->getProperty("SectionAddress");
	long int sectionAddress = 0;
	if (val) {
		sectionAddress = val->getInt();
	}

	snprintf(pos, 32, "%s:%08x", sectionName.c_str(), _linies.at(line) + sectionAddress);

	// Put the file position of this opcode
	viewline.push_back(ViewBlock(pos, false));
	viewline.push_back(ViewBlock("    ", false));

	// Disasm
	int size = disasmOp(_linies.at(line), &op);

	// Put hex representation
	for (int i = 0; i < 8; i++) {
		if (i < size) {
			// Read the next byte
			_dataSource->readBytes(hexbyte, 1, _linies.at(line) + i);
			sprintf(hexbyte, "%02hhx", *hexbyte);

			// Put the file position of this opcode
			viewline.push_back(ViewBlock(hexbyte, true));
			viewline.push_back(ViewBlock(" ", false));
		} else
			viewline.push_back(ViewBlock("   ", false));
	}

	viewline.push_back(ViewBlock("    ", false));

	// put disasm
	viewline.push_back(ViewBlock(op.mnemonic, true));
	if (op.src[0]) {
		//viewline.push_back(ViewBlock("op.src", true));
		viewline.push_back(ViewBlock(op.src, true));
		if (op.dest[0]) {
			//viewline.push_back(ViewBlock("op.dest", true));
			viewline.push_back(ViewBlock(op.dest, true));
			if (op.arg[0]) {
				//viewline.push_back(ViewBlock("op.arg", true));
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
