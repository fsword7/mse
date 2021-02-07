/*
 * load.cpp - loading executable files
 *
 *  Created on: Jan 22, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/fw.h"

bool AlphaProcessor::load(ifstream &fin, offs_t off)
{
	uint8_t blkData[512];
	offs_t  soff = off;

	romHeader romHdr;
	uint64_t  romSize;

	assert(mapProgram != nullptr);

//	if (off == 0)
//	{
//		// Check if it contains LFU image
//		fin.read((char *)blkData, sizeof(blkData));
//		if (!memcmp((char *)&blkData[4], "LFU ", 4)) {
//			// strip first LFU block as not needed.
//			fin.seekg(0x240, ios_base::beg);
//
//			// Reserve first 9 MB memory space uncompressed
//			// firmware space and loading image at starting at
//			// address 90.0000
//			soff = off = 0x900000;
//		} else {
//			fmt::printf("Not LFU executable file\n");
//			return false;
//		}
//	}
//	else
	{
		fin.read((char *)&romHdr, sizeof(romHdr));
		if ((romHdr.magic1 != ROM_MAGIC1) ||
			(romHdr.magic2 != ROM_MAGIC2))
		{
			fmt::printf("Not executable firmware file\n");
			return false;
		}

		soff = off = romHdr.dstAddrLow;
		romSize = romHdr.romSize;
		fmt::printf("%s: Loading firmware image at %X (%d bytes)\n",
			getDeviceName(), off, romSize);
	}

	while (!fin.eof())
	{
		fin.read((char *)blkData, sizeof(blkData));
		mapProgram->writeBlock(off, blkData, fin.gcount());
		off += fin.gcount();
	}
	fin.close();

	fmt::printf("%s: Loaded %s into %llX-%llX (length: %d bytes)\n",
		getDeviceName(), "LFU" /* fname */, soff, off, off - soff);

	// Set starting PC and PAL address
	setPCAddress(soff|1);
	setPALAddress(soff);

	return true;
}


