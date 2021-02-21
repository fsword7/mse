/*
 * telnet.cpp
 *
 *  Created on: May 7, 2018
 *      Author: Timothy Stark
 */

#include "osd/telnet.h"

#include "emu/emucore.h"

// Telnet Code Initialization
static uint8_t tlnCodes[] =
{
	TLN_IAC, TLN_WILL, TLN_LINEMODE,  // IAC WILL LINEMODE
	TLN_IAC, TLN_WILL, TLN_SGA,       // IAC WILL SGA
	TLN_IAC, TLN_WILL, TLN_ECHO,      // IAC WILL ECHO
	TLN_IAC, TLN_WILL, TLN_BINARY,    // IAC WILL BINARY
	TLN_IAC, TLN_DO,   TLN_BINARY     // IAC DO BINARY
};




