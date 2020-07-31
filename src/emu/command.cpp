/*
 * command.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/syslist.h"

CommandStatus cmdCreate(Console *, int argc, args_t &args)
{
	SystemList sysList;

	return cmdOk;
}

CommandStatus cmdExit(Console *, int argc, args_t &args)
{
	return cmdShutdown;
}

CommandStatus cmdListSystem(Console *, int argc, args_t &args)
{
	SystemList sysList;

	sysList.list(cout);

	return cmdOk;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "create",		cmdCreate,	nullptr },
		{ "exit",		cmdExit,	nullptr },
		{ "list",		nullptr,	CommandHandler::mseListCommands },
		{ "set",		nullptr,	CommandHandler::mseSetCommands },
		{ "show",		nullptr,	CommandHandler::mseShowCommands },
		{ "quit",		cmdExit,	nullptr },
		// Terminator
		nullptr
};

// General list command list
command_t CommandHandler::mseListCommands[] =
{
		{ "system",		cmdListSystem,		nullptr },
		// Terminator
		nullptr
};

command_t CommandHandler::mseSetCommands[] =
{
		// Terminator
		nullptr
};

command_t CommandHandler::mseShowCommands[] =
{
		// Terminator
		nullptr
};
