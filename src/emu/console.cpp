/*
 * console.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"

// Command Handler

CommandStatus CommandHandler::execute(Console *cty, string cmdLine)
{
	command_t *cmdList;
	int    argc = 0;
	args_t args;

	cmdList = mseCommands;

	for (int idx = 0; cmdList[idx].name; idx++)
		if (cmdList[idx].name == cmdLine)
			return cmdList[idx].func(cty, argc, args);

	// Command not found
	return cmdNotFound;
}

// Console handler

void Console::prompt()
{
	CommandHandler cmd;
	string cmdLine;
	CommandStatus st = cmdOk;

	while(st == cmdOk)
	{

		cout << fmt::sprintf("MSE> ") << flush;

		getline(cin, cmdLine);

		st = cmd.execute(this, cmdLine);
		if (st == cmdShutdown)
			break;
		if (st == cmdNotFound)
		{
			cout << fmt::sprintf("*** Invalid command: %s\n", cmdLine) << flush;
			st = cmdOk;
		}
	}
}
