#include "DataSourceRange.h"

// Constructor
DataSourceRange::DataSourceRange() {
}

void DataSourceRange::setRange(DataSource *ds, unsigned int offset, unsigned int size) {
	//printf("setRange ds=%p, size=%d, offset=%d\n", ds, size, offset);
	_ds = ds;
	_offset = offset;
	_size = size;
}

bool DataSourceRange::open(const char *filename) {
	return true;
}

bool DataSourceRange::close() {
	return true;
}

int DataSourceRange::size() {
	return _size;
}

int DataSourceRange::readBytes(char *buff, unsigned int size, unsigned int offset) {
	// if offset is out the range of the datasorucerange, return 0
	//printf("readRange buff=%p, size=%d, offset=%d\n", buff, size, offset);
	if (offset >= _size) {
		printf("Accedint fora de rang! intent: size:%d, offset:%d, limit: 0-%d\n", size, offset, _size);
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

int DataSourceRange::replaceBytes(const char *buff, unsigned int size, unsigned int offset) {
	// TODO: like readBytes
	return 0;
}

int DataSourceRange::insertBytes(const char *buff, unsigned int size, unsigned int offset) {
	return 0;
}

int DataSourceRange::removeBytes(unsigned int size, unsigned int offset) {
	return 0;
}

bool DataSourceRange::flushWrites() {
	return true;
}
