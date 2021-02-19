/*
 * 21174.h - DEC 21174 logic chipset package
 *
 *  Created on: Feb 19, 2021
 *      Author: Tim Stark
 */

#pragma once

// SYSTEM MEMORY MAP
//
// Note: Address bit 37 and 38 by 21164 are ignored. 80.000.000 - 8F.FFF.FFFF space
// are aliases (repeating). Write/read accesses to flash ROM must be done through
// byte instructions.  All other transactions produces undefined results.
//
// System memory map (byte/word mode disabled)
//
// 00.0000.0000 - 01.FFFF.FFFF - Main memory (up to 8 GB memory space)
// 0E.0000.0000 - 0E.FFFF.FFFF - Dummy memory region
// 80.0000.0000 - 93.FFFF.FFFF - PCI sparse memory region 0 (512 MB space)
// 84.0000.0000 - 84.FFFF.FFFF - PCI sparse memory region 1 (128 MB space)
// 85.0000.0000 - 85.7FFF.FFFF - PCI sparse memory region 2 (64 MB space)
// 85.8000.0000 - 85.BFFF.FFFF - PCI sparse I/O space region A (32 MB space)
// 85.C000.0000 - 85.FFFF.FFFF - PCI sparse I/O space region B (32 MB space)
// 86.0000.0000 - 86.FFFF.FFFF - PCI dense memory
// 87.0000.0000 - 87.1FFF.FFFF - PCI sparse configuration space
// 87.2000.0000 - 87.3FFF.FFFF - PCI special/interrupt acknowledge
// 87.4000.0000 - 87.4FFF.FFFF - 21174 main CSR space
// 87.5000.0000 - 87.5FFF.FFFF - 21774 memory control CSR space
// 87.6000.0000 - 87.6FFF.FFFF - 21174 PCI address translation
// 87.7000.0000 - 87.7FFF.FFFF - Reserved
// 87.8000.0000 - 87.8FFF.FFFF - 21174 miscellaneous CSR space
// 87.9000.0000 - 87.9FFF.FFFF - 21174 power management CSR space
// 87.A000.0000 - 87.AFFF.FFFF - 21174 interrupt control CSR space
// 87.B000.0000 - 87.BFFF.FFFF - Reserved
// 87.C000.0000 - 87.FFFF.FFFF - Flash ROM (byte/word access only,
//                               otherwise - undefined results)
//
// 88.0000.0000 - 88.FFFF.FFFF - PCI memory space INT8
// 98.0000.0000 - 98.FFFF.FFFF - PCI memory space INT4
// A8.0000.0000 - A8.FFFF.FFFF - PCI memory space INT2
// B8.0000.0000 - B8.FFFF.FFFF - PCI memory space INT1
//
// 89.0000.0000 - 89.FFFF.FFFF - PCI I/O space INT8
// 99.0000.0000 - 99.FFFF.FFFF - PCI I/O space INT4
// A9.0000.0000 - A9.FFFF.FFFF - PCI I/O space INT2
// B9.0000.0000 - B9.FFFF.FFFF - PCI I/O space INT1
//
// 8A.0000.0000 - 8A.FFFF.FFFF - PCI configuration 0 INT8
// 9A.0000.0000 - 9A.FFFF.FFFF - PCI configuration 0 INT4
// AA.0000.0000 - AA.FFFF.FFFF - PCI configuration 0 INT2
// BA.0000.0000 - BA.FFFF.FFFF - PCI configuration 0 INT1
//
// 8B.0000.0000 - 8B.FFFF.FFFF - PCI configuration 1 INT8
// 9B.0000.0000 - 9B.FFFF.FFFF - PCI configuration 1 INT4
// AB.0000.0000 - AB.FFFF.FFFF - PCI configuration 1 INT2
// BB.0000.0000 - BB.FFFF.FFFF - PCI configuration 1 INT1
//
// C7.C000.0000 - C7.FFFF.FFFF - Flash ROM read/write space

class axp21174_pciDevice : public DeviceInterface
{
public:

	axp21174_pciDevice() = default;
	~axp21174_pciDevice() = default;

//	struct cchip_t
//	{
//		uint64_t dim[4]; // DIMx - Device interrupt mask register
//		uint64_t drir;   // DRIR - Device interrupt request register
//		uint64_t misc;   // MISC - Miscellaneous register (R/W)
//		uint64_t csc;    //
//	};
//
//	struct dchip_t
//	{
//		uint8_t  drev;
//		uint8_t  dsc;
//		uint8_t  dsc2;
//		uint8_t  str;
//	};
//
//	struct pchip_t
//	{
//		uint64_t wsba[4];	// (R/W) WSBAn    - Window Space Base Address Register
//		uint64_t wsm[4];	// (R/W) WSMn     - Window Space Mask Register
//		uint64_t tba[4];	// (R/W) TBAn     - Translated Base Address Register
//		uint64_t pctl;		// (R/W) PCTL     - Pchip Control Register
//		uint64_t plat;		// (R/W) PLAT     - Pchip Master Latency Register
//		uint64_t perr;		// (R/W) PERROR   - Pchip Error Register
//		uint64_t perrmask;	// (R/W) PERRMASK - Pchip Error Mask Register
//	};

//	// Read access function calls
//	uint64_t ccRead(uint32_t pAddr, cpuDevice *cpu = nullptr);  // Cchip address space
//	uint64_t pcRead0(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI 0 address space
//	uint64_t pcRead1(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI 1 address space
//	uint8_t  dcRead(uint32_t pAddr, cpuDevice *cpu = nullptr);  // Dchip address space
//	uint8_t  tigRead(uint32_t pAddr, cpuDevice *cpu = nullptr); // Flash ROM, interrupts, etc.
//
//	template <int pcIndex>
//	uint64_t pcRead(uint32_t pAddr, cpuDevice *cpu = nullptr); // Pchip PCI address space
//
//	// Write access function calls
//	void     ccWrite(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr);  // Cchip address space
//	void     pcWrite0(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI 0 address space
//	void     pcWrite1(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI 1 address space
//	void     dcWrite(uint32_t pAddr, uint8_t data, cpuDevice *cpu = nullptr);   // Dchip address space
//	void     tigWrite(uint32_t pAddr, uint8_t data, cpuDevice *cpu = nullptr);  // Flash ROM, interrupts, etc
//
//	template <int pcIndex>
//	void     pcWrite(uint32_t pAddr, uint64_t data, cpuDevice *cpu = nullptr); // PCI address space

private:
};
