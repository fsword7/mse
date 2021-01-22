/*
 * axp_pal.h
 *
 *  Created on: Jan 22, 2021
 *      Author: Tim Stark
 */

#pragma once

#define DOPC_HW_REI		setPC(state.iRegs[RB])

#define DOPC_HW_LDL                                        \
	switch (func)                                          \
	{                                                      \
		case 0: /* longword physical */                    \
			pAddr = RBV + DISP12;                          \
			RAV = mapProgram->read32(pAddr);               \
			break;                                         \
		case 2: /* longword physical conditional */        \
		case 4: /* longword virtual */                     \
		case 12: /* longword virtual alt */                \
		default:                                           \
			UNKNOWN_OPCODE2;                               \
			break;                                         \
	}

#define DOPC_HW_LDQ                                        \
	switch (func)                                          \
	{                                                      \
		case 1: /* quadword physical */                    \
			pAddr = RBV + DISP12;                          \
			mapProgram->write64(pAddr, RAV);               \
			break;                                         \
		case 3: /* quadword physical conditional */        \
		case 5: /* quadword virtual */                     \
		case 13: /* quadword virtual alt */                \
		default:                                           \
			UNKNOWN_OPCODE2;                               \
			break;                                         \
	}


#define DOPC_HW_STL                                        \
	switch (func)                                          \
	{                                                      \
		case 0: /* longword physical */                    \
			pAddr = RBV + DISP12;                          \
			mapProgram->write32(pAddr, RAV);               \
			break;                                         \
		case 2: /* longword physical conditional */        \
		case 4: /* longword virtual */                     \
		case 12: /* longword virtual alt */                \
		default:                                           \
			UNKNOWN_OPCODE2;                               \
			break;                                         \
	}


#define DOPC_HW_STQ                                        \
	switch (func)                                          \
	{                                                      \
		case 1: /* quadword physical */                    \
			pAddr = RBV + DISP12;                          \
			mapProgram->write64(pAddr, RAV);               \
			break;                                         \
		case 3: /* quadword physical conditional */        \
		case 5: /* quadword virtual */                     \
		case 13: /* quadword virtual alt */                \
		default:                                           \
			UNKNOWN_OPCODE2;                               \
			break;                                         \
	}

#define DOPC_HW_MFPR
#define DOPC_HW_MTPR
