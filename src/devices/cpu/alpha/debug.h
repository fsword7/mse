/*
 * debug.h - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

#ifdef DEBUG

#define UNKNOWN_OPCODE1 \
	dbg.log("*** Unknown opcode %02X\n", opCode);

#define UNKNOWN_OPCODE2 \
	dbg.log("*** Unknown opcode %02X.%02X\n", opCode, func);

#define POST_X8(VAL)  dbg.log(" ==> %02X\n", ZXTB(val))
#define POST_X16(val) dbg.log(" ==> %04X\n", ZXTW(val))
#define POST_X32(val) dbg.log(" ==> %08X\n", ZXTL(val))
#define POST_X64(val) dbg.log(" ==> %016llX\n", ZXTQ(val))


// Condition instructions
#define PRE_COND(opcode)                                \
	dbgval = (state.vpcReg + (DISP21 << 2)) & ~0x3;     \
	dbg.log("%016llx %-8s r%d, %016llX : (%016llX)\n",  \
		state.cpcAddr, #opcode, RA, dbgval, RAV);

#define POST_COND

// Memory instructions
#define PRE_MEM(opcode) 				                \
	dbg.log("%016llX %-8s r%d, %04xH(r%d)",             \
		state.cpcAddr, #opcode, RA, ZXTW(opWord), RB);  \
	dbg.log(" : (%016llX)", RBV);

#define POST_MEM POST_X64(RAV)

// Operate instructions
#define PRE_RAB_RC(opcode) \
	dbg.log("%016llX %-8s r%d,", state.cpcAddr, #opcode, RA); \
	if (opWord & OPC_LIT)                                     \
		dbg.log("#%02X,", OP_GETLIT(opWord));                 \
	else                                                      \
		dbg.log("R%d,", RB);                                  \
	dbg.log("R%d", RC);                                       \
	dbg.log(" : (%016llX, %016llX)", RAV, RBVL);

#define POST_RAB_RC POST_X64(RCV)

#define PRE_HW_LD(opcode)                                 \
	dbg.log("%016llX %-8s r%d,%04Xh(r%d)", state.cpcAddr, \
		#opcode, RA, ZXTW(DISP12), RB);                   \
	dbg.log(" : (%llX)", RBV)

#define POST_HW_LD POST_X64(RCV)

#define PRE_HW_ST(opcode)                                 \
	dbg.log("%016llX %-8s r%d,%04Xh(r%d)", state.cpcAddr, \
		#opcode, RA, ZXTW(DISP12), RB);                   \
	dbg.log(" : (%llX)", RBV)

#define POST_HW_ST POST_X64(RCV)

#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, format)

#define OPC_EXEC2(opcode, format) \
	PRE_##format(opcode);         \
	DOPC_##opcode;                \
	POST_##format;

#else

#define UNKNOWN_OPCODE1
#define UNKNOWN_OPCODE2

#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, format)

#endif
