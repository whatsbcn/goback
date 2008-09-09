#ifndef DATASOURCERANGE_H
#define DATASOURCERANGE_H

#include "DataSource.h"

class DataSourceRange : public DataSource {
public:
	DataSourceRange();

	void setRange(DataSource *ds, unsigned int offset, unsigned int size);

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

	/** Properties */
	Value* getProperty(std::string key);
	void setProperty(std::string key, Value* value);
	void removeProperty(std::string key);
	void setDSProperties(std::map<std::string, Value*> *properties);

private:
	DataSource *_ds;
	unsigned int _size;
	unsigned int _offset;
	unsigned int _range;	

	std::map<std::string, Value*> *_DSProperties;
};

#endif // DATASOURCERANGE_H
