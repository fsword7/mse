/*
 * cmctl.h - CVAX Memory Controller
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

// CMCTL - Memory Controller Registers
//#define CM_BASE     (REG_BASE + 0x100)     // CMCTL Base Address
////#define CM_NREGS    18                     //    Number of Registers
//#define CM_NREGS    19                     //    Number of Registers
//#define CM_SIZE     (CM_NREGS << 2)        //    Total Bytes
//#define CM_END      (CM_BASE + CM_SIZE)    // End of CMCTL Registers
//#define CM_REG(rn)  cmReg[rn]

#define CM_nCNF00		0	// CMCTL Configuration 0
#define CM_nCNF01		1	// CMCTL Configuration 1
#define CM_nCNF02		2	// CMCTL Configuration 2
#define CM_nCNF03		3	// CMCTL Configuration 3
#define CM_nCNF04		4	// CMCTL Configuration 4
#define CM_nCNF05		5	// CMCTL Configuration 5
#define CM_nCNF06		6	// CMCTL Configuration 6
#define CM_nCNF07		7	// CMCTL Configuration 7
#define CM_nCNF08		8	// CMCTL Configuration 8
#define CM_nCNF09		9	// CMCTL Configuration 9
#define CM_nCNF10		10	// CMCTL Configuration 10
#define CM_nCNF11		11	// CMCTL Configuration 11
#define CM_nCNF12		12	// CMCTL Configuration 12
#define CM_nCNF13		13	// CMCTL Configuration 13
#define CM_nCNF14		14	// CMCTL Configuration 14
#define CM_nCNF15		15	// CMCTL Configuration 15
#define CM_nERR			16	// CMCTL Error Register
#define CM_nCSR			17	// CMCTL Control/Status Register
#define CM_nMSZ			18	// CMCTL Memory Size

#define CM_ERR      CM_REG(CM_nERR)
#define CM_CSR      CM_REG(CM_nCSR)
#define CM_MSZ      CM_REG(CM_nMSZ)

// CMCTL Configuration Register
#define CMCNF_VLD   0x80000000  // Address Valid
#define CMCNF_BA    0x1FF00000  // Base Address
#define CMCNF_LOCK  0x00000040  // Lock
#define CMCNF_SRQ   0x00000020  // Signature Request
#define CMCNF_SIG   0x0000001F  // Signature

#define CMCNF_RW    (CMCNF_VLD|CMCNF_BA)
#define CMCNF_MASK  (CMCNF_RW|CMCNF_SIG)

#define MEM_BANK    (1u << 22) // Bank Size
#define MEM_SIG     0x17       // ECC, 4 x 4 MB

// CMCTL Error Register
#define CMERR_RDS  0x80000000 // Uncorrected Error
#define CMERR_FRQ  0x40000000 // 2nd RDS
#define CMERR_CRD  0x20000000 // CRD Error
#define CMERR_PAG  0x1FFFFC00 // Page Address
#define CMERR_DMA  0x00000100 // DMA Error
#define CMERR_BUS  0x00000080 // Bus Error
#define CMERR_SYN  0x0000007F // Syndrome

#define CMERR_W1C  (CMERR_RDS|CMERR_FRQ|CMERR_CRD|CMERR_DMA|CMERR_BUS)

// CMCTL Control/Status Register
#define CMCSR_PMI  0x00002000 // PMI Speed
#define CMCSR_CRD  0x00001000 // Enable CRD Interrupt
#define CMCSR_FRF  0x00000800 // Force Reference
#define CMCSR_DET  0x00000400 // Dis Error
#define CMCSR_FDT  0x00000200 // Fast Diagnostic
#define CMCSR_DCM  0x00000080 // Diagnostic Mode
#define CMCSR_SYN  0x0000007F // Syndrome

#define CMCSR_RW (CMCSR_PMI|CMCSR_CRD|CMCSR_DET|CMCSR_FDT|CMCSR_DCM|CMCSR_SYN)

class cmctl_device : public device_t
{
public:
	cmctl_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);
	~cmctl_device();

	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	uint32_t read(offs_t offset, int acc);
	void write(offs_t offset, uint32_t data, int acc);

private:
	uint32_t cmReg[32];
};

DECLARE_DEVICE_TYPE(CMCTL, cmctl_device)
