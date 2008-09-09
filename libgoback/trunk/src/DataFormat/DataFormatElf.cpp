#include "DataFormatElf.h"
#include "DataSource.h"

#include <cstring>
#include <elf.h>

// DataFormatElfModule

std::string DataFormatElfModule::id() const {
	return "elf";
}

std::string DataFormatElfModule::name() const {
	return "ELF file";
}

/**V
 * Verifying if the file is an elf file.
 * @param *ds is a DataSource
 * @return true if the file is an elf file.
*/
bool DataFormatElf::isElfFile(DataSource *ds) { 
	// Load elf headers.
	Elf32_Ehdr elfHeader;
	ds->readBytes((char *)&elfHeader, sizeof(Elf32_Ehdr), 0);

	return (memcmp(elfHeader.e_ident, "\x7f\x45\x4c\x46", 4) == 0);
}

bool DataFormatElfModule::detect(DataSource *ds) const {
	return DataFormatElf::isElfFile(ds);
}
/**
 * Return the DataFormat object
 * @param *ds is a DataSource,
 * @return the DataFormat object.
*/

DataFormat *DataFormatElfModule::create(DataSource *ds) const {
	return new DataFormatElf(ds);
}

// DataFormatElf

DataFormatElf::DataFormatElf(DataSource *ds) :
	DataFormat(ds) {
}

/**
 * Return the default format for a section
 * @param type the number of the section
 * @return 
*/
std::string DataFormatElf::getSectionType(int type, bool program) const {
	if (!program) {
		// TODO: Add the arch after "code"
		switch(type) {
		case SHT_PROGBITS:
			return "code";
		case SHT_NOTE:
			return "";
		case SHT_REL:
			return "code";
		default:
			return "";
		}
	} else {
		// TODO: Add the arch after "code"
		switch(type) {
		case PT_NOTE:
			return "";
		case PT_LOAD:
			return "code";
		default:
			return "";
		}
	}
}

/**
 * Return the name for the program header, based on type
 * @param type the number of the section
 * @return name for program header
*/
std::string DataFormatElf::getProgramHeaderName(int type) const {
	// TODO: Add the arch after "code"
	switch(type) {
	case PT_NULL:
		return "NULL";
	case PT_LOAD:
		return "LOAD";
	case PT_DYNAMIC:
		return "DYNAMIC";
	case PT_INTERP:
		return "INTERP";
	case PT_NOTE:
		return "NOTE";
	case PT_SHLIB:
		return "SHLIB";
	case PT_PHDR:
		return "PHDR";
	case PT_LOPROC:
		return "LOPROC";
	default:
		return "";
	}
}
/**
 * Setup the arch property to de datasource
 */
void DataFormatElf::setArch(int arch) {
    switch (arch) {
        case EM_386:
		_dataSource->setProperty("ElfArch", new ValueInt(1));
        break;

        case EM_860:
		_dataSource->setProperty("ElfArch", new ValueInt(2));
        break;

		case EM_ARM:
		_dataSource->setProperty("ElfArch", new ValueInt(3));
		break;
	
        case EM_MIPS:
		_dataSource->setProperty("ElfArch", new ValueInt(4));
        break;

        case EM_PPC:
		_dataSource->setProperty("ElfArch", new ValueInt(5));
        break;

		default:
		_dataSource->setProperty("ElfArch", new ValueInt(-1));
    }
}

bool DataFormatElf::load(std::vector<DataSource *> &sections) {
	// Check if it is a elf file.
	if (!isElfFile(_dataSource)) {
		return false;
/**
 * Setup the arch property to de datasource
 */	}

	// Check if is 32 or 64 bits
	char bits;
	_dataSource->readBytes(&bits, 1, 4);

	if (bits == 0x01) {
		_dataSource->setProperty("ElfBits", new ValueString("32"));
		return load32(sections);
	} else if (bits == 0x02) {
		_dataSource->setProperty("ElfBits", new ValueString("64"));
		return load64(sections);
	} else {
		return false;
	}
}

