/*
 * axp_mem.cpp - Alpha processor - memory management
 *
 *  Created on: Feb 7, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/axp.h"
#include "dev/cpu/alpha/axp_mmu.h"

// Virtual address to physical conversion
uint64_t AlphaProcessor::checkv(uint64_t vAddr, uint32_t flags, bool &asmb, int &status)
{
//	int spe = (flags & ACC_EXEC) ? state.ispe : state.mspe;
//	int cm  = (flags & ALTCM) ? state.altcm : state.cm;

	asmb   = true;
	status = 0;
	return vAddr;
}

// Aligned read access with physical address
uint64_t AlphaProcessor::readp(uint64_t pAddr, int size)
{
	switch (size)
	{
		case LN_QUAD: return mapProgram->read64(pAddr, this);
		case LN_LONG: return mapProgram->read32(pAddr, this);
		case LN_WORD: return mapProgram->read16(pAddr, this);
		case LN_BYTE: return mapProgram->read8(pAddr, this);
	}
	return 0;
}

// Aligned write access with physical address
void AlphaProcessor::writep(uint64_t pAddr, uint64_t data, int size)
{
	switch (size)
	{
		case LN_QUAD: mapProgram->write64(pAddr, data, this); break;
		case LN_LONG: mapProgram->write32(pAddr, data, this); break;
		case LN_WORD: mapProgram->write16(pAddr, data, this); break;
		case LN_BYTE: mapProgram->write8(pAddr, data, this);  break;
	}
}

uint64_t AlphaProcessor::readv(uint64_t vAddr, int size)
{
	uint64_t pAddr = vAddr;

	// Check if address is aligned for single memory access
	if ((pAddr & (size - 1)) != 0)
		dbg.log("** Unaligned read access **");

	return readp(pAddr, size);
}

void AlphaProcessor::writev(uint64_t vAddr, uint64_t data, int size)
{
	uint64_t pAddr = vAddr;

	// Check if address is aligned for single memory access
	if ((pAddr & (size - 1)) != 0)
		dbg.log("** Unaligned write access **");

	writep(pAddr, data, size);
}

int AlphaProcessor::findTBEntry(uint64_t vAddr, uint32_t accFlags)
{
	int tbidx = (accFlags & ACC_EXEC) ? 1 : 0;
	int asn   = (accFlags & ACC_EXEC) ? state.asn : state.asn0;
	int rw    = (accFlags & ACC_WRITE) ? 1 : 0;

	int idx = state.tbLast[tbidx][rw];

	if ((state.tb[tbidx][idx].valid == true) &&
		!((state.tb[tbidx][idx].vAddr ^ vAddr) & state.tb[tbidx][idx].matchMask) &&
		(state.tb[tbidx][idx].asmb || (state.tb[tbidx][idx].asn == asn)))
		return idx;

	for (idx = 0; idx < TB_ENTRIES; idx++)
	{
		if ((state.tb[tbidx][idx].valid == true) &&
			!((state.tb[tbidx][idx].vAddr ^ vAddr) & state.tb[tbidx][idx].matchMask) &&
			(state.tb[tbidx][idx].asmb || (state.tb[tbidx][idx].asn == asn)))
		{
			state.tbLast[tbidx][rw] = idx;
			return idx;
		}
	}

	return -1;
}
