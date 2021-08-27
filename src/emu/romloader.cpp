/*
 * romloader.cpp - ROM loader package
 *
 *  Created on: Mar 30, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/map/map.h"
#include "emu/romloader.h"
#include "emu/machine.h"

using namespace aspace;

romLoader::romLoader(Machine *sys, Console &cty)
: system(sys), cty(cty)
{
	processRegionList();
}


cromEntry_t *romLoader::first(Device &dev)
{
	const romEntry_t *entry = dev.mapGetROMEntries();

	return !ROMENTRY_ISEND(entry) ? entry : nullptr;
}

cromEntry_t *romLoader::next(cromEntry_t *entry)
{
	entry++;
	if (ROMENTRY_ISREGION(entry))
		entry++;

	return !ROMENTRY_ISEND(entry) ? entry : nullptr;
}

void romLoader::openImageFile(ctag_t *name, cromEntry_t *entry)
{

}

void romLoader::closeImageFile()
{

}

cromEntry_t *romLoader::processImageEntries(ctag_t *name, cromEntry_t *parent, cromEntry_t *entry, const Device &dev)
{
	while (!ROMENTRY_ISREGIONEND(entry))
	{
		if (ROMENTRY_ISFILE(entry))
		{
			cromEntry_t *base = entry;
			int imageLength = 0;

			cty.printf("%s: Loading image file '%s'...\n", dev.getDeviceName(), ROM_GETNAME(entry));
			openImageFile(name, entry);
		}
		entry++;
	}
	return ++entry;
}

void romLoader::processRegionList()
{
	tag_t      *rgnName;
	uint32_t    rgnLength;
	cromEntry_t *entry;
	BusManager  &sbus = system->getExternalBusManager();

	for (Device &dev : DeviceIterator(*system->getSystemDevice()))
	{
		cty.printf("%s: Initializing ROM entries...\n", dev.getDeviceName());
		for (entry = first(dev); entry != nullptr; entry = next(entry))
		{
			rgnName = ROMREGION_GETNAME(*entry);
			rgnLength = ROMREGION_GETLENGTH(*entry);

			cty.printf("%s: Processing ROM region '%s' length %d (%X) bytes\n",
				dev.getDeviceName(), rgnName, rgnLength, rgnLength);


			if (ROMREGION_ISROMDATA(*entry))
			{
				uint8_t  width = ROMREGION_GETWIDTH(*entry);
				endian_t type  = ROMREGION_ISBIGENDIAN(*entry) ? BigEndian : LittleEndian;
				uint8_t  fill  = 0;

				region = sbus.allocateRegion(rgnName, rgnLength, width, type);

				if (ROMREGION_HASFILLVALUE(*entry))
					fill = ROMREGION_GETFILL(*entry);
				memset(region->getBase(), fill, region->getSize());

				entry = processImageEntries(rgnName, entry, entry+1, dev);
				cty.printf("%s: Emd of ROM region\n", dev.getDeviceName());
			}
		}
		cty.printf("%s: End of ROM entries\n", dev.getDeviceName());
	}
}
