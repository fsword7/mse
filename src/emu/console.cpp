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
	if (args.empty()) {
		// Check if repeating command for continuing
		if (lastCommand != nullptr)
			lastCommand->func(cty, args);
		return cmdOk;
	}

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

			if (cmdList[idx].func != nullptr) {
				lastCommand = &cmdList[idx];
				return cmdList[idx].func(cty, args);
			}

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

void Console::script(fs::path fname)
{
	// fmt::printf("Script: %s\n", fname);

	string   cmdLine;
	int      noLine = 0;

	CommandStatus st = cmdOk;

	try {
		ifstream fin(fname);
		while (!fin.eof())
		{
			getline(fin, cmdLine);
			noLine++;

			fmt::printf("/// %04d: %s\n", noLine, cmdLine);

			// Check comment character first
			if (cmdLine[0] == '#' || cmdLine[0] == ';')
				continue;

			st = cmd.execute(this, cmdLine);
			if (st == cmdShutdown)
				break;
			if (st == cmdNotFound)
			{
				cout << fmt::sprintf("*** Invalid command: %s\n", cmdLine) << flush;
				st = cmdOk;
			}
		}

		fin.close();
	}

	catch (system_error &e)
	{
		// cerr << fmt::sprintf("%s: file error: %s\n", fname, e.code().message());
		cout << flush;
	}
}

void Console::prompt()
{
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
