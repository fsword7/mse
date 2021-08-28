/*
 * romloader.h - ROM image loader package
 *
 *  Created on: Mar 30, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/romentry.h"
#include "emu/fileio.h"

class romLoader
{
public:
	romLoader(Machine *sys, Console &cty);
	~romLoader() = default;

	inline Machine *getMachine() const { return system; }

protected:

	cromEntry_t *first(Device &dev);
	cromEntry_t *next(cromEntry_t *entry);

	emu::ioFile *processImageFile(fs::path pname, cromEntry_t *entry);
	void openImageFile(fs::path pkgName, cromEntry_t *entry);
	void closeImageFile();
	int readImageData(uint8_t *buffer, int length, cromEntry_t *entry);
	bool loadImageData(cromEntry_t *parent, cromEntry_t *entry);

	void processImageEntries(ctag_t *pkgName, cromEntry_t *&entry, const Device &dev);
	void processRegionList();

	Console &cty;
	Machine *system = nullptr;

	emu::ioFile *imageFile = nullptr;
	aspace::MemoryRegion *region = nullptr;
};
