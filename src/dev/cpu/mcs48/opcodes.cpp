/*
 * opcodes.cpp - MCS 48 instruction table package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/mcs48/mcs48.h"

#define INST(opName, opReg, opCode, opMask, cycle) \
	{ opName, opReg, opCode, opMask, cycle }

mcs48op_t mcs48_cpuDevice::opTable[] =
{
	INST("ADD",	  "A,R%d",     0x68, 0x07, 1),
	INST("ADD",   "A,@R%d",    0x60, 0x01, 1),
	INST("ADD",   "A,#%02X",   0x03, 0x00, 2),
	INST("ADDC",  "A,R%d",     0x78, 0x07, 1),
	INST("ADDC",  "A,@R%d",    0x70, 0x01, 1),
	INST("ADDC",  "A,#%02X",   0x13, 0x00, 2),
	INST("ANL",   "A,R%d",     0x58, 0x07, 1),
	INST("ANL",   "A,@R%d",    0x50, 0x01, 1),
	INST("ANL",   "A,#%02X",   0x53, 0x00, 2),
	INST("ANL",   "BUS,#%02X", 0x98, 0x00, 2),
	INST("ANL",   "P%d,#%02X", 0x98, 0x03, 2),
	INST("ANLD",  "P%d,A",     0x9C, 0x03, 2),
	INST("CALL",  "%03X",      0x14, 0xE0, 2),
	INST("CLR",   "A",         0x27, 0x00, 1),
	INST("CLR",   "C",         0x97, 0x00, 1),
	INST("CLR",   "F0",        0x85, 0x00, 1),
	INST("CLR",   "F1",        0xA5, 0x00, 1),
	INST("CPL",   "A",         0x37, 0x00, 1),
	INST("CPL",   "C",         0xA7, 0x00, 1),
	INST("CPL",   "F0",        0x95, 0x00, 1),
	INST("CPL",   "F1",        0xB5, 0x00, 1),
	INST("DA",    "A",         0x57, 0x00, 1),
	INST("DEC",   "A",         0x07, 0x00, 1),
	INST("DEC",   "R%d",       0xC8, 0x07, 1),
	INST("DIS",   "I",         0x15, 0x00, 1),
	INST("DIS",   "TCNTI",     0x35, 0x00, 1),
	INST("DJNZ",  "R%d,%03X",  0xE8, 0x07, 2),
	INST("EN",    "I",         0x05, 0x00, 1),
	INST("EN",    "TCNTI",     0x25, 0x00, 1),
	INST("ENT0",  "CLK",       0x75, 0x00, 1),
	INST("IN",    "A,P%d",     0x08, 0x03, 2),
	INST("INC",   "A",         0x17, 0x00, 1),
	INST("INC",   "R%d",       0x18, 0x07, 1),
	INST("INC",   "@R%d",      0x10, 0x01, 1),
	INST("IN",    "A,P0",      0x08, 0x00, 2), // 8021/8022
	INST("INS",   "A,BUS",     0x08, 0x00, 2),
	INST("JBb",   "%03X",      0x12, 0xE0, 2), // Not 8021/8022
	INST("JC",    "%03X",      0xF6, 0x00, 2),
	INST("JF0",   "%03X",      0xB6, 0x00, 2), // Not 8021/8022
	INST("JF1",   "%03X",      0x76, 0x00, 2), // Not 8021/8022
	INST("JMP",   "%03X",      0x40, 0xE0, 2),
	INST("JMPP",  "@A",        0xB3, 0x00, 2),
	INST("JNC",   "%03X",      0xE6, 0x00, 2),
	INST("JNI",   "%03X",      0x86, 0x00, 2), // Not 8021/8022
	INST("JNT0",  "%03X",      0x26, 0x00, 2), // Not 8021/8022
	INST("JNT1",  "503X",      0x46, 0x00, 2),
	INST("JNZ",   "%03X",      0x96, 0x00, 2),
	INST("JTF",   "%03X",      0x16, 0x00, 2),
	INST("JT0",   "%03X",      0x36, 0x00, 2), // Not 8021/8022
	INST("JT1",   "%03X",      0x56, 0x00, 2),
	INST("JZ",    "%03X",      0xC6, 0x00, 2),
	INST("MOV",   "A,#%02X",   0x23, 0x00, 2),
	INST("MOV",   "A,PSW",     0xC7, 0x00, 1),
	INST("MOV",   "A,R%d",     0xF8, 0x07, 1),
	INST("MOV",   "A,T",       0x42, 0x00, 1),
	INST("MOV",   "PSW,A",     0xD7, 0x00, 1),
	INST("MOV",   "R%d,A",     0xA8, 0x07, 1),
	INST("MOV",   "R%d,#%02X", 0xB8, 0x07, 2),
	INST("MOV",   "@R%d,A",    0xA0, 0x01, 1),
	INST("MOV",   "@%d,#%02X", 0xB0, 0x01, 2),
	INST("MOV",   "T,A",       0x62, 0x00, 1),
	INST("MOV",   "A,P%d",     0x0C, 0x03, 2),
	INST("MOVD",  "P%d,A",     0x3C, 0x03, 2),
	INST("MOVP",  "A,@A",      0xA3, 0x00, 2),
	INST("MOVP3", "A,@A",      0xE3, 0x00, 2),
	INST("MOVX",  "A,@R%d",    0x80, 0x01, 2),
	INST("MOVX",  "@R%d,A",    0x90, 0x01, 2),
	INST("NOP",   "",          0x00, 0x00, 1),
	INST("ORL",   "A,R%d",     0x48, 0x07, 1),
	INST("ORL",   "A,@R%d",    0x40, 0x01, 1),
	INST("ORL",   "A,%02X",    0x43, 0x00, 2),
	INST("ORL",   "BUS,#%02X", 0x88, 0x00, 2), // Not 8021/8022
	INST("ORL",   "P%d,#%02X", 0x88, 0x03, 2),
	INST("ORLD",  "P%d,A",     0x8C, 0x03, 2),
	INST("OUTL",  "P0,A",      0x90, 0x00, 1), // Not 8021/8022
	INST("OUTL",  "BUS,A",     0x02, 0x00, 2),
	INST("OUTL",  "P%d,A",     0x38, 0x03, 2),
	INST("RAD",   "",          0x80, 0x00, 2),
	INST("RET",   "",          0x83, 0x00, 2),
	INST("RETI",  "",          0x93, 0x00, 2), // Only 8022
	INST("RETR",  "",          0x93, 0x00, 2), // Not 8021/8022
	INST("RL",    "A",         0xE7, 0x00, 1),
	INST("RLC",   "A",         0xF7, 0x00, 1),
	INST("RR",    "A",         0x77, 0x00, 1),
	INST("RRC",   "A",         0x67, 0x00, 1),
	INST("SEL",   "AN0",       0x95, 0x00, 1), // Only 8022
	INST("SEL",   "AN1",       0x85, 0x00, 1), // Only 8022
	INST("SEL",   "MB0",       0xE5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "MB1",       0xF5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "RB0",       0xC5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "RB1",       0xD5, 0x00, 1), // Not 8021/8022
	INST("STOP",  "TCNT",      0x65, 0x00, 1),
	INST("STRT",  "CNT",       0x45, 0x00, 1),
	INST("STRT",  "T",         0x55, 0x00, 1),
	INST("SWAP",  "A",         0x47, 0x00, 1),
	INST("XCH",   "A,R%d",     0x28, 0x07, 1),
	INST("XCH",   "A,@R%d",    0x20, 0x01, 1),
	INST("XCHD",  "A,@R%d",    0x30, 0x01, 1),
	INST("XRL",   "A,R%d",     0xD8, 0x07, 1),
	INST("XRL",   "A,@R%d",    0xD0, 0x01, 1),
	INST("XRL",   "A,#%02X",   0xD3, 0x00, 2),

	// Null terminator
	nullptr
};
