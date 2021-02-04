/*
 * debug.h
 *
 *  Created on: Jan 20, 2021
 *      Author: Tim Stark
 */

#pragma once

#define DBG_NONE		0x00000000 // All log flags off
#define DBG_ALL			0xFFFFFFFF // All log flags

// Debug flags
#define DBG_TRACE		0x80000000 // Tracing execution
#define DBG_OPERAND 	0x40000000 // Operand activity
#define DBG_IOREGS		0x20000000 // I/O register activity
#define DBG_IODATA		0x10000000 // I/O data activity
#define DBG_EXCEPTION	0x08000000 // Interrupt/Exception activity

#define LOG_NFILES		8

// Log flags
#define LOG_CONSOLE		0x80000000 // Console output
#define LOG_ALLFILES	0x000000FF
#define LOG_FILE8		0x00000080
#define LOG_FILE7		0x00000040
#define LOG_FILE6		0x00000020
#define LOG_FILE5		0x00000010
#define LOG_FILE4		0x00000008
#define LOG_FILE3		0x00000004
#define LOG_FILE2		0x00000002
#define LOG_FILE1		0x00000001

// Enable/disable log slots
#define LOG_CTYSLOT		-1
#define LOG_ALLSLOTS	-2

class Console;

class LogFile
{
public:
	LogFile() = default;
	~LogFile();

	void setConsole(Console *user);
	void out(uint32_t flags, cstag_t &message);

	bool open(fs::path fname, int slot);
	void close(int slot);

private:
	uint32_t  logFlags = 0;
	Console  *cty = nullptr;
	ofstream  fout[LOG_NFILES];
};

class Debug
{
public:
	struct DebugOption
	{
		cstag_t  dbgName;
		uint64_t dbgFlag;
	};

	Debug() = default;
	~Debug() = default;

	void setLogFlag(int slot, bool enable);
	bool setOptionFlag(cstag_t &option, bool enable);

	inline void setLogFile(LogFile *log) { logFile = log; }

	template <typename... Args>
	void log(string format, Args... args)
	{
		string out = fmt::sprintf(format, args...);

		assert(logFile != nullptr);
		logFile->out(logFlags, out);

		cout << out << flush;

	//	if ((logFlags & LOG_CONSOLE) && (user != nullptr)
	//		user->printf(format, args...);
	}

private:
	uint64_t dbgFlags = 0;
	uint32_t logFlags = 0;

	LogFile *logFile = nullptr;
	Console *user = nullptr;

	static DebugOption dbgList[2];
};

using dbgOption = Debug::DebugOption;
