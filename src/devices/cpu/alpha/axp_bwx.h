/*
 * axp_bwx.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Tim Stark
 */

#define DOPC_EXTBL		RCV = (RAV >> ((RBV & 7) * 8)) & MSKQ_BYTE
#define DOPC_EXTWL		RCV = (RAV >> ((RBV & 7) * 8)) & MSKQ_WORD
#define DOPC_EXTLL		RCV = (RAV >> ((RBV & 7) * 8)) & MSKQ_LONG
#define DOPC_EXTQL		RCV = RAV >> ((RBV & 7) * 8)
#define DOPC_EXTWH		RCV = (RAV << ((64 - ((RBV & 7) * 8)) & 63)) & MSKQ_WORD
#define DOPC_EXTLH		RCV = (RAV << ((64 - ((RBV & 7) * 8)) & 63)) & MSKQ_LONG
#define DOPC_EXTQH		RCV = (RAV << ((64 - ((RBV & 7) * 8)) & 63)) & MSKQ_QUAD

#define DOPC_INSBL		RCV = (RAV & MSKQ_BYTE) << ((RBV & 7) * 8)
#define DOPC_INSWL		RCV = (RAV & MSKQ_WORD) << ((RBV & 7) * 8)
#define DOPC_INSLL		RCV = (RAV & MSKQ_LONG) << ((RBV & 7) * 8)
#define DOPC_INSQL		RCV = RAV << ((RBV & 7) * 8)
#define DOPC_INSWH		RCV = (RBV & 7) ? ((RAV & MSKQ_WORD) >> ((64 - ((RBV & 7) * 8)) & 63)) : 0
#define DOPC_INSLH		RCV = (RBV & 7) ? ((RAV & MSKQ_LONG) >> ((64 - ((RBV & 7) * 8)) & 63)) : 0
#define DOPC_INSQH		RCV = (RBV & 7) ? ((RAV & MSKQ_QUAD) >> ((64 - ((RBV & 7) * 8)) & 63)) : 0

#define DOPC_MSKBL		RCV = RAV & ~(MSKQ_BYTE << (RBV & 7) * 8)
#define DOPC_MSKWL		RCV = RAV & ~(MSKQ_WORD << (RBV & 7) * 8)
#define DOPC_MSKLL		RCV = RAV & ~(MSKQ_LONG << (RBV & 7) * 8)
#define DOPC_MSKQL		RCV = RAV & ~(MSKQ_QUAD << (RBV & 7) * 8)
#define DOPC_MSKWH		RCV = (RCV & 7) ? (RAV & ~(MSKQ_WORD >> ((64 - ((RCV & 7) * 8)) & 63))) : RAV
#define DOPC_MSKLH		RCV = (RCV & 7) ? (RAV & ~(MSKQ_LONG >> ((64 - ((RCV & 7) * 8)) & 63))) : RAV
#define DOPC_MSKQH		RCV = (RCV & 7) ? (RAV & ~(MSKQ_QUAD >> ((64 - ((RCV & 7) * 8)) & 63))) : RAV

#define DOPC_SEXTB		RCV = SXTB(RBV)
#define DOPC_SEXTW		RCV = SXTW(RBV)

#define DOPC_ZAP                                     \
	RCV = RAV &                                      \
		(((RBV & 1)   ? 0 : 0x00000000000000FFULL) | \
		 ((RBV & 2)   ? 0 : 0x000000000000FF00ULL) | \
		 ((RBV & 4)   ? 0 : 0x0000000000FF0000ULL) | \
		 ((RBV & 8)   ? 0 : 0x00000000FF000000ULL) | \
		 ((RBV & 16)  ? 0 : 0x000000FF00000000ULL) | \
		 ((RBV & 32)  ? 0 : 0x0000FF0000000000ULL) | \
		 ((RBV & 64)  ? 0 : 0x00FF000000000000ULL) | \
		 ((RBV & 128) ? 0 : 0xFF00000000000000ULL))

#define DOPC_ZAPNOT                                  \
	RCV = RAV &                                      \
		(((RBV & 1)   ? 0x00000000000000FFULL : 0) | \
		 ((RBV & 2)   ? 0x000000000000FF00ULL : 0) | \
		 ((RBV & 4)   ? 0x0000000000FF0000ULL : 0) | \
		 ((RBV & 8)   ? 0x00000000FF000000ULL : 0) | \
		 ((RBV & 16)  ? 0x000000FF00000000ULL : 0) | \
		 ((RBV & 32)  ? 0x0000FF0000000000ULL : 0) | \
		 ((RBV & 64)  ? 0x00FF000000000000ULL : 0) | \
		 ((RBV & 128) ? 0xFF00000000000000ULL : 0))
