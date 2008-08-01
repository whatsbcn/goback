#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <Module.h>

#include <list>
#include <vector>

class DataSource;

// DataFormat
class DataFormat {
public:
	// Detection and instantiation
	static std::list<std::string> detect(DataSource *ds);
	static DataFormat *create(std::string id, DataSource *ds);

	DataFormat(DataSource *ds);

	virtual bool load() = 0;

	unsigned int getNumberSections();
	DataSource *getSection(unsigned int section);

protected:
	// Input DataSource
	DataSource *_dataSource;

	// Output DataSources
	std::vector<DataSource *> _formatSections;
};

// DataFormat module
class DataFormatModule : public Module {
public:
	// Format detection
	virtual bool detect(DataSource *ds) const = 0;

	// DataFormat instantiation
	virtual DataFormat *create(DataSource *ds) const = 0;
};

#endif // DATAFORMAT_H
