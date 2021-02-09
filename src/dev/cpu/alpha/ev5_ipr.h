/*
 * ev5_ipr.h - 21164 series - EV5 IPR registers
 *
 *  Created on: Feb 6, 2021
 *      Author: Tim Stark
 */

#pragma once

#define IPR_ISR					0x100	// (R)
#define IPR_ITB_TAG				0x101	// (W)   Istream Translation Buffer Tag Register
#define IPR_ITB_PTE				0x102	// (R/W) Instruction Translation Buffer Page Table Entry Register
#define IPR_ITB_ASN				0x103	// (R/W) Instruction Translation Buffer Address Space Number Register
#define IPR_ITB_PTE_TEMP		0x104	// (R)
#define IPR_ITB_IA				0x105	// (W)
#define IPR_ITB_IAP				0x106	// (W)
#define IPR_ITB_IS				0x107	// (W)
#define IPR_SIRR				0x108	// (R/W)
#define IPR_ASTRR				0x109	// (R/W)
#define IPR_ASTER				0x10A	// (R/W)
#define IPR_EXC_ADDR			0x10B	// (R/W)
#define IPR_EXC_SUM				0x10C	// (R/W0C)
#define IPR_EXC_MASK			0x10D	// (R)
#define IPR_PAL_BASE			0x10E	// (R/W)
#define IPR_ICM					0x10F	// (R/W)
#define IPR_IPLR				0x110	// (R/W)
#define IPR_INTID				0x111	// (R)
#define IPR_IFAULT_VA_FORM		0x112	// (R)
#define IPR_IVPTBR				0x113	// (R/W)
#define IPR_HWINT_CLR			0x115	// (W)
#define IPR_SL_XMIT				0x116	// (W)
#define IPR_SL_RCV				0x117	// (R)
#define IPR_ICSR				0x118	// (R/W)
#define IPR_IC_FLUSH_CTL		0x119	// (W)
#define IPR_ICPERR_STAT			0x11A	// (R/W1C)
#define IPR_PMCTR				0x11C	// (R/W)

#define IPR_PALtemp0			0x140	// (R/W)
#define IPR_PALtemp1			0x141	// (R/W)
#define IPR_PALtemp2			0x142	// (R/W)
#define IPR_PALtemp3			0x143	// (R/W)
#define IPR_PALtemp4			0x144	// (R/W)
#define IPR_PALtemp5			0x145	// (R/W)
#define IPR_PALtemp6			0x146	// (R/W)
#define IPR_PALtemp7			0x147	// (R/W)
#define IPR_PALtemp8			0x148	// (R/W)
#define IPR_PALtemp9			0x149	// (R/W)
#define IPR_PALtemp10			0x14A	// (R/W)
#define IPR_PALtemp11			0x14B	// (R/W)
#define IPR_PALtemp12			0x14C	// (R/W)
#define IPR_PALtemp13			0x14D	// (R/W)
#define IPR_PALtemp14			0x14E	// (R/W)
#define IPR_PALtemp15			0x14F	// (R/W)
#define IPR_PALtemp16			0x150	// (R/W)
#define IPR_PALtemp17			0x151	// (R/W)
#define IPR_PALtemp18			0x152	// (R/W)
#define IPR_PALtemp19			0x153	// (R/W)
#define IPR_PALtemp20			0x154	// (R/W)
#define IPR_PALtemp21			0x155	// (R/W)
#define IPR_PALtemp22			0x156	// (R/W)
#define IPR_PALtemp23			0x157	// (R/W)

#define IPR_DTB_ASN				0x200	// (W)
#define IPR_DTB_CM				0x201	// (W)
#define IPR_DTB_TAG				0x202	// (W)
#define IPR_DTB_PTE				0x203	// (R/W)
#define IPR_DTB_PTE_TEMP		0x204	// (R)
#define IPR_MM_STAT				0x205	// (R)
#define IPR_VA					0x206	// (R)
#define IPR_VA_FORM				0x207	// (R)
#define IPR_MVPTBR				0x208	// (W)
#define IPR_DTB_IAP				0x209	// (W)
#define IPR_DTB_IA				0x20A	// (W)
#define IPR_DTB_IS				0x20B	// (W)
#define IPR_ALT_MODE			0x20C	// (W)
#define IPR_CC					0x20D	// (W)
#define IPR_CC_CTL				0x20E	// (W)
#define IPR_MCSR				0x20F	// (R/W)
#define IPR_DC_FLUSH			0x210	// (W)
#define IPR_DC_PERR_STAT		0x212	// (R/W1C)
#define IPR_DC_TEST_CTL			0x213	// (R/W)
#define IPR_DC_TEST_TAG			0x214	// (R/W)
#define IPR_DC_TEST_TAG_TEMP	0x215	// (R/W)
#define IPR_DC_MODE				0x216	// (R/W)
#define IPR_MAF_MODE			0x217	// (R/W)


#define PAL_BASE_MASK  0x000000FFFFFFC000ull
