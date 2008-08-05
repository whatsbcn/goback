#ifndef DATAFORMATEXE_H
#define DATAFORMATEXE_H

#include "DataFormat.h"

class DataFormatExeModule : public DataFormatModule {
public:
	// Identification
	std::string id() const;
	std::string name() const;

	// Format detection
	bool detect(DataSource *ds) const;

	// DataFormat instantiation
	DataFormat *create(DataSource *ds) const;
};

class DataFormatExe : public DataFormat {
public:
	DataFormatExe(DataSource *ds);

	bool load();
};

#endif // DATAFORMATEXE_H
