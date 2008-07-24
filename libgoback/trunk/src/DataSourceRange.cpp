#include "DataSourceRange.h"

// Constructor
DataSourceRange::DataSourceRange() {
}

void DataSourceRange::setRange(DataSource *ds, int offset, int size) {
	_ds = ds;
	_offset = offset;
	_size = size;
}

bool DataSourceRange::open(const char * filename) {
	return true;
}

bool DataSourceRange::close() {
	return true;
}

int DataSourceRange::size() {
	return _size;
}

int DataSourceRange::readBytes(char * buff, int size, unsigned int offset) {
	// if offset is out the range of the datasorucerange, return 0
	if (offset >= size) {
		return 0;
	} else {
		// we have to read the real offset
		if ((offset + size) > _size) {
			size = _size - offset;
		} 
		return _ds->readBytes(buff, size, _offset + offset);
	}
}

bool DataSourceRange::requestWrite() {
	return _ds->requestWrite();
}
bool DataSourceRange::requestInsert() {
	return _ds->requestInsert();	
}

bool DataSourceRange::requestRemove() {
	return _ds->requestRemove();
}

int DataSourceRange::replaceBytes(const char * buff, int size, unsigned int offset) {
	// TODO: like readBytes
	return 0;
}

int DataSourceRange::insertBytes(const char * buff, int size, unsigned int offset) {
	return 0;
}

int DataSourceRange::removeBytes(int size, unsigned int offset) {
	return 0;
}

bool DataSourceRange::flushWrites() {
	return true;
}
