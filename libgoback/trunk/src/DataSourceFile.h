#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include "DataSource.h"

class DataSourceFile : public DataSource {
public:
	DataSourceFile();

	bool open(const char * filename);
	bool close();

	int size();
	int readBytes(char * buff, int size, unsigned int offset = -1);

	bool requestWrite();
	bool requestInsert();
	bool requestRemove();

	int replaceBytes(const char * buff, int size, unsigned int offset = -1);
	int insertBytes(const char * buff, int size, unsigned int offset);
	int removeBytes(int size, unsigned int offset);

	bool flushWrites();

private:
	int _fd;
	bool _writable;
	char * _filename;
};

#endif // FILEDATASOURCE_H
