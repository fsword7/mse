/*
 * 21272.h - DEC Alpha - 21272/21274 (Tsunami/Typhoon) chipset package
 *
 *  Created on: Dec 20, 2020
 *      Author: Tim Stark
 */

#pragma once

// System memory map
//
// Space				Size		System Address <43:0>				Comments
// Memory    			4GB			000.0000.0000 - 000.FFFF.FFFF		Cacheable and prefechable
// Reserved  			8188GB		001.0000.0000 - 7FF.FFFF.FFFF
//
// Pchip0 PCI			4GB			800.0000.0000 - 800.FFFF.FFFF		Linear addressing
// TIGbus				1GB			801.0000.0000 - 801.3FFF.FFFF		addr<5:0> = 0, single byte valid
//																		in quadword access 16MB accessible.
// Reserved				1GB			801.4000.0000 - 801.7FFF.FFFF
// Pchip0 CSRs			256MB		801.8000.0000 - 801.8FFF.FFFF		addr<5:0> = 0, quadword access
// Reserved				256MB		801.9000.0000 - 801.9FFF.FFFF
// Cchip CSRs			256MB		801.A000.0000 - 801.AFFF.FFFF		addr<5:0> = 0, quadword access
// Dchip CSRs			256MB		801.B000.0000 - 801.BFFF.FFFF		addr<5:0> = 0, all 8 bytes in quadword
//																		access must be identical.
// Reserved				768MB		801.C000.0000 - 801.EFFF.FFFF
// Reserved				128MB		801.F000.0000 - 801.F7FF.FFFF
// Pchio0 PCI IACK		64MB		801.F800.0000 - 801.FBFF.FFFF		Linear addressing
// Pchip0 PCI I/O		32MB		801.FC00.0000 - 801.FDFF.FFFF		Linear addressing
// Pchip0 PCI cfg		16MB		801.FE00.0000 - 801.FEFF.FFFF		Linear addressing
// Reserved				16MB		801.FF00.0000 - 801.FFFF.FFFF
//
// Pchip1 PCI			4GB			802.0000.0000 - 802.FFFF.FFFF		Linear addressing
// Reserved				2GB			803.0000.0000 - 803.7FFF.FFFF
// Pchip1 CSRs			256MB		803.8000.0000 - 803.8FFF.FFFF		addr<5:0> = 0, quadword access.
// Reserved				1536MB		803.9000.0000 - 803.EFFF.FFFF
// Reserved				128MB		803.F000.0000 - 803.F7FF.FFFF
// Pchip1 PCI IACK		64MB		803.F800.0000 - 803.FBFF.FFFF		Linear addressing
// Pchip1 PCI I/O		32MB		803.FC00.0000 - 803.FDFF.FFFF		Linear addressing
// Pchip1 PCI cfg		16MB		803.FE00.0000 - 803.FEFF.FFFF		Linear addressing
// Reserved				16MB		803.FF00.0000 - 803.FFFF.FFFF

// Reserved  			8172GB		804.0000.0000 - FFF.FFFF.FFFF		Bits <42:35> are don't cares if bit
//																		<43> is asserted.

// TIGBus memory map
//
// Flash ROM space		2MB			801.0000.0000 - 801.07FF.FFFF		addr<5:0> = 0, 21-bit addressing


#define CCHIP_CSC		0x0000	// (RW) CSC Chip System Configuration (HRM Section 10.2.2)
#define CCHIP_MTR		0x0040	// (RW) MTR
#define CCHIP_MISC		0x0080	// (RW) MISC
#define CCHIP_MPD		0x00C0	// (RW) MPD
#define CCHIP_AAR0		0x0100	// (RW) AAR0
#define CCHIP_AAR1		0x0140	// (RW) AAR1
#define CCHIP_AAA2		0x0180	// (RW) AAR2
#define CCHIP_AAR3		0x01C0	// (RW) AAR3
#define CCHIP_DIM0		0x0200	// (RW) DIM0
#define CCHIP_DIM1		0x0240	// (RW) DIM1
#define CCHIP_DIR0		0x0280	// (RO) DIR0
#define CCHIP_DIR1		0x02C0	// (RO) DIR1
#define CCHIP_DRIR		0x0300	// (RO) DRIR
#define CCHIP_PRBEN		0x0340	//      PRBEN
#define CCHIP_IIC0		0x0380	// (RW) IIC0
#define CCHIP_IIC1		0x03C0	// (RW) IIC1
#define CCHIP_MPR0		0x0400	// (WO) MPR0
#define CCHIP_MPR1		0x0440	// (WO) MPR1
#define CCHIP_MPR2		0x0480	// (WO) MPR2
#define CCHIP_MPR3		0x04C0	// (WO) MPR3
// Reserved				0x0500	// (RW)
#define CCHIP_TTR		0x0580	// (RW) TTR
#define CCHIP_TDR		0x05C0	// (RW) TDR

// Typhoon only
#define CCHIP_DIM2		0x0600	// (RW) DIM2
#define CCHIP_DIM3		0x0640	// (RW) DIM3
#define CCHIP_DIR2		0x0680	// (RO)	DIR2
#define CCHIP_DIR3		0x06C0	// (RO) DIR3
#define CCHIP_IIC2		0x0700	// (RW) IIC2
#define CCHIP_IIC3		0x0740	// (RW) IIC3
#define CCHIP_PWR		0x0780	// (RW) PWR
#define CCHIP_CMONCTLA	0x0C00	// (RW) CMONCTLA
#define CCHIP_CMONCTLB	0x0C40	// (RW) CMONCTLB
#define CCHIP_CMONCNT01	0x0C80	// (RO) CMONCNT01
#define CCHIP_CMONCNT02	0x0CC0	// (RO) CMONCNT02

