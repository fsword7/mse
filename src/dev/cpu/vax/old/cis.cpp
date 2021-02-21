/*
 * cis.cpp
 *
 *  Created on: Apr 9, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

static const uint32_t mvcSize[] = { LN_BYTE, LN_LONG, LN_BYTE };

// For MOVC3/MOVC5 instruction
void vax_cpu_base::movc(int c5flg)
{
	uint32_t src, dst;
	uint16_t slen, dlen;
	uint32_t fill;
	uint32_t dpc;
	uint32_t mvcLen[3];
	uint32_t data;

	if ((psReg & PSL_FPD) == 0) {
		if (c5flg & 1) {
			// For MOVC5 instruction
			slen = ZXTW(opReg[0]);
			src  = ZXTL(opReg[1]);
			fill = ZXTB(opReg[2]);
			dlen = ZXTW(opReg[3]);
			dst  = ZXTL(opReg[4]);

			// Set R0-R4 registers
			REG_R1 = src;
			REG_R2 = (slen < dlen) ? slen : dlen;
			REG_R3 = dst;
			REG_R4 = dlen - slen;

			// Update condition codes
			SetNZ(ccReg, SXTW(slen), SXTW(dlen), 0);
			SetC(ccReg, ZXTW(slen), ZXTW(dlen));
		} else {
			// For MOVC3 instruction
			slen = ZXTW(opReg[0]);
			src  = ZXTL(opReg[1]);
			dst  = ZXTL(opReg[2]);
			dlen = slen;
			fill = 0;

			// Set R0-R4 registers
			REG_R1 = src;
			REG_R2 = slen;
			REG_R3 = dst;
			REG_R4 = 0;

			// Update condition codes
			ccReg = CC_Z;
		}

#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: SRC %08X (%04X bytes) to DST %08X (%04X bytes) with fill %02X: %s\n",
					deviceName(), src, slen, dst, dlen, fill, stringCC(ccReg));
#endif /* ENABLE_DEBUG */

		dpc = REG_PC - faultAddr;
		REG_R0 = STR_PACK(dpc, fill, REG_R2);
		if (REG_R2 != 0) {
			if (ZXTL(REG_R1) < ZXTL(REG_R3)) {
				REG_R1 += REG_R2;
				REG_R3 += REG_R2;
				REG_R5  = MVC_BACK;
			} else
				REG_R5 = MVC_FWD;
		} else
			REG_R5 = MVC_FILL;
		REG_R5 |= (ccReg << MVC_P_CC);

		// Set FPD on PSL register
		psReg |= PSL_FPD;
	} else {
		fill = STR_GETCHR(REG_R0);
		REG_R2 &= STR_M_LEN;
		if (REG_R4 > 0)
			REG_R4 &= STR_M_LEN;

#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: (FPD) SRC %08X (%04X bytes) to DST %08X (%04X bytes) with fill %02X\n",
					deviceName(), ZXTL(REG_R1), ZXTW(REG_R2), ZXTL(REG_R3), ZXTW(REG_R4), fill);
