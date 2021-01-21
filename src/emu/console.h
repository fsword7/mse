/*
 * console.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"

class Console
{
public:
	Console() = default;
	~Console() = default;

	void script(fs::path fname);
	void prompt();

	inline void setDialedSystem(Device *device) { dialedSystem = device; }
	inline void setDialedDevice(Device *device) { dialedDevice = device; }

	inline Device *getDialedSystem() const { return dialedSystem; }
	inline Device *getDialedDevice() const { return dialedDevice; }

	inline void setLastAddress(Device *device, offs_t addr) { cmd.lastDevice = device; cmd.lastAddress = addr; }
	inline offs_t  getLastAddress(Device *&device) { device = cmd.lastDevice; return cmd.lastAddress; }

	template <typename ... Args>
	void printf(cstag_t &format, Args ... args)
	{
		fmt::printf(format, forward<Args>(args) ...);
	}


private:
	CommandHandler cmd;

	Device *dialedSystem = nullptr;
	Device *dialedDevice = nullptr;
};
