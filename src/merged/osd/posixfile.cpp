/*
 * posixFile.cpp - POSIX file I/O library
 *
 *  Created on: Feb 26, 2019
 *      Author: Tim Stark
 */

#include <string>
#include <cassert>
#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "osd/posixfile.h"



osdFile::error osdError(int errCode)
{
	switch (errCode)
	{
	case 0:
		return osdFile::NONE;

	case ENOENT:
	case ENOTDIR:
		return osdFile::NOT_FOUND;

	case EACCES:
	case EROFS:
	case EEXIST:
	case EPERM:
	case EISDIR:
	case EINVAL:
		return osdFile::ACCESS_DENIED;

//	case ENFILE:
//	case EMFILE:
//		return osdFile::TOO_MANY_FILES;

	default:
		return osdFile::FAILURE;
	}
}


class posixFile : public osdFile
{
public:
	posixFile(int fd)
	: fd(fd)
	{
		assert(fd >= 0);
	}

	virtual ~posixFile()
	{
		::close(fd);
	}

	virtual osdFile::error read(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual)
	{
		ssize_t result;

		if (lseek(fd, offset, SEEK_SET) < 0)
			return osdError(errno);
		result = ::read(fd, buffer, size_t(count));
		if (result < 0)
			return osdError(errno);
		actual = result;
		return osdFile::NONE;
	}

	virtual osdFile::error write(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual)
	{
		ssize_t result;

		if (lseek(fd, offset, SEEK_SET) < 0)
			return osdError(errno);
		result = ::write(fd, buffer, size_t(count));
		if (result < 0)
			return osdError(errno);
		actual = result;
		return osdFile::NONE;
	}

private:
	int fd;
};

osdFile::error osdFile::open(const std::string &path, uint32_t openFlags, ptr &file, uint64_t &fsize)
{
	int access;
	int fd = -1;
	int mode = 0666;

	// Determine POSIX access flags
	if (openFlags & OPEN_FLAG_WRITE) {
		access = (openFlags & OPEN_FLAG_READ) ? O_RDWR : O_WRONLY;
		access |= (openFlags & OPEN_FLAG_CREATE) ? (O_CREAT|O_TRUNC) : 0;
	} else if (openFlags & OPEN_FLAG_READ) {
		access = O_RDONLY;
	}
	access |= O_BINARY;

	fd = ::open(path.c_str(), access, mode);

	if (fd < 0)
		return osdError(errno);

	struct stat fst;
	if (::fstat(fd, &fst) < 0) {
		int errCode = errno;
		::close(fd);
		return osdError(errCode);
	}

	fsize = fst.st_size;
	file  = new posixFile(fd);

	return osdFile::NONE;
}
