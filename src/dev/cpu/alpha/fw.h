/*
 * fw.h - firmware loader package
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#pragma once

struct romHeader
{
	uint32_t magic1;      // Validation Pattern         (5A5AC3C3)
	uint32_t magic2;      // Inverse Validation Pattern (A5A53C3C)
	uint32_t hdrSize;     // Header Size (in bytes)
	uint32_t imgChkSum;   // Image Checksum
	uint32_t imgSize;     // Image Size;
	uint32_t flgDecmp;    // Decompression Flag
	uint32_t dstAddrLow;  // Destination Address (Low)
	uint32_t dstAddrHigh; // Destination Address (High)
	uint8_t  hdrRevision; // Revision Header
	uint8_t  idRom;       // Firmware ID
	uint8_t  hdrExtRev;   // External Revision Header
	uint8_t  rsvd;        // Reserved
	uint32_t romSize;     // Flash ROM Image Size
	uint32_t idOptFwLow;  // Optional Firmware ID (Low)
	uint32_t idOptFwHigh; // Optional Firmware ID (High)
	uint32_t romOffset;   // ROM Offset
	uint32_t hdrChkSum;   // Header Checksum
};

// Pattern validation for ROM header
#define ROM_MAGIC1 0x5A5AC3C3
#define ROM_MAGIC2 0xA5A53C3C

// Firmware ID
#define FWID_DBM   0 // Debug Monitor
#define FWID_WNT   1 // Windows NT ARC
#define FWID_SRM   2 // Alpha SRM Console
#define FWID_FSB   6 // Fail-safe Booter
#define FWID_MILO  7 // Linux Miniloader
