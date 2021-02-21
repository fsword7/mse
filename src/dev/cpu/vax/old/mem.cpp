/*
 * mem.cpp
 *
 *  Created on: Mar 24, 2017
 *      Author: Timothy Stark
 *
 *  Memory access routines
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/opcodes.h"

#define MEMB(addr) ((uint8_t *)mem)[addr]
#define MEMW(addr) ((uint16_t *)mem)[addr]
#define MEML(addr) ((uint32_t *)mem)[addr]

static const uint32_t align[] =
	{ 0xFFFFFFFF, 0x00FFFFFF, 0x0000FFFF, 0x000000FF };
static const uint32_t insert[] =
	{ 0x00000000, 0x000000FF, 0x0000FFFF, 0x00FFFFFF };

// Virtual Memory Space
//
// 00000000 +-------------------------------------------+
//          |      |   User Process 0 Space      |      |
//          |- - - v - - - - - - - - - - - - - - v - - -|
//          |                                           |
// 40000000 +-------------------------------------------+
//          |                                           |
//          |- - - ^ - - - - - - - - - - - - - - ^ - - -|
//          |      |    User Process 1 Space     |      |
// 80000000 +-------------------------------------------+
//          |      |    System Memory Space      |      |
//          |- - - v - - - - - - - - - - - - - - v - - -|
//          |                                           |
// C0000000 +-------------------------------------------+
//          |             Reserved Area                 |
// FFFFFFFF +-------------------------------------------+

// Translation Access Table (Protection Bits)

// TLB Access Mask
#define TLB_ACCR(mask)  ((mask) << TLB_P_RDACC)
#define TLB_ACCW(mask)  ((mask) << TLB_P_WRACC)

// Access Mode
#define KERN            AM_MASK(AM_KERNEL)
#define EXEC            AM_MASK(AM_EXECUTIVE)
#define SUPR            AM_MASK(AM_SUPERVISOR)
#define USER            AM_MASK(AM_USER)

static const uint32_t accTable[16] =
{
//	Read Access                     Write Access
//	-----------                     ------------
	0                             | 0,
	0                             | 0,
	TLB_ACCR(KERN)                | TLB_ACCW(KERN),
	TLB_ACCR(KERN)                | 0,
	TLB_ACCR(KERN|EXEC|SUPR|USER) | TLB_ACCW(KERN|EXEC|SUPR|USER),
	TLB_ACCR(KERN|EXEC)           | TLB_ACCW(KERN|EXEC),
	TLB_ACCR(KERN|EXEC)           | TLB_ACCW(KERN),
	TLB_ACCR(KERN|EXEC)           | 0,
	TLB_ACCR(KERN|EXEC|SUPR)      | TLB_ACCW(KERN|EXEC|SUPR),
	TLB_ACCR(KERN|EXEC|SUPR)      | TLB_ACCW(KERN|EXEC),
	TLB_ACCR(KERN|EXEC|SUPR)      | TLB_ACCW(KERN),
	TLB_ACCR(KERN|EXEC|SUPR)      | 0,
	TLB_ACCR(KERN|EXEC|SUPR|USER) | TLB_ACCW(KERN|EXEC|SUPR),
	TLB_ACCR(KERN|EXEC|SUPR|USER) | TLB_ACCW(KERN|EXEC),
	TLB_ACCR(KERN|EXEC|SUPR|USER) | TLB_ACCW(KERN),
	TLB_ACCR(KERN|EXEC|SUPR|USER) | 0,
};

static const tlb_t tlbZero = { 0, 0 };

static const char *pftMessages[] =
{
	"Access Control Violation",
	"Length Not Valid",
	"PTE Access Control Violation",
	"PTE Length Not Valid",
	"Translation Not Valid",
	"<Unknown Code 05>",
	"PTE Translation Not Valid",
	"Successful"
};

// ***********************************************************

// Aligned longword read access with physical address
uint32_t vax_cpu_base::readpl(uint32_t pAddr)
{
//	pAddr &= paMask;
//	if (pAddr < memSize)
//		return MEML(pAddr >> 2);
//	return sdev->readio(this, pAddr, LN_LONG);

	mchkAddr = pAddr;
	mchkRef  = REF_P;
	return mapProgram->read32(pAddr);
}

// Aligned longword read access with physical address
void vax_cpu_base::writepl(uint32_t pAddr, uint32_t data)
{
//	pAddr &= paMask;
//	if (pAddr < memSize)
//		MEML(pAddr >> 2) = data;
//	sdev->writeio(this, pAddr, data, LN_LONG);

	mchkAddr = pAddr;
	mchkRef  = REF_P;
	mapProgram->write32(pAddr, data);
}

// Aligned read access with physical address
uint32_t vax_cpu_base::readp(uint32_t pAddr, int size)
{
//	pAddr &= paMask;
//	if (pAddr < memSize) {
//		if (size == LN_LONG)
//			return MEML(pAddr >> 2);
//		if (size == LN_WORD)
//			return MEMW(pAddr >> 1);
//		return MEMB(pAddr);
//	}
//	return sdev->readio(this, pAddr, size);

	mchkRef = REF_V;
	if (size == LN_LONG)
		return mapProgram->read32(pAddr);
	else if (size == LN_WORD)
		return mapProgram->read16(pAddr);
	else
		return mapProgram->read8(pAddr);
}

// Aligned write access with physical address
void vax_cpu_base::writep(uint32_t pAddr, uint32_t data, int size)
{
//	pAddr &= paMask;
//	if (pAddr < memSize) {
//		if (size == LN_LONG)
//			MEML(pAddr >> 2) = data;
//		if (size == LN_WORD)
//			MEMW(pAddr >> 1) = data;
//		else
//			MEMB(pAddr) = data;
//	}
//	sdev->writeio(this, pAddr, data, size);

	mchkRef = REF_V;
	if (size == LN_LONG)
		mapProgram->write32(pAddr, data);
	else if (size == LN_WORD)
		mapProgram->write16(pAddr, data);
	else
		mapProgram->write8(pAddr, data);
}

void vax_cpu_base::cleartlb(bool sysFlag)
{
	for (int idx = 0; idx < TLB_SIZE; idx++) {
		// Clear all process TLB entries
		tlbProcess[idx].tag = -1;
		tlbProcess[idx].pte = -1;
		// Clear all system TLB entries
		if (sysFlag == true) {
			tlbSystem[idx].tag = -1;
			tlbSystem[idx].pte = -1;
		}
	}
}

void vax_cpu_base::cleartlb(uint32_t vAddr)
{
	uint32_t vpn = VA_GETVPN(vAddr);
	uint32_t tbi = VA_GETTBI(vpn);

	if (vAddr & VA_ADDR_S0) {
		tlbSystem[tbi].tag = -1;
		tlbSystem[tbi].pte = -1;
	} else {
		tlbProcess[tbi].tag = -1;
		tlbProcess[tbi].pte = -1;
	}
}

bool vax_cpu_base::checktlb(uint32_t vAddr)
{
	uint32_t vpn = VA_GETVPN(vAddr);
	uint32_t tbi = VA_GETTBI(vpn);

	if (vAddr & VA_ADDR_S0)
		return (tlbSystem[tbi].tag == vpn);
	else
		return (tlbProcess[tbi].tag == vpn);
}

tlb_t vax_cpu_base::pagefault(uint32_t errCode, uint32_t vAddr, uint32_t acc, uint32_t *sts) noexcept(false)
{
#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_EXCEPTION))
		dbg.log("%s: (PFT) Page fault on %08X at PC %08X (%s)\n", deviceName(),
			vAddr, faultAddr, (errCode < 8) ? pftMessages[errCode] : "<Unknown>");
#endif /* ENABLE_DEBUG */

	if (sts != nullptr) {
		*sts = errCode;
		return tlbZero;
	}

	// During interrupt/exception routine,
	// aborting as kernel stack not valid
	if (flags & CPU_INIE) {
		flags &= ~CPU_INIE;
		throw PAGE_KSNV;
	}

	// Set parameter registers
	paCount = 2;
	paReg[0] = ((acc & TLB_WRACC) ? MM_WRITE : 0) | (errCode & MM_EMASK);
	paReg[1] = vAddr;

	throw ((errCode & MM_TNV) ? PAGE_TNV : PAGE_ACV);
}

