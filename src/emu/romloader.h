/*
 * romloader.h - ROM image loader package
 *
 *  Created on: Mar 30, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/map/romentry.h"

class romLoader
{
public:
	romLoader(Machine *sys, Console &cty);
	~romLoader() = default;

	inline Machine *getMachine() const { return system; }

protected:

	cromEntry_t *first(Device &dev);
	cromEntry_t *next(cromEntry_t *entry);

	void openImageFile(ctag_t *name, cromEntry_t *entry);
	void closeImageFile();

	void processImageEntries(ctag_t *name, cromEntry_t *parent, cromEntry_t *entry, const Device &dev);
	void processRegionList();

	Console &cty;
	Machine *system = nullptr;

	aspace::MemoryRegion *region = nullptr;
};
