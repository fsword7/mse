/*
 * cmctl.cpp - CVAX Memory Controller
 *
 *  Created on: Mar 8, 2019
 *      Author: Tim Stark
 */

#include "emu/emucore.h"
#include "dev/chip/dec/vax/cmctl.h"

DEFINE_DEVICE_TYPE(CMCTL, cmctl_device, "CMCTL", "CVAX Memory Controller")

#ifdef DEBUG
static const char *cmNames[] =
{
	"CMCNF00", "CMCNF01", "CMCNF02", "CMCNF03",
	"CMCNF04", "CMCNF05", "CMCNF06", "CMCNF07",
	"CMCNF08", "CMCNF09", "CMCNF10", "CMCNF11",
	"CMCNF12", "CMCNF13", "CMCNF14", "CMCNF15",
	"CMERR",   "CMCSR",   "CMSIZE",
};
#endif /* DEBUG */

cmctl_device::cmctl_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: device_t(config, CMCTL, tag, owner, clock)
{
}

cmctl_device::~cmctl_device()
{
}

void cmctl_device::devStart()
{
}

//void cmctl_device::devStop()
//{
//}
//
//void cmctl_device::devReset()
//{
//}

uint32_t cmctl_device::read(offs_t offset, int acc)
{
	return cmReg[offset];
}

void cmctl_device::write(offs_t offset, uint32_t data, int acc)
{
//	if (acc < LN_LONG) {
//		uint32_t sc = (offset & ALIGN_LONG) << 3;
//		uint32_t mask = ((acc == LN_WORD) ? W_MASK : B_MASK) << sc;
//		ndata = ((data << sc) & mask) | (sscReg[offset] & ~mask);
//	} else
//		ndata = data;
	data <<= (offset & ALIGN_LONG) << 3;

	switch (offset) {
	case CM_nCNF00: case CM_nCNF01: case CM_nCNF02: case CM_nCNF03:
	case CM_nCNF04: case CM_nCNF05: case CM_nCNF06: case CM_nCNF07:
	case CM_nCNF08: case CM_nCNF09: case CM_nCNF10: case CM_nCNF11:
	case CM_nCNF12: case CM_nCNF13: case CM_nCNF14: case CM_nCNF15:
		if (data & CMCNF_SRQ) {

		}
		cmReg[offset] = (data & CMCNF_RW) | (cmReg[offset] & ~CMCNF_RW);
		break;

	case CM_nERR:
		cmReg[offset] &= ~(data & CMERR_W1C);
		break;

	case CM_nCSR:
		cmReg[offset] = data & CMCSR_RW;
		break;

	case CM_nMSZ:
		break;
	}
}
