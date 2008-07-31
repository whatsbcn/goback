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
	~DataFormatElf();

	bool load();

protected:
	struct ElfSection {
		std::string name;
		unsigned int offset;
		unsigned int size;
		unsigned int address;
	};

	std::list<ElfSection> _sections;
};

#endif // DATAFORMATELF_H
