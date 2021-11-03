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
	while (!ROMENTRY_ISREGIONEND(entry))
		entry++;

	return !ROMENTRY_ISEND(entry) ? entry : nullptr;
}

emu::ioFile *romLoader::processImageFile(fs::path pathName, cromEntry_t *entry)
{
	fs::path fwPath = "fw";
	auto imageFile = new emu::ioFile(0);

	fs::path fileName = fwPath / pathName / ROM_GETNAME(entry);

	if (!imageFile->open(fileName))
	{
		// cty.printf("Failed to open %s\n", fileName);

		delete imageFile;
		imageFile = nullptr;
	}
	// cty.printf("Opening '%s'...\n", fileName);
	return imageFile;
}

void romLoader::openImageFile(fs::path pkgName, cromEntry_t *entry)
{
	imageFile = processImageFile(pkgName, entry);
}

void romLoader::closeImageFile()
{
	imageFile->close();
	delete imageFile;
	imageFile = nullptr;
}

int romLoader::readImageData(uint8_t *buffer, int length, cromEntry_t *entry)
{
	int actual = 0;

	if (imageFile != nullptr)
		actual = imageFile->read(buffer, length);
	return actual;
}

bool romLoader::loadImageData(cromEntry_t *parent, cromEntry_t *entry)
{
	int offset = ROM_GETOFFSET(entry);
	int length = ROM_GETLENGTH(entry);
	// int skip = ROM_GETSKIP(entry);
	// int dShift = ROM_GETBITSHIFT(entry);
	// int dMask = ((1 << ROM_GETBITWIDTH(entry)));
	// int gSize = ROM_GETGROUPSIZE(entry);
	// int reversed = ROM_ISREVERSED(entry);
	// int nGroups = (length + gSize - 1) / gSize;
	uint8_t *base = region->getBase() + offset;
	int actual;
	// int bufSize;

	cty.printf("%s: Reading image data: off=%X len=%X\n", ROM_GETNAME(entry), offset, length);
	actual = readImageData(base, length, entry);
	if (actual != length)
	{
		cty.printf("%s: Got %d (%X) bytes - expected %d (%X) bytes\n",
			ROM_GETNAME(entry), actual, actual, length, length);
		return false;
	}
	return true;
}

// cromEntry_t *romLoader::processImageEntries(ctag_t *name, cromEntry_t *parent, cromEntry_t *entry, const Device &dev)
void romLoader::processImageEntries(ctag_t *pkgName, cromEntry_t *&entry, const Device &dev)
{
	cromEntry_t *parent = entry++;

	while (!ROMENTRY_ISREGIONEND(entry))
	{
		if (ROMENTRY_ISFILE(entry))
		{
			cromEntry_t *base = entry;
			int imageLength = 0;

			cty.printf("%s: Loading image file '%s'...\n", dev.getDeviceName(), ROM_GETNAME(entry));
			openImageFile(pkgName, entry);

			if (imageFile != nullptr)
			{
				loadImageData(parent, entry);
				closeImageFile();
			}
			entry++;
		}
		else
			entry++;
	}
}

void romLoader::processRegionList()
{
	tag_t      *pkgName;
	tag_t      *rgnName;
	uint32_t    rgnLength;
	cromEntry_t *entry;
	BusManager  &sbus = system->getExternalBusManager();

	for (Device &dev : DeviceIterator(*system->getSystemDevice()))
	{
		pkgName = dev.getShortName();

		cty.printf("%s: Initializing ROM entries for %s...\n", dev.getDeviceName(), pkgName);
		for (entry = first(dev); entry != nullptr; entry = next(entry))
		{
			if (ROMENTRY_ISCONTAINER(entry))
			{
				pkgName = ROM_GETNAME(entry);
				cty.printf("%s: Package '%s' container\n",
					dev.getDeviceName(), pkgName);
				continue;
			}

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

				// entry = processImageEntries(rgnName, entry, entry+1, dev);
				processImageEntries(pkgName, entry, dev);
			}
		}
		cty.printf("%s: End of ROM entries\n", dev.getDeviceName());
	}
}