#endif /* ENABLE_DEBUG */

		// Reset PC address and clear instruction look-ahead
		REG_PC = faultAddr + STR_GETDPC(REG_R0);
		flushvi();
	}

	switch (REG_R5 & MVC_M_STATE) {
	case MVC_FWD:
		mvcLen[0] = (LN_LONG - REG_R3) & ALIGN_LONG;
		if (mvcLen[0] > REG_R2)
			mvcLen[0] = REG_R2;
		mvcLen[1] = (REG_R2 - mvcLen[0]) & ~ALIGN_LONG;
		mvcLen[2] = REG_R2 - mvcLen[0] - mvcLen[1];

		for (int step = 0; step < 3; step++) {
			uint32_t len = mvcSize[step];
			for (int idx = 0; idx < mvcLen[step]; idx++) {
				data = readv(REG_R1, len, RACC);
				writev(REG_R3, data, len, WACC);
				REG_R1 += len;
				REG_R3 += len;
				REG_R2 -= len;
			}
		}
		goto FILL;

	case MVC_BACK:
		mvcLen[0] = (LN_LONG - REG_R3) & ALIGN_LONG;
		if (mvcLen[0] > REG_R2)
			mvcLen[0] = REG_R2;
		mvcLen[1] = (REG_R2 - mvcLen[0]) & ~ALIGN_LONG;
		mvcLen[2] = REG_R2 - mvcLen[0] - mvcLen[1];

		for (int step = 0; step < 3; step++) {
			uint32_t len = mvcSize[step];
			for (int idx = 0; idx < mvcLen[step]; idx++) {
				data = readv(REG_R1 - len, len, RACC);
				writev(REG_R3 - len, data, len, WACC);
				REG_R1 -= len;
				REG_R3 -= len;
				REG_R2 -= len;
			}
		}
		REG_R1 += (REG_R0 & STR_M_LEN);
		REG_R3 += (REG_R0 & STR_M_LEN);
		// Fall through...

	case MVC_FILL:
	FILL:
		if (SXTL(REG_R4) <= 0)
			break;
		REG_R5 |= MVC_FILL;

		mvcLen[0] = (LN_LONG - REG_R3) & ALIGN_LONG;
		if (mvcLen[0] > REG_R4)
			mvcLen[0] = REG_R4;
		mvcLen[1] = (REG_R4 - mvcLen[0]) & ~ALIGN_LONG;
		mvcLen[2] = REG_R4 - mvcLen[0] - mvcLen[1];

		// Fill repeating characters
		fill &= MSK_BYTE;
		fill  = (fill << 24) | (fill << 16) | (fill << 8) | fill;
		for (int step = 0; step < 3; step++) {
			uint32_t len = mvcSize[step];
			for (int idx = 0; idx < mvcLen[step]; idx++) {
				writev(REG_R3, fill, len, WACC);
				REG_R3 += len;
				REG_R4 -= len;
			}
		}
		break;

	default:
		throw RSVD_OPND_FAULT;
	}

	// Clear FPD on PSL register
	psReg &= ~PSL_FPD;
	ccReg  = (REG_R5 >> MVC_P_CC) & PSW_CC;
	REG_R0 = -REG_R4;
	// Clear all registers
	REG_R2 = 0;
	REG_R4 = 0;
	REG_R5 = 0;
}

// For CMPC3/CMPC5 instruction
void vax_cpu_base::cmpc(int c5flg)
{
	uint32_t src, dst;
	uint16_t slen, dlen;
	uint8_t  src1, src2, fill;
	uint32_t dpc;

	if ((psReg & PSL_FPD) == 0) {
		if (c5flg & 1) {
			// For CMPC5 instruction
			slen = ZXTW(opReg[0]);
			src  = ZXTL(opReg[1]);
			fill = ZXTB(opReg[2]);
			dlen = ZXTW(opReg[3]);
			dst  = ZXTL(opReg[4]);

			// Set R1-R3 registers
			REG_R1 = src;
			REG_R2 = dlen;
			REG_R3 = dst;
		} else {
			// For CMPC3 instruction
			slen = ZXTW(opReg[0]);
			src  = ZXTL(opReg[1]);
			dst  = ZXTL(opReg[2]);
			dlen = slen;
			fill = 0;

			// Set R1-R3 registers
			REG_R1 = src;
			REG_R2 = dlen;
			REG_R3 = dst;
		}

#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: Compare %08X (%04X bytes) with %08X (%04X bytes) with fill %02X\n",
				deviceName(), src, slen, dst, dlen, fill);
#endif /* ENABLE_DEBUG */

		dpc = REG_PC - faultAddr;
		REG_R0  = STR_PACK(dpc, fill, slen);
		REG_R2 &= STR_M_LEN;
		// Set FPD on PSL register
		psReg |= PSL_FPD;
	} else {
		fill = STR_GETCHR(REG_R0);
		REG_R2 &= STR_M_LEN;

#ifdef ENABLE_DEBUG
		if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
			dbg.log("%s: Compare %08X (%04X bytes) with %08X (%04X bytes) with fill %02X\n",
				deviceName(), ZXTL(REG_R1), ZXTW(REG_R0), ZXTL(REG_R3), ZXTW(REG_R2), fill);
#endif /* ENABLE_DEBUG */

		// Reset PC address and clear instruction look-ahead
		REG_PC = faultAddr + STR_GETDPC(REG_R0);
		flushvi();
	}

	src1 = 0;
	src2 = 0;
	while ((REG_R0|REG_R2) & STR_M_LEN) {
		src1 = (REG_R0 & STR_M_LEN) ? readv(REG_R1, LN_BYTE, RACC) : fill;
		src2 = (REG_R2 > 0) ? readv(REG_R3, LN_BYTE, RACC) : fill;
		if (src1 != src2)
			break;
		if (REG_R0 & STR_M_LEN) {
			REG_R0 = (REG_R0 & ~STR_M_LEN) | ((REG_R0 - 1) & STR_M_LEN);
			REG_R1++;
		}
		if (REG_R2 > 0) {
			REG_R2 = (REG_R2 - 1) & STR_M_LEN;
			REG_R3++;
		}
	}

	// Clear FPD on PSL register
	psReg  &= ~PSL_FPD;
	// Update condition codes
	SetNZ(ccReg, SXTB(src1), SXTB(src2), 0);
	SetC(ccReg, ZXTB(src1), ZXTB(src2));