#define DCHIP_DSC		0x0800	// (RO) DSC
#define DCHIP_STR		0x0840	// (RW) STR
#define DCHIP_DREV		0x0880	// (RW) DREV
#define DCHIP_DSC2		0x08C0	// (RO) DSC2

#define PCHIP_WSBA0		0x0000	// (RW) WSBA0
#define PCHIP_WSBA1		0x0040	// (RW) WSBA1
#define PCHIP_WSBA2		0x0080	// (RW) WSBA2
#define PCHIP_WSBA3		0x00C0	// (RW) WSBA3
#define PCHIP_WSM0		0x0100	// (RW) WSM0
#define PCHIP_WSM1		0x0140	// (RW) WSM1
#define PCHIP_WSM2		0x0180	// (RW) WSM2
#define PCHIP_WSM3		0x01C0	// (RW) WSM3
#define PCHIP_TBA0		0x0200	// (RW) TBA0
#define PCHIP_TBA1		0x0240	// (RW) TBA1
#define PCHIP_TBA2		0x0280	// (RW) TBA2
#define PCHIP_TBA3		0x02C0	// (RW) TBA3
#define PCHIP_PCTL		0x0300	// (RW) PCTL
#define PCHIP_PLAT		0x0340	// (RW) PLAT
#define PCHIP_RES		0x0380	// (RW) RES
#define PCHIP_PERROR	0x03C0	// (RW) PERROR
#define PCHIP_PERRMASK	0x0400	// (RW) PERRMASK
#define PCHIP_PERRSET	0x0440	// (WO) PERRSET
#define PCHIP_TLBIV		0x0480	// (WO) TLBIV
#define PCHIP_TLBIA		0x04C0	// (WO) TLBIA
#define PCHIP_PMONCTL	0x0500	// (RW) PMONCTL
#define PCHIP_PMONCNT	0x0540	// (RO) PMONCNT
#define PCHIP_SPRST		0x0800	// (WO)	SPRST


#define PCI0_BASE			0x800'0000'0000LL		// PCI 0 base address
#define PCI1_BASE			0x802'0000'0000LL		// PCI 1 base address

#define TIG_BASE			0x001'0000'0000LL		// TIGbus base address

#define PCHIP_CSR_BASE		0x001'8000'0000LL		// Pchip CSR base address
#define PCHIP_IACK_BASE		0x001'F800'0000LL		// Pchip PCI IACK base address
#define PCHIP_IO_BASE		0x001'FC00'0000LL		// Pchip PCI I/O base address
#define PCHIP_CFG_BASE		0x001'FE00'0000LL		// Pchip PCI configuration base address

#define DCHIP_CSR_BASE		0x001'8000'0000LL		// Dchip CSR base address

#define FLASH_ADDR			(PCI0_BASE|TIG_BASE)	// 2MB flash ROM space (801.0000.0000)
#define FLASH_SIZE			(1u << 21)				//   Flash ROM size (2MB space)
#define FLASH_MASK			(FLASH_SIZE - 1)		//   Address mask

class axp21272_pciDevice : public DeviceInterface
{
public:

	axp21272_pciDevice() = default;
	~axp21272_pciDevice() = default;

	struct cchip_t
	{
		uint64_t dim[4]; // DIMx - Device interrupt mask register
		uint64_t drir;   // DRIR - Device interrupt request register
		uint64_t misc;   // MISC - Miscellaneous register (R/W)
		uint64_t csc;    //
	};

	struct dchip_t
	{
		uint8_t  drev;
		uint8_t  dsc;
		uint8_t  dsc2;
		uint8_t  str;
	};

	struct pchip_t
	{
		uint64_t wsba[4];	// (R/W) WSBAn    - Window Space Base Address Register
		uint64_t wsm[4];	// (R/W) WSMn     - Window Space Mask Register
		uint64_t tba[4];	// (R/W) TBAn     - Translated Base Address Register
		uint64_t pctl;		// (R/W) PCTL     - Pchip Control Register
		uint64_t plat;		// (R/W) PLAT     - Pchip Master Latency Register
		uint64_t perr;		// (R/W) PERROR   - Pchip Error Register
		uint64_t perrmask;	// (R/W) PERRMASK - Pchip Error Mask Register
	};

	// Read access function calls
	uint64_t ccRead(uint32_t pAddr, cpuDevice *cpu = nullptr);  // Cchip address space
	uint64_t pcRead0(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI 0 address space
	uint64_t pcRead1(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI 1 address space
	uint8_t  dcRead(uint32_t pAddr, cpuDevice *cpu = nullptr);  // Dchip address space
	uint8_t  tigRead(uint32_t pAddr, cpuDevice *cpu = nullptr); // Flash ROM, interrupts, etc.

	template <int pcIndex>
	uint64_t pcRead(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI address space

	// Write access function calls
	void     ccWrite(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr);  // Cchip address space
	void     pcWrite0(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI 0 address space
	void     pcWrite1(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI 1 address space
	void     dcWrite(uint32_t pAddr, uint8_t data, cpuDevice *cpu = nullptr);   // Dchip address space
	void     tigWrite(uint32_t pAddr, uint8_t data, cpuDevice *cpu = nullptr);  // Flash ROM, interrupts, etc

	template <int pcIndex>
	void     pcWrite(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI address space


private:

	struct {
		cchip_t  cchip;
		dchip_t  dchip;
		pchip_t  pchip[2];
		uint32_t cf8addr[2];
	} state;
};
