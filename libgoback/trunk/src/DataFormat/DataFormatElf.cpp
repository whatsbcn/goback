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
	if (!isElfFile(_dataSource)) {
		return false;
	}

	// Load section headers
	Elf32_Shdr *sectionHeaders = new Elf32_Shdr[elfHeader.e_shnum];
	_dataSource->readBytes((char *)sectionHeaders, sizeof(Elf32_Shdr) * elfHeader.e_shnum, elfHeader.e_shoff);

	char *stringTable = new char[sectionHeaders[elfHeader.e_shstrndx].sh_size];
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

		// Index section
		//TODO: indexSection(sectionHeaders[i].sh_offset, sectionHeaders[i].sh_size);
	}

	// Free the temporary arrays
	delete[] sectionHeaders;
	delete[] stringTable;

	// Create the section DataSources
	std::list<ElfSection>::iterator section = _sections.begin();
	while (section != _sections.end()) {
		//printf("Creant rang %s: offset:%d, size:%d\n", section->name.c_str(), section->offset, section->size);
		/** Check if the section size is > 0 */
		if (section->size > 0) {
			DataSource *ds = _dataSource->createRange(section->name, section->offset, section->size, section->address, getSectionType(section->type));
			_formatSections.push_back(ds);
		}
		section++;
	}

	return true;
}
