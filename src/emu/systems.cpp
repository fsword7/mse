/*
 * systems.cpp - system list
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/syslist.h"

#include "system/dec/vax/externs.h"

const SystemDriver *SystemList::sysList[] =
{
		// Digital Electric Corporation (DEC)
		&SYSTEM_NAME(mv3900),
		&SYSTEM_NAME(mv3900x),

		&SYSTEM_NAME(vs3900),
		&SYSTEM_NAME(vs3900x),

		// terminator
		nullptr
};