#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND))
		dbg.log("%s: CC Status: %s\n", deviceName(), stringCC(ccReg));
#endif /* ENABLE_DEBUG */
	// Reset R0 register
	REG_R0 &= STR_M_LEN;
}

void vax_cpu_base::locc(bool skpflg)
{
	uint8_t  ch, match;
	uint32_t dpc;

	if ((psReg & PSL_FPD) == 0) {
		match   = opReg[0];
		dpc     = REG_PC - faultAddr;
		REG_R0  = STR_PACK(dpc, match, opReg[1]);
		REG_R1  = opReg[2];
		// Set "First Part Done" flag.
		psReg |= PSL_FPD;
	} else {
		match = STR_GETCHR(REG_R0);

		// Reset PC address
		REG_PC = faultAddr + STR_GETDPC(REG_R0);
		flushvi();
	}

	while (REG_R0 & STR_M_LEN) {
		// Check character to match or not.
		// If found, get out of loop.
		ch = readv(REG_R1, LN_BYTE, RACC);
		if ((ch == match) ^ skpflg)
			break;
		// Next Character
		REG_R0 = (REG_R0 & ~STR_M_LEN) |
			((REG_R0 - 1) & STR_M_LEN);
		REG_R1++;
	}

	// Reset R0 register.
	REG_R0 &= STR_M_LEN;
	// Clear "First Part Done" flag when done.
	psReg  &= ~PSL_FPD;
	// Update Condition Codes
	ccReg = (REG_R0 > 0) ? 0 : CC_Z;
}

void vax_cpu_base::scanc(bool spnflg)
{
	uint8_t  ch, tch, mask;
	uint32_t dpc;

	if ((psReg & PSL_FPD) == 0) {
		mask    = opReg[3];
		dpc     = REG_PC - faultAddr;
		REG_R0  = STR_PACK(dpc, mask, opReg[0]);
		REG_R1  = opReg[1];
		REG_R3  = opReg[2];
		// Set "First Part Done" flag.
		psReg |= PSL_FPD;
	} else {
		mask = STR_GETCHR(REG_R0);
		// Reset PC address
		REG_PC = faultAddr + STR_GETDPC(REG_R0);
		flushvi();
	}

	while (REG_R0 & STR_M_LEN) {
		// Check character to match or not.
		// If found, get out of loop.
		ch  = readv(REG_R1, LN_BYTE, RACC);
		tch = readv(REG_R3 + ch, LN_BYTE, RACC);
		if (((tch & mask) != 0) ^ spnflg)
			break;
		// Next Character
		REG_R0 = (REG_R0 & ~STR_M_LEN) |
			((REG_R0 - 1) & STR_M_LEN);
		REG_R1++;
	}

	// Reset R0 register.
	REG_R0 &= STR_M_LEN;
	REG_R2  = 0;
	// Clear "First Part Done" flag when done.
	psReg &= ~PSL_FPD;
	// Update Condition Codes
	ccReg = (REG_R0 > 0) ? 0 : CC_Z;
}
