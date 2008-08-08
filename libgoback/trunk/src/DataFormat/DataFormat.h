#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <Module.h>

#include <list>
#include <vector>

class DataSource;

// DataFormat
class DataFormat {
public:
	DataFormat(DataSource *ds) : _dataSource(ds) {}

	virtual bool load(std::vector<DataSource *> &sections) = 0;

protected:
	// Input DataSource
	DataSource *_dataSource;
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
