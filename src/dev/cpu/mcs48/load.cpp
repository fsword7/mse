/*
 * load.cpp - loading executable files
 *
 *  Created on: Mar 26, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/mcs48/mcs48.h"

bool mcs48_cpuDevice::load(ifstream &fin, offs_t off)
{
	uint8_t blkData[512];
	offs_t  soff = off;

	assert(mapProgram != nullptr);

//	if (off == 0)
//	{
//		// Check if it contains LFU image
//		fin.seekg(0, ios_base::beg);
//		fin.read((char *)blkData, sizeof(blkData));
//
//		// Check if file is LFU image
//		if (!memcmp((char *)&blkData[4], "LFU ", 4)) {
//			// strip first LFU block as not needed.
//			fin.seekg(0x240, ios_base::beg);
//
//			// Reserve first 9 MB memory space uncompressed
//			// firmware space and loading image at starting at
//			// address 90.0000
//			soff = off = 0x900000;
//		}
//		else if ((*(uint32_t *)&blkData[0] == ROM_MAGIC1) &&
//				 (*(uint32_t *)&blkData[4] == ROM_MAGIC2))
//		{
//			fin.seekg(0, ios_base::beg);
//			fin.read((char *)&romHdr, sizeof(romHdr));
////			if ((romHdr.magic1 == ROM_MAGIC1) &&
////				(romHdr.magic2 == ROM_MAGIC2))
//			soff = off = romHdr.dstAddrLow;
//			romSize = romHdr.romSize;
//			fmt::printf("%s: Loading firmware image at %X (%d bytes)\n",
//				getDeviceName(), off, romSize);
//		}
//		else
//		{
//			fmt::printf("%s: Not executable firmware image - aborted.\n", getDeviceName());
//			return false;
//		}
//	}

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
	setPCAddress(soff);

	return true;
}


