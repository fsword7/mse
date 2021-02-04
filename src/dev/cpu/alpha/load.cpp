/*
 * load.cpp - loading executable files
 *
 *  Created on: Jan 22, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"

bool AlphaProcessor::load(ifstream &fin)
{
	uint8_t blkData[512];
	offs_t  soff, off;

	assert(mapProgram != nullptr);

	// Check if it contains LFU image
	fin.read((char *)blkData, sizeof(blkData));
	if (!memcmp((char *)&blkData[4], "LFU ", 4)) {
		// strip first LFU block as not needed.
		fin.seekg(0x240, ios_base::beg);

		// Reserve first 9 MB memory space uncompressed
		// firmware space and loading image at starting at
		// address 90.0000
		soff = off = 0x900000;
	} else {
		fmt::printf("Not LFU executable file\n");
		return false;
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
	setPCAddress(0x900001);
	setPALAddress(0x900000);

	return true;
}


