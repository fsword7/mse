/*
 * ka780.cpp
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  KA780/KA785 CPU series
 *
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/mtpr.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/fpu.h"
#include "dev/cpu/vax/ka780.h"

#include "dev/cpu/vax/opcodes.h"

DEFINE_DEVICE_TYPE(KA780, ka780_cpu, "KA780", "VAX-11/780")

ka780_cpu::ka780_cpu(const system_config &config, tag_t *tag, device_t *owner, uint64_t clock)
: vax_cpu_base(config, KA780, tag, owner, clock, 32, 30)
{
}

ka780_cpu::~ka780_cpu()
{
}

void ka780_cpu::reset()
{
	// Initialize SID register
	ipReg[IPR_nSID] = (SID_ID|SID_ECO|SID_PLANT);
	// Set specific serial number
	ipReg[IPR_nSID] |= SID_SN;
}

void ka780_cpu::mfpr()
{
}

void ka780_cpu::mtpr()
{
}

//#define CPU_KA780
//#define CPU_CLASS ka780_cpu
//#include "dev/cpu/vax/executes.h"
