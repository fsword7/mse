/*
 * console.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"
#include "emu/engine.h"

class Console
{
public:
	Console() : engine(this) {}
	~Console() = default;

	void script(fs::path fname);
	void prompt();

	inline void setLastAddress(Device *device, offs_t addr) { cmd.lastDevice = device; cmd.lastAddress = addr; }
	inline offs_t  getLastAddress(Device *&device) { device = cmd.lastDevice; return cmd.lastAddress; }

	template <typename ... Args>
	void printf(cstag_t &format, Args ... args)
	{
		fmt::printf(format, forward<Args>(args) ...);
	}

public:
	SystemEngine engine;

private:
//	SystemEngine engine;
	CommandHandler cmd;
};
