/*
 * rom.h - ROM image management
 *
 *  Created on: Feb 9, 2021
 *      Author: Tim Stark
 */

#pragma once

#define ROM_TYPE_MASK		0x0000000F
#define   ROM_TYPE_END		0
#define   ROM_TYPE_REGION	1
#define   ROM_TYPE_IMAGE	2
#define   ROM_TYPE_FILL		3
#define   ROM_TYPE_COPY		4
#define   ROM_TYPE_RELOAD	5
#define   ROM_TYPE_IGNORE	6
#define   ROM_TYPE_CONTINUE	7

#define ROM_REGION_WIDTH      0x00000030 // Data bit width
#define   ROM_REGION_8BIT     0x00000000
#define   ROM_REGION_16BIT    0x00000010
#define   ROM_REGION_32BIT    0x00000020
#define   ROM_REGION_64BIT    0x00000030

#define ROM_REGION_ENDIAN     0x00000040 // Endianess Type (0 = Little, 1 = Big)
#define   ROM_REGION_LE       0x00000000 // Little-endian
#define   ROM_REGION_BE       0x00000040 // Big-endian

#define ROM_REGION_INVERT     0x00000080 // Invert data
#define   ROM_REGION_NOINVERT 0x00000000
#define   ROM_REGION_INVERT   0x00000080

#define ROM_REGION_ERASE      0x00000100 // Erase region before loading

#define ROM_REGION_ERASEVAL	  0x00FF0000
#define   ROM_ERASE_VAL(x)    ((((x) & 0xFF) << 16) | ROM_REGION_ERASE)
#define   ROM_REGION_ERASE00  ROM_ERASE_VAL(0x00)
#define   ROM_REGION_ERASEFF  ROM_ERASE_VAL(0xFF)






#define ROM_END { nullptr, nullptr, 0, 0, 0, ROM_TYPE_END }

struct romEntry_t
{
	ctag_t			*name;
	ctag_t			*hash;
	const uint32_t	offset;
	const uint32_t	length;
	const uint32_t	value;
	const uint32_t	flags;
};
