#ifndef FILEDATASOURCE_H
#define FILEDATASOURCE_H

#include "DataSource.h"

class DataSourceFile : public DataSource {
public:
	DataSourceFile();

	bool open(const char *filename);
	bool close();

	unsigned int size();
	unsigned int readBytes(char *buff, unsigned int size, unsigned int offset = -1);

	bool requestWrite();
	bool requestInsert();
	bool requestRemove();

	unsigned int replaceBytes(const char *buff, unsigned int size, unsigned int offset = -1);
	unsigned int insertBytes(const char *buff, unsigned int size, unsigned int offset);
	unsigned int removeBytes(unsigned int size, unsigned int offset);

	bool flushWrites();

private:
	int _fd;
	bool _writable;
	char *_filename;
};

#endif // FILEDATASOURCE_H