tlb_t vax_cpu_base::translate(uint32_t vAddr, int size, uint32_t acc, uint32_t *sts)
{
	uint32_t pteAddr;
	uint32_t pteIndex = (vAddr & VA_VPN) >> 7;
	uint32_t pte, tlb, tbi, vpn;

	if (vAddr & VA_ADDR_S0) {
		// Virtual System Region Area
//		printf("%s: System Space: Base %08X + (%08X < %08X) => %08X\n", deviceName(),
//			IPR_SBR, pteIndex, IPR_SLR << 2, (IPR_SBR + pteIndex));
		if (pteIndex >= (IPR_SLR << 2))
			return pagefault(MM_LNV, vAddr, acc, sts);
		pteAddr = IPR_SBR + pteIndex;
	} else {
		if (vAddr & VA_ADDR_P1) {
			// Virtual Process P1 Region Area
			if (pteIndex < (IPR_P1LR << 2))
				return pagefault(MM_LNV, vAddr, acc, sts);
			pteAddr = IPR_P1BR + pteIndex;
		} else {
			// Virtual Process P0 Region Area
			if (pteIndex >= (IPR_P0LR << 2))
				return pagefault(MM_LNV, vAddr, acc, sts);
			pteAddr = IPR_P0BR + pteIndex;
		}

		// Break PTE address for TLB entry
		vpn = VA_GETVPN(pteAddr);
		tbi = VA_GETTBI(vpn);

		if (tlbSystem[tbi].tag != vpn) {
			pteIndex = ((pteAddr & VA_VPN) >> 7);

			if (pteIndex >= (IPR_SLR << 2))
				return pagefault(MM_PLNV, vAddr, acc, sts);
			pte = readpl(IPR_SBR + pteIndex);

			if ((pte & PTE_PROT) == 0)
				return pagefault(MM_PACV, vAddr, acc, sts);
			if ((pte & PTE_V) == 0)
				return pagefault(MM_PTNV, vAddr, acc, sts);

			// Successful, update TLB entry
			tlbSystem[tbi].tag = vpn;
			tlbSystem[tbi].pte = ((pte << PAG_WIDTH) & TLB_PFN) |
				accTable[PTE_GETACC(pte)];
		}

		// Translate PTE address into physical address.
		pteAddr = (tlbSystem[tbi].pte & TLB_PFN) | VA_GETOFF(pteAddr);
	}

	// Get PTE entry and generate new TLB entry
	pte = readpl(pteAddr);
	tlb = ((pte << PAG_WIDTH) & TLB_PFN) |
		accTable[PTE_GETACC(pte)];

//	printf("%s: PTE %08X (on %08X) => TLB %08X\n", deviceName(),
//		pte, pteAddr, tlb);

	// Check legal access
	if ((tlb & acc) == 0) {
//		printf("%s: TLB %08X ACC %08X\n", deviceName(), tlb, acc);
		return pagefault(MM_ACV, vAddr, acc, sts);
	}
	if ((pte & PTE_V) == 0)
		return pagefault(MM_TNV, vAddr, acc, sts);

	// Mark PTE as modified if access is writable.
	if (acc & TLB_WRACC) {
		if ((pte & PTE_M) == 0)
			writepl(pteAddr, pte | PTE_M);
//		tlb |= PTE_M;
	}

	// Break virtual address
	vpn = VA_GETVPN(vAddr);
	tbi = VA_GETTBI(vpn);

//	printf("%s: TLB %08X, Address %08X => %08X %08X\n", deviceName(),
//		tlb, vAddr, vpn, tbi);

	// Update TLB cache and return TLB entry
	if (vAddr & VA_ADDR_S0) {
		tlbSystem[tbi].tag = vpn;
		tlbSystem[tbi].pte = tlb;
		return tlbSystem[tbi];
	} else {
		tlbProcess[tbi].tag = vpn;
		tlbProcess[tbi].pte = tlb;
		return tlbProcess[tbi];
	}
}

