/*
 * ev6.cpp - DEC 21264 Alpha processor series (EV6 series)
 *
 *  Created on: Dec 11, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/alpha/ev6.h"

DEFINE_DEVICE_TYPE(dec21264, dec21264_cpuDevice, "21264", "DEC 21264 Alpha Processor")

dec21264_cpuDevice::dec21264_cpuDevice(const SystemConfig &config,
		cstag_t &tagName, Device *owner, uint64_t clock)
: AlphaProcessor(config, dec21264, tagName, owner, clock, 44)
{
	setArchType(ARCH_EV6);
	setArchFlags(ARCH_PFM|ARCH_PRC|ARCH_MVI|ARCH_CIX|ARCH_BWX);
}

void dec21264_cpuDevice::preset()
{
	// Initialize Ibox control register
	ev6.ictl.spce = 0;
	ev6.ictl.ic_en = 3;
	ev6.ictl.spe = 0;
	ev6.ictl.sde = 0;
	ev6.ictl.sbe = 0;
	ev6.ictl.bpMode = 0;
	ev6.ictl.hwe = 0;
	ev6.ictl.sl_xmit = 0;
	ev6.ictl.sl_rcv = 0;
	ev6.ictl.va48 = 0;
	ev6.ictl.vaForm32 = 0;
	ev6.ictl.single_issue_h = 0;
	ev6.ictl.pct0_en = 0;
	ev6.ictl.pct1_en = 0;
	ev6.ictl.call_pal_r23 = 0;
	ev6.ictl.mchk_en = 0;
	ev6.ictl.tb_mb_en = 0;
	ev6.ictl.bist_fail = 1;
	ev6.ictl.chip_id = 5;
	ev6.ictl.vptb = 0;
	ev6.ictl.sext_vptb = 0;
}
