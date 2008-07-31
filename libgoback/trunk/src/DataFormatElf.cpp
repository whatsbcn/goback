#include "DataFormatElf.h"
#include "DataSource.h"

#include <elf.h>

// DataFormatElfModule

std::string DataFormatElfModule::id() const {
	return "elf";
}

std::string DataFormatElfModule::name() const {
	return "ELF file";
}

bool DataFormatElfModule::detect(DataSource *ds) const {
	// Load elf headers.
	Elf32_Ehdr elfHeader;
	ds->readBytes((char *)&elfHeader, sizeof(Elf32_Ehdr), 0);

	// Check if it is a elf file.
	if (memcmp(elfHeader.e_ident, "\x7f\x45\x4c\x46", 4)) {
		// Invalid signature
		return false;
	}

	// It's an ELF file
	return true;
}

DataFormat *DataFormatElfModule::create(DataSource *ds) const {
	// Return the DataFormat object
	return new DataFormatElf(ds);
}

// DataFormatElf

DataFormatElf::DataFormatElf(DataSource *ds) :
	DataFormat(ds) {
}

// Return the default format for a section
std::string DataFormatElf::getSectionType(int type) const {
	// TODO: Add the arch after "code"
	switch(type) {
	case SHT_PROGBITS:
		return "code";
		break;
	case SHT_NOTE:
		return "";
		break;
	case SHT_REL:
		return "code";
		break;
	default:
		return "";
	}
}

bool DataFormatElf::load() {
	// TODO: get elf arch

	// Load elf headers.
	Elf32_Ehdr elfHeader;
	_dataSource->readBytes((char *)&elfHeader, sizeof(Elf32_Ehdr), 0);

	// Check if it is a elf file.
	if (memcmp(elfHeader.e_ident, "\x7f\x45\x4c\x46", 4)) {
		return false;
	}

	// Load section headers
	Elf32_Shdr *sectionHeaders = (Elf32_Shdr *) malloc(sizeof(Elf32_Shdr) * elfHeader.e_shnum);
	_dataSource->readBytes((char *)sectionHeaders, sizeof(Elf32_Shdr) * elfHeader.e_shnum, elfHeader.e_shoff);

	char *stringTable = (char *)malloc(sectionHeaders[elfHeader.e_shstrndx].sh_size);
	_dataSource->readBytes(stringTable, sectionHeaders[elfHeader.e_shstrndx].sh_size, sectionHeaders[elfHeader.e_shstrndx].sh_offset);

	// Index all sections
	for (int i = 0; i < elfHeader.e_shnum; i++) {
		// Collect the information about this section
		ElfSection section;
		section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.offset = sectionHeaders[i].sh_offset;
		section.address = sectionHeaders[i].sh_addr;
		section.size = sectionHeaders[i].sh_size;
		section.type = sectionHeaders[i].sh_type;

		// Save this section
		_sections.push_back(section);

		// printf("name %s offset %d addr %x, type: %d flags: %d\n",section.name.c_str(),section.offset,section.addr, sectionHeaders[i].sh_type, sectionHeaders[i].sh_flags);
		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}

	// Free the temporary arrays
	free(sectionHeaders);
	free(stringTable);

	// Create the section DataSources
	std::list<ElfSection>::iterator section = _sections.begin();
	while (section != _sections.end()) {
		DataSource *ds = _dataSource->createRange(section->offset, section->size, getSectionType(section->type));
		_formatSections.push_back(ds);
		section++;
	}
}
