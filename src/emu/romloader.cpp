/*
 * romloader.cpp - ROM loader package
 *
 *  Created on: Mar 30, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/romloader.h"
#include "emu/machine.h"

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

void romLoader::processRegionList()
{
	tag_t      *rgnName;
	uint32_t    rgnLength;
	cromEntry_t *entry;

	for (Device &dev : DeviceIterator(*system->getSystemDevice()))
	{
		cty.printf("%s: Initializing ROM entries...\n", dev.getDeviceName());
		for (entry = first(dev); entry != nullptr; entry = next(entry))
		{
			rgnName = ROMREGION_GETNAME(*entry);
			rgnLength = ROMREGION_GETLENGTH(*entry);

			cty.printf("%s: Processing ROM region '%s' length %d (%X) bytes\n",
				dev.getDeviceName(), rgnName, rgnLength, rgnLength);


		}
	}
}