uint32_t vax_cpu_base::probev(uint32_t vAddr, uint32_t acc, uint32_t *sts)
{
	uint32_t vpn, off, tbi;
	tlb_t    tlb;

	*sts = MM_OK;
	if ((IPR_MAPEN & 1) == 0)
		return vAddr;
//		return vAddr & paMask;

	// Break virtual address down
	vpn = VA_GETVPN(vAddr);
	tbi = VA_GETTBI(vpn);
	off = VA_GETOFF(vAddr);

	// Lookup translation buffer first. If not found,
	// translate virtual address into TLB entry and physical
	// address.
	tlb = (vAddr & VA_ADDR_S0) ? tlbSystem[tbi] : tlbProcess[tbi];
	if (((tlb.pte & acc) == 0) || (tlb.tag != vpn))
		tlb = translate(vAddr, LN_BYTE, acc, sts);

//	printf("%s: TLB (%08X) %08X %08X => V %08X P %08X\n", deviceName(),
//			tbi, tlb.tag, tlb.pte, vAddr, ((tlb.pte & TLB_PFN) | off));

	return (*sts == MM_OK) ? ((tlb.pte & TLB_PFN) | off) : MM_FAIL;
}

uint32_t vax_cpu_base::readv(uint32_t vAddr, uint32_t size, uint32_t acc)
{
	uint32_t pAddr, pAddr1;
	uint32_t dl, dh;
	uint32_t vpn, tbi, off;
	tlb_t    tlb;
	int      sc;

	mchkAddr = vAddr;
	if (IPR_MAPEN & 1) {
		// Break virtual address down
		vpn = VA_GETVPN(vAddr);
		tbi = VA_GETTBI(vpn);
		off = VA_GETOFF(vAddr);

		// Translate virtual address into physical address
		tlb = (vAddr & VA_ADDR_S0) ? tlbSystem[tbi] : tlbProcess[tbi];
		if (((tlb.pte & acc) == 0) || (tlb.tag != vpn) ||
			((acc & TLB_WRACC) && ((tlb.pte & TLB_M) == 0)))
			tlb = translate(vAddr, size, acc);
		pAddr = (tlb.pte & TLB_PFN) | off;
	} else
		pAddr = vAddr;

//	printf("mem: Read %08X (scale %d)\n", pAddr, size);

	// Check physical address is aligned
	if ((pAddr & (size - 1)) == 0) {
//		printf("mem: Aligned %08X => %08X\n", pAddr, readp(pAddr, size));
		return readp(pAddr, size);
	}

	if ((IPR_MAPEN & 1) && ((off + size) >= PAG_SIZE)) {
		// Break virtual address down
		vpn = VA_GETVPN(vAddr + LN_LONG);
		tbi = VA_GETTBI(vpn);
		off = VA_GETOFF(vAddr + LN_LONG);

		// Translate virtual address into physical address
		tlb = (vAddr & VA_ADDR_S0) ? tlbSystem[tbi] : tlbProcess[tbi];
		if (((tlb.pte & acc) == 0) || (tlb.tag != vpn) ||
			((acc & TLB_WRACC) && ((tlb.pte & TLB_M) == 0)))
			tlb = translate(vAddr + LN_LONG, size, acc);
		pAddr1 = (tlb.pte & TLB_PFN) | off;
	} else
		pAddr1 = (pAddr + size);

	// Read 16/32-bit data in unaligned address
	// for crossing a page, etc.
	if (size < LN_LONG) {
		if ((pAddr & 03) == 1)
			return uint16_t(readpl(pAddr) >> 8);
		else {
			dl = readpl(pAddr);
			dh = readpl(pAddr1);
			return uint16_t((dl >> 24) | (dh << 8));
		}
	} else {
		dl = readpl(pAddr);
		dh = readpl(pAddr1);
		sc = (pAddr & 03) << 3;
		return (dl >> sc) | (dh << (32 - sc));
	}

	return 0;
}

