#ifndef DATASOURCERANGE_H
#define DATASOURCERANGE_H

#include "DataSource.h"

class DataSourceRange : public DataSource {
public:
	DataSourceRange();

	void setRange(DataSource *ds, unsigned int offset, unsigned int size);

	bool open(const char *filename);
	bool close();

	int size();
	int readBytes(char *buff, unsigned int size, unsigned int offset = -1);

	bool requestWrite();
	bool requestInsert();
	bool requestRemove();

	int replaceBytes(const char *buff, unsigned int size, unsigned int offset = -1);
	int insertBytes(const char *buff, unsigned int size, unsigned int offset);
	int removeBytes(unsigned int size, unsigned int offset);

	bool flushWrites();

private:
	DataSource *_ds;
	unsigned int _size;
	unsigned int _offset;
	unsigned int _range;	
};

#endif // DATASOURCERANGE_H
