/*
 * ev4.cpp - 21064 series - main package
 *
 *  Created on: Feb 11, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev4.h"
#include "dev/cpu/alpha/axp_mmu.h"

DEFINE_DEVICE_TYPE(dec21064, dec21064_cpuDevice, "21064", "DEC 21064 Alpha Processor")

dec21064_cpuDevice::dec21064_cpuDevice(const SystemConfig &config,
		cstag_t &tagName, Device *owner, uint64_t clock)
: AlphaProcessor(config, dec21064, tagName, owner, clock, 36)
{
	setArchType(ARCH_EV4);
	setArchFlags(0);
}

void dec21064_cpuDevice::preset()
{
//	ev5.icsr = ICSR_MBO;
}

int dec21064_cpuDevice::abort(int why)
{
//	switch (why)
//	{
//	case EXC_OPCODE:   // Illegal opcode
//		break;
//	case EXC_ITBMISS:  // Istream TB miss
//		break;
//	case EXC_DTBMISS1: // Dstream TB miss single
//		break;
//	case EXC_DTBMISS2: // Dstream TB miss double with VPTE
//		break;
//	case EXC_IACV:     // Istream access violation
//		break;
//	case EXC_FPDIS:    // Floating-point operation attempted
//		break;
//	}
//
	// Always return error code
	return -1;
}

int dec21064_cpuDevice::translate(uint64_t vAddr, uint64_t &pAddr, bool &asmb, int accFlags)
{
//	int spe = (accFlags & ACC_EXEC) ? state.ispe : state.mspe;
//	int cm  = (accFlags & ACC_ALTCM) ? state.altcm : state.icm;
//
//	// Try super page translation first
//	if (spe & cm != ACC_KERNEL)
//	{
//		if (((vAddr & SPE1_MASK) == SPE1_MATCH) && (spe & 2))
//		{
//			asmb  = false;
//			pAddr = (vAddr & SPE1_MAP) | (vAddr & SPE1_TEST) ? SPE1_ADD : 0;
//			return 0;
//		}
//		else if (((vAddr & SPE0_MASK) == SPE0_MATCH) && (spe & 1))
//		{
//			asmb  = false;
//			pAddr = vAddr & SPE0_MAP;
//			return 0;
//		}
//	}
//
//	int eidx;
//	if ((eidx = findTBEntry(vAddr, accFlags)) < 0)
//	{
//		// TB miss (page fault) - issue PAL exception
//		if (accFlags & ACC_VPTE)
//		{
//			state.fpcAddr = vAddr;
//			state.excMask = 1u << RA;
//			setPC(state.palBase + PAL_DTBM_DOUBLE + 1);
//		}
//		else if (accFlags & ACC_EXEC)
//		{
//			setPC(state.palBase + PAL_ITB_MISS + 1);
//		}
//		else
//		{
//			state.fpcAddr = vAddr;
//			state.excMask = 1u << RA;
//			int opCode = OP_GETOP(state.opWord);
//			state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 11 |
//					(RA << 6) | (accFlags & ACC_WRITE);
//			setPC(state.palBase + PAL_DTBM_SINGLE + 1);
//		}
//
//		return -1;
//	}
//
//	// Got TB entry now, check access flags for any access violation
//	int tbidx = (accFlags & ACC_EXEC) ? 1 : 0;
//
//	{
//		if (!state.tb[tbidx][eidx].access[accFlags & ACC_WRITE][cm])
//		{
//			if (accFlags & ACC_EXEC)
//			{
//				state.excAddr = state.pcAddr;
//				setPC(state.palBase + PAL_IACCVIO + 1);
//			}
//			else
//			{
//				state.excAddr = state.pcAddr;
//				state.fpcAddr = vAddr;
//
//				uint32_t opCode = OP_GETOP(state.opWord);
//				state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 11 |
//					(RA << 6) | (accFlags & ACC_WRITE) | 2;
//				setPC(state.palBase + PAL_DFAULT + 1);
//			}
//			return -1;
//		}
//
//		if (state.tb[tbidx][eidx].fault[accFlags & ACC_MODE])
//		{
//			if (accFlags & ACC_EXEC)
//			{
//				state.excAddr = state.pcAddr;
//				setPC(state.palBase + PAL_IACCVIO + 1);
//			}
//			else
//			{
//				state.excAddr = state.pcAddr;
//				state.fpcAddr = vAddr;
//
//				uint32_t opCode = OP_GETOP(state.opWord);
//				state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 11 |
//					(RA << 6) | (accFlags & ACC_WRITE) | ((accFlags & ACC_WRITE) ? 8 : 4);
//				setPC(state.palBase + PAL_DFAULT + 1);
//			}
//			return -1;
//		}
//	}
//
//	// Finally no access violations or faults
//	// Now convert to physical address
//	pAddr = state.tb[tbidx][eidx].pAddr | (vAddr & state.tb[tbidx][eidx].keepMask);
//	asmb  = state.tb[tbidx][eidx].asmb;

	return 0;
}
