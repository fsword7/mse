/*
 * v11.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  V-11 (Scorpio) processor emulation
 */


#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/v11.h"
#include "dev/cpu/vax/fpu.h"
#include "dev/cpu/vax/opcodes.h"

DEFINE_DEVICE_TYPE(V11, v11_cpu, "V11", "V11")

v11_cpu::v11_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: vax_cpu_base(config, V11, tag, owner, clock, 32, 30)
{
}

v11_cpu::~v11_cpu()
{
}

void v11_cpu::reset()
{
	// Initialize SID register
	ipReg[IPR_nSID] = (SID_ID|SID_CPUREV|SID_PATREV|SID_MBO|SID_UCODE);
}

void v11_cpu::mfpr()
{
}

void v11_cpu::mtpr()
{
}

//#define CPU_V11
//#define CPU_CLASS v11_cpu
//#include "dev/cpu/vax/executes.h"
