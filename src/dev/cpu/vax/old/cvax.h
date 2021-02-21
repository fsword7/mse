/*
 * cvax.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX 78034 processor emulation
 *
 */

#pragma once

#include "dev/cpu/vax/vax.h"

// CVAX System ID register definition
//
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |      10       |           Reserved            |   uCode rev   |
// +-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-^-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define SID_ID		(10 << 24)
#define SID_UCODE	6

#define SRM_START	0x20040000

// Internal Processor Registers
// TODR - Time of Day Register
#define TODR_BASE  (1u << 28)
#define TODR_SEC   100

// ICCS - Interval Count Control Status Register
#define ICCS_IE     0x0040     // Interrupt Enable
#define ICCS_WMASK  (ICCS_IE)  // Writable Mask
#define ICCS_SECOND 100        // Ticks Each Second
#define ICCS_IPL    UQ_BR6     // Interrupt Level BR6
#define ICCS_VEC    SCB_TIMER  // System Vector
#define ICCS_PRIO   255        // Device Priority

// CONPC/CONPSL Register
#define CON_PWRUP  (HALT_PWRON << 8)    // Power Up State

// CADR (IPR 37) - Cache Disable Register
#define CADR_S1E   0x0040 // Set 1 Cache Enable
#define CADR_ISE   0x0020 // I-Stream Cache Enable
#define CADR_DSE   0x0010 // D-Stream Cache Enable
#define CADR_MBO   0x000C // Unused - Always On.
#define CADR_WW    0x0002 // Write Wrong Priority
#define CADR_DIA   0x0001 // Diagnostic Mode
#define CADR_RW    0x00F3 // Read/Write Access Mask

// MSER (IPR 39) - Memory System Error Register
#define MSER_HM    0x0080 // Hit/Miss
#define MSER_DSL   0x0040 // DAL Parity Error
#define MSER_MCD   0x0020 // Machine Check DAL Parity Error
#define MSER_MCC   0x0010 // Machine Check Cache Parity Error
#define MSER_DAT   0x0002 // Data Parity Error
#define MSER_TAG   0x0001 // Tag Parity Error

// CONPSL (IPR 42) - Saved PSL Register
#define CONPSL_MASK     0xFFFF00FF  // Processor Status Mask
#define CONPSL_MAPEN    0x00008000  // MAPEN<0> Bit
#define CONPSL_INVALID  0x00004000  // Invalid Bit
#define CONPSL_RESTART  0x00003F00  // Restart Code

// Machine Check Codes
#define MCHK_TBM_P0  0x03 // PPTE in P0 region
#define MCHK_TBM_P1  0x06 // PPTE in P1 region
#define MCHK_MO_P0   0x07 // PPTE in P0 region
#define MCHK_M0_P1   0x08 // PPTE in P1 region
#define MCHK_INTIPL  0x09 // Invalid Interrupt Request
#define MCHK_READ    0x80 // Read Reference Check
#define MCHK_WRITE   0x82 // Write Reference Check

class cvax_cpu : public vax_cpu_base
{
public:
	cvax_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);
	~cvax_cpu();

//  void reset();
//	int  boot();

	void devReset() override;

	//	static cvax_cpu *create(sysDevice *sdev, std::string devName);

protected:
	// Instructions
	void mfpr();
	void mtpr();

	void halt(uint32_t code);
	void check(uint32_t code);
};

DECLARE_DEVICE_TYPE(CVAX, cvax_cpu)
