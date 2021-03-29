/*
 * execute.cpp - MCS-48 microprocessor - execute routines
 *
 *  Created on: Mar 26, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/mcs48/mcs48.h"

void mcs48_cpuDevice::init()
{
	// Initialize all registers
	pcAddr  = 0;
	aReg    = 0;
	pswReg  = 0;
	f1Flag  = false;
	a11Addr = 0x000;
	pReg[0] = 0; // DBBI/DBBO
	pReg[1] = 0;
	pReg[2] = 0;
//	dbbiReg = 0;
//	dbboReg = 0;
//	tReg    = 0; // timer register
//	psReg   = 0; // prescaler register
//  tiHist  = 0; // T1 history

	// Reset all memory configurations
	mapProgram = getAddressSpace(AS_PROGRAM);
	mapData = getAddressSpace(AS_DATA);
	mapPort = getAddressSpace(AS_IOPORT);
}

void mcs48_cpuDevice::setPCAddress(offs_t addr)
{
	pcAddr = addr;
}

uint8_t mcs48_cpuDevice::fetchi()
{
	uint16_t addr = pcAddr;
	pcAddr = ((pcAddr + 1) & 0x7FF) | (pcAddr & 0x800);
	return mapProgram->read8(addr);
}

void mcs48_cpuDevice::updateRegisters()
{
	gReg = &idata[pswReg & PSW_RB ? 24 : 0];
}

void mcs48_cpuDevice::push_pc_psw()
{
	uint8_t sp = pswReg & PSW_STK;
	writed(8 + (sp*2), pcAddr);
	writed(9 + (sp*2), ((pcAddr >> 8) & 0x0F) | (pswReg & 0xF0));
	pswReg = (pswReg & 0xF0) | ((sp + 1) & PSW_STK);
}

void mcs48_cpuDevice::pull_pc_psw()
{
	uint8_t sp = (pswReg - 1) & PSW_STK;
	pcAddr  = readd(8 + (sp*2));
	pcAddr |= readd(9 + (sp*2)) << 8;
	pswReg = ((pcAddr >> 8) & 0xF0) | sp;
	pcAddr &= irqInProgress ? 0x7FF : 0xFFF;
	updateRegisters();
}

void mcs48_cpuDevice::pull_pc()
{
	uint8_t sp = (pswReg - 1) & PSW_STK;
	pcAddr  = readd(8 + (sp*2));
	pcAddr |= readd(9 + (sp*2)) << 8;
	pcAddr &= irqInProgress ? 0x7FF : 0xFFF;
}

void mcs48_cpuDevice::executeJcc(bool flag)
{
	uint16_t pch = pcAddr & 0xF00;
	uint8_t  offset = fetchi();
	if (flag == true)
		pcAddr = pch | offset;
}

void mcs48_cpuDevice::opADD(uint8_t val)
{
	uint16_t tmp8 = aReg + val;
	uint16_t tmp4 = (aReg & 0x0F) + (val & 0x0F);

	// Update condition status
	pswReg &= ~(PSW_CY|PSW_AC);
	pswReg |= (tmp4 << 2) & PSW_AC;
	pswReg |= (tmp8 >> 1) & PSW_CY;

	// Update A register as result
	aReg = tmp8;
}

void mcs48_cpuDevice::opADDC(uint8_t val)
{
	uint16_t carry = (pswReg & PSW_CY) >> 7;
	uint16_t tmp8 = aReg + val + carry;
	uint16_t tmp4 = (aReg & 0x0F) + (val & 0x0F) + carry;

	// Update condition status
	pswReg &= ~(PSW_CY|PSW_AC);
	pswReg |= (tmp4 << 2) & PSW_AC;
	pswReg |= (tmp8 >> 1) & PSW_CY;

	// Update A register as result
	aReg = tmp8;

}

void mcs48_cpuDevice::step(Console *user)
{
//	// Save current log flags and
//	// enable console output
//	uint32_t flags = dbg.getLogFlags();
//	dbg.setLogFlags(LOG_CONSOLE);
//
//	// Execute one instruction as single step
//	log = user;
//	execute();
//
//	// Restore log flags
//	dbg.loadLogFlags(flags);
}

void mcs48_cpuDevice::run()
{
//	// Start execution state
//	pState = execRunning;
//
//	try {
//		while(pState == execRunning)
//		{
//			execute();
//		}
//		dbg.flushAll();
//	}
//
//	catch (...)
//	{
//		// Flush all remaining buffers
//		dbg.flushAll();
//	}
//
//	// Stop execution state
//	pState = execStopped;
}

void mcs48_cpuDevice::execute()
{
	uint16_t newAddr;
	uint8_t tmp;

	// Fetch instruction from instruction stream
	opCode = fetchi();

	switch(opCode)
	{
	case 0x68: case 0x69: case 0x6A: case 0x6B:
	case 0x6C: case 0x6D: case 0x6E: case 0x6F:
		// ADD A,Rn instruction
		chargeCycles(1);
		opADD(RREG(opCode));
		return;

	case 0x60: case 0x61:
		// ADD A,@Rn instruction
		chargeCycles(1);
		opADD(readd(RREG(opCode)));
		return;

	case 0x03:
		// ADD A,#n instruction
		chargeCycles(2);
		opADD(fetchi());
		return;

	case 0x78: case 0x79: case 0x7A: case 0x7B:
	case 0x7C: case 0x7D: case 0x7E: case 0x7F:
		// ADDC A,Rn instruction
		chargeCycles(1);
		opADDC(RREG(opCode));
		return;

	case 0x70: case 0x71:
		// ADDC A,@Rn instruction
		chargeCycles(1);
		opADDC(readd(RREG(opCode)));
		return;

	case 0x13:
		// ADDC A,#n instruction
		chargeCycles(2);
		opADDC(fetchi());
		return;

	case 0x58: case 0x59: case 0x5A: case 0x5B:
	case 0x5C: case 0x5D: case 0x5E: case 0x5F:
		// ANL A,Rn instruction
		chargeCycles(1);
		aReg &= RREG(opCode);
		return;

	case 0x50: case 0x51:
		// ANL A,@Rn instruction
		chargeCycles(1);
		aReg &= readd(RREG(opCode));
		return;

	case 0x53:
		// ANL A,#n instruction
		chargeCycles(2);
		aReg &= fetchi();
		return;

	case 0x14: case 0x34: case 0x54: case 0x74:
	case 0x94: case 0xB4: case 0xD4: case 0xF4:
		// CALL instruction
		chargeCycles(2);
		push_pc_psw();
		newAddr = ((uint16_t(opCode) << 3) & 0x700) | fetchi();
		pcAddr  = (newAddr & 0x7FF) | (pcAddr & 0x800);
		return;

	case 0x27:
		// CLR A instruction
		chargeCycles(1);
		aReg = 0;
		return;

	case 0x97:
		// CLR C instruction
		chargeCycles(1);
		pswReg &= ~PSW_CY;
		return;

	case 0x85:
		// CLR F0 instruction
		chargeCycles(1);
		pswReg &= ~PSW_F0;
		return;

	case 0xA5:
		// CLR F1 instruction
		chargeCycles(1);
		f1Flag = false;
		return;

	case 0x37:
		// CPL A instruction
		chargeCycles(1);
		aReg ^= 0xFF;
		return;

	case 0xA7:
		// CPL C instruction
		chargeCycles(1);
		pswReg ^= PSW_CY;
		return;

	case 0x95:
		// CPL F0 instruction
		chargeCycles(1);
		pswReg ^= PSW_F0;
		return;

	case 0xB5:
		// CPL F1 instruction
		chargeCycles(1);
		f1Flag = !f1Flag;
		return;

	case 0x57:
		// DA instruction
		chargeCycles(1);
		if (((aReg & 0x0F) > 0x09) || (pswReg & PSW_AC))
		{
			if (aReg > 0xF9)
				pswReg |= PSW_CY;
			aReg += 0x06;
		}
		if (((aReg & 0xF0) > 0x90) || (pswReg & PSW_CY))
		{
			aReg += 0x60;
			pswReg |= PSW_CY;
		}
		return;

	case 0x07:
		// DEC A instruction
		chargeCycles(1);
		aReg--;
		return;

	case 0xC8: case 0xC9: case 0xCA: case 0xCB:
	case 0xCC: case 0xCD: case 0xCE: case 0xCF:
		// DEC Rn instruction
		chargeCycles(1);
		RREG(opCode)--;
		return;

	case 0x15:
		// DIS I instruction
		chargeCycles(1);
		xirqEnable = false;
		return;

	case 0x35:
		// DIS TCNTI instruction
		tirqEnable = false;
		return;

	case 0xE8: case 0xE9: case 0xEA: case 0xEB:
	case 0xEC: case 0xED: case 0xEE: case 0xEF:
		// DJNZ Rn,address instruction
		chargeCycles(2);
		executeJcc(--RREG(opCode) != 0);
		return;

	case 0x05:
		// EN I instruction
		chargeCycles(1);
		xirqEnable = true;
		return;

	case 0x25:
		// EN TCNTI instruction
		tirqEnable = true;
		return;

	case 0x08:
		// INS A,BUS instruction
		chargeCycles(2);
//		aReg = readb();
		return;

	case 0x09:
		// IN A,P1 instruction
		chargeCycles(2);
//		aReg = readio(1);
		return;

	case 0x0A:
		// IN A,P2 instruction
		chargeCycles(2);
//		aReg = readio(2);
		return;

	case 0x17:
		// INC A instruction
		chargeCycles(1);
		aReg++;
		return;

	case 0x18: case 0x19: case 0x1A: case 0x1B:
	case 0x1C: case 0x1D: case 0x1E: case 0x1F:
		// INC Rn instruction
		chargeCycles(1);
		RREG(opCode)++;
		return;

	case 0x10: case 0x11:
		// INC @Rn instruction
		chargeCycles(1);
		writed(RREG(opCode), readd(RREG(opCode)) + 1);
		return;

	case 0x12: case 0x32: case 0x52: case 0x72:
	case 0x92: case 0xB2: case 0xD2: case 0xE2:
		// JBx address instruction
		chargeCycles(2);
		executeJcc(aReg & (1u << (opCode >> 5)));
		return;

	case 0xF6:
		// JC address instruction
		chargeCycles(2);
		executeJcc((pswReg & PSW_CY) != 0);
		return;

	case 0xB6:
		// JF0 address instruction
		chargeCycles(2);
		executeJcc((pswReg & PSW_F0) != 0);
		return;

	case 0x76:
		// JF1 address instruction
		chargeCycles(2);
		executeJcc(f1Flag);
		return;

	case 0xE6:
		// JNC address instruction
		chargeCycles(2);
		executeJcc((pswReg & PSW_CY) == 0);
		return;

	case 0x96:
		// JNZ address instruction
		chargeCycles(2);
		executeJcc(aReg != 0);
		return;

	case 0xC6:
		// JZ address instruction
		chargeCycles(2);
		executeJcc(aReg == 0);
		return;

	case 0x04: case 0x24: case 0x44: case 0x64:
	case 0x84: case 0xA4: case 0xC4: case 0xE4:
		// JMP address instruction
		chargeCycles(2);
		newAddr = ((uint16_t(opCode) << 3) & 0x700) | fetchi();
		pcAddr  = (newAddr & 0x7FF) | (pcAddr & 0x800);
		return;

	case 0xB3:
		// JMPP @A instruction
		chargeCycles(2);
		pcAddr &= 0xF00;
		pcAddr |= readp(pcAddr | aReg);
		return;

	case 0xF8: case 0xF9: case 0xFA: case 0xFB:
	case 0xFC: case 0xFD: case 0xFE: case 0xFF:
		// MOV A,Rn instruction
		chargeCycles(1);
		aReg = RREG(opCode);
		return;

	case 0xF0: case 0xF1:
		// MOV A,@Rn instruction
		chargeCycles(1);
		aReg = readd(RREG(opCode));
		return;

	case 0x23:
		// MOV A,#n instruction
		chargeCycles(2);
		aReg = fetchi();
		return;

	case 0xC7:
		// MOV A,PSW instruction
		chargeCycles(1);
		aReg = pswReg | PSW_MBO;
		return;

	case 0x42:
		// MOV A,T instruction
		chargeCycles(1);
		aReg = tReg;
		return;

	case 0xA8: case 0xA9: case 0xAA: case 0xAB:
	case 0xAC: case 0xAD: case 0xAE: case 0xAF:
		// MOV Rn,A instruction
		chargeCycles(1);
		RREG(opCode) = aReg;
		return;

	case 0xB8: case 0xB9: case 0xBA: case 0xBB:
	case 0xBC: case 0xBD: case 0xBE: case 0xBF:
		// MOV Rn,#m instruction
		chargeCycles(1);
		RREG(opCode) = fetchi();
		return;

	case 0xA0: case 0xA1:
		// MOV @Rn,A instruction
		chargeCycles(1);
		writed(RREG(opCode), aReg);
		return;

	case 0xB0: case 0xB1:
		// MOV @Rn,#n instruction
		chargeCycles(1);
		writed(RREG(opCode), fetchi());
		return;

	case 0xD7:
		// MOV PSW,A instruction
		chargeCycles(1);
		pswReg = aReg & ~PSW_MBO;
		updateRegisters();
		return;

	case 0xA3:
		// MOVP A,@A instruction
		chargeCycles(2);
		aReg = readp((pcAddr & 0xF00) | aReg);
		return;

	case 0xE3:
		// MOVP3 A,@A instruction
		chargeCycles(2);
		aReg = readp(0x300 | aReg);
		return;

	case 0x62:
		// MOV T,A instruction
		chargeCycles(1);
		tReg = aReg;
		return;


	case 0x00:
		// NOP instruction
		chargeCycles(1);
		// Do nothiing
		return;

	case 0x48: case 0x49: case 0x4A: case 0x4B:
	case 0x4C: case 0x4D: case 0x4E: case 0x4F:
		// ORL A,Rn instruction
		chargeCycles(1);
		aReg |= RREG(opCode);
		return;

	case 0x40: case 0x41:
		// ORL A,@Rn instruction
		chargeCycles(1);
		aReg |= readd(RREG(opCode));
		return;

	case 0x43:
		// ORL A,#n instruction
		chargeCycles(2);
		aReg |= fetchi();
		return;

	case 0x02:
		// OUTL BUS,A instruction
		chargeCycles(2);
		writeb(aReg);
		return;

	case 0x39:
		// OUTL P1,A instruction
		chargeCycles(2);
		writeio(1, aReg);
		return;

	case 0x3A:
		// OUTL P2,A instruction
		chargeCycles(2);
		writeio(2, aReg);
		return;

	case 0x80:
		// RAD instruction
		chargeCycles(2);
		return;

	case 0x83:
		// RET instruction
		chargeCycles(2);
		pull_pc();
		return;

	case 0x93:
		// RETI/RETR instruction
		chargeCycles(2);
		irqInProgress = false;
		pull_pc_psw();
		return;

	case 0xE7:
		// RL A instruction
		chargeCycles(1);
		aReg = (aReg << 1) | (aReg >> 7);
		return;

	case 0xF7:
		// RLC A instruction
		chargeCycles(1);
		tmp = aReg & PSW_CY;
		aReg = (aReg << 1) | (pswReg >> 7);
		pswReg = (pswReg & ~PSW_CY) | tmp;
		return;

	case 0x77:
		// RR A instruction
		chargeCycles(1);
		aReg = (aReg >> 1) | (aReg << 7);
		return;

	case 0x67:
		// RRC A instruction
		chargeCycles(1);
		tmp = (aReg << 7) & PSW_CY;
		aReg = (aReg >> 1) | (pswReg & PSW_CY);
		pswReg = (pswReg & ~PSW_CY) | tmp;
		return;

	case 0xE5:
		// SEL MB0 instruction
		chargeCycles(1);
		a11Addr = 0x000;
		return;

	case 0xF5:
		// SEL MB1 instruction
		chargeCycles(1);
		a11Addr = 0x800;
		return;

	case 0xC5:
		// SEL RB0 instruction
		chargeCycles(1);
		pswReg &= ~PSW_RB;
		updateRegisters();
		return;

	case 0xD5:
		// SEL RB1 instruction
		chargeCycles(1);
		pswReg |= PSW_RB;
		updateRegisters();
		return;

	case 0x47:
		// SWAP A instruction
		chargeCycles(1);
		aReg = (aReg >> 4) | (aReg << 4);
		return;

	case 0x28: case 0x29: case 0x2A: case 0x2B:
	case 0x2C: case 0x2D: case 0x2E: case 0x2F:
		// XCH A,Rn instruction
		chargeCycles(1);
		tmp = aReg;
		aReg = RREG(opCode);
		RREG(opCode) = tmp;
		return;

	case 0x20: case 0x21:
		// XCH A,@Rn instruction
		chargeCycles(1);
		tmp = aReg;
		aReg = readd(RREG(opCode));
		writed(RREG(opCode), tmp);
		return;

	case 0x30: case 0x31:
		// XCHD A,@Rn instruction
		chargeCycles(1);
		tmp = readd(RREG(opCode));
		writed(RREG(opCode), (tmp & 0xF0) | (aReg & 0x0F));
		aReg = (aReg & 0xF0) | (tmp & 0x0F);
		return;

	case 0xD8: case 0xD9: case 0xDA: case 0xDB:
	case 0xDC: case 0xDD: case 0xDE: case 0xDF:
		// XRL A,Rn instruction
		chargeCycles(1);
		aReg ^= RREG(opCode);
		return;

	case 0xD0: case 0xD1:
		// XRL A,@Rn instruction
		chargeCycles(1);
		aReg ^= readd(RREG(opCode));
		return;

	case 0xD3:
		// XRL A,#n instruction
		chargeCycles(2);
		aReg ^= fetchi();
		return;

	default:
		dbg.log("*** Unimplemented opcode = %02X\n", opCode);
		return;
	}
}
