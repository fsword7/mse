/*
 * debug.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: Tim STark
 */

#include "emu/core.h"
#include "emu/debug.h"

LogFile::~LogFile()
{
	// Close all logging files
	close(-1);
}

bool LogFile::open(fs::path fname, int slot)
{
	if (slot < 0 || slot >= LOG_NFILES)
		return false;

	// Check if old log files is still opened
	if (fout[slot].is_open())
	{
		fout[slot] << "------ End of logging ------" << endl << flush;
		fout[slot].close();
	}

	// Open new log file at appending mode
	fout[slot].open(fname, ios::app);
	fout[slot] << "------ Start of logging ------" << endl << flush;

	logFlags |= (1u << slot);

	return false;
}

void LogFile::close(int slot)
{
	if (slot < 0)
	{
		// For any negative value, flush remaining buffers
		// and close all logging files
		logFlags = 0;
		for (int idx = 0; idx < LOG_NFILES; idx++)
		{
			if (fout[idx].is_open())
			{
				fout[slot] << "------ End of logging ------" << endl << flush;
				fout[idx].close();
			}
		}
	}
	else if (slot < LOG_NFILES)
	{
		logFlags &= ~(1u << slot);
		if (fout[slot].is_open())
		{
			fout[slot] << "------ End of logging ------" << endl << flush;
			fout[slot].close();
		}
	}
}

void LogFile::setConsole(Console *user)
{
	cty = user;
	if (cty != nullptr)
		logFlags |= LOG_CONSOLE;
	else
		logFlags &= ~LOG_CONSOLE;
}

void LogFile::flushAll()
{
	// Flush all remaining buffers
	for (int idx = 0; idx < LOG_NFILES; idx++)
		if (logFlags & (1u << idx))
			fout[idx] << flush;
}

void LogFile::out(const uint32_t flags, cstag_t &message)
{
	if ((logFlags & (flags & LOG_ALLFILES)) == 0)
		return;

	// Record message into some logging files
	for (int idx = 0; idx < LOG_NFILES; idx++)
		if (logFlags & (flags & (1u << idx)))
			fout[idx] << message;

	// Print message on operator's terminal
	if (logFlags & (flags & LOG_CONSOLE))
	{
		assert(cty != nullptr);
		cty->print(message);
	}
}

// *********************************************************

void Debug::setLogFlag(int slot, bool enable)
{
	uint32_t flags = 0;

	if (slot >= 0 && slot < LOG_NFILES)
		flags = 1u << slot;
	else if (slot == LOG_CTYSLOT)
		flags = LOG_CONSOLE;
	else if (slot == LOG_ALLSLOTS)
		flags = LOG_ALLFILES;

	if (enable == true)
		logFlags |= flags;
	else
		logFlags &= ~flags;
}

dbgOption Debug::dbgList[2] =
{
		{ "trace",		DBG_TRACE },
		{ "all",		DBG_ALL }
};

bool Debug::setOptionFlag(cstag_t &option, bool enable)
{
	uint64_t flags = 0;

	for (auto &dbg : dbgList)
		if (option == dbg.dbgName)
			flags = dbg.dbgFlag;

	if (enable == true)
		dbgFlags |= flags;
	else
		dbgFlags &= ~flags;

	return flags ? true : false;
}
