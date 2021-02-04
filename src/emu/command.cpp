/*
 * command.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/machine.h"
#include "emu/engine.h"
#include "emu/syslist.h"

// create <device> <suboptions ...>
CommandStatus cmdCreate(Console *user, args_t &args)
{
	user->engine.cmdCreate(user, args);
	return cmdOk;
}

CommandStatus cmdDebug(Console *user, args_t &args)
{
	user->engine.cmdDebug(user, args);
	return cmdOk;
}

CommandStatus cmdDial(Console *user, args_t &args)
{
	user->engine.cmdDial(user, args);
	return cmdOk;
}

// dump <device> <suboptions ...>
CommandStatus cmdDump(Console *user, args_t &args)
{
	user->engine.cmdDump(user, args);
	return cmdOk;
}

// execute <device> [address]
CommandStatus cmdExecute(Console *user, args_t &args)
{
	user->engine.cmdExecute(user, args);
	return cmdOk;
}

// exit
CommandStatus cmdExit(Console *user, args_t &args)
{
//	user->engine.cmdExit(user, args);
	return cmdShutdown;
}

// halt <device>
CommandStatus cmdHalt(Console *user, args_t &args)
{
	user->engine.cmdHalt(user, args);
	return cmdOk;
}

// list <device> <suboptions ...>
CommandStatus cmdList(Console *user, args_t &args)
{
	user->engine.cmdList(user, args);
	return cmdOk;
}

// load <device> <file> <address>
CommandStatus cmdLoad(Console *user, args_t &args)
{
	user->engine.cmdLoad(user, args);
	return cmdOk;
}

CommandStatus cmdLog(Console *user, args_t &args)
{
	user->engine.cmdLog(user, args);
	return cmdOk;
}

// reset <device>
CommandStatus cmdReset(Console *user, args_t &args)
{
	user->engine.cmdReset(user, args);
	return cmdOk;
}

// set <device> <suboptions ...>
CommandStatus cmdSet(Console *user, args_t &args)
{
	user->engine.cmdSet(user, args);
	return cmdOk;
}

// show <device> <suboptions ...>
CommandStatus cmdShow(Console *user, args_t &args)
{
	user->engine.cmdShow(user, args);
	return cmdOk;
}

// start <device>
CommandStatus cmdStart(Console *user, args_t &args)
{
	user->engine.cmdStart(user, args);
	return cmdOk;
}

// step <device>
CommandStatus cmdStep(Console *user, args_t &args)
{
	user->engine.cmdStep(user, args);
	return cmdOk;
}

// stop <device>
CommandStatus cmdStop(Console *user, args_t &args)
{
	user->engine.cmdStop(user, args);
	return cmdOk;
}

// show device
CommandStatus cmdShowDevice(Console *user, args_t &args)
{
	user->engine.cmdShowDevice(user, args);
	return cmdOk;
}

// show system
CommandStatus cmdShowSystem(Console *user, args_t &args)
{
	user->engine.cmdShowSystem(user, args);
	return cmdOk;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "create",		cmdCreate,	nullptr },
		{ "debug",		cmdDebug,	nullptr },
		{ "dial",		cmdDial,	nullptr },
		{ "dump",		cmdDump,	nullptr },
		{ "execute",	cmdExecute, nullptr },
		{ "exit",		cmdExit,	nullptr },
		{ "halt",		cmdHalt,	nullptr },
		{ "list",		cmdList,	nullptr },
		{ "load",		cmdLoad,	nullptr },
		{ "log",		cmdLog,		nullptr },
		{ "reset",		cmdReset,	nullptr },
		{ "set",		cmdSet,		CommandHandler::mseSetCommands },
		{ "show",		cmdShow,	CommandHandler::mseShowCommands },
		{ "start",		cmdStart,	nullptr },
		{ "step",		cmdStep,	nullptr },
		{ "stop",		cmdStop,	nullptr },
		{ "quit",		cmdExit,	nullptr },
		// Terminator
		nullptr
};

// General list command list
command_t CommandHandler::mseListCommands[] =
{
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
		{ "device",		cmdShowDevice,		nullptr },
		{ "system",		cmdShowSystem,		nullptr },
		// Terminator
		nullptr
};
