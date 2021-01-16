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
			args.add(word);
	}

	return args.size();
}

CommandStatus CommandHandler::execute(Console *cty, string cmdLine)
{
	command_t *cmdList;
	args_t args;

	// Split command line into words
	args.clear();
	split(cmdLine, args);
	if (args.empty())
		return cmdOk;

	cmdList = mseCommands;
	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args.current()) {
			args.next();

			command_t *optList = cmdList[idx].ext;
			if (optList != nullptr)
			{
				for (int idx2 = 0; optList[idx2].name; idx2++) {
					if (optList[idx2].name == args.current()) {
						args.next();
						assert(optList[idx2].func != nullptr);
						return optList[idx2].func(cty, args);
					}
				}
			}

			if (cmdList[idx].func != nullptr)
				return cmdList[idx].func(cty, args);

//			cout << fmt::sprintf("Usage: %s <%s%s%s> [arguments...]\n", args[0],
//				cmdList[idx].func != nullptr ? "device" : "",
//				(cmdList[idx].func != nullptr && cmdList[idx].ext != nullptr) ? "|" : "",
//				cmdList[idx].ext != nullptr ? "options" : "");
		}
	}

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
