#ifndef DATASOURCE_H
#define DATASOURCE_H

#include <map>
#include <string>
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

	~DataSource();

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
	std::string getProperty(std::string key);
	void setProperty(std::string key, std::string value);

	// Work modes
	virtual std::vector<std::string> getWorkModes();

	unsigned int getAddress();
	DataSource *createRange(unsigned int offset, unsigned int size, unsigned int address);

private:
	// Data format
	std::string _dataFormat;
	std::vector<DataSource *> _formatSections;
	void clearFormat();

	// Properties
	std::map<std::string, std::string> _properties;

	void setAddress(unsigned int address);
	unsigned int _address;
};

#endif // DATASOURCE_H
