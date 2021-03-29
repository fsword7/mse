/*
 * opcodes.cpp - MCS 48 instruction table package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/mcs48/mcs48.h"

#define INST(opName, opReg, opType, opCode, opMask, cycle) \
	{ opName, opReg, opType, opCode, opMask, cycle }

mcs48op_t mcs48_cpuDevice::opTable[] =
{
	INST("ADD",	  "A,R%d",      OPR_REG,           0x68, 0x07, 1),
	INST("ADD",   "A,@R%d",     OPR_REG,           0x60, 0x01, 1),
	INST("ADD",   "A,#%02X",    OPR_LIT,           0x03, 0x00, 2),
	INST("ADDC",  "A,R%d",      OPR_REG,           0x78, 0x07, 1),
	INST("ADDC",  "A,@R%d",     OPR_REG,           0x70, 0x01, 1),
	INST("ADDC",  "A,#%02X",    OPR_LIT,           0x13, 0x00, 2),
	INST("ANL",   "A,R%d",      OPR_REG,           0x58, 0x07, 1),
	INST("ANL",   "A,@R%d",     OPR_REG,           0x50, 0x01, 1),
	INST("ANL",   "A,#%02X",    OPR_LIT,           0x53, 0x00, 2),
	INST("ANL",   "P%d,#%02X",  OPR_REG|OPR_LIT,   0x98, 0x03, 2),
	INST("ANL",   "BUS,#%02X",  OPR_LIT,           0x98, 0x00, 2),
	INST("ANLD",  "P%d,A",      OPR_REG,           0x9C, 0x03, 2),
	INST("CALL",  "%03X",       OPR_ADDR3,         0x14, 0xE0, 2),
	INST("CLR",   "A",          OPR_NOPE,          0x27, 0x00, 1),
	INST("CLR",   "C",          OPR_NOPE,          0x97, 0x00, 1),
	INST("CLR",   "F0",         OPR_NOPE,          0x85, 0x00, 1),
	INST("CLR",   "F1",         OPR_NOPE,          0xA5, 0x00, 1),
	INST("CPL",   "A",          OPR_NOPE,          0x37, 0x00, 1),
	INST("CPL",   "C",          OPR_NOPE,          0xA7, 0x00, 1),
	INST("CPL",   "F0",         OPR_NOPE,          0x95, 0x00, 1),
	INST("CPL",   "F1",         OPR_NOPE,          0xB5, 0x00, 1),
	INST("DA",    "A",          OPR_NOPE,          0x57, 0x00, 1),
	INST("DEC",   "A",          OPR_NOPE,          0x07, 0x00, 1),
	INST("DEC",   "R%d",        OPR_REG,           0xC8, 0x07, 1),
	INST("DIS",   "I",          OPR_NOPE,          0x15, 0x00, 1),
	INST("DIS",   "TCNTI",      OPR_NOPE,          0x35, 0x00, 1),
	INST("DJNZ",  "R%d,%03X",   OPR_REG|OPR_ADDR2, 0xE8, 0x07, 2),
	INST("EN",    "I",          OPR_NOPE,          0x05, 0x00, 1),
	INST("EN",    "TCNTI",      OPR_NOPE,          0x25, 0x00, 1),
	INST("ENT0",  "CLK",        OPR_NOPE,          0x75, 0x00, 1),
	INST("IN",    "A,P%d",      OPR_REG,           0x08, 0x03, 2),
	INST("INC",   "A",          OPR_NOPE,          0x17, 0x00, 1),
	INST("INC",   "R%d",        OPR_REG,           0x18, 0x07, 1),
	INST("INC",   "@R%d",       OPR_REG,           0x10, 0x01, 1),
	INST("IN",    "A,P0",       OPR_NOPE,          0x08, 0x00, 2), // 8021/8022
	INST("INS",   "A,BUS",      OPR_NOPE,          0x08, 0x00, 2),
	INST("JB0",   "%03X",       OPR_ADDR2,         0x12, 0x00, 2), // Not 8021/8022
	INST("JB1",   "%03X",       OPR_ADDR2,         0x32, 0x00, 2), // Not 8021/8022
	INST("JB2",   "%03X",       OPR_ADDR2,         0x52, 0x00, 2), // Not 8021/8022
	INST("JB3",   "%03X",       OPR_ADDR2,         0x72, 0x00, 2), // Not 8021/8022
	INST("JB4",   "%03X",       OPR_ADDR2,         0x92, 0x00, 2), // Not 8021/8022
	INST("JB5",   "%03X",       OPR_ADDR2,         0xB2, 0x00, 2), // Not 8021/8022
	INST("JB6",   "%03X",       OPR_ADDR2,         0xD2, 0x00, 2), // Not 8021/8022
	INST("JB7",   "%03X",       OPR_ADDR2,         0xF2, 0x00, 2), // Not 8021/8022
	INST("JC",    "%03X",       OPR_ADDR2,         0xF6, 0x00, 2),
	INST("JF0",   "%03X",       OPR_ADDR2,         0xB6, 0x00, 2), // Not 8021/8022
	INST("JF1",   "%03X",       OPR_ADDR2,         0x76, 0x00, 2), // Not 8021/8022
	INST("JMP",   "%03X",       OPR_ADDR3,         0x04, 0xE0, 2),
	INST("JMPP",  "@A",         OPR_NOPE,          0xB3, 0x00, 2),
	INST("JNC",   "%03X",       OPR_ADDR2,         0xE6, 0x00, 2),
	INST("JNI",   "%03X",       OPR_ADDR2,         0x86, 0x00, 2), // Not 8021/8022
	INST("JNT0",  "%03X",       OPR_ADDR2,         0x26, 0x00, 2), // Not 8021/8022
	INST("JNT1",  "%03X",       OPR_ADDR2,         0x46, 0x00, 2),
	INST("JNZ",   "%03X",       OPR_ADDR2,         0x96, 0x00, 2),
	INST("JTF",   "%03X",       OPR_ADDR2,         0x16, 0x00, 2),
	INST("JT0",   "%03X",       OPR_ADDR2,         0x36, 0x00, 2), // Not 8021/8022
	INST("JT1",   "%03X",       OPR_ADDR2,         0x56, 0x00, 2),
	INST("JZ",    "%03X",       OPR_ADDR2,         0xC6, 0x00, 2),
	INST("MOV",   "A,#%02X",    OPR_LIT,           0x23, 0x00, 2),
	INST("MOV",   "A,PSW",      OPR_NOPE,          0xC7, 0x00, 1),
	INST("MOV",   "A,R%d",      OPR_REG,           0xF8, 0x07, 1),
	INST("MOV",   "A,@R%d",     OPR_REG,           0xF0, 0x01, 1),
	INST("MOV",   "A,T",        OPR_NOPE,          0x42, 0x00, 1),
	INST("MOV",   "PSW,A",      OPR_NOPE,          0xD7, 0x00, 1),
	INST("MOV",   "R%d,A",      OPR_REG,           0xA8, 0x07, 1),
	INST("MOV",   "R%d,#%02X",  OPR_REG|OPR_LIT,   0xB8, 0x07, 2),
	INST("MOV",   "@R%d,A",     OPR_REG,           0xA0, 0x01, 1),
	INST("MOV",   "@R%d,#%02X", OPR_REG|OPR_LIT,   0xB0, 0x01, 2),
	INST("MOV",   "T,A",        OPR_NOPE,          0x62, 0x00, 1),
	INST("MOV",   "A,P%d",      OPR_REG,           0x0C, 0x03, 2),
	INST("MOVD",  "P%d,A",      OPR_REG,           0x3C, 0x03, 2),
	INST("MOVP",  "A,@A",       OPR_NOPE,          0xA3, 0x00, 2),
	INST("MOVP3", "A,@A",       OPR_NOPE,          0xE3, 0x00, 2),
	INST("MOVX",  "A,@R%d",     OPR_REG,           0x80, 0x01, 2),
	INST("MOVX",  "@R%d,A",     OPR_REG,           0x90, 0x01, 2),
	INST("NOP",   "",           OPR_NOPE,          0x00, 0x00, 1),
	INST("ORL",   "A,R%d",      OPR_REG,           0x48, 0x07, 1),
	INST("ORL",   "A,@R%d",     OPR_REG,           0x40, 0x01, 1),
	INST("ORL",   "A,#%02X",    OPR_LIT,           0x43, 0x00, 2),
	INST("ORL",   "BUS,#%02X",  OPR_LIT,           0x88, 0x00, 2), // Not 8021/8022
	INST("ORL",   "P%d,#%02X",  OPR_REG|OPR_LIT,   0x88, 0x03, 2),
	INST("ORLD",  "P%d,A",      OPR_REG,           0x8C, 0x03, 2),
	INST("OUTL",  "P0,A",       OPR_NOPE,          0x90, 0x00, 1), // Not 8021/8022
	INST("OUTL",  "BUS,A",      OPR_NOPE,          0x02, 0x00, 2),
	INST("OUTL",  "P%d,A",      OPR_REG,           0x38, 0x03, 2),
	INST("RAD",   "",           OPR_NOPE,          0x80, 0x00, 2),
	INST("RET",   "",           OPR_NOPE,          0x83, 0x00, 2),
	INST("RETI",  "",           OPR_NOPE,          0x93, 0x00, 2), // Only 8022
	INST("RETR",  "",           OPR_NOPE,          0x93, 0x00, 2), // Not 8021/8022
	INST("RL",    "A",          OPR_NOPE,          0xE7, 0x00, 1),
	INST("RLC",   "A",          OPR_NOPE,          0xF7, 0x00, 1),
	INST("RR",    "A",          OPR_NOPE,          0x77, 0x00, 1),
	INST("RRC",   "A",          OPR_NOPE,          0x67, 0x00, 1),
	INST("SEL",   "AN0",        OPR_NOPE,          0x95, 0x00, 1), // Only 8022
	INST("SEL",   "AN1",        OPR_NOPE,          0x85, 0x00, 1), // Only 8022
	INST("SEL",   "MB0",        OPR_NOPE,          0xE5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "MB1",        OPR_NOPE,          0xF5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "RB0",        OPR_NOPE,          0xC5, 0x00, 1), // Not 8021/8022
	INST("SEL",   "RB1",        OPR_NOPE,          0xD5, 0x00, 1), // Not 8021/8022
	INST("STOP",  "TCNT",       OPR_NOPE,          0x65, 0x00, 1),
	INST("STRT",  "CNT",        OPR_NOPE,          0x45, 0x00, 1),
	INST("STRT",  "T",          OPR_NOPE,          0x55, 0x00, 1),
	INST("SWAP",  "A",          OPR_NOPE,          0x47, 0x00, 1),
	INST("XCH",   "A,R%d",      OPR_REG,           0x28, 0x07, 1),
	INST("XCH",   "A,@R%d",     OPR_REG,           0x20, 0x01, 1),
	INST("XCHD",  "A,@R%d",     OPR_REG,           0x30, 0x01, 1),
	INST("XRL",   "A,R%d",      OPR_REG,           0xD8, 0x07, 1),
	INST("XRL",   "A,@R%d",     OPR_REG,           0xD0, 0x01, 1),
	INST("XRL",   "A,#%02X",    OPR_LIT,           0xD3, 0x00, 2),

	// Null terminator
	nullptr
};
