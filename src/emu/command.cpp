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
CommandStatus cmdCreate(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.create(cty, args);

	return cmdOk;
}

// dial <device|none>
CommandStatus cmdDial(Console *cty, args_t &args)
{
	SystemEngine engine;
	string devName = args.getNext();
	Machine *sys = nullptr;

	if (devName == "none") {
		cty->setDialedSystem(nullptr);
		cty->setDialedDevice(nullptr);
		fmt::printf("Dialed system to none\n");
	} else {
		Machine *sys = engine.findSystem(devName);
		if (sys != nullptr) {
			cty->setDialedSystem(sys->getSystemDevice());
			cty->setDialedDevice(nullptr);
		} else
			fmt::printf("%s: system not found\n", devName);
	}

	return cmdOk;
}

// dump <device> <suboptions ...>
CommandStatus cmdDump(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.dump(cty, args);

	return cmdOk;
}

// exit
CommandStatus cmdExit(Console *, args_t &args)
{
	return cmdShutdown;
}

// list <device> <suboptions ...>
CommandStatus cmdList(Console *, args_t &args)
{
	return cmdOk;
}

// load <device> <file> <address>
CommandStatus cmdLoad(Console *cty, args_t &args)
{
	SystemEngine engine;
	engine.load(cty, args);
	return cmdOk;
}

// reset <device>
CommandStatus cmdReset(Console *, args_t &args)
{
	return cmdOk;
}

// set <device> <suboptions ...>
CommandStatus cmdSet(Console *cty, args_t &args)
{
	SystemEngine engine;
	engine.set(cty, args);
	return cmdOk;
}

// show <device> <suboptions ...>
CommandStatus cmdShow(Console *cty, args_t &args)
{
	SystemEngine engine;
	engine.show(cty, args);
	return cmdOk;
}

// start <device>
CommandStatus cmdStart(Console *cty, args_t &args)
{
	SystemEngine engine;

	engine.start(cty, args);

	return cmdOk;
}

// stop <device>
CommandStatus cmdStop(Console *, args_t &args)
{
	return cmdOk;
}

// list system
CommandStatus cmdListSystem(Console *cty, args_t &args)
{
	SystemList sysList;

	sysList.list(cout);

	return cmdOk;
}

// show device
CommandStatus cmdShowDevice(Console *cty, args_t &args)
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
		{ "load",		cmdLoad,	nullptr },
		{ "reset",		cmdReset,	nullptr },
		{ "set",		cmdSet,		CommandHandler::mseSetCommands },
		{ "show",		cmdShow,	CommandHandler::mseShowCommands },
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
		{ "device",		cmdShowDevice,		nullptr },
		// Terminator
		nullptr
};
