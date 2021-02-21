/*
 * mmu.h
 *
 *  Created on: Apr 14, 2017
 *      Author: Timothy Stark
 *
 *  Memory Management Unit
 *
 */

#pragma once

// Virtual address regions
#define VA_ADDR_P0		(0u << 30)  // 00000000 - 3FFFFFFF
#define VA_ADDR_P1		(1u << 30)  // 40000000 - 7FFFFFFF
#define VA_ADDR_S0		(1u << 31)  // 80000000 - FFFFFFFF

// Page offset (each page holds 512 bytes)
#define PAG_WIDTH		9
#define PAG_SIZE		(1u << PAG_WIDTH)
#define PAG_MASK		(PAG_SIZE - 1)

// Virtual Page Number (VPN)
#define VPN_WIDTH		(31u - PAG_WIDTH)
#define VPN_SIZE		(1u << VPN_WIDTH)
#define VPN_POS			PAG_WIDTH
#define VPN_MASK		(VPN_SIZE - 1)
#define VA_VPN			((VPN_MASK << VPN_POS) & paMask)

// Translation Buffer Index
#define TBI_WIDTH		12
#define TBI_SIZE		(1u << TBI_WIDTH)
#define TBI_MASK		(TBI_SIZE - 1)
#define TLB_SIZE		TBI_SIZE

// Virtual Address Field Extractions
#define VA_GETBASE(va)	((va) & ~PAG_MASK)
#define VA_GETOFF(va)	((va) & PAG_MASK)
#define VA_GETVPN(va)	(((va) >> VPN_POS) & VPN_MASK)
#define VA_GETTBI(vpn)	((vpn) & TBI_MASK)

// Page Table Entry
#define PTE_V			0x80000000 // Valid Bit
#define PTE_PROT		0x78000000 // Protection Code
#define PTE_M			0x04000000 // Modified Bit
#define PTE_OWN			0x01800000 // Ownership
#define PTE_GLOBAL		0x00400000 // Global Page Table Entry
#define PTE_PFN			0x0001FFFF // Page Frame Number

#define PTE_M_PROT		0xF       // Protection Code Mask
#define PTE_M_OWN		0x3       // Owner Mode Mask
#define PTE_M_PFN		0x1FFFF   // Page Frame Number Mask

#define PTE_P_PROT		27        // Position of Protection Code
#define PTE_P_OWN		23        // Position of Ownership Mode
#define PTE_P_PFN		0         // Position of Page Frame Number

#define PTE_GETACC(pte)	(((pte) >> PTE_P_PROT) & PTE_M_PROT)

// Translation Buffer Entry
//
// |                                             |   Access Bits   |
// |                                             | | Write | Read  |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |           Physical Frame Number             |M|U|S|E|K|U|S|E|K|
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define TLB_N_ACC		4
#define TLB_M_ACC		((1u << TLB_N_ACC) - 1)
#define TLB_P_RDACC		0
#define TLB_P_WRACC		TLB_N_ACC
#define TLN_P_M			(TLB_N_ACC * 2)
#define TLB_RDACC		(TLB_M_ACC << TLB_P_RDACC)
#define TLB_WRACC		(TLB_M_ACC << TLB_P_WRACC)
#define TLB_M			(1u << (TLB_N_ACC * 2))

#define TLB_N_PFN		(32u - PAG_WIDTH)
#define TLB_M_PFN		((1u << TLB_N_PFN) - 1)
#define TLB_PFN			(TLB_M_PFN << PAG_WIDTH)

#define MM_WRITE		4  // Write Access
#define MM_EMASK		3  // Mask Against Probe

#define MM_ACV			0  // Access-Control Violation
#define MM_LNV			1  // Length Not Valid
#define MM_PACV			2  // PTE Access-Control Violation (11/780 only)
#define MM_PLNV			3  // PTE Length Not Valid
#define MM_TNV			4  // Translation Not Valid
#define MM_PTNV			6  // PTE Translation Not Valid
#define MM_OK			7  // Successful Translation

#define MM_FAIL			-1  // Check Memory Access Failure
