/*
 * m6500.cpp - MOS 6500 processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/m6500/m6502.h"
#include "dev/cpu/m6500/m6502op.h"

m6502_cpuDevice::m6502_cpuDevice(const SystemConfig &config, const DeviceType &type,
	const string &tagName, Device *owner, uint64_t clock, int addrWidth)
: cpuDevice(config, type, tagName, owner, clock),
  mapProgramConfig("program", LittleEndian, 8, 16, 8, addrWidth, 16, 0)
{
	// Initialize opcode table for disassembler
//	initOpcodeTable();
}

mapConfigList m6502_cpuDevice::getAddressConfigList() const
{
	return mapConfigList {
		{ aspace::asProgram, &mapProgramConfig },
	};
}

DEFINE_DEVICE_TYPE(m6502,  m6502_cpuDevice,  "m6502",  "MOS 6502")

// MOS 6502 processor (with 16-bit addressing)
m6502_cpuDevice::m6502_cpuDevice(const SystemConfig &config,
	const string &tagName, Device *owner, uint64_t clock)
: m6502_cpuDevice(config, m6502, tagName, owner, clock, 16)
{
}

// // MOS 6504 processor (with 13-bit addressing)
// m6504_cpuDevice::m6504_cpuDevice(const SystemConfig &config,
// 	const string &tagName, Device *owner, uint64_t clock)
// : m6502_cpuDevice(config, m6502, tagName, owner, clock, 13)
// {
// }

void m6502_cpuDevice::reset()
{
    // Clear all CPU registers
    aReg = 0;
    xReg = 0;
    yReg = 0;
    pReg = 0;
    spReg.uw = 0x0100;
    pcReg = 0;

}

void m6502_cpuDevice::startDevice()
{

    // Register CPU registers
    addState(m6502_A,  "A",  aReg);
    addState(m6502_X,  "X",  xReg);
    addState(m6502_Y,  "Y",  yReg);
    addState(m6502_P,  "P",  pReg);
    addState(m6502_S,  "SP", spReg.uw);
    addState(m6502_PC, "PC", pcReg);

    getAddressSpace(AS_PROGRAM)->setMemorySpecific(mapProgram);
}


void m6502_cpuDevice::step(Console *user)
{

}

void m6502_cpuDevice::run()
{

}

void m6502_cpuDevice::execute()
{

    uint8_t opCode = mapProgram.read8(pcReg, this);

    switch (opCode)
    {
        case 0x00: // BRK instruction
            DO_OPC(BRK, IMP);
            break;

        case 0x18: // CLC instruction
            DO_OPC(CLC, IMP);
            break;

        case 0xD8: // CLD instruction
            DO_OPC(CLD, IMP);
            break;
        
        case 0x58: // CLI instruction
            DO_OPC(CLI, IMP);
            break;

        case 0xB8: // CLV instruction
            DO_OPC(CLV, IMP);
            break;

        case 0x38: // SEC instruction
            DO_OPC(SEC, IMP);
            break;

        case 0xF8: // SED instruction
            DO_OPC(SED, IMP);
            break;

        case 0x78: // SEI instruction
            DO_OPC(SEI, IMP);
            break;

        case 0xAA: // TAX instruction
            DO_OPC(TAX, IMP);
            break;

        case 0xA8: // TAY instruction
            DO_OPC(TAY, IMP);
            break;

        case 0xBA: // TSX instruction
            DO_OPC(TSX, IMP);
            break;

        case 0x8A: // TXA instruction
            DO_OPC(TXA, IMP);
            break;

        case 0x9A: // TXS instruction
            DO_OPC(TXS, IMP);
            break;

        case 0x98: // TYA instruction
            DO_OPC(TYA, IMP);
            break;
    }
}