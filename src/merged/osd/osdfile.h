/*
 * osdfile.h
 *
 *  Created on: Feb 26, 2019
 *      Author: fswor
 */

#pragma once

#define OPEN_FLAG_READ		0x0001
#define OPEN_FLAG_WRITE		0x0002
#define OPEN_FLAG_CREATE	0x0004

class osdFile
{
public:
	typedef osdFile *ptr;

	enum error {
		NONE,
		FAILURE,
		OUT_OF_MEMORY,
		NOT_FOUND,
		ACCESS_DENIED,
		INVALID_DATA,
		INVALID_ACCESS
	};

	virtual ~osdFile() {}

	static error open(const std::string &path, uint32_t openFlags, ptr &file, uint64_t &fsize);

	virtual error read(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual) = 0;
	virtual error write(void *buffer, uint64_t offset, uint32_t count, uint32_t &actual) = 0;

};
