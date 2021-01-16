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

class args_t
{
public:
	args_t() = default;
	~args_t() = default;

	using args = vector<string>;

	inline void clear() { count = 0; index = 0; params.clear(); }
	inline bool empty() { return (count == 0); }
	inline int  size()  { return count; }
	inline void next()  { index++; }

	inline string operator [](int idx) { return (idx < count) ? params[idx] : ""; }
	inline void add(string arg)        { params.push_back(arg); count++; }
	inline string current()            { return (index < count) ? params[index] : ""; }

	inline string getNext() { string arg = current(); next(); return arg; }

private:
	int	 count = 0;
	int	 index = 0;
	args params;
};

typedef CommandStatus (*cmdFunc_t)(Console *, args_t &);
typedef CommandStatus (*cmdDevice_t)(Console *, Device *, args_t &);

struct command_t
{
	const char	*name;
	cmdFunc_t	func;
	command_t	*ext;
};

struct devCommand_t
{
	const char	*name;
	cmdDevice_t	func;
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
