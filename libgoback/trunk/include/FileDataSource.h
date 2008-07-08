#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

class FileDataSource {

	public:
		FileDataSource();
		bool requestWrite();
		bool requestInsert();
		bool requestRemove();
		bool flushWrites();
		bool open(const char * filename);
		bool close();
		int readBytes(char * buff, int size, unsigned int offset=-1);
		int replaceBytes(const char * buff, int size, unsigned int offset=-1);
		int insertBytes(const char * buff, int size, unsigned int offset);
		int removeBytes(int size, unsigned int offset);

	private:
		int _fd;
		bool _writable;
		char * _filename;
};
