/*
 * cqbic.cpp - CVAX Qbus Interface Chip
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cqbic.h"

#ifdef DEBUG
static const char *bicNames[] = \
	{ "SCR", "DSER", "MEAR", "SEAR", "MBR" };
#endif /* DEBUG */

DEFINE_DEVICE_TYPE(CQBIC, cqbic_device, "CQBIC", "CVAX Qbus Interface Chip")

cqbic_device::cqbic_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CQBIC, tag, owner, clock)
{
}

cqbic_device::~cqbic_device()
{
}

void cqbic_device::devStart()
{
}

//void cqbic_device::devStop()
//{
//}
//
//void cqbic_device::devReset()
//{
//}

uint32_t cqbic_device::read(offs_t offset, int acc)
{
	return cqReg[offset];
}

void cqbic_device::write(offs_t offset, uint32_t data, int acc)
{
	uint32_t ndata;

	if (acc < LN_LONG) {
		uint32_t sc = (offset & ALIGN_LONG) << 3;
		uint32_t mask = ((acc == LN_WORD) ? MSK_WORD : MSK_BYTE) << sc;
		ndata = ((data << sc) & mask) | (cqReg[offset] & ~mask);
	} else
		ndata = data;

	switch (offset) {
	case CQ_nSCR:
		cqReg[offset] = (ndata & SCR_RW) | (cqReg[offset] & ~SCR_RW) | SCR_PWROK;
		break;

	case CQ_nDSER:
		cqReg[offset] &= ~ndata;
		break;

	case CQ_nMEAR:
	case CQ_nSEAR:
		break;

	case CQ_nMBR:
		cqReg[offset] = ndata & MBR_ADDR;
		break;
	}
}

uint32_t cqbic_device::read_map(offs_t offset, int acc)
{
//	return mapReg[offset];
	return 0;
}

void cqbic_device::write_map(offs_t offset, uint32_t data, int acc)
{
}

uint32_t cqbic_device::read_mem(offs_t offset, int acc)
{
//	return mapReg[offset];
	return 0;
}

void cqbic_device::write_mem(offs_t offset, uint32_t data, int acc)
{
}

uint16_t cqbic_device::read_io(offs_t offset, int acc)
{
//	return mapReg[offset];
	return 0;
}

void cqbic_device::write_io(offs_t offset, uint16_t data, int acc)
{
}

uint16_t cqbic_device::read_ipc(offs_t offset, int acc)
{
//	return mapReg[offset];
	return 0;
}

void cqbic_device::write_ipc(offs_t offset, uint16_t data, int acc)
{
}
