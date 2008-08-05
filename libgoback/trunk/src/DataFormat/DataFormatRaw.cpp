#include "DataFormatRaw.h"
#include "DataSource.h"

#include <cstring>

// DataFormatRawModule

std::string DataFormatRawModule::id() const {
	return "raw";
}

std::string DataFormatRawModule::name() const {
	return "RAW file";
}

/**V
 * Verifying if the file is a raw file.
 * @param *ds is a DataSource
 * @return true if the file is an elf file.
*/
bool DataFormatRaw::isRawFile(DataSource *ds) { 
	return true;
}

bool DataFormatRawModule::detect(DataSource *ds) const {
	return DataFormatRaw::isRawFile(ds);
}
/**
 * Return the DataFormat object
 * @param *ds is a DataSource,
 * @return the DataFormat object.
*/

DataFormat *DataFormatRawModule::create(DataSource *ds) const {
	return new DataFormatRaw(ds);
}

// DataFormatRaw

DataFormatRaw::DataFormatRaw(DataSource *ds) :
	DataFormat(ds) {
}

bool DataFormatRaw::load() {
	// Check if it is a raw file.
	if (!isRawFile(_dataSource)) {
		return false;
	}
	_formatSections.push_back(_dataSource);
	return true;
}
