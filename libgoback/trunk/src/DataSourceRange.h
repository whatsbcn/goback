#ifndef DATASOURCERANGE_H
#define DATASOURCERANGE_H

#include "DataSource.h"

class DataSourceRange : public DataSource {

public:
	DataSourceRange();

	void setRange(DataSource *ds, int offset, int size);

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
	DataSource *_ds;
	int _size;
	int _offset;
	int _range;	
};

#endif // DATASOURCERANGE_H
