/*
 * command.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/syslist.h"

CommandStatus cmdExit(Console *, int argc, args_t &args)
{
	return cmdShutdown;
}

CommandStatus cmdList(Console *, int argc, args_t &args)
{
	SystemList sysList;

	sysList.list(cout);

	return cmdOk;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "exit", cmdExit },
		{ "list", cmdList },
		{ "quit", cmdExit },
		// Terminator
		nullptr
};