void vax_cpu_base::writev(uint32_t vAddr, uint32_t data, uint32_t size, uint32_t acc)
{
	uint32_t pAddr, pAddr1;
	uint32_t dl, dh;
	uint32_t vpn, tbi, off;
	tlb_t    tlb;
	int      sc, boff;

	mchkAddr = vAddr;
	if (IPR_MAPEN & 1) {
		// Break virtual address down
		vpn = VA_GETVPN(vAddr);
		tbi = VA_GETTBI(vpn);
		off = VA_GETOFF(vAddr);

		// Translate virtual address into physical address
		tlb = (vAddr & VA_ADDR_S0) ? tlbSystem[tbi] : tlbProcess[tbi];
		if (((tlb.pte & acc) == 0) || (tlb.tag != vpn) ||
			((tlb.pte & TLB_M) == 0))
			tlb = translate(vAddr, size, acc);
		pAddr = (tlb.pte & TLB_PFN) | off;
	} else
		pAddr = vAddr;

	// Check physical address is aligned
	if ((pAddr & (size - 1)) == 0) {
		writep(pAddr, data, size);
		return;
	}

	if ((IPR_MAPEN & 1) && ((off + size) >= PAG_SIZE)) {
		// Break virtual address down
		vpn = VA_GETVPN(vAddr + LN_LONG);
		tbi = VA_GETTBI(vpn);
		off = VA_GETOFF(vAddr + LN_LONG);

		// Translate virtual address into physical address
		tlb = (vAddr & VA_ADDR_S0) ? tlbSystem[tbi] : tlbProcess[tbi];
		if (((tlb.pte & acc) == 0) || (tlb.tag != vpn) ||
			((tlb.pte & TLB_M) == 0))
			tlb = translate(vAddr + LN_LONG, size, acc);
		pAddr1 = (tlb.pte & TLB_PFN) | off;
	} else
		pAddr1 = pAddr + size;

	// Write 16/32-bit data in unaligned address
	// for crossing a page, etc.
	boff = pAddr & 03;
	dl = readpl(pAddr);
	if (size < LN_LONG) {
		// Unaligned word (16-bit words)
		if (boff == 1) {
			dl = (dl & 0xFF0000FF) | (uint16_t(data) << 8);
			writepl(pAddr, dl);
		} else {
			dh = readpl(pAddr1);
			dl = (dl & 0x00FFFFFF) | (data << 24);
			dh = (dh & 0xFFFFFF00) | uint8_t(data >> 8);
			writepl(pAddr, dl);
			writepl(pAddr1, dh);
		}
	} else {
		// Unaligned longword (32-bit words)
		dh = readpl(pAddr1);
		sc = boff << 3;
		dl = (dl & insert[boff])  | (data << sc);
		dh = (dh & ~insert[boff]) | (data >> (32 - sc));
		writepl(pAddr, dl);
		writepl(pAddr1, dh);
	}
}

