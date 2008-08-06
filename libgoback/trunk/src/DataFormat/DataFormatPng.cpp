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
	FILE *fd;

	fd = fopen ((char *)_dataSource->getName().c_str(), "r");

	png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!png_ptr) {
		return (false);
	}

	png_infop info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
		return (false);
	}

	png_infop end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
		return (false);
	}

	png_init_io(png_ptr, fd);
	png_read_info(png_ptr, info_ptr);

	png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

	return true;
}
