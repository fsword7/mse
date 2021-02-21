/*
 * cvax.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  CVAX 78034 Processor emulation
 *
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/cvax.h"
#include "dev/cpu/vax/fpu.h"
#include "dev/cpu/vax/opcodes.h"

static int   iprSize = 0x40;
static const char *iprName[] = {
	"KSP",     // (R/W) 00 Kernel Stack Pointer
	"ESP",     // (R/W) 01 Executive Stack Pointer
	"SSP",     // (R/W) 02 Supervisor Stack Pointer
	"USP",     // (R/W) 03 User Stack Pointer
	"ISP",     // (R/W) 04 Interrupt Stack Pointer
	nullptr,   //       05 Unknown
	nullptr,   //       06 Unknown
	nullptr,   //       07 Unknown
	"P0BR",    // (R/W) 08 P0 Base Register
	"P0LR",    // (R/W) 09 P0 Length Register
	"P1BR",    // (R/W) 0A P1 Base Register
	"P1LR",    // (R/W) 0B P1 Length Register
	"SBR",     // (R/W) 0C System Base Register
	"SLR",     // (R/W) 0D System Length Register
	nullptr,   //       0E Unknown
	nullptr,   //       0F Unknown
	"PCBB",    // (R/W) 10 Process Control Block Base
	"SCBB",    // (R/W) 11 System Control Block Base
	"IPL",     // (R/W) 12 Interrupt Priority Level
	"ASTLVL",  // (R/W) 13 AST Level
	"SIRR",    // (W)   14 Software Interrupt Request
	"SISR",    // (R/W) 15 Software Interrupt Summary
	nullptr,   //       16 Unknown
	nullptr,   //       17 Unknown
	"ICCS",    // (R/W) 18 Interval Clock Control
	"NICR",    // (W)   19 Next Interval Count
	"ICR",     // (R)   1A Interval Count
	"TODR",    // (R/W) 1B Time of Year
	nullptr,   //       1C Unknown
	nullptr,   //       1D Unknown
	nullptr,   //       1E Unknown
	nullptr,   //       1F Unknown
	"RXCS",    // (R/W) 20 Console Receiver Status
	"RXDB",    // (R)   21 Console Receiver Data Buffer
	"TXCS",    // (R/W) 22 Console Transmit Status
	"TXDB",    // (W)   23 Console Transmit Data Buffer
	nullptr,   //       24 Unknown
	"CADR",    //       25 Cache Disable Register
	nullptr,   //       26 Unknown
	"MSER",    //       27 Memory System Error Register
	"ACCS",    //       28 *Accelerator Control and Status
	nullptr,   //       29 *Accelerator Maintenance
	"CONPC",   //       2A Console PC
	"CONPSL",  //       2B Console PSL
	nullptr,   //       2C *Writable-Control-Store Address
	nullptr,   //       2D *Writable-Control-Store Data
	nullptr,   //       2E Unknown
	nullptr,   //       2F Unknown
	"SBIFS",   //       30 *SBI Fault Status
	"SBIS",    //       31 *SBI silo
	"SBISC",   //       32 *SBI silo comparator
	"SBIMT",   //       33 *SBI Maintenance
	"SBIER",   //       34 *SBI Error
	"SBITA",   //       35 *SBI Timeout Address
	"SBIQC",   //       36 *SBI Quadword Clear
	nullptr,   //       37 Unknown
	"MAPEN",   // (R/W) 38 Memory Management Enable
	"TBIA",    // (W)   39 Translation Buffer Invalidate All
	"TBIS",    // (W)   3A Translation Buffer Invalidate Single
	nullptr,   //       3B Unknown
	"MBRK",    //       3C Microprogram Breakpoint
	"PME",     // (R/W) 3D Performance Monitor Enable
	"SID",     // (R)   3E System Identification
	"TBCHK",   // (W)   3F Translation Buffer Check
};

DEFINE_DEVICE_TYPE(CVAX, cvax_cpu, "CVAX", "CVAX Processor")

cvax_cpu::cvax_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: vax_cpu_base(config, CVAX, tag, owner, clock, 32, 30)
{
}

cvax_cpu::~cvax_cpu()
{
}

//cvax_cpu *cvax_cpu::create(sysDevice *sdev, std::string name())
//{
//	cvax_cpu *cpu = new cvax_cpu();
//
//	if (cpu == nullptr)
//		return nullptr;
//
//	cpu->name() = name();
//	cpu->devType = "CVAX";
////	cpu->devDesc = model->desc;
////	cpu->driver  = model->driver;
//
//	// Assign system device for I/O access
//	cpu->setSystemDevice(sdev);
//
//	// Initialize CPU processor
//	cpu->reset();
//
//	// Add CPU device to system device
//	sdev->addCPUDevice(cpu);
//
//	return cpu;
//}

void cvax_cpu::devReset()
{
	// Initialize all working registers
	for (int idx = 0; idx < CPU_nGREGS; idx++)
		gpReg[idx].l = 0;

	// Powerup initialization
	psReg  = PSL_IS | PSL_IPL;
	ccReg  = 0;
	REG_SP = 0x00000000;
	REG_PC = SRM_START;

	// Standard IPR registers
	IPR_SID    = (SID_ID|SID_UCODE);
	IPR_MAPEN  = 0;
	IPR_ICCS   = 0;
	IPR_MSER   = 0;
	IPR_CADR   = 0;
	IPR_SISR   = 0;
	IPR_ASTLVL = 0;

	// Model-specific IPR registers
	IPR_CONPC  = 0;
	IPR_CONPSL = (psReg|ccReg) | CON_PWRUP;
	IPR_MSER   = 0;
	IPR_CADR   = 0;

	// Set 30-bit physical addressing mask
	paMask  = PA_MASK30;
	// Reset current access mode
	curMode  = AM_MASK(PSL_GETCUR(psReg));
	irqFlags = 0;
	scbCode  = -1;

	// Clear all process/system TLB entries
	cleartlb(true);
}

//int cvax_cpu::boot()
//{
//	return 0;
//}

void cvax_cpu::halt(uint32_t code)
{
	int mode = PSL_GETCUR(psReg);

	// Save current PC and PSL registers
	IPR_CONPC  = REG_PC;
	IPR_CONPSL = (psReg|ccReg) | (code << 8) |
		((IPR_MAPEN & 1) ? CONPSL_MAPEN : 0);

	// Switch to interrupt stack mode and IPL 31
	if (mode > AM_INTERRUPT)
		IPR_CONPSL |= CONPSL_INVALID;
	else
		ipReg[mode] = REG_SP;

	// Reset processor registers
	REG_SP    = IPR_ISP;
	psReg     = PSL_IS|PSL_IPL;
	ccReg     = 0;
	IPR_MAPEN = 0;

	// Start address in ROM space
	REG_PC = SRM_START;
	flushvi();

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_EXCEPTION))
		dbg.log("%s: (HALT) Halt Action Code %08X - Jumped to %08X\n",
			deviceName(), code, REG_PC);
#endif /* ENABLE_DEBUG */

	throw HALT_ACTION;
}

