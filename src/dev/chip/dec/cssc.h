/*
 * cssc.h - CVAX System Support Chip
 *
 *  Created on: Dec 14, 2018
 *      Author: Tim Stark
 */

#pragma once

// SSC Registers for MicroVAX III series
//#define SSC_BASE    0x20140000             // SSC Base Address
//#define SSC_END     (SSC_BASE + SSC_SIZE)  // End of SSC Registers
//
//#define SSC_NREGS   84                     // Number of SSC Registers
//#define SSC_SIZE    (SSC_NREGS << 2)       //    Total Bytes
//#define SSC_REG(rn) KA650_REG(cpu, sscRegs[rn])

#define SSC_nBASE   0x00 // Base Address
#define SSC_nCR     0x04 // Configuration Register
#define SSC_nBTO    0x08 // CDAL
#define SSC_nDLEDR  0x0C // Display LED Register
#define SSC_nTODR   0x1B // Time of Day Register
#define SSC_nCSRS   0x1C // Console Storage Receive Status
#define SSC_nCSRD   0x1D // Console Storage Receive Data
#define SSC_nCSTS   0x1E // Console Storage Transmit Status
#define SSC_nCSTD   0x1F // Console Storage Transmit Data
#define SSC_nRXCS   0x20 // Receive Control/Status Register
#define SSC_nRXDB   0x21 // Receive Data Buffer
#define SSC_nTXCS   0x22 // Transmit Control/Status Register
#define SSC_nTXDB   0x23 // Transmit Data Buffer
#define SSC_nTCR0   0x40 // Timer 0 Configuration Register
#define SSC_nTIR0   0x41 // Timer 0 Interval Register
#define SSC_nTNIR0  0x42 // Timer 0 Next Interval Register
#define SSC_nTIVR0  0x43 // Timer 0 Interrupt Vector Register
#define SSC_nTCR1   0x44 // Timer 1 Configuration Register
#define SSC_nTIR1   0x45 // Timer 1 Interval Register
#define SSC_nTNIR1  0x46 // Timer 1 Next Interval Register
#define SSC_nTIVR1  0x47 // Timer 1 Interrupt Vector Register
#define SSC_nAD0MAT 0x4C // Address Strobe 0 Match Register
#define SSC_nAD0MSK 0x4D // Address Strobe 0 Mask Register
#define SSC_nAD1MAT 0x50 // Address Strobe 1 Match Register
#define SSC_nAD1MSK 0x51 // Address Strobe 1 Mask Register

#define SSC_xBASE  SSC_REG(SSC_nBASE)   // SSC Base Address
#define SSC_CR     SSC_REG(SSC_nCR)     // SSC Configuration Register
#define SSC_BTO    SSC_REG(SSC_nBTO)    // SSC Bus Timeout Register
#define SSC_LED    SSC_REG(SSC_nDLEDR)  // SSC Display LED Register
#define SSC_TCR0   SSC_REG(SSC_nTCR0)   // SSC Timer #0 - Config Register
#define SSC_TIR0   SSC_REG(SSC_nTIR0)   // SSC Timer #0 - Interval Register
#define SSC_TNIR0  SSC_REG(SSC_nTNIR0)  // SSC Timer #0 - Next Interval Reg
#define SSC_TIVR0  SSC_REG(SSC_nTIVR0)  // SSC Timer #0 - Int Vec Register
#define SSC_TCR1   SSC_REG(SSC_nTCR1)   // SSC Timer #1 - Config Register
#define SSC_TIR1   SSC_REG(SSC_nTIR1)   // SSC Timer #1 - Interval Register
#define SSC_TNIR1  SSC_REG(SSC_nTNIR1)  // SSC Timer #1 - Next Interval Reg
#define SSC_TIVR1  SSC_REG(SSC_nTIVR1)  // SSC Timer #1 - Int Vec Register
#define SSC_AD0MAT SSC_REG(SSC_nAD0MAT) // SSC Address Strobe #0 Match
#define SSC_AD0MSK SSC_REG(SSC_nAD0MSK) // SSC Address Strobe #0 Mask
#define SSC_AD1MAT SSC_REG(SSC_nAD1MAT) // SSC Address Strobe #1 Match
#define SSC_AD1MSK SSC_REG(SSC_nAD1MSK) // SSC Address Strobe #1 Mask

#define SSC_TCR(tmr)  SSC_REG(SSC_nTCR0 + ((tmr) * SSC_TMR))
#define SSC_TIR(tmr)  SSC_REG(SSC_nTIR0 + ((tmr) * SSC_TMR))
#define SSC_TNIR(tmr) SSC_REG(SSC_nTNIR0 + ((tmr) * SSC_TMR))
#define SSC_TIVR(tmr) SSC_REG(SSC_nTIVR0 + ((tmr) * SSC_TMR))

// SSC - Base Register
#define BASE_ADDR  0x1FFFFC00 // SSC Base Address
#define BASE_MBO   0x20000000 //   Must Be One

// SSC - Configuration Register
#define CNF_BLO    0x80000000 // Low Battery
#define CNF_IVD    0x08000000
#define CNF_IPL    0x03000000
#define CNF_ROM    0x00F70000 // ROM Parameters
#define CNF_CTLP   0x00008000 // CTRL-P Enable
#define CNF_BAUD   0x00007700 // Baud Rates
#define CNF_ADR    0x00000077 // Address

#define CNF_W1C    (CNF_BLO)
#define CNF_RW     0x0BF7F777

// SSC - Bus Timeout Register
#define BTO_BTO    0x80000000 // Bus Timeout
#define BTO_RWT    0x40000000 // Read/Write Access
#define BTO_INTV   0x00FFFFFF

#define BTO_W1C    (BTO_BTO|BTO_RWT)
#define BTO_RW     (BTO_INTV)

// SSC - Display LED Register
#define LED_MASK   0x0000000F

// SSC - Timer Registers
#define TCR_ERR    0x80000000 // Timer Error
#define TCR_DONE   0x00000080 // Done
#define TCR_IE     0x00000040 // Interrupt Enable
#define TCR_STEP   0x00000020 // Single Step
#define TCR_XFER   0x00000010 // Transfer
#define TCR_STOP   0x00000004 // Stop
#define TCR_RUN    0x00000001 // Run
#define TCR_W1C    (TCR_ERR|TCR_DONE)
#define TCR_RW     (TCR_IE|TCR_STOP|TCR_RUN)

#define TIVR_VEC   0x3FC      // Interrupt Vector Mask

#define TMR_NREGS  2      // Number of Registers
#define TMR0_VEC   0x78   // Timer #0 Vector Address
#define TMR1_VEC   0x7C   // Timer #1 Vector Address
#define TMR_IPL    UQ_BR4 // Interrupt Priority Level
#define TMR_PRIO   25     // Device Priority
#define TMR_TICK   1000   // Each 1 kHz
#define SSC_TMR    4      // Timer Index
#define SSC_TMR0   0
#define SSC_TMR1   4

// SSC - Address Strobes
#define ADS_MASK   0x3FFFFFFC


class cssc_device : public device_t
{
public:
	cssc_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);
	~cssc_device();

	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	uint32_t read(offs_t offset, int acc);
	void write(offs_t offset, uint32_t data, int acc);

private:
	uint32_t sscReg[0x400];
};

DECLARE_DEVICE_TYPE(CSSC, cssc_device)
