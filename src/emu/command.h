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
	const char	*name;
	cmdFunc_t	func;
	command_t	*ext;
};

class CommandHandler {
public:

	CommandHandler() = default;
	~CommandHandler() = default;

	CommandStatus execute(Console *cty, string cmdLine);

private:
	int split(const string &cmdLine, args_t &args);

private:
	static command_t mseCommands[];
	static command_t mseListCommands[];
	static command_t mseSetCommands[];
	static command_t mseShowCommands[];
};
