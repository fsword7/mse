/*
 * vax.cpp - DEC VAX processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/vax/vax.h"

vax_cpuDevice::vax_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int aWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 32, 16, 8, aWidth, 16, 0)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}

void vax_cpuDevice::startDevice()
{
	{
		addState(VAX_R0,  "R0",   state.gpReg[0].l);
		addState(VAX_R1,  "R1",   state.gpReg[1].l);
		addState(VAX_R2,  "R2",   state.gpReg[2].l);
		addState(VAX_R3,  "R3",   state.gpReg[3].l);
		addState(VAX_R4,  "R4",   state.gpReg[4].l);
		addState(VAX_R5,  "R5",   state.gpReg[5].l);
		addState(VAX_R6,  "R6",   state.gpReg[6].l);
		addState(VAX_R7,  "R7",   state.gpReg[7].l);
		addState(VAX_R8,  "R8",   state.gpReg[8].l);
		addState(VAX_R9,  "R9",   state.gpReg[9].l);
		addState(VAX_R10, "R10",  state.gpReg[10].l);
		addState(VAX_R11, "R11",  state.gpReg[11].l);
		addState(VAX_R12, "R12",  state.gpReg[12].l);
		addState(VAX_R13, "R13",  state.gpReg[13].l);
		addState(VAX_R14, "R14",  state.gpReg[14].l);
		addState(VAX_R15, "R15",  state.gpReg[15].l);

		addState(VAX_AP,  "AP",   state.gpReg[12].l);
		addState(VAX_FP,  "FP",   state.gpReg[13].l);
		addState(VAX_SP,  "SP",   state.gpReg[14].l);
		addState(VAX_PC,  "PC",   state.gpReg[15].l);

	}

	getAddressSpace(AS_PROGRAM)->setMemorySpecific(mapProgram);
}