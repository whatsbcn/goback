#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <list>
#include <string>

class DataSource {
public:
	static DataSource *create(std::string name, std::string format = "");

	virtual bool open(const char * filename) = 0;
	virtual bool close() = 0;

	virtual int size() = 0;
	virtual int readBytes(char * buff, int size, unsigned int offset = -1) = 0;

	virtual bool requestWrite() = 0;
	virtual bool requestInsert() = 0;
	virtual bool requestRemove() = 0;

	virtual int replaceBytes(const char * buff, int size, unsigned int offset) = 0;
	virtual int insertBytes(const char * buff, int size, unsigned int offset) = 0;
	virtual int removeBytes(int size, unsigned int offset) = 0;

	virtual bool flushWrites() = 0;

	virtual std::list<std::string> getWorkModes();

	DataSource *createRange(int offset, int size, std::string format);

private:
	// Data format
	std::string _dataFormat;
	void setDataFormat(std::string);
};

#endif // DATASOURCE_H
