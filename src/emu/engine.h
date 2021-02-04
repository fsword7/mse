/*
 * engine.h - System engine package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"

class Machine;
class SystemEngine
{
public:
	using cmdFunc_t = CommandStatus (SystemEngine::*)(Console *, args_t &);
	struct command_t
	{
		ctag_t	  *name;
		cmdFunc_t  func;
		command_t *options;
	};

	SystemEngine() = default;
	SystemEngine(Console *user) : user(user) {}
	~SystemEngine() = default;

	// Global system initialization
	void ginit();
	void gexit();

	Machine *findSystem(cstag_t name);
	Machine *findSystem(Console *user);
	Device  *findDevice(Console *user, cstag_t name);
	Device  *findDevice(cstag_t name);

	void dial(Console *user, Machine *system);

	uint64_t getValue(cstag_t sValue);

	// command handlers
	CommandStatus cmdCreate(Console *user, args_t &args);
	CommandStatus cmdDial(Console *user, args_t &args);
	CommandStatus cmdDebug(Console *user, args_t &args);
	CommandStatus cmdDump(Console *user, args_t &args);
	CommandStatus cmdExit(Console *user, args_t &args);
	CommandStatus cmdHalt(Console *user, args_t &args);
	CommandStatus cmdList(Console *user, args_t &args);
	CommandStatus cmdLoad(Console *user, args_t &args);
	CommandStatus cmdLog(Console *user, args_t &args);
	CommandStatus cmdReset(Console *user, args_t &args);
	CommandStatus cmdRun(Console *user, args_t &args);
	CommandStatus cmdSet(Console *user, args_t &args);
	CommandStatus cmdShow(Console *user, args_t &args);
	CommandStatus cmdShowDevice(Console *user, args_t &args);
	CommandStatus cmdShowSystem(Console *user, args_t &args);
	CommandStatus cmdStart(Console *user, args_t &args);
	CommandStatus cmdStep(Console *user, args_t &args);
	CommandStatus cmdStop(Console *user, args_t &args);

private:
	Console *user = nullptr;

	Machine *dialedMachine = nullptr;
	Device *dialedSystem   = nullptr;

	// Continuously output for last command
	command_t *lastCommand = nullptr;
	Device    *lastDevice  = nullptr;
	offs_t     lastAddress = 0;

	// Machines
	static vector<Machine *> machines;

	static command_t mseCommands[];
//	static command_t mseShowCommands[];
//	static command_t mseSetCommands[];
};
