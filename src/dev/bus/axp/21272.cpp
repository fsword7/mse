/*
 * 21272.cpp - DEC Alpha - 21272 Tsunami/Typhoon chipset package
 *
 *  Created on: Dec 20, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/bus/axp/21272.h"

// Read access function calls
uint64_t axp21272_pciDevice::ccRead(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x0000: // CSC
		return state.cchip.csc;

	case 0x0080: // MISC
		return state.cchip.misc; /* | cpu->getProcessorID(); */

	case 0x0100: // memory size
		return 0;

	case 0x0140:
	case 0x0180:
	case 0x01C0:
		return 0;

	case 0x0200:
	case 0x0240:
	case 0x0600:
	case 0x0640:
		return state.cchip.dim[((pAddr >> 10) & 2) | ((pAddr >> 6) & 1)];

	case 0x0280:
	case 0x02C0:
	case 0x0680:
	case 0x06C0:
		return state.cchip.drir & state.cchip.dim[((pAddr >> 10) & 2) | ((pAddr >> 6) & 1)];

	case 0x0300:
		return state.cchip.drir;
	}
	return 0;
}

template <int pcIndex>
uint64_t axp21272_pciDevice::pcRead(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		return state.pchip[pcIndex].wsba[(pAddr >> 6) & 3];

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		return state.pchip[pcIndex].wsm[(pAddr >> 6) & 3];

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		return state.pchip[pcIndex].tba[(pAddr >> 6) & 3];

	case 0x0300:
		return state.pchip[pcIndex].pctl;

	case 0x03C0:
		return state.pchip[pcIndex].perr;

	case 0x0400:
		return state.pchip[pcIndex].perrmask;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		return 0;

	case 0x0800: // PCI reset
		return 0;
	}

	return 0;
}

uint64_t axp21272_pciDevice::pcRead0(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		return state.pchip[0].wsba[(pAddr >> 6) & 3];

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		return state.pchip[0].wsm[(pAddr >> 6) & 3];

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		return state.pchip[0].tba[(pAddr >> 6) & 3];

	case 0x0300:
		return state.pchip[0].pctl;

	case 0x03C0:
		return state.pchip[0].perr;

	case 0x0400:
		return state.pchip[0].perrmask;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		return 0;

	case 0x0800: // PCI reset
		return 0;
	}

	return 0;
}

uint64_t axp21272_pciDevice::pcRead1(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		return state.pchip[1].wsba[(pAddr >> 6) & 3];

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		return state.pchip[1].wsm[(pAddr >> 6) & 3];

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		return state.pchip[1].tba[(pAddr >> 6) & 3];

	case 0x0300:
		return state.pchip[1].pctl;

	case 0x03C0:
		return state.pchip[1].perr;

	case 0x0400:
		return state.pchip[1].perrmask;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		return 0;

	case 0x0800: // PCI reset
		return 0;
	}

	return 0;
}

uint8_t axp21272_pciDevice::dcRead(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x0000: // DSC
		return state.dchip.dsc;

	case 0x0040: // STR
		return state.dchip.str;

	case 0x0080: // DREV
		return state.dchip.drev;

	case 0x00C0: // DSC2
		return state.dchip.dsc2;
	}
	return 0;
}

uint8_t axp21272_pciDevice::tigRead(uint32_t pAddr)
{
	switch (pAddr)
	{
	case 0x3000'0000: // TRR
		return 0;
	case 0x3000'0040: // SMIR
		return 0;
	case 0x3000'0100: // MOD information
		return 0;
	case 0x3000'03C0: // TTCR
		return 0;
	case 0x3000'0480: // clr_pwr_flt_det
		return 0;
	case 0x3000'05C0: // ev6_halt
		return 0;
	case 0x3000'0180: // Arbiter revision
		return 0xFE;
	default:
		// Flash ROM access
		return 0;
	}
	return 0;
}

