/*
 * debug.h - Alpha CPU Processor - debugging routines
 *
 *  Created on: Nov 21, 2020
 *      Author: Tim Stark
 */

#pragma once

#ifdef DEBUG


#else

#define OP(opcode, format) DO_##opcode

#endif
