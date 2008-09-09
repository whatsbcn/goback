#ifndef DATASOURCE_H
#define DATASOURCE_H

#include "Value.h"

#include <map>
#include <vector>

/**
 * DataSource
 *
 * This is the interface to an abstract source of data. DataSource derivated
 * classes can be used to access files, memory, network, etc.
 */
class DataSource {
public:
	static DataSource *create(std::string name);

	virtual ~DataSource();

	virtual bool open(const char *filename) = 0;
	virtual bool close() = 0;

	virtual unsigned int size() = 0;
	virtual unsigned int readBytes(char *buff, unsigned int size, unsigned int offset = -1) = 0;

	// Write
	virtual bool requestWrite() = 0;
	virtual bool requestInsert() = 0;
	virtual bool requestRemove() = 0;

	virtual unsigned int replaceBytes(const char *buff, unsigned int size, unsigned int offset) = 0;
	virtual unsigned int insertBytes(const char *buff, unsigned int size, unsigned int offset) = 0;
	virtual unsigned int removeBytes(unsigned int size, unsigned int offset) = 0;

	virtual bool flushWrites() = 0;

	// Data format
	std::vector<std::string> detectFormat();
	bool setDataFormat(std::string format);
	unsigned int getNumberSections();
	DataSource *getSection(unsigned int section);

	// Properties
	virtual Value* getProperty(std::string key);
	virtual void setProperty(std::string key, Value* value);
	virtual void removeProperty(std::string key);

	// Work modes
	virtual std::vector<std::string> getWorkModes();

	DataSource *createRange(unsigned int offset, unsigned int size);

private:
	// Data format
	std::string _dataFormat;
	std::vector<DataSource *> _formatSections;
	void clearFormat();

protected:
	// Properties
	std::map<std::string, Value*> _properties;
};

#endif // DATASOURCE_H
