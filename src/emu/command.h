/*
 * command.h - command handler package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

class Console;

enum CommandStatus
{
	cmdOk = 0,
	cmdNotFound,
	cmdError,
	cmdShutdown
};

typedef vector<string> args_t;
typedef CommandStatus (*cmdFunc_t)(Console *, int, args_t &);

struct command_t
{
	const char *name;
	cmdFunc_t   func;
};

class CommandHandler {
public:

	CommandHandler() = default;
	~CommandHandler() = default;

	CommandStatus execute(Console *cty, string cmdLine);

private:
	static command_t mseCommands[];
};
