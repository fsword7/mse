/*
 * axp_logical.h - logical instructions
 *
 *  Created on: Jan 19, 2021
 *      Author: Tim Stark
 */

#define DOPC_AND		RCV = RAV & RBVL
#define DOPC_EQV		RCV = RAV ^ ~RBVL
#define DOPC_ORNOT		RCV = RAV | ~ RBVL
#define DOPC_XOR		RCV = RAV ^ RBVL

#define DOPC_BIC		RCV = RAV & ~RBVL
#define DOPC_BIS		RCV = RAV | RBVL

#define DOPC_CMOVEQ		if (SXTQ(RAV) == 0) RCV = RBVL
#define DOPC_CMOVGE		if (SXTQ(RAV) >= 0) RCV = RBVL
#define DOPC_CMOVGT		if (SXTQ(RAV) > 0) RCV = RBVL
#define DOPC_CMOVLBC	if ((RAV & 1) == 0) RCV = RBVL
#define DOPC_CMOVLBS	if ((RAV & 1) == 1) RCV = RBVL
#define DOPC_CMOVLE		if (SXTQ(RAV) <= 0) RCV = RBVL
#define DOPC_CMOVLT		if (SXTQ(RAV) < 0) RCV = RBVL
#define DOPC_CMOVNE		if (SXTQ(RAV) != 0) RCV = RBVL

#define DOPC_SLL		RCV = RAV << (RBVL & 0x3F)
#define DOPC_SRL		RCV = RAV >> (RBVL & 0x3F)
#define DOPC_SRA		RCV = SXTQ(RAV) >> (RBVL & 0x3F)

//#define DOPC_SRA		\
//	RCV = (RBVL & 0x3F)  \
//		? ((RAV >> (RCV & 0x3F))   \
//			| ((RAV >> 63) ? (MSK_QUAD << (64 - (RBVL & 63))) : 0)) \
//		: RAV
