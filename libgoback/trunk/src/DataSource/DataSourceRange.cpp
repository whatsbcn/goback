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

unsigned int DataSourceRange::size() {
	return _size;
}

unsigned int DataSourceRange::readBytes(char *buff, unsigned int size, unsigned int offset) {
	// if offset is out the range of the datasorucerange, return 0
	//printf("readRange buff=%p, size=%d, offset=%d\n", buff, size, offset);
	if (offset >= _size) {
		printf("Intent: size:%d, offset:%d, limit: 0-%d\n", size, offset, _size);
		return 0;
	} else {
		// we have to read the real offset
		if ((offset + size) > _size) {
			size = _size - offset;
		} 
		return _ds->readBytes(buff, size, _offset + offset);
	}
}

void DataSourceRange::setDSProperties(std::map<std::string, Value*> *properties) {
	_DSProperties = properties;
}
 
Value *DataSourceRange::getProperty(std::string key) {
	if (_properties[key] != NULL) return _properties[key];
    return (*_DSProperties)[key];
}

void DataSourceRange::setProperty(std::string key, Value *value) {
    // TODO: Delete the old value when replacing?
    _properties[key] = value;
}

void DataSourceRange::removeProperty(std::string key) {
    // TODO: Delete the value?
    _properties.erase(key);
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

unsigned int DataSourceRange::replaceBytes(const char *buff, unsigned int size, unsigned int offset) {
	// TODO: like readBytes
	return 0;
}

unsigned int DataSourceRange::insertBytes(const char *buff, unsigned int size, unsigned int offset) {
	return 0;
}

unsigned int DataSourceRange::removeBytes(unsigned int size, unsigned int offset) {
	return 0;
}

bool DataSourceRange::flushWrites() {
	return true;
}
