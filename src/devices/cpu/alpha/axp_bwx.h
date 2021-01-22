/*
 * axp_bwx.h
 *
 *  Created on: Jan 19, 2021
 *      Author: Tim Stark
 */

#define DOPC_EXTBL		RCV = (RAV >> ((RBVL & 7) * 8)) & MSKQ_BYTE
#define DOPC_EXTWL		RCV = (RAV >> ((RBVL & 7) * 8)) & MSKQ_WORD
#define DOPC_EXTLL		RCV = (RAV >> ((RBVL & 7) * 8)) & MSKQ_LONG
#define DOPC_EXTQL		RCV = RAV >> ((RBVL & 7) * 8)
#define DOPC_EXTWH		RCV = (RAV << ((64 - ((RBVL & 7) * 8)) & 63)) & MSKQ_WORD
#define DOPC_EXTLH		RCV = (RAV << ((64 - ((RBVL & 7) * 8)) & 63)) & MSKQ_LONG
#define DOPC_EXTQH		RCV = (RAV << ((64 - ((RBVL & 7) * 8)) & 63)) & MSKQ_QUAD

#define DOPC_INSBL		RCV = (RAV & MSKQ_BYTE) << ((RBVL & 7) * 8)
#define DOPC_INSWL		RCV = (RAV & MSKQ_WORD) << ((RBVL & 7) * 8)
#define DOPC_INSLL		RCV = (RAV & MSKQ_LONG) << ((RBVL & 7) * 8)
#define DOPC_INSQL		RCV = RAV << ((RBVL & 7) * 8)
#define DOPC_INSWH		RCV = (RBVL & 7) ? ((RAV & MSKQ_WORD) >> ((64 - ((RBVL & 7) * 8)) & 63)) : 0
#define DOPC_INSLH		RCV = (RBVL & 7) ? ((RAV & MSKQ_LONG) >> ((64 - ((RBVL & 7) * 8)) & 63)) : 0
#define DOPC_INSQH		RCV = (RBVL & 7) ? ((RAV & MSKQ_QUAD) >> ((64 - ((RBVL & 7) * 8)) & 63)) : 0

#define DOPC_MSKBL		RCV = RAV & ~(MSKQ_BYTE << (RBVL & 7) * 8)
#define DOPC_MSKWL		RCV = RAV & ~(MSKQ_WORD << (RBVL & 7) * 8)
#define DOPC_MSKLL		RCV = RAV & ~(MSKQ_LONG << (RBVL & 7) * 8)
#define DOPC_MSKQL		RCV = RAV & ~(MSKQ_QUAD << (RBVL & 7) * 8)
#define DOPC_MSKWH		RCV = (RBVL & 7) ? (RAV & ~(MSKQ_WORD >> ((64 - ((RBVL & 7) * 8)) & 63))) : RAV
#define DOPC_MSKLH		RCV = (RBVL & 7) ? (RAV & ~(MSKQ_LONG >> ((64 - ((RBVL & 7) * 8)) & 63))) : RAV
#define DOPC_MSKQH		RCV = (RBVL & 7) ? (RAV & ~(MSKQ_QUAD >> ((64 - ((RBVL & 7) * 8)) & 63))) : RAV

#define DOPC_SEXTB		RCV = SXTB(RBVL)
#define DOPC_SEXTW		RCV = SXTW(RBVL)

#define DOPC_ZAP                                      \
	RCV = RAV &                                       \
		(((RBVL & 1)   ? 0 : 0x00000000000000FFULL) | \
		 ((RBVL & 2)   ? 0 : 0x000000000000FF00ULL) | \
		 ((RBVL & 4)   ? 0 : 0x0000000000FF0000ULL) | \
		 ((RBVL & 8)   ? 0 : 0x00000000FF000000ULL) | \
		 ((RBVL & 16)  ? 0 : 0x000000FF00000000ULL) | \
		 ((RBVL & 32)  ? 0 : 0x0000FF0000000000ULL) | \
		 ((RBVL & 64)  ? 0 : 0x00FF000000000000ULL) | \
		 ((RBVL & 128) ? 0 : 0xFF00000000000000ULL))

#define DOPC_ZAPNOT                                   \
	RCV = RAV &                                       \
		(((RBVL & 1)   ? 0x00000000000000FFULL : 0) | \
		 ((RBVL & 2)   ? 0x000000000000FF00ULL : 0) | \
		 ((RBVL & 4)   ? 0x0000000000FF0000ULL : 0) | \
		 ((RBVL & 8)   ? 0x00000000FF000000ULL : 0) | \
		 ((RBVL & 16)  ? 0x000000FF00000000ULL : 0) | \
		 ((RBVL & 32)  ? 0x0000FF0000000000ULL : 0) | \
		 ((RBVL & 64)  ? 0x00FF000000000000ULL : 0) | \
		 ((RBVL & 128) ? 0xFF00000000000000ULL : 0))
