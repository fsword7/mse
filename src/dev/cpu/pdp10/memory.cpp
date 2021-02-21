/*
 * memory.cpp - PDP-10 family processors - memory access package
 *
 *  Created on: Oct 28, 2019
 *      Author: Tim Stark
 *
 */

#include "emu/emucore.h"
#include "dev/cpu/pdp10/kx10.h"

int pdp10_cpu_base::readp()
{
	if (abReg < AC_NREGS) {
		mbReg = curReg[abReg];
		return 0;
	} else {
		if (abReg >= memSize) {
			nxmFlag = true;
			return 1;
		}
		mbReg = mem[abReg];
	}
	return 0;
}

int pdp10_cpu_base::writep()
{
	if (abReg < AC_NREGS) {
		curReg[abReg] = mbReg;
		return 0;
	} else {
		if (abReg >= memSize) {
			nxmFlag = true;
			return 1;
		}
		mem[abReg] = mbReg;
	}
	return 0;
}

int pdp10_cpu_base::readv(uint32_t ctx, uint32_t flags)
{
	if (abReg < AC_NREGS) {
		mbReg = curReg[abReg];
		return 0;
	} else {
		if (abReg >= memSize) {
			nxmFlag = true;
			return 1;
		}
		mbReg = mem[abReg];
	}
	return 0;
}

int pdp10_cpu_base::writev(uint32_t ctx, uint32_t flags)
{
	if (abReg < AC_NREGS) {
		curReg[abReg] = mbReg;
		return 0;
	} else {
		if (abReg >= memSize) {
			nxmFlag = true;
			return 1;
		}
		mem[abReg] = mbReg;
	}
	return 0;
}
