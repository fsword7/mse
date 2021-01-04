/*
 * command.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/engine.h"
#include "emu/syslist.h"

CommandStatus cmdCreate(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.create(cout, args);

	return cmdOk;
}

CommandStatus cmdDial(Console *cty, args_t &args)
{
	return cmdOk;
}

CommandStatus cmdDump(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.dump(cout, args);

	return cmdOk;
}

CommandStatus cmdExit(Console *, args_t &args)
{
	return cmdShutdown;
}

CommandStatus cmdReset(Console *, args_t &args)
{
	return cmdOk;
}

CommandStatus cmdStart(Console *, args_t &args)
{
	return cmdOk;
}

CommandStatus cmdStop(Console *, args_t &args)
{
	return cmdOk;
}


CommandStatus cmdListSystem(Console *cty, args_t &args)
{
	SystemList sysList;

	sysList.list(cout);

	return cmdOk;
}

CommandStatus cmdShowDevices(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.showDevices(cty, args);

	return cmdOk;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "create",		cmdCreate,	nullptr },
		{ "dial",		cmdDial,	nullptr },
		{ "dump",		cmdDump,	nullptr },
		{ "exit",		cmdExit,	nullptr },
		{ "list",		nullptr,	CommandHandler::mseListCommands },
		{ "reset",		cmdReset,	nullptr },
		{ "set",		nullptr,	CommandHandler::mseSetCommands },
		{ "show",		nullptr,	CommandHandler::mseShowCommands },
		{ "start",		cmdStart,	nullptr },
		{ "stop",		cmdStop,	nullptr },
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
		{ "devices",	cmdShowDevices,		nullptr },
		// Terminator
		nullptr
};
