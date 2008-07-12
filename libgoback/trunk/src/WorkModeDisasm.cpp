#include "WorkModeDisasm.h"
#include "DataSource.h"
#include <dis-asm.h>
#include <stdarg.h>

struct ASM_INSN {
    char mnemonic[16];
    char src[32];
    char dest[32];
    char arg[32];
} curr_insn;

/*
 * fprintf wrapper for do nothing.
 */
int disNone(FILE *stream, const char *format, ...){
	return 0;
}

/* 
 * fprintf wrapper for opcodes.
 */
int disPrintfWrapper(FILE *stream, const char *format, ...){
	// per accedir a tots els parametres
    va_list args;
    char *str;
    va_start(args, format);
    str = va_arg(args, char*);

    if (!curr_insn.mnemonic[0]) {
		snprintf(curr_insn.mnemonic, 16, format, str);
    }  else if (!curr_insn.src[0]) {
        strncpy(curr_insn.src, str, 31);
    } else if (!curr_insn.dest[0]) {
        strncpy(curr_insn.dest, str, 31);
        if (strncmp(curr_insn.dest, "DN", 2) == 0)
        	curr_insn.dest[0] = 0;
    } else {
        if (!strcmp( curr_insn.src, curr_insn.dest)) {
            // src is passed twise!
            strncpy(curr_insn.dest, str, 31);
        } else {
            strncpy(curr_insn.arg, str, 31);
        }
    }
    va_end(args);
    return(0);
}

int disasmOp(unsigned char *shellcode, int size, struct ASM_INSN *op) {
	// TODO: do the info initalization, for multi arch
	disassembler_ftype disassemble_fn;
    disassemble_info info;
	INIT_DISASSEMBLE_INFO(info, stdout, disPrintfWrapper);
	info.flavour = bfd_target_unknown_flavour;
	info.arch = bfd_arch_i386;
	info.mach = bfd_mach_i386_i386;
	info.endian = BFD_ENDIAN_LITTLE;
	disassemble_fn = print_insn_i386;
	info.buffer = shellcode;
	info.buffer_length = size;
	info.buffer_vma = 0;

	size=0;
	if (op != NULL) {
	// here will be a mutual exclusion
    memset(&curr_insn, 0, sizeof(struct ASM_INSN));
    size = (*disassemble_fn)(0, &info);
	memcpy(op, &curr_insn, sizeof(struct ASM_INSN));
	}
	// here will finish the mutual exclusion
	return size;
}

WorkModeDisasm::WorkModeDisasm(DataSource *ds) : WorkMode(ds) {
	// get file size
	int size = _dataSource->size();
	// loads the entire file in memory. It is a problem ?¿
	char *data = (char *)malloc(size);
	_dataSource->readBytes(data, size, 0);

    int bytes;
    disassembler_ftype disassemble_fn;
    disassemble_info info;

	INIT_DISASSEMBLE_INFO(info, stdout, disNone);

    info.flavour = bfd_target_unknown_flavour;
    info.arch = bfd_arch_i386;
    info.mach = bfd_mach_i386_i386;
    info.endian = BFD_ENDIAN_LITTLE;
    disassemble_fn = print_insn_i386;

    info.buffer = (unsigned char *)data;
    info.buffer_length = size;
    info.buffer_vma = 0;

    bytes = 0;
	// index opcodes
    while ( bytes < size ) {
		_linies.push_back(bytes);
        bytes  += (*disassemble_fn) (0 + bytes, &info);
    }
}

int WorkModeDisasm::getNumberLines() {
	return _linies.size();
}

ViewLine WorkModeDisasm::getLine(int line) {
	ViewLine viewline;
	char shellcode[10];
	char hexbyte[3];
	struct ASM_INSN op;
	char pos[10];
	snprintf(pos, 9, "0x%08x", _linies.at(line));

	// put the file position of this opcode
	viewline.push_back(ViewBlock(pos, false));
	viewline.push_back(ViewBlock("    ", false));

	// read the line. 8 bytes is suficient for the largest opcode
	int bytes = _dataSource->readBytes(shellcode, 8, _linies.at(line));

	// disasm
	int size = disasmOp((unsigned char *)shellcode, bytes, &op); 

	// put hex representation
	for (int i = 0; i < 8; i++ ) {
		if ( i < size ) {
			sprintf(hexbyte,"%02x ", shellcode[i]);
			hexbyte[2] = '\0';
			// put the file position of this opcode
			viewline.push_back(ViewBlock(hexbyte, true));
			viewline.push_back(ViewBlock(" ", false));
		}
		else
			viewline.push_back(ViewBlock("   ", false));
	}

	viewline.push_back(ViewBlock("    ", false));

	// put disasm
	viewline.push_back(ViewBlock(op.mnemonic, true));
    if (op.src[0]) {
		viewline.push_back(ViewBlock(op.src, true));
        if (op.dest[0]) {
			viewline.push_back(ViewBlock(op.dest, true));
            if (op.arg[0]) {
			viewline.push_back(ViewBlock(op.arg, true));
            }
        }
    }
	return viewline;
}