// Flush instruction buffer
void vax_cpu_base::flushvi()
{
    ibpAddr   = ~0;
	ibCount   = 0;
	ibData[0] = 0;
	ibData[1] = 0;
}

// Instruction read access with current PC address (unaligned)
uint32_t vax_cpu_base::readvi(int size)
{
	int      boff = REG_PC & 03;
	int      sc   = boff << 3;
	uint32_t pAddr, data;
	uint32_t sts;

	if ((boff + size) > ibCount) {
		if (ibpAddr == ~0) {
			pAddr    = probev(REG_PC & ALIGN_LONG, RACC, &sts);
			if (sts != MM_OK)
				pagefault(sts, REG_PC, RACC, nullptr);
			ibpAddr  = (pAddr + ibCount) & ~03;
		}
		ibData[ibCount >> SC_LONG] = readpl(ibpAddr);
		ibpAddr += LN_LONG;
		ibCount += LN_LONG;
	}

	// Extract data from aligned longwords
	if (size == LN_BYTE)
		data = (ibData[0] >> sc) & MSK_BYTE;
	else if (size == LN_WORD) {
		data = (ibData[0] >> sc) & MSK_WORD;
		if (boff == 3)
			data |= (ibData[1] << (32 - sc)) & MSK_WORD;
	} else if (boff == 0)
		data = ibData[0];
	else
		data = (ibData[0] >> sc) | (ibData[1] << (32 - sc));

	// Ahead access to next data
	REG_PC += size;
	if ((boff + size) >= LN_LONG) {
		ibData[0] = ibData[1];
		ibCount  -= LN_LONG;
	}

	return data;
}

