/*
 * axp_pal.h
 *
 *  Created on: Jan 22, 2021
 *      Author: Tim Stark
 */

#pragma once

// Call PAL instruction

#define DOPC_CALL_PAL \
	if (((func < 0x40) && (state.cm == ACC_KERNEL)) ||         \
       ((func >= 0x80) && (func < 0xC0)))                      \
	{                                                          \
		if (func == 0)                                         \
		{                                                      \
			dbg.logf(LOG_CONSOLE, "%s: *** HALT instruction at %X\n", \
				getDeviceName(), state.fpcAddr);               \
			dbg.flushAll();                                    \
			pState = execStopping;                             \
			break;                                             \
		}                                                      \
		state.iRegs[32 + 23] = state.pcAddr;                   \
		pAddr = state.palBase | (1u << 13) | 1 |               \
			((func & 0x80) << 5) | ((func & 0x3F) << 6);       \
		setPC(pAddr);                                          \
	} else {                                                   \
		UNKNOWN_OPCODE2;                                       \
	}

// Hardware instructions

#define DOPC_HW_REI		setPC(state.iRegs[RB])

#define DOPC_HW_LDL		RAV = mapProgram->read32(RBV + DISP12);
#define DOPC_HW_LDQ		RAV = mapProgram->read64(RBV + DISP12);
#define DOPC_HW_STL		mapProgram->write32(RBV + DISP12, RAV);
#define DOPC_HW_STQ		mapProgram->write64(RBV + DISP12, RAV);

//#define DOPC_HW_LDL                                        \
//	switch (func)                                          \
//	{                                                      \
//		case 0: /* longword physical */                    \
//			pAddr = RBV + DISP12;                          \
//			RAV = mapProgram->read32(pAddr);               \
//			break;                                         \
//		case 2: /* longword physical conditional */        \
//		case 4: /* longword virtual */                     \
//		case 12: /* longword virtual alt */                \
//		default:                                           \
//			UNKNOWN_OPCODE2;                               \
//			break;                                         \
//	}
//
//#define DOPC_HW_LDQ                                        \
//	switch (func)                                          \
//	{                                                      \
//		case 1: /* quadword physical */                    \
//			pAddr = RBV + DISP12;                          \
//			RAV = mapProgram->read64(pAddr);               \
//			break;                                         \
//		case 3: /* quadword physical conditional */        \
//		case 5: /* quadword virtual */                     \
//		case 13: /* quadword virtual alt */                \
//		default:                                           \
//			UNKNOWN_OPCODE2;                               \
//			break;                                         \
//	}
//
//
//#define DOPC_HW_STL                                        \
//	switch (func)                                          \
//	{                                                      \
//		case 0: /* longword physical */                    \
//			pAddr = RBV + DISP12;                          \
//			mapProgram->write32(pAddr, RAV);               \
//			break;                                         \
//		case 2: /* longword physical conditional */        \
//		case 4: /* longword virtual */                     \
//		case 12: /* longword virtual alt */                \
//		default:                                           \
//			UNKNOWN_OPCODE2;                               \
//			break;                                         \
//	}
//
//
//#define DOPC_HW_STQ                                        \
//	switch (func)                                          \
//	{                                                      \
//		case 1: /* quadword physical */                    \
//			pAddr = RBV + DISP12;                          \
//			mapProgram->write64(pAddr, RAV);               \
//			break;                                         \
//		case 3: /* quadword physical conditional */        \
//		case 5: /* quadword virtual */                     \
//		case 13: /* quadword virtual alt */                \
//		default:                                           \
//			UNKNOWN_OPCODE2;                               \
//			break;                                         \
//	}

#define DOPC_HW_MFPR
#define DOPC_HW_MTPR

