#include "DataFormatPng.h"
#include "DataSource.h"

#include <png.h>

/**
 * DataFormatPng
*/

std::string DataFormatPngModule::id() const {
	return "png";
}

std::string DataFormatPngModule::name() const {
	return "PNG file";
}

/**
 * Verifying if the file is an png file.
 * @param *ds is a DataSource
 * @return true if the file is an png file.
*/
bool DataFormatPng::isPngFile(DataSource *ds) { 

	char header[8];
	ds->readBytes(header, 8, 0);

	return ( png_sig_cmp((png_byte*)header, 0, 8) == 0);
}

bool DataFormatPngModule::detect(DataSource *ds) const {
	return DataFormatPng::isPngFile(ds);
}
/**
 * Return the DataFormat object
 * @param *ds is a DataSource,
 * @return the DataFormat object.
*/

DataFormat *DataFormatPngModule::create(DataSource *ds) const {
	return new DataFormatPng(ds);
}

DataFormatPng::DataFormatPng(DataSource *ds) :
	DataFormat(ds) {
}

bool DataFormatPng::load() {
	return true;
}
