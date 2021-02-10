/*
 * debug.h - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

#ifdef DEBUG

#define ONTRACE if (dbg.checkAnyFlags(DBG_TRACE))

#define UNKNOWN_OPCODE1 \
	dbg.log("*** Unknown opcode %02X\n", opCode);

#define UNKNOWN_OPCODE2 \
	dbg.log("*** Unknown opcode %02X.%02X\n", opCode, func);

#define POST_X8(VAL)  ONTRACE dbg.log(" ==> %02X\n", ZXTB(val))
#define POST_X16(val) ONTRACE dbg.log(" ==> %04X\n", ZXTW(val))
#define POST_X32(val) ONTRACE dbg.log(" ==> %08X\n", ZXTL(val))
#define POST_X64(val) ONTRACE dbg.log(" ==> %016llX\n", ZXTQ(val))

#define POST_X8S(VAL)  ONTRACE dbg.log(" <== %02X\n", ZXTB(val))
#define POST_X16S(val) ONTRACE dbg.log(" <== %04X\n", ZXTW(val))
#define POST_X32S(val) ONTRACE dbg.log(" <== %08X\n", ZXTL(val))
#define POST_X64S(val) ONTRACE dbg.log(" <== %016llX\n", ZXTQ(val))

// Condition instructions
#define PRE_COND(opcode)                                   \
	{                                                      \
		dbgval = state.pcAddr + (DISP21 << 2) & ~0x3;      \
		dbg.log("%llX %-8s R%d,%llX : (%016llX)\n",        \
				state.fpcAddr, #opcode, RA, dbgval, RAV);  \
	}

#define POST_COND

#define PRE_FCOND(opcode)                                  \
	{                                                      \
		dbgval = state.pcAddr + (DISP21 << 2) & ~0x3;      \
		dbg.log("%llX %-8s F%d,%llX : (%016llX)\n",        \
				state.fpcAddr, #opcode, FA, dbgval, FAV);  \
	}

#define POST_FCOND

// Memory instructions
#define PRE_MEM(opcode) 				                      \
	{                                                         \
		dbg.log("%llX %-8s R%d,%04X(R%d)",                    \
			state.fpcAddr, #opcode, RA, ZXTW(opWord), RB);    \
			dbg.log(" : (%016llX)", RBV + SXTW(opWord));      \
	}

#define POST_MEM POST_X64(RAV)

#define PRE_MEMS  PRE_MEM
#define POST_MEMS POST_X64S(RAV)

// Memory instructions (floating)
#define PRE_FMEM(opcode) 				                      \
	{                                                         \
		dbg.log("%llX %-8s F%d,%04X(R%d)",                    \
			state.fpcAddr, #opcode, FA, ZXTW(opWord), RB);    \
			dbg.log(" : (%016llX)", RBV + SXTW(opWord));      \
	}

#define POST_FMEM POST_X64(FAV)

#define PRE_FMEMS  PRE_FMEM
#define POST_FMEMS POST_X64S(FAV)

// Operate instructions
#define PRE_RAB_RC(opcode)                                        \
	{                                                             \
		dbg.log("%llX %-8s R%d,", state.fpcAddr, #opcode, RA);    \
		if (opWord & OPC_LIT)                                     \
			dbg.log("#%02X,", OP_GETLIT(opWord));                 \
		else                                                      \
			dbg.log("R%d,", RB);                                  \
		dbg.log("R%d", RC);                                       \
		dbg.log(" : (%016llX, %016llX)", RAV, RBVL);              \
	}

#define POST_RAB_RC POST_X64(RCV)

#define PRE_RB_RC(opcode)                                         \
	{                                                             \
		dbg.log("%llX %-8s ", state.fpcAddr, #opcode, RA);        \
		if (opWord & OPC_LIT)                                     \
			dbg.log("#%02X,", OP_GETLIT(opWord));                 \
		else                                                      \
			dbg.log("R%d,", RB);                                  \
		dbg.log("R%d", RC);                                       \
		dbg.log(" : (%016llX)", RBVL);                            \
	}

#define POST_RB_RC POST_X64(RCV)

#define PRE_X_RA(opcode)                                        \
	{                                                           \
		dbg.log("%llX %-8s R%d", state.fpcAddr, #opcode, RA); \
	}

#define POST_X_RA POST_X64(RAV)

#define PRE_X_RC(opcode)                                        \
	{                                                           \
		dbg.log("%llX %-8s R%d", state.fpcAddr, #opcode, RC); \
	}

#define POST_X_RC POST_X64(RCV)

#define PRE_X_FA(opcode)                                        \
	{                                                           \
		dbg.log("%llX %-8s F%d", state.fpcAddr, #opcode, FA); \
	}

#define POST_X_FA POST_X64(FAV)

#define PRE_PAL(opcode)                             \
	{                                               \
		dbg.log("%llX %-8s #%X\n", state.fpcAddr,   \
			#opcode, OP_GETPAL(opWord));            \
	}

#define POST_PAL

#define PRE_BR(opcode)                                  \
	{                                                   \
		dbgval = state.pcAddr + (DISP21 << 2) & ~0x3;   \
		dbg.log("%llX %-8s R%d,%llX\n", state.fpcAddr,  \
			#opcode, RA, dbgval);                       \
	}

#define POST_BR

#define PRE_JMP(opcode)                                               \
	{                                                                 \
		dbg.log("%llX %-8s R%d,R%d", state.fpcAddr, #opcode, RA, RB); \
		dbg.log(" : (%llX)\n", RBV);                                  \
	}

#define POST_JMP

#define PRE_RET(opcode)                                       \
	{                                                         \
		dbg.log("%llX %-8s R%d", state.fpcAddr, #opcode, RB); \
		dbg.log(" : (%llX)\n", RBV);                          \
	}

#define POST_RET


#define PRE_HW_LD(opcode)                                       \
	{                                                           \
		dbgstr = #opcode;                                       \
		switch (func & ~1)                                      \
		{                                                       \
		case 0: dbgstr += "/P"; break;                          \
		case 2: dbgstr += "/PL"; break;                         \
		case 4: dbgstr += "/V"; break;                          \
		case 10: dbgstr += "/C"; break;                         \
		case 12: dbgstr += "/A"; break;                         \
		case 14: dbgstr += "/AC"; break;                        \
		default: dbgstr += fmt::sprintf("(%d)", func); break;   \
		}                                                       \
		dbg.log("%llX %-8s R%d,%04X(R%d)", state.fpcAddr,       \
			dbgstr, RA, ZXTW(DISP12), RB);                      \
		dbg.log(" : (%llX)", RBV + DISP12);                     \
	}


#define POST_HW_LD POST_X64(RAV)

#define PRE_HW_ST(opcode)                                       \
	{                                                           \
		dbgstr = #opcode;                                       \
		switch (func & ~1)                                      \
		{                                                       \
		case 0: dbgstr += "/P"; break;                          \
		case 2: dbgstr += "/PC"; break;                         \
		case 12: dbgstr += "/A"; break;                         \
		default: dbgstr += fmt::sprintf("(%d)", func); break;   \
		}                                                       \
		dbg.log("%llX %-8s R%d,%04X(R%d)", state.fpcAddr,       \
			dbgstr, RA, ZXTW(DISP12), RB);                      \
		dbg.log(" : (%llX)", RBV + DISP12);                     \
	}

//#define PRE_HW_STL(opcode) PRE_HE_ST(opcode)
//#define PRE_HW_STQ(opcode) PRE_HE_ST(opcode)
//
//#define POST_HW_STL POST_X32S(RAV)
#define POST_HW_ST POST_X64S(RAV)

#define PRE_MFPR(opcode)                                   \
	{                                                      \
		if (ahType == ARCH_EV6)                            \
			dbg.log("%llX %-8s R%d,#%02X", state.fpcAddr,  \
				#opcode, RB, ((opWord >> 8) & 0xFF));      \
		else                                               \
			dbg.log("%llX %-8s R%d,#%04X", state.fpcAddr,  \
				#opcode, RB, (opWord & 0xFFFF));           \
	}

#define POST_MFPR POST_X64(RAV)

#define PRE_MTPR(opcode)                                   \
	{                                                      \
		if (ahType == ARCH_EV6)                            \
			dbg.log("%llX %-8s R%d,#%02X", state.fpcAddr,  \
				#opcode, RB, ((opWord >> 8) & 0xFF));      \
		else                                               \
			dbg.log("%llX %-8s R%d,#%04X", state.fpcAddr,  \
				#opcode, RB, (opWord & 0xFFFF));           \
	}

#define POST_MTPR POST_X64S(RBV)

#define PRE_NOP(opcode)                                 \
	{                                                   \
		dbg.log("%llX %-8s\n", state.fpcAddr, #opcode); \
	}

#define POST_NOP

#define OPC_EXEC(opcode, format)       \
	ONTRACE PRE_##format(opcode);      \
	DOPC_##opcode;                     \
	POST_##format;

#define OPC_FUNC(opcode, call, format) \
	ONTRACE PRE_##format(opcode);      \
	call(opWord);                      \
	POST_##format;

#else

#define UNKNOWN_OPCODE1
#define UNKNOWN_OPCODE2

#define OPC_EXEC(opcode, format) DOPC_##opcode
#define OPC_FUNC(opcode, call, format)

#endif
