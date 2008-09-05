#ifndef DATAFORMATELF_H
#define DATAFORMATELF_H

#include "DataFormat.h"

class DataFormatElfModule : public DataFormatModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Format detection
	bool detect(DataSource *ds) const;

	// DataFormat instantiation
	DataFormat *create(DataSource *ds) const;

};

class DataFormatElf : public DataFormat {
public:
	DataFormatElf(DataSource *ds);

	bool load(std::vector<DataSource *> &sections);

	static bool isElfFile(DataSource *ds);

protected:
	std::string getSectionType(int type, bool program) const;
	std::string getProgramHeaderName(int type) const;
	void createSections(std::vector<DataSource *> &sections);
	bool load32(std::vector<DataSource *> &sections);
	bool load64(std::vector<DataSource *> &sections);

	struct ElfSection {
		std::string name;
		unsigned int offset;
		unsigned int size;
		unsigned int address;
		unsigned int type;
		bool program;
	};

	std::list<ElfSection> _sections;
};

#endif // DATAFORMATELF_H