void cvax_cpu::check(uint32_t code)
{
	uint32_t sirq;

	// Find highest software interrupt
	for (sirq = IPL_SMAX; sirq > 0; sirq--)
		if ((IPR_SISR >> sirq) & 1)
			break;

	// Set parameters for exception.
	paCount  = 5;
	paReg[0] = (paCount - 1) * LN_LONG;
	paReg[1] = (code & 0x80) ? mchkRef : 0;
	paReg[2] = mchkAddr + LN_LONG;
	paReg[3] = (opCode << 24) | (sirq << 16) |
		((IPR_CADR & 0xFF) << 8) | (IPR_MSER & 0xFF);
	paReg[4] = 0x00C07000 | ((faultAddr - REG_PC) & 0xFF);

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_EXCEPTION))
		dbg.log("%s: (MCHK) Machine Check Exception Code %08X\n",
			deviceName(), code);
#endif /* ENABLE_DEBUG */

	// Entering an exception routine.
	exception(IE_SVE, SCB_MCHK, 0);
}

//#define CPU_CVAX
//#define CPU_CLASS cvax_cpu
//#include "dev/cpu/vax/executes.h"

void cvax_cpu::mfpr()
{
	uint32_t ipr = opReg[0];
	uint32_t dst;

	// Must be in kernel mode.
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Standard IPR Registers
	switch (ipr) {
		case IPR_nKSP: // Kernel Stack Register
			dst = ((psReg & PSL_IS) ? ipReg[ipr] : REG_SP);
			break;

		case IPR_nESP: // Executive Stack Register
		case IPR_nSSP: // Supervisor Stack Register
		case IPR_nUSP: // User Stack Register
			dst = ipReg[ipr];
			break;

		case IPR_nISP: // Interrupt Stack Register
			dst = ((psReg & PSL_IS) ? REG_SP : ipReg[ipr]);
			break;

		case IPR_nIPL: // Interrupt Priority Level
			dst = PSL_GETIPL(psReg);
			break;

		case IPR_nPME:
			dst = ipReg[ipr];
			break;

		default:
			dst = (ipr < iprSize) ? ipReg[ipr] : 0;
			// Model-Specific IPR Registers
			// Otherwise, reserved operand fault trap.
//			if (ssc != nullptr)
//				dst = ssc->ReadIPR(cpu, ipr);
			break;
	}

	// Write results back.
	storel(opReg[1], dst);

	// Update Condition Codes
	SetNZ(ccReg, SXTL(dst), 0, (ccReg & CC_C));

	// Evaluate hardware/software interrupts.
	UpdateIRQ();

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_IOREGS)) {
		const char *regName = "Undefined Register";
		if ((ipr < iprSize) && iprName[ipr])
			regName = iprName[ipr];
		dbg.log("%s: (R) %s (%02X) => %08X: %s\n", deviceName(),
				regName, ipr, dst, stringCC(ccReg));
	}
