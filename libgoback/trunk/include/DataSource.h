class DataSource {
public:
	virtual bool requestWrite() = 0;
	virtual bool requestInsert() = 0;
	virtual bool requestRemove() = 0;
	virtual bool flushWrites() = 0;
	virtual bool open(const char * filename) = 0;
	virtual bool close() = 0;
	virtual int readBytes(char * buff, int size, unsigned int offset = -1) = 0;
	virtual int replaceBytes(const char * buff, int size, unsigned int offset = -1) = 0;
	virtual int insertBytes(const char * buff, int size, unsigned int offset) = 0;
	virtual int removeBytes(int size, unsigned int offset) = 0;
};
