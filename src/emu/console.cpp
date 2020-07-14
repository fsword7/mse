/*
 * console.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"

// Command Handler

int CommandHandler::split(const string &cmdLine, args_t &args)
{
	istringstream line(cmdLine);

	while (line) {
		string word;

		line >> word;
		if (!word.empty())
			args.push_back(word);
	}
	return args.size();
}

CommandStatus CommandHandler::execute(Console *cty, string cmdLine)
{
	command_t *cmdList;
	int    argc = 0;
	args_t args;

	// Split command line into words
	args.clear();
	argc = split(cmdLine, args);
	if (args.empty())
		return cmdOk;

	cmdList = mseCommands;
	for (int idx = 0; cmdList[idx].name; idx++)
		if (cmdList[idx].name == args[0])
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
