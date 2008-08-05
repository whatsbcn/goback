#ifndef DATAFORMATRAW_H
#define DATAFORMATRAW_H

#include "DataFormat.h"

class DataFormatRawModule : public DataFormatModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Format detection
	bool detect(DataSource *ds) const;

	// DataFormat instantiation
	DataFormat *create(DataSource *ds) const;
};

class DataFormatRaw : public DataFormat {
public:
	DataFormatRaw(DataSource *ds);
	~DataFormatRaw();

	std::string getSectionType(int type) const;
	bool load();
	static bool isRawFile(DataSource *ds); 
};

#endif // DATAFORMATRAW_H
