/*
 * 21272.cpp - DEC 21272 Tsunami/Typhoon chipset package
 *
 *  Created on: Dec 20, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/chip/dec/21272.h"

// System memory map
//
// Space				Size		System Address <43:0>				Comments
// Memory    			4GB			000.0000.0000 - 000.FFFF.FFFF		Cacheable and prefechable
// Reserved  			8188GB		001.0000.0000 - 7FF.FFFF.FFFF
//
// Pchip0 PCI			4GB			800.0000.0000 - 800.FFFF.FFFF		Linear addressing
// TIGbus				1GB			801.0000.0000 - 801.3FFF.FFFF		addr<5:0> = 0, single byte valid
//																		in quadword access 16MB accessible.
// Reserved				1GB			801.4000.0000 - 801.7FFF.FFFF
// Pchip0 CSRs			256MB		801.8000.0000 - 801.8FFF.FFFF		addr<5:0> = 0, quadword access
// Reserved				256MB		801.9000.0000 - 801.9FFF.FFFF
// Cchip CSRs			256MB		801.A000.0000 - 801.AFFF.FFFF		addr<5:0> = 0, quadword access
// Dchip CSRs			256MB		801.B000.0000 - 801.BFFF.FFFF		addr<5:0> = 0, all 8 bytes in quadword
//																		access must be identical.
// Reserved				768MB		801.C000.0000 - 801.EFFF.FFFF
// Reserved				128MB		801.F000.0000 - 801.F7FF.FFFF
// Pchio0 PCI IACK		64MB		801.F800.0000 - 801.FBFF.FFFF		Linear addressing
// Pchip0 PCI I/O		32MB		801.FC00.0000 - 801.FDFF.FFFF		Linear addressing
// Pchip0 PCI cfg		16MB		801.FE00.0000 - 801.FEFF.FFFF		Linear addressing
// Reserved				16MB		801.FF00.0000 - 801.FFFF.FFFF
//
// Pchip1 PCI			4GB			802.0000.0000 - 802.FFFF.FFFF		Linear addressing
// Reserved				2GB			803.0000.0000 - 803.7FFF.FFFF
// Pchip1 CSRs			256MB		803.8000.0000 - 803.8FFF.FFFF		addr<5:0> = 0, quadword access.
// Reserved				1536MB		803.9000.0000 - 803.EFFF.FFFF
// Reserved				128MB		803.F000.0000 - 803.F7FF.FFFF
// Pchip1 PCI IACK		64MB		803.F800.0000 - 803.FBFF.FFFF		Linear addressing
// Pchip1 PCI I/O		32MB		803.FC00.0000 - 803.FDFF.FFFF		Linear addressing
// Pchip1 PCI cfg		16MB		803.FE00.0000 - 803.FEFF.FFFF		Linear addressing
// Reserved				16MB		803.FF00.0000 - 803.FFFF.FFFF

// Reserved  			8172GB		804.0000.0000 - FFF.FFFF.FFFF		Bits <42:35> are don't cares if bit
//																		<43> is asserted.