// Write access function calls
void axp21272_pciDevice::ccWrite(uint32_t pAddr, uint64_t data)
{
	switch (pAddr)
	{
	case 0x0000: // CSC
		state.cchip.csc &= ~0x0777'7777'FF3F'0000LL;
		state.cchip.csc |= (data & 0x0777'7777'FF3F'0000LL);
		return;

	case 0x0080: // MISC
		return;

	case 0x0200: // DIM
	case 0x0240:
	case 0x0600:
	case 0x0640:
		state.cchip.dim[((pAddr >> 10) & 2) | ((pAddr >> 6) & 1)] = data;
		return;
	}
}

template <int pcIndex>
void axp21272_pciDevice::pcWrite(uint32_t pAddr, uint64_t data)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		state.pchip[pcIndex].wsba[(pAddr >> 6) & 3] = data;
		return;

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		state.pchip[pcIndex].wsm[(pAddr >> 6) & 3] = data;
		break;

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		state.pchip[pcIndex].tba[(pAddr >> 6) & 3] = data;
		break;

	case 0x0300: // PCTL
		state.pchip[pcIndex].pctl = data;
		break;

	case 0x03C0: // PERR
		break;

	case 0x0400: // PERRMASK
		state.pchip[pcIndex].perrmask = data;
		break;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		break;

	case 0x0800: // PCI reset
		// Reset all PCI devices
		break;
	}
}

void axp21272_pciDevice::pcWrite0(uint32_t pAddr, uint64_t data)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		state.pchip[0].wsba[(pAddr >> 6) & 3] = data;
		return;

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		state.pchip[0].wsm[(pAddr >> 6) & 3] = data;
		break;

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		state.pchip[0].tba[(pAddr >> 6) & 3] = data;
		break;

	case 0x0300: // PCTL
		state.pchip[0].pctl = data;
		break;

	case 0x03C0: // PERR
		break;

	case 0x0400: // PERRMASK
		state.pchip[0].perrmask = data;
		break;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		break;

	case 0x0800: // PCI reset
		// Reset all PCI devices
		break;
	}
}

void axp21272_pciDevice::pcWrite1(uint32_t pAddr, uint64_t data)
{
	switch (pAddr)
	{
	case 0x0000: // WSBA0
	case 0x0040: // WSBA1
	case 0x0080: // WSBA2
	case 0x00C0: // WSBA3
		state.pchip[1].wsba[(pAddr >> 6) & 3] = data;
		return;

	case 0x0100: // WSM0
	case 0x0140: // WSM1
	case 0x0180: // WSM2
	case 0x01C0: // WSM3
		state.pchip[1].wsm[(pAddr >> 6) & 3] = data;
		break;

	case 0x0200: // TLBA0
	case 0x0240: // TLBA1
	case 0x0280: // TLBA2
	case 0x02C0: // TLBA3
		state.pchip[1].tba[(pAddr >> 6) & 3] = data;
		break;

	case 0x0300: // PCTL
		state.pchip[1].pctl = data;
		break;

	case 0x03C0: // PERR
		break;

	case 0x0400: // PERRMASK
		state.pchip[1].perrmask = data;
		break;

	case 0x0440: // TLBIV (not implemented yet)
	case 0x04C0: // TLBIA (not implemented yet)
		break;

	case 0x0800: // PCI reset
		// Reset all PCI devices
		break;
	}
}

void axp21272_pciDevice::dcWrite(uint32_t pAddr, uint8_t data)
{
	// Do nothing due to all read-only registers
}

void axp21272_pciDevice::tigWrite(uint32_t pAddr, uint8_t data)
{
	switch (pAddr)
	{
	case 0x3000'0000: // TRR
		return;
	case 0x3000'0040: // SMIR
		return;
	case 0x3000'0100: // MOD information
		return;
	case 0x3000'03C0: // TTCR
		return;
	case 0x3000'0480: // clr_pwr_flt_det
		return;
	case 0x3000'05C0: // ev6_halt
		return;
	default:
		// Flash ROM access
		return;
	}
}

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

//void axp21272_pciDevice::setAddressSpace()
//{
//
//}
