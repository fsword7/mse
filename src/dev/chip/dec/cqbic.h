/*
 * cqbic.h - CVAX Q-bus Interface Chip
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#include "dev/bus/dec/uqbus.h"

#define QBA_REG(reg)  uqReg[reg]

// 1F40 (17500) IPC - Interprocessor Communication Register
// (Up to 4 IPCR registers - 1F40 to 1F48)
// Note: It supports up to 4 processors on the same Q22 bus.

#define IPC_CSR   017500
#define IPC_NREGS 1
#define IPC_IPL   UQ_BR4
#define IPC_VEC   0004

#define IPC_QME   0x8000 // (R/C) Q22Bus Read Non-Existant Memory
#define IPC_INV   0x4000 // (R/?) CAM Invalid
#define IPC_HLT   0x0100 // (R/W) Auxilliary Halt
#define IPC_IE    0x0040 // (R/W) Interrupt Enable
#define IPC_LME   0x0020 // (R/W) Local Memory External
#define IPC_IRQ   0x0001 // (R/W) Interrupt Done (Request)

#define IPC_W1C   (IPC_QME)
#define IPC_RW    (IPC_HLT|IPC_IE|IPC_LME|IPC_IRQ)

// CQBIC Flags
#define CQ_NXMERR 0x80000000 // Write Access Error

// CQ Map Registers
#define MAP_VALID 0x80000000 // Valid Map Entry
#define MAP_PAGE  0x000FFFFF // Memory Page Address
#define MAP_RW    (MAP_VALID|MAP_PAGE)

// CQ BIC Registers
//#define CQBIC_BASE   (REG_BASE) // CQBIC Base Address
//#define CQBIC_NREGS  5          //   Number of Registers
//#define CQBIC_SIZE   (CQBIC_NREGS << 2) // Total Bytes
//#define CQBIC_END    (CQBIC_BASE + CQBIC_SIZE)
#define CQ_REG(reg)  cqRegs[reg]

#define CQ_nSCR  0 // System Configuration Register
#define CQ_nDSER 1 // DMA System Error Register
#define CQ_nMEAR 2 // Master Error Address Register
#define CQ_nSEAR 3 // Slave Error Address Register
#define CQ_nMBR  4 // Map Base Register

#define CQ_SCR   CQ_REG(CQ_nSCR)
#define CQ_DSER  CQ_REG(CQ_nDSER)
#define CQ_MEAR  CQ_REG(CQ_nMEAR)
#define CQ_SEAR  CQ_REG(CQ_nSEAR)
#define CQ_MBR   CQ_REG(CQ_nMBR)

//// CQBIC Q22-Bus Map Registers
//#define CQMAP_BASE   (REG_BASE + 0x8000)
//#define CQMAP_WIDTH  15
//#define CQMAP_SIZE   (1u << CQMAP_WIDTH)
//#define CQMAP_MASK   (CQMAP_SIZE - 1)
//#define CQMAP_END    (CQMAP_BASE + CQMAP_SIZE)
//
//// CQBIC Q22-Bus I/O Space Area
//#define CQIO_BASE   (0x20000000)
//#define CQIO_WIDTH  13
//#define CQIO_SIZE   (1u << CQIO_WIDTH)
//#define CQIO_MASK   (CQIO_SIZE - 1)
//#define CQIO_END    (CQIO_BASE + CQIO_SIZE)
//
//// CQBIC Q22-Bus Memory Space Area
//#define CQMEM_BASE   (0x30000000)
//#define CQMEM_WIDTH  22
//#define CQMEM_SIZE   (1u << CQMEM_WIDTH)
//#define CQMEM_MASK   (CQMEM_SIZE - 1)
//#define CQMEM_END    (CQMEM_BASE + CQMEM_SIZE)

// SCR - System Configuration Register
#define SCR_PWROK   0x00008000 // Power Ok
#define SCR_BHL     0x00004000 // BHALT Enable
#define SCR_AUX     0x00000400 // Aux Mode
#define SCR_DBO     0x0000000C // Offset

#define SCR_RW      (SCR_BHL|SCR_DBO)
#define SCR_MASK    (SCR_RW|SCR_PWROK|SCR_AUX)

// DSER - DMA System Error Register
#define DSER_BHL   0x00008000 // BHALT
#define DSER_DCN   0x00004000 // DC ~OK
#define DSER_MNX   0x00000080 // Master Non-Existant Memory
#define DSER_MPE   0x00000020 // Master Parity Error
#define DSER_SME   0x00000010 // Slave Memory Error
#define DSER_LST   0x00000008 // Lost Error
#define DSER_TMO   0x00000004 // Timeout (No Grant)
#define DSER_SNX   0x00000001 // Slave Non-Existant Memory

#define DSER_ERR   (DSER_MNX|DSER_MPE|DSER_TMO|DSER_SNX)

// MEAR - Master Error Address Register
#define MEAR_ADDR 0x00001FFF // Q22-Bus Page Address

// SEAR - Slave Error Address Register
#define SEAR_ADDR 0x000FFFFF // Memory Address

// MBR - Map Base Register
#define MBR_ADDR  0x1FFF8000 // 32KB-Aligned Map Base Address


class cqbic_device : public device_t
{
public:
	cqbic_device(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock);
	~cqbic_device();

	virtual void devStart() override;
//	virtual void devStop() override;
//	virtual void devReset() override;

	uint32_t read(offs_t offset, int acc);
	void write(offs_t offset, uint32_t data, int acc);

	uint32_t read_map(offs_t offset, int acc);
	void write_map(offs_t offset, uint32_t data, int acc);

	uint32_t read_mem(offs_t offset, int acc);
	void write_mem(offs_t offset, uint32_t data, int acc);

	uint16_t read_io(offs_t offset, int acc);
	void write_io(offs_t offset, uint16_t data, int acc);

	uint16_t read_ipc(offs_t offset, int acc);
	void write_ipc(offs_t offset, uint16_t data, int acc);

private:
	uint32_t cqReg[8];
};

DECLARE_DEVICE_TYPE(CQBIC, cqbic_device)
