/*
 * axp_branch.h - Alpha CPU Processor - branch/control instructions
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

// branch condition instructions
#define DOPC_BEQ		if (SXTQ(RAV) == 0) addPC(DISP21 << 2)
#define DOPC_BGE		if (SXTQ(RAV) >= 0) addPC(DISP21 << 2)
#define DOPC_BGT		if (SXTQ(RAV) > 0)  addPC(DISP21 << 2)
#define DOPC_BLE		if (SXTQ(RAV) <= 0) addPC(DISP21 << 2)
#define DOPC_BLT		if (SXTQ(RAV) < 0)  addPC(DISP21 << 2)
#define DOPC_BNE		if (SXTQ(RAV) != 0) addPC(DISP21 << 2)

// branch set/clear instructions
#define DOPC_BLBC		if ((RAV & 1) == 0) addPC(DISP21 << 2)
#define DOPC_BLBS		if ((RAV & 1) == 1) addPC(DISP21 << 2)

// branch instructions
#define DOPC_BSR		DOPC_BR
#define DOPC_BR									\
	{											\
		RAV = state.vpcReg & ~0x03LL;			\
		addPC(DISP21 << 2);						\
	}


// JMP, JSR, RET and JSR_COROUTINE are the same
// instructions with different prediction bits
#define DOPC_JSR		DOPC_JMP
#define DOPC_RET		DOPC_JMP
#define DOPC_JMP								\
	{											\
		uint64_t temp = RBV & ~0x03LL;			\
		RAV = state.vpcReg & ~0x03LL; 			\
		setPC(temp | (state.vpcReg & 0x03LL));	\
	}
