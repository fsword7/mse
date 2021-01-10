/*
 * systems.cpp - system list
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/syslist.h"

#include "system/dec/axp/externs.h"
#include "system/dec/vax/externs.h"

const SystemDriver *SystemList::sysList[] =
{
		// Digital Electric Corporation (DEC)
		// AXP family series
		&SYSTEM_NAME(es40),
		&SYSTEM_NAME(es45),

		// VAX family series
		&SYSTEM_NAME(mv3900),
		&SYSTEM_NAME(mv3900x),

		&SYSTEM_NAME(vs3900),
		&SYSTEM_NAME(vs3900x),

		// terminator
		nullptr
};
