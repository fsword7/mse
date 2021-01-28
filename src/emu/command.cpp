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

// create <device> <suboptions ...>
CommandStatus cmdCreate(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.create(user, args);
	return cmdOk;
}

CommandStatus cmdDebug(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.debug(user, args);
	return cmdOk;
}

// dial <device|none>
CommandStatus cmdDial(Console *user, args_t &args)
{
	SystemEngine engine;
	string devName = args.getNext();
	Machine *sys = nullptr;

	if (devName == "none") {
		user->setDialedSystem(nullptr);
		user->setDialedDevice(nullptr);
		user->printf("Dialed system to none\n");
	} else {
		Machine *sys = engine.findSystem(devName);
		if (sys != nullptr) {
			user->setDialedSystem(sys->getSystemDevice());
			user->setDialedDevice(nullptr);
		} else
			user->printf("%s: system not found\n", devName);
	}

	return cmdOk;
}

// dump <device> <suboptions ...>
CommandStatus cmdDump(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.dump(user, args);
	return cmdOk;
}

// exit
CommandStatus cmdExit(Console *, args_t &args)
{
	return cmdShutdown;
}

// list <device> <suboptions ...>
CommandStatus cmdList(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.list(user, args);
	return cmdOk;
}

// load <device> <file> <address>
CommandStatus cmdLoad(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.load(user, args);
	return cmdOk;
}

CommandStatus cmdLog(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.log(user, args);
	return cmdOk;
}

// reset <device>
CommandStatus cmdReset(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.reset(user, args);
	return cmdOk;
}

// set <device> <suboptions ...>
CommandStatus cmdSet(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.set(user, args);
	return cmdOk;
}

// show <device> <suboptions ...>
CommandStatus cmdShow(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.show(user, args);
	return cmdOk;
}

// start <device>
CommandStatus cmdStart(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.start(user, args);
	return cmdOk;
}

// step <device>
CommandStatus cmdStep(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.step(user, args);
	return cmdOk;
}

// stop <device>
CommandStatus cmdStop(Console *, args_t &args)
{
	return cmdOk;
}

// list system
CommandStatus cmdListSystem(Console *user, args_t &args)
{
	SystemList sysList;
	sysList.list(cout);
	return cmdOk;
}

// show device
CommandStatus cmdShowDevice(Console *user, args_t &args)
{
	SystemEngine engine;
	engine.showDevices(user, args);
	return cmdOk;
}

// General command list
command_t CommandHandler::mseCommands[] =
{
		{ "create",		cmdCreate,	nullptr },
		{ "debug",		cmdDebug,	nullptr },
		{ "dial",		cmdDial,	nullptr },
		{ "dump",		cmdDump,	nullptr },
		{ "exit",		cmdExit,	nullptr },
		{ "list",		cmdList,	CommandHandler::mseListCommands },
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
		{ "device",		cmdShowDevice,		nullptr },
		// Terminator
		nullptr
};
