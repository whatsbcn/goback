#include "DataSourceFile.h"

#include <cstring>
#include <fcntl.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>

// Constructor
DataSourceFile::DataSourceFile() {
	_fd = -1;
	_writable = false;
	_seek = 0;
	_size = 0;
}

/*
 * Open the file in read-write.
 */
bool DataSourceFile::requestWrite() {
	if (_writable) return true;

	int fdw = ::open(_filename, O_RDWR);

	if (fdw == -1) return false;
	_writable = true;
	::close(_fd);
	_fd = fdw;
	return true;
}

/*
 * Open the file in read-write, and let insert.
 */
bool DataSourceFile::requestInsert() {
	return requestWrite();
}

bool DataSourceFile::requestRemove() {
	return requestWrite();
}

/*
 * Flush the writes.
 */
bool DataSourceFile::flushWrites() {
	if (fsync(_fd) == 0) return true;
	return false;
}

/*
 * Open file in read-only
 */
bool DataSourceFile::open(const char *filename) {
	// Open file
	_fd = ::open(filename, O_RDONLY);

	// Check errors
	if (_fd == -1) return false;

	_filename = strdup(filename);

	return true;
}

/*
 * Close the file.
 */
bool DataSourceFile::close() {
	if (::close(_fd) == 0) return true;
	_fd = -1;
	return false;
}

/*
 * Put at buff, size bytes, from offset.
 */
unsigned int DataSourceFile::readBytes(char *buff, unsigned int size, unsigned int offset) {
	int result = 0;

	// Check file opened
	if (_fd == -1) return 0;
	
	//printf("read buff=%p, size=%d, offset=%d\n", buff, size, offset);
	// Seek the offset
	if (_seek == offset) {
		result = read(_fd, buff, size);
	} else {
		_seek = lseek(_fd, offset, SEEK_SET);
		result = read(_fd, buff, size);
	}
	_seek += result;
	return result;
}

/*
 * Replace at offset, size bytes, with buff.
 */
unsigned int DataSourceFile::replaceBytes(const char *buff, unsigned int size, unsigned int offset) {
	int result = 0;

	// Check file opened and writable
	if (_fd == -1 || !_writable) return 0;

	// Seek the offset
	if (lseek(_fd, offset, SEEK_SET) == (int)offset) {
		result = write(_fd, buff, size);
	} else {
		result = write(_fd, buff, size);
	}
	return result;
}

/*
 * Insert size bytes from buff to the offset.
 * This is for small sizes. ~mbytes.
 */
unsigned int DataSourceFile::insertBytes(const char *buff, unsigned int size, unsigned int offset) {
	// Check file opened and writable
	if (_fd == -1 || !_writable) return 0;

	int fdtmp = ::open(_filename, O_RDONLY);

	unsigned int buffsize = (size < 4096) ? 4096 : size;
	unsigned char *bufftmp = new unsigned char[buffsize];

	// Set the seek and the end of file
	int filesize = lseek(_fd, 0, SEEK_END);
	lseek(_fd, -size, SEEK_END);

	// Add a chunk of size bytes at the end of file
	int bytes = read(_fd, bufftmp, size);
	write(_fd, bufftmp, bytes);

	// Move the content
	int i = 0;
	while ((filesize - offset - buffsize * i - size) > buffsize){
		lseek(_fd, filesize - (buffsize * (i + 1)) - size, SEEK_SET);
		bytes = read(_fd, bufftmp, buffsize);
		lseek(_fd, filesize - (buffsize * (i + 1)), SEEK_SET);
		write(_fd, bufftmp, bytes);
		i++;
	}

	// There are more bytes to move?
	bytes = filesize - offset - buffsize * i - size;
	if (bytes > 0) {
		lseek(_fd, filesize - buffsize * i - size - bytes, SEEK_SET);
		read(_fd, bufftmp, bytes);
		lseek(_fd, filesize - buffsize * i - bytes, SEEK_SET);
		write(_fd, bufftmp, bytes);
	}

	// Clean
	delete[] bufftmp;
	::close(fdtmp);

	// Insert the bytes
	lseek(_fd, offset, SEEK_SET);
	return write(_fd, buff, size);
}

/*
 * Remove size bytes at offset.
 * This is for small sizes. ~mbytes.
 */
unsigned int DataSourceFile::removeBytes(unsigned int size, unsigned int offset) {
	// Check file opened and writable
	if (_fd == -1 || !_writable) return 0;

	int buffsize = (size < 4096) ? 4096 : size;
	unsigned char *bufftmp = new unsigned char[buffsize];
	int fdtmp = ::open(_filename, O_RDONLY);
	int filesize = lseek(_fd, 0, SEEK_END);
	lseek(_fd, offset, SEEK_SET);
	lseek(fdtmp, offset + size, SEEK_SET);

	int bytes;
	while ((bytes = read(fdtmp, bufftmp, buffsize)) > 0) {
		write(_fd, bufftmp, bytes);
	}

	delete[] bufftmp;
	::close(fdtmp);

	ftruncate(_fd, filesize - size);

	return size;
}

/*
 * Get size in bytes.
 */
unsigned int DataSourceFile::size() {
	// Check file opened
	if (_fd == -1) return 0;

	if (_size != 0) return _size;
	_size = lseek(_fd, 0, SEEK_END);

	return _size;
}
