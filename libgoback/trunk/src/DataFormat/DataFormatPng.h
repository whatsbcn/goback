#ifndef DATAFORMATPng_H
#define DATAFORMATPng_H

#include "DataFormat.h"

class DataFormatPngModule : public DataFormatModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Format detection
	bool detect(DataSource *ds) const;

	// DataFormat instantiation
	DataFormat *create(DataSource *ds) const;

};

class DataFormatPng : public DataFormat {
public:
	DataFormatPng(DataSource *ds);
	~DataFormatPng();

	bool load(std::vector<DataSource *> &sections);
	static bool isPngFile(DataSource *ds); 

protected:
	struct PngSection {
		std::string name;
		unsigned int offset;
		unsigned int size;
		unsigned int address;
		unsigned int type;
	};

	std::list<PngSection> _sections;
};

#endif // DATAFORMATPng_H
