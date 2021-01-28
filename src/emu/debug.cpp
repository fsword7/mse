/*
 * debug.cpp
 *
 *  Created on: Jan 20, 2021
 *      Author: Tim STark
 */

#include "emu/core.h"
#include "emu/debug.h"

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
	fout[slot].open(fname, ios::ate);
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

void LogFile::out(const uint32_t flags, cstag_t &message)
{
	if ((logFlags & (flags & LOG_ALLFILES)) == 0)
		return;

	// Record message into some logging files
	for (int idx = 0; idx < LOG_NFILES; idx++)
		if (logFlags & (flags & (1u << idx)))
			fout[idx] << message << flush;
}

// *********************************************************