#endif /* ENABLE_DEBUG */
}

void cvax_cpu::mtpr()
{
	uint32_t src = opReg[0];
	uint32_t ipr = opReg[1];
	uint32_t irq;

	// Must be in kernel mode.
	if (PSL_GETCUR(psReg) != AM_KERNEL)
		throw PRIV_INST_FAULT;

	// Update Condition Codes
	SetNZ(ccReg, SXTL(src), 0, (ccReg & CC_C));

	// Update Standard IPR Registers
	switch (ipr) {
		case IPR_nKSP: // Kernel Stack Register
			if (psReg & PSL_IS)
				ipReg[ipr] = src;
			else
				gpReg[REG_nSP].l = src;
			break;
		case IPR_nESP: // Executive Stack Register
		case IPR_nSSP: // Supervisor Stack Register
		case IPR_nUSP: // User Stack Register
			ipReg[ipr] = src;
			break;
		case IPR_nISP: // Interrupt Stack Register
			if (psReg & PSL_IS)
				gpReg[REG_nSP].l = src;
			else
				ipReg[ipr] = src;
			break;

		case IPR_nP0BR:
		case IPR_nP1BR:
		case IPR_nSBR:
			ipReg[ipr] = src & BR_MASK;
			cleartlb(ipr == IPR_nSBR);
			break;

		case IPR_nP0LR:
		case IPR_nP1LR:
		case IPR_nSLR:
			ipReg[ipr] = src & LR_MASK;
			cleartlb(ipr == IPR_nSLR);
			break;

		case IPR_nPCBB:
		case IPR_nSCBB:
			ipReg[ipr] = src & ALIGN_LONG;
			break;

		case IPR_nIPL:
			ipReg[ipr] = src & IPL_MASK;
			psReg = PSL_SETIPL(src) | (psReg & ~PSL_IPL);
			break;

		case IPR_nSIRR:
//			if ((src > IPL_SMAX) || (src == 0))
//				RSVD_OPND_FAULT;
			if (irq = (src & SIRR_MASK))
				IPR_SISR |= (1u << irq);
			break;

		case IPR_nSISR:
			ipReg[ipr] = src & SISR_MASK;
			break;

		case IPR_nASTLVL:
			if (src > AST_MAX)
				throw RSVD_OPND_FAULT;
			ipReg[ipr] = src;
			break;

		case IPR_nMAPEN:
			ipReg[ipr] = src & 1;
		case IPR_nTBIA:
			cleartlb(true);
			break;

		case IPR_nTBIS:
			cleartlb(src);
			break;

		case IPR_nTBCHK:
			if (checktlb(src))
				ccReg |= CC_V;
			break;

		case IPR_nPME:
			ipReg[ipr] = src & 1;
			break;

		default:
			// Update Model-Specific IPR Registers
			// Otherwise, reserved operand fault trap.
//			if (ssc != nullptr)
//				ssc->WriteIPR(cpu, ipr, src);
			break;
	}

	// Evaluate hardware/software interrupts.
	UpdateIRQ();

#ifdef ENABLE_DEBUG
	if (dbg.checkFlags(DBG_TRACE|DBG_OPERAND) || dbg.checkFlags(DBG_IOREGS)) {
		const char *regName = "Undefined Register";
		if ((ipr < iprSize) && iprName[ipr])
			regName = iprName[ipr];
		dbg.log("%s: (W) %s (%02X) <= %08X: %s\n", deviceName(),
			regName, ipr, src, stringCC(ccReg));
	}
#endif /* ENABLE_DEBUG */
}
