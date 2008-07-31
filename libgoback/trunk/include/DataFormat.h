#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <list>
#include <string>

class DataSource;

// DataFormat
class DataFormat {
public:
	DataFormat(DataSource *ds);

	virtual bool load() = 0;

	unsigned int getNumberSections();
	DataSource *getSection(unsigned int section);

protected:
	DataSource *_dataSource;
	std::list<DataSource *> _formatSections;
};

// DataFormat module
class DataFormatModule {
public:
	// Identification
	virtual std::string id() const = 0;
	virtual std::string name() const = 0;

	// Format detection
	virtual bool detect(DataSource *ds) const = 0;

	// DataFormat instantiation
	virtual DataFormat *create(DataSource *ds) const = 0;
};

// DataFormat manager
class DataFormatManager {
public:
	DataFormatManager();
	~DataFormatManager();

	std::list<std::string> detect(DataSource *ds) const;
	DataFormat *create(std::string id, DataSource *ds) const;

private:
	std::list<DataFormatModule *> _dataFormats;
};

#endif // DATAFORMAT_H
