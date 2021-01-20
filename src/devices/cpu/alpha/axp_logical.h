/*
 * axp_logical.h - logical instructions
 *
 *  Created on: Jan 19, 2021
 *      Author: Tim Stark
 */

#define DOPC_AND		RCV = RAV & RBV
#define DOPC_EQV		RCV = RAV ^ ~RBV
#define DOPC_ORNOT		RCV = RAV | ~ RBV
#define DOPC_XOR		RCV = RAV ^ RBV

#define DOPC_BIC		RCV = RAV & ~RBV
#define DOPC_BIS		RCV = RAV | RBV

#define DOPC_CMOVEQ		if (RAV != 0) RCV = RBV
#define DOPC_CMOVGE		if (RAV >= 0) RCV = RBV
#define DOPC_CMOVGT		if (RAV > 0) RCV = RBV
#define DOPC_CMOVLBC	if ((RAV & 1) == 0) RCV = RBV
#define DOPC_CMOVLBS	if ((RAV & 1) == 1) RCV = RBV
#define DOPC_CMOVLE		if (RAV <= 0) RCV = RBV
#define DOPC_CMOVLT		if (RAV < 0) RCV = RBV
#define DOPC_CMOVNE		if (RAV != 0) RCV = RBV

#define DOPC_SLL		RCV = RAV << (RBV & 0x3F)
#define DOPC_SRL		RCV = RAV >> (RBV & 0x3F)

#define DOPC_SRA		\
	RCV = (RBV & 0x3F)  \
		? ((RAV >> (RCV & 0x3F))   \
			| ((RAV >> 63) ? (MSK_QUAD << (64 - (RBV & 63))) : 0)) \
		: RAV
