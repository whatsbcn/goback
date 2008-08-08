#include "DataFormatExe.h"
#include "DataSource.h"

#include <cstring>

// DataFormatExeModule

std::string DataFormatExeModule::id() const {
	return "exe";
}

std::string DataFormatExeModule::name() const {
	return "EXE file";
}

bool DataFormatExeModule::detect(DataSource *ds) const {
	// Load EXE headers.
	char signature[2];
	ds->readBytes(signature, 2, 0);

	// Check if it is an EXE file.
	if (memcmp(signature, "MZ", 2)) {
		// Invalid signature
		return false;
	}

	// It's an EXE file
	return true;
}

DataFormat *DataFormatExeModule::create(DataSource *ds) const {
	// Return the DataFormat object
	return new DataFormatExe(ds);
}

// DataFormatExe

DataFormatExe::DataFormatExe(DataSource *ds) :
	DataFormat(ds) {
}

bool DataFormatExe::load(std::vector<DataSource *> &sections) {
	return true;
}