// Console read/write access routines

// Console write access with physical address
uint32_t vax_cpu_base::readpc(uint32_t pAddr, int size)
{
//	pAddr &= paMask;
//	if (pAddr < memSize) {
//		if (size == LN_LONG)
//			return MEML(pAddr >> 2);
//		if (size == LN_WORD)
//			return MEMW(pAddr >> 1);
//		return MEMB(pAddr);
//	}
	mchkRef |= REF_C;
	if (size == LN_LONG)
		return mapProgram->read32(pAddr);
	else if (size == LN_WORD)
		return mapProgram->read16(pAddr);
	else
		return mapProgram->read8(pAddr);
//	return sdev->readio(this, pAddr, size);
	return 0;
}

// Console read access with physical address
void vax_cpu_base::writepc(uint32_t pAddr, uint32_t data, int size)
{
//	pAddr &= paMask;
//	if (pAddr < memSize) {
//		if (size == LN_LONG)
//			MEML(pAddr >> 2) = data;
//		if (size == LN_WORD)
//			MEMW(pAddr >> 1) = data;
//		else
//			MEMB(pAddr) = data;
//	}
	mchkRef |= REF_C;
	if (size == LN_LONG)
		mapProgram->write32(pAddr, data);
	else if (size == LN_WORD)
		mapProgram->write16(pAddr, data);
	else
		mapProgram->write8(pAddr, data);
//	sdev->writeio(this, pAddr, data, size);
}

// Console write access with virtual address
uint32_t vax_cpu_base::readc(uint32_t vAddr, int size, uint32_t *sts)
{
	uint32_t pAddr, pAddr1;
	uint32_t dl, dh;
	int      sc;

	if ((pAddr = probev(vAddr, RACC, sts)) == MM_FAIL)
		return 0;

	// Check physical address is aligned
	if ((pAddr & (size - 1)) == 0)
		return readpc(pAddr, size);

	if ((VA_GETOFF(vAddr) + size) >= PAG_SIZE) {
		if ((pAddr1 = probev(vAddr + LN_LONG, RACC, sts)) == MM_FAIL)
			return 0;
	} else
		pAddr1 = pAddr + LN_LONG;

	// Read 16/32-bit data in unaligned address
	// for crossing a page, etc.
	if (size < LN_LONG) {
		if ((pAddr & 03) == 1)
			return ZXTW(readpc(pAddr, LN_LONG) >> 8);
		else {
			dl = readpc(pAddr,  LN_LONG);
			dh = readpc(pAddr1, LN_LONG);
			return ZXTW((dl >> 24) | (dh << 8));
		}
	} else {
		dl = readpc(pAddr,  LN_LONG);
		dh = readpc(pAddr1, LN_LONG);
		sc = (pAddr & 03) << 3;
		return (dl >> sc) | (dh << (32 - sc));
	}

	return 0;
}

// Console write access with virtual address
void vax_cpu_base::writec(uint32_t vAddr, uint32_t data, int size, uint32_t *sts)
{
	uint32_t pAddr;

	if ((pAddr = probev(vAddr, WACC, sts)) == MM_FAIL)
		return;
	writepc(pAddr, data, size);
}
