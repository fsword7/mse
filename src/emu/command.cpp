/*
 * command.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"

CommandStatus cmdExit(Console *, int argc, args_t &args)
{
	return cmdShutdown;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "exit", cmdExit },
		{ "quit", cmdExit },
		// Terminator
		nullptr
};
