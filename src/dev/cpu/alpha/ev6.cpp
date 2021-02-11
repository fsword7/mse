/*
 * ev6.cpp - DEC 21264 Alpha processor series (EV6 series)
 *
 *  Created on: Dec 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev6.h"
#include "dev/cpu/alpha/axp_mmu.h"

DEFINE_DEVICE_TYPE(dec21264, dec21264_cpuDevice, "21264", "DEC 21264 Alpha Processor")

dec21264_cpuDevice::dec21264_cpuDevice(const SystemConfig &config,
		cstag_t &tagName, Device *owner, uint64_t clock)
: AlphaProcessor(config, dec21264, tagName, owner, clock, 44)
{
	setArchType(ARCH_EV6);
	setArchFlags(ARCH_PFM|ARCH_PRC|ARCH_MVI|ARCH_CIX|ARCH_BWX);
}

void dec21264_cpuDevice::preset()
{
	// Initialize Ibox control register
	ev6.ictl.spce = 0;
	ev6.ictl.ic_en = 3;
	ev6.ictl.spe = 0;
	ev6.ictl.sde = 0;
	ev6.ictl.sbe = 0;
	ev6.ictl.bpMode = 0;
	ev6.ictl.hwe = 0;
	ev6.ictl.sl_xmit = 0;
	ev6.ictl.sl_rcv = 0;
	ev6.ictl.va48 = 0;
	ev6.ictl.vaForm32 = 0;
	ev6.ictl.single_issue_h = 0;
	ev6.ictl.pct0_en = 0;
	ev6.ictl.pct1_en = 0;
	ev6.ictl.call_pal_r23 = 0;
	ev6.ictl.mchk_en = 0;
	ev6.ictl.tb_mb_en = 0;
	ev6.ictl.bist_fail = 1;
	ev6.ictl.chip_id = 5;
	ev6.ictl.vptb = 0;
	ev6.ictl.sext_vptb = 0;
}

int dec21264_cpuDevice::abort(int why)
{
	switch (why)
	{
	case EXC_OPCODE:   // Illegal opcode
		break;
	case EXC_ITBMISS:  // Istream TB miss
		break;
	case EXC_DTBMISS1: // Dstream TB miss single
		break;
	case EXC_DTBMISS2: // Dstream TB miss double with VPTE
		break;
	case EXC_IACV:     // Istream access violation
		break;
	case EXC_FPDIS:    // Floating-point operation attempted
		break;
	}

	// Always return error code
	return -1;
}

int dec21264_cpuDevice::translate(uint64_t vAddr, uint64_t &pAddr, bool &asmb, int accFlags)
{
	int spe = (accFlags & ACC_EXEC) ? state.ispe : state.mspe;
	int cm  = (accFlags & ACC_ALTCM) ? state.altcm : state.cm;

	// Try super page translation first
	if (spe & cm != ACC_KERNEL)
	{
		if (((vAddr & SPE2_MASK) == SPE2_MATCH) && (spe & 4))
		{
			asmb  = false;
			pAddr = vAddr & SPE2_MAP;
			return 0;
		}
		else if (((vAddr & SPE1_MASK) == SPE1_MATCH) && (spe & 2))
		{
			asmb  = false;
			pAddr = (vAddr & SPE1_MAP) | (vAddr & SPE1_TEST) ? SPE1_ADD : 0;
			return 0;
		}
		else if (((vAddr & SPE0_MASK) == SPE0_MATCH) && (spe & 1))
		{
			asmb  = false;
			pAddr = vAddr & SPE0_MAP;
			return 0;
		}
	}

	int eidx;
	if ((eidx = findTBEntry(vAddr, accFlags)) < 0)
	{
//		// TB miss (page fault) - issue PAL exception
//		if (accFlags & ACC_VPTE)
//		{
//			state.fpcAddr = vAddr;
//			state.sum = RA << 8;
//			setPC(state.palBase + PAL_DTBM_DOUBLE_3 + 1);
//		}
//		else if (accFlags & ACC_EXECUTE)
//		{
//			setPC(state.palBase + PAL_ITB_MISS + 1);
//		}
//		else
//		{
//			state.fpcAddr = vAddr;
//			state.sum = RA << 8;
//			int opCode = OP_GETOP(state.opWord);
//			state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 4 |
//					(accFlags & ACC_WRITE);
//			setPC(state.palBase + PAL_DTBM_SINGLE + 1);
//		}

		return -1;
	}

	// Got TB entry now, check access flags for any access violation
	int tbidx = (accFlags & ACC_EXEC) ? 1 : 0;

	{
		if (!state.tb[tbidx][eidx].access[accFlags & ACC_WRITE][cm])
		{
//			if (accFlags & ACC_EXEC)
//			{
//				state.excAddr = state.pcAddr;
//				state.excSum = 0;
//				setPC(state.palBase + PAL_IACV + 1);
//			}
//			else
//			{
//				state.excAddr = state.pcAddr;
//				state.fpcAddr = vAddr;
//				state.excSum = RA << 8;
//
//				uint32_t opCode = OP_GETOP(state.opWord);
//				state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 4 |
//					(accFlags & ACC_WRITE) | 2;
//				setPC(state.palBase + PAL_DFAULT + 1);
//			}
			return -1;
		}

		if (state.tb[tbidx][eidx].fault[accFlags & ACC_MODE])
		{
//			if (accFlags & ACC_EXEC)
//			{
//				state.excAddr = state.pcAddr;
//				state.excSum = 0;
//				setPC(state.palBase + PAL_IACV + 1);
//			}
//			else
//			{
//				state.excAddr = state.pcAddr;
//				state.fpcAddr = vAddr;
//				state.excSum = RA << 8;
//
//				uint32_t opCode = OP_GETOP(state.opWord);
//				state.mmstat = (((opCode == 0x1B) || (opCode == 0x1F)) ? opCode - 0x18 : opCode) << 4 |
//					(accFlags & ACC_WRITE) | ((accFlags & ACC_WRITE) ? 8 : 4);
//				setPC(state.palBase + PAL_DFAULT + 1);
//			}
			return -1;
		}
	}

	// Finally no access violations or faults
	// Now convert to physical address
	pAddr = state.tb[tbidx][eidx].pAddr | (vAddr & state.tb[tbidx][eidx].keepMask);
	asmb  = state.tb[tbidx][eidx].asmb;

	return 0;
}