//TODO: compact the load32 and load64 to eliminate duplicate code
bool DataFormatElf::load32(std::vector<DataSource *> &sections) {
	// Load elf headers.
	Elf32_Ehdr elfHeader;
	_dataSource->readBytes((char *)&elfHeader, sizeof(Elf32_Ehdr), 0);

	// Load section headers
	Elf32_Shdr *sectionHeaders = new Elf32_Shdr[elfHeader.e_shnum];
	_dataSource->readBytes((char *)sectionHeaders, sizeof(Elf32_Shdr) * elfHeader.e_shnum, elfHeader.e_shoff);

	// Load program headers
	Elf32_Phdr *programHeaders = new Elf32_Phdr[elfHeader.e_phnum];
	_dataSource->readBytes((char *)programHeaders, sizeof(Elf32_Phdr) * elfHeader.e_phnum, elfHeader.e_phoff);

	char *stringTable = new char[sectionHeaders[elfHeader.e_shstrndx].sh_size];
	_dataSource->readBytes(stringTable, sectionHeaders[elfHeader.e_shstrndx].sh_size, sectionHeaders[elfHeader.e_shstrndx].sh_offset);

	// Detect arch
	setArch(elfHeader.e_machine);	

	// Index all sections
	for (int i = 0; i < elfHeader.e_shnum; i++) {
		// Collect the information about this section
		ElfSection section;
		section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.offset = sectionHeaders[i].sh_offset;
		section.address = sectionHeaders[i].sh_addr;
		section.size = sectionHeaders[i].sh_size;
		section.type = sectionHeaders[i].sh_type;
		section.program = false;

		// Save this section
		_sections.push_back(section);

		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}

	// Index all program headers
	for (int i = 0; i < elfHeader.e_phnum; i++) {
		// Collect the information about this section
		ElfSection section;
		section.type = programHeaders[i].p_type;
		//section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.name = getProgramHeaderName(section.type);
		section.offset = programHeaders[i].p_offset;
		section.address = programHeaders[i].p_vaddr;
		section.size = programHeaders[i].p_filesz;
		section.program = true;

		// Save this section
		_sections.push_back(section);

		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}
	// Free the temporary arrays
	delete[] sectionHeaders;
	delete[] programHeaders;
	delete[] stringTable;

	createSections(sections);

	return true;
}

bool DataFormatElf::load64(std::vector<DataSource *> &sections) {

	// Load elf headers.
	Elf64_Ehdr elfHeader;
	_dataSource->readBytes((char *)&elfHeader, sizeof(Elf64_Ehdr), 0);

	// Load section headers
	Elf64_Shdr *sectionHeaders = new Elf64_Shdr[elfHeader.e_shnum];
	_dataSource->readBytes((char *)sectionHeaders, sizeof(Elf64_Shdr) * elfHeader.e_shnum, elfHeader.e_shoff);

	// Load program headers
	Elf64_Phdr *programHeaders = new Elf64_Phdr[elfHeader.e_phnum];
	_dataSource->readBytes((char *)programHeaders, sizeof(Elf64_Phdr) * elfHeader.e_phnum, elfHeader.e_phoff);

	char *stringTable = new char[sectionHeaders[elfHeader.e_shstrndx].sh_size];
	_dataSource->readBytes(stringTable, sectionHeaders[elfHeader.e_shstrndx].sh_size, sectionHeaders[elfHeader.e_shstrndx].sh_offset);

	// Detect arch
	setArch(elfHeader.e_machine);	

	// Index all sections
	for (int i = 0; i < elfHeader.e_shnum; i++) {
		// Collect the information about this section
		ElfSection section;
		section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.offset = sectionHeaders[i].sh_offset;
		section.address = sectionHeaders[i].sh_addr;
		section.size = sectionHeaders[i].sh_size;
		section.type = sectionHeaders[i].sh_type;
		section.program = false;

		// Save this section
		_sections.push_back(section);

		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}

	// Index all program headers
	for (int i = 0; i < elfHeader.e_phnum; i++) {
		// Collect the information about this section
		ElfSection section;
		section.type = programHeaders[i].p_type;
		//section.name = std::string(&stringTable[sectionHeaders[i].sh_name]);
		section.name = getProgramHeaderName(section.type);
		section.offset = programHeaders[i].p_offset;
		section.address = programHeaders[i].p_vaddr;
		section.size = programHeaders[i].p_filesz;
		section.program = true;

		// Save this section
		_sections.push_back(section);

		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}
	// Free the temporary arrays
	delete[] sectionHeaders;
	delete[] programHeaders;
	delete[] stringTable;

	createSections(sections);

	return true;
}

void DataFormatElf::createSections(std::vector<DataSource *> &sections) {
	// Create the section DataSources
	std::list<ElfSection>::iterator section = _sections.begin();

	while (section != _sections.end()) {

		//printf("Creant rang %s: offset:%d, size:%d\n", section->name.c_str(), section->offset, section->size);
		/** Check if the section size is > 0 */
		if (section->size > 0) {
			// Create a new range DataSource for each section
			DataSource *ds = _dataSource->createRange(section->offset, section->size);

			// Initialize the extra information we know about the
			// new data source
			ds->setProperty("SectionName", new ValueString(section->name));
			ds->setProperty("SectionAddress", new ValueInt(section->address));
			ds->setProperty("SectionOffset", new ValueInt(section->offset));

			// TODO: Verify the format is applied correctly
			if (ds->setDataFormat(getSectionType(section->type, section->program))) {
				sections.push_back(ds);
			} else {
				printf("Can't initialize dataSourceRangeFormat");
			}
		}
		section++;
	}
}

