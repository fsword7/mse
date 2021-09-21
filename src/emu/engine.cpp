/*
 * engine.cpp - System engine handler package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/didebug.h"
#include "emu/diexec.h"
#include "emu/distate.h"
#include "emu/machine.h"
#include "emu/engine.h"
#include "emu/syslist.h"

vector<Machine *> SystemEngine::machines;

// Global system initialization routines
void SystemEngine::ginit()
{
	machines.clear();
}

// Global system shutdown and cleanup routines
void SystemEngine::gexit()
{
	// Release all machines from memory allocation
	for (auto mach : machines)
		delete mach;
	machines.clear();
}


Machine *SystemEngine::findSystem(const string sysName)
{
	for (auto mach : machines)
		if (sysName == mach->getDeviceName())
			return mach;
	return nullptr;
}

Machine *SystemEngine::findSystem(Console *user)
{
	if (dialedMachine == nullptr)
	{
		user->printf("Please dial system first\n");
		return nullptr;
	}

	return dialedMachine;
}

Device *SystemEngine::findDevice(Console *user, cstag_t name)
{
	if (dialedSystem == nullptr)
	{
		user->printf("Please dial system first\n");
		return nullptr;
	}

	for (Device &dev : DeviceIterator(*dialedSystem))
		if (dev.getDeviceName() == name)
			return &dev;

	return nullptr;
}

Device *SystemEngine::findDevice(cstag_t name)
{
	if (dialedSystem == nullptr)
		return nullptr;

	for (Device &dev : DeviceIterator(*dialedSystem))
		if (dev.getDeviceName() == name)
			return &dev;

	return nullptr;
}

void SystemEngine::dial(Console *user, Machine *system)
{
	assert(user != nullptr);
	if (dialedMachine != nullptr)
		system->setConsole(nullptr);
	if (system != nullptr)
		system->setConsole(user);

	dialedMachine = system;
	dialedSystem = system->getSystemDevice();
}

// K = kilobyte - 1024 bytes
// M = megabyte - 1024*1024 bytes
// G = gigabyte - 1024*1024*1024 bytes
// T = terabyte - 1024*1024*1024*1024 bytes

static ctag_t *byteScale = " kmgt";

uint64_t SystemEngine::getValue(cstag_t sValue)
{
	uint64_t value = 0;
	char     bType = 0;

	stringstream ssValue(sValue);

	ssValue >> value;
	ssValue >> bType;

//	fmt::printf("Value: %lld Scale: %c\n", value, bType);
	if (bType != 0) {
		uint64_t scale = 1;
		for (int idx = 0; idx < strlen(byteScale); idx++) {
//			fmt::printf("Scale: %c  Value: %lld\n", byteScale[idx], scale);
			if (bType == byteScale[idx]) {
				value *= scale;
				break;
			}
			scale *= 1024;
		}
	}
//	fmt::printf("Value: %lld (%llX) bytes\n", value, value);

	return value;
}

CommandStatus SystemEngine::cmdCreate(Console *user, args_t &args)
{
	SystemList list;
	string devName = args.getNext();
	string sysName = args.getNext();
	const SystemDriver *driver = nullptr;
	Machine *sys = nullptr;

	if (findSystem(devName) != nullptr) {
		user->printf("%s: system already created.\n", devName);
		return CommandStatus::cmdOk;
	}

	driver = list.find(sysName);
	if (driver == nullptr) {
		user->printf("%s: system '%s' not recognized.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	sys = Machine::create(cout, *this, driver, devName);
	if (sys == nullptr)
	{
		user->printf("%s: system creation failure for %s - aborted.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	// Add new system to machines list
//	user->printf("%s: system %s created successfully\n", devName, sysName);
	machines.push_back(sys);

	// Dial system as default
	dial(user, sys);

	return cmdOk;
}

CommandStatus SystemEngine::cmdDial(Console *user, args_t &args)
{
	string devName = args.getNext();
	Machine *sys = nullptr;

	if (devName == "none") {
		dial(user, nullptr);
		user->printf("(%s): Dialed system to none\n", args[0]);
	} else {
		Machine *sys = findSystem(devName);
		if (sys != nullptr)
		{
			user->printf("(%s): system %s not found\n",
				args[0], devName);
			return cmdOk;
		}

		dial(user, sys);
	}

	return cmdOk;
}

// debug <device> <option|all> <on|off>
// debug <device> log <slot|all|{console|cty}> <on|off>
CommandStatus SystemEngine::cmdDebug(Console *user, args_t &args)
{
	Device *dev = nullptr;

	dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("(%s): Unknown device: %s\n",
			args[0], args.current());
		return cmdOk;
	}

	diDebug *devDebug = nullptr;
	if (!dev->hasInterface(devDebug))
	{
		user->printf("%s(%s): do not have debugging interface\n",
			dev->getDeviceName(), args[0]);
		return cmdOk;
	}
	Debug *dbg = devDebug->getDebugSetting();

	args.next();
	if (args.current() == "log")
	{
		// Parse cty, all or slot parameter
		int slot;
		args.next();
		if (args.current() == "console" || args.current() == "cty")
			slot = LOG_CTYSLOT;
		else if (args.current() == "all")
			slot = LOG_ALLSLOTS;
		else if (sscanf(args.current().c_str(), "%d", &slot) != 1)
		{
			user->printf("%s(%s): Invalid slot option\n",
				dev->getDeviceName(), args[0]);
			return cmdOk;
		}
		else if ((slot < 0) || (slot >= LOG_NFILES))
		{
			user->printf("%s(%s): Invalid slot number (0-%d)\n",
				dev->getDeviceName(), args[0], LOG_NFILES-1);
			return cmdOk;
		}

		// Parse on/off parameter
		bool onFlag;
		args.next();
		if (args.current() == "on")
			onFlag = true;
		else if (args.current() == "off")
			onFlag = false;
		else {
			user->printf("%s(%s): Invalid log on/off option\n",
				dev->getDeviceName(), args[0]);
			return cmdOk;
		}

		// Update logging parameters
		dbg->setLogFlag(slot, onFlag);
	}
	else
	{
		string option = args.current();

		// Parse on/off parameter
		bool onFlag;
		args.next();
		if (args.current() == "on")
			onFlag = true;
		else if (args.current() == "off")
			onFlag = false;
		else {
			user->printf("%s(%s): Invalid debug on/off option\n",
				dev->getDeviceName(), args[0]);
			return cmdOk;
		}

		if (!dbg->setOptionFlag(option, onFlag))
		{
			user->printf("%s(%s): Invalid '%s' option argument\n",
				dev->getDeviceName(), args[0], option);
			return cmdOk;
		}
	}

	return cmdOk;
}

CommandStatus SystemEngine::cmdDump(Console *user, args_t &args)
{
	using namespace aspace;

	offs_t sAddr, eAddr = -1ull;
	Device *dev = nullptr;

	if (!args.empty())
	{
		char *strAddr;

		dev = findDevice(user, args.current());
		if (dev == nullptr) {
			user->printf("%s: unknown device\n", args.current());
			return cmdOk;
		}

		args.next();
		sscanf(args.current().c_str(), "%x", &sAddr);
		if ((strAddr = strchr(args.current().c_str(), '-')) != nullptr)
			sscanf(strAddr+1, "%x", &eAddr);
		else {
			if (args.size() > 3) {
				args.next();
				sscanf(args.current().c_str(), "%x", &eAddr);
				eAddr = sAddr + eAddr - 1;
			} else if (eAddr == -1)
				eAddr = sAddr + 0x140 - 1;
		}
	} else {
		sAddr = user->getLastAddress(dev);
		eAddr = sAddr + 0x140 - 1;
	}

	diExternalBus *sbus;
	if (!dev->hasInterface(sbus))
	{
		user->printf("%s: do not have external bus interface\n", dev->getDeviceName());
		return cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	int       idx;
	char      line[256], lasc[32];
	char      *lptr, *pasc;
	uint32_t  data;
	uint32_t  sts;

	while (sAddr <= eAddr) {
		lptr = line;
		pasc = lasc;
		lptr += sprintf(lptr, "%08X: ", sAddr);
		for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++) {
			data = space->read8(sAddr++);
			lptr += sprintf(lptr, "%02X%c", data, (idx == 7) ? '-' : ' ');
			*pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
		}
		*pasc = '\0';
		*lptr = '\0';

		user->printf("%s |%-16s|\n", line, lasc);
	}

	// Save device and current address for more output
	user->setLastAddress(dev, sAddr);
	return cmdOk;
}

CommandStatus SystemEngine::cmdDumpr(Console *user, args_t &args)
{
	using namespace aspace;

	offs_t sAddr, eAddr = -1ull;
	Machine *sys = nullptr;
	string rgnName;

	char *strAddr;

	sys = findSystem(args.current());
	if (sys == nullptr) {
		user->printf("%s: unknown system\n", args.current());
		return cmdOk;
	}

	args.next();
	rgnName = args.getNext();

	sscanf(args.current().c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args.current().c_str(), '-')) != nullptr)
		sscanf(strAddr+1, "%x", &eAddr);
	else {
		if (args.size() > 3) {
			args.next();
			sscanf(args.current().c_str(), "%x", &eAddr);
			eAddr = sAddr + eAddr - 1;
		} else if (eAddr == -1)
			eAddr = sAddr + 0x140 - 1;
	}

	mapMemoryRegion *region = sys->getExternalBusManager().findRegion(rgnName);
	if (region == nullptr)
	{
		user->printf("%s: Unknown region %s - aborted.\n", sys->getDeviceName(), rgnName);
		return cmdOk;
	}

	const uint8_t *base = region->getBase();
	
	offs_t rgnSize = region->getSize();
	if (eAddr >= rgnSize)
		eAddr = rgnSize - 1;

	int       idx;
	char      line[256], lasc[32];
	char      *lptr, *pasc;
	uint32_t  data;
	uint32_t  sts;

	while (sAddr <= eAddr) {
		lptr = line;
		pasc = lasc;
		lptr += sprintf(lptr, "%08X: ", sAddr);
		for (idx = 0; (idx < 16) && (sAddr <= eAddr); idx++) {
			data = base[sAddr++];
			lptr += sprintf(lptr, "%02X%c", data, (idx == 7) ? '-' : ' ');
			*pasc++ = ((data >= 32) && (data < 127)) ? data : '.';
		}
		*pasc = '\0';
		*lptr = '\0';

		user->printf("%s |%-16s|\n", line, lasc);
	}

	return cmdOk;
}

CommandStatus SystemEngine::cmdExecute(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	diExecute *exec;
	if (!dev->hasInterface(exec))
	{
		user->printf("%s: do not have execution interface\n", dev->getDeviceName());
		return cmdOk;
	}

	exec->execute();

	return cmdOk;
}

CommandStatus SystemEngine::cmdExit(Console *user, args_t &args)
{
	return cmdShutdown;
}

CommandStatus SystemEngine::cmdHalt(Console *user, args_t &args)
{
	return cmdOk;
}

CommandStatus SystemEngine::cmdList(Console *user, args_t &args)
{
	using namespace aspace;

	Device *dev = nullptr;
	offs_t sAddr, eAddr = -1ull;
	int    count = 20;

	if (!args.empty())
	{
		char *strAddr;

		dev = findDevice(user, args.current());
		if (dev == nullptr) {
			user->printf("%s: unknown device\n", args.current());
			return cmdOk;
		}

		args.next();
		sscanf(args.current().c_str(), "%llx", &sAddr);
		if ((strAddr = strchr(args.current().c_str(), '-')) != nullptr)
		{
			sscanf(strAddr+1, "%llx", &eAddr);
			count = (eAddr - sAddr) / 4;
		}
		else if (args.size() > 3)
		{
			args.next();
			sscanf(args.current().c_str(), "%d", &count);
		}
	} else
		sAddr = user->getLastAddress(dev);

	diExternalBus *sbus;
	diDebug *debug;
	if (!dev->hasInterface(sbus))
	{
		user->printf("%s: do not have external bus interface\n", dev->getDeviceName());
		return cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	if (!dev->hasInterface(debug))
	{
		user->printf("%s: do not have debug tools\n", dev->getDeviceName());
		return cmdOk;
	}

	uint64_t addr = sAddr;
	uint32_t opCode;
	for (int idx = 0; idx < count; idx++)
	{
//		opCode = space->read32(addr);
//		fmt::printf("%llX  %08X\n", addr, opCode);
//
//		// next PC addrees
//		addr += 4;
		addr += debug->list(user, addr);
	}

	// Save device and current address for more output
	user->setLastAddress(dev, addr);
	return cmdOk;
}

CommandStatus SystemEngine::cmdLoad(Console *user, args_t &args)
{
	using namespace aspace;

	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	diExternalBus *sbus;
	if (!dev->hasInterface(sbus))
	{
		user->printf("%s: do not have external bus interface\n", dev->getDeviceName());
		return cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	diExecute *exec;
	if (!dev->hasInterface(exec)) {
		user->printf("%s: do not have execution interface\n", dev->getDeviceName());
		return cmdOk;
	}

	args.next();
	fs::path fname = args.current();

	args.next();
	offs_t off;
	sscanf(args.current().c_str(), "%llx", &off);
	offs_t soff = off;

	try {
		ifstream fin(fname, ios::binary);
//		uint8_t blkData[512];
//		while (!fin.eof())
//		{
//			fin.read((char *)blkData, sizeof(blkData));
//			space->writeBlock(off, blkData, fin.gcount());
//			off += fin.gcount();
//		}
//		fin.close();
//		user->printf("%s: Loaded %s into %llX-%llX (length: %d bytes)\n",
//			dev->getDeviceName(), fname, soff, off, off - soff);

		exec->load(fin, off);
		fin.close();
	}

	catch (system_error &e)
	{
		user->printf("%s: file error: %s\n", fname, e.code().message());
		cout << flush;

		return cmdOk;
	}

	// Assigning starting address at execution
	// if device has execution interface
//	diExecute *exec;
//	if (dev->hasInterface(exec)) {
//		exec->setPCAddress(soff);
//		user->printf("%s: Set starting execution at address %llX\n", dev->getDeviceName(), soff);
//	}

	return cmdOk;
}

CommandStatus SystemEngine::cmdLog(Console *user, args_t &args)
{
	Machine *sys = nullptr;
	LogFile *log = nullptr;
	string   fname = args.getNext();
	string   sSlot = args.getNext();

	if ((sys = findSystem(user)) == nullptr)
		return cmdOk;

	log = sys->getLogFile();

	int slot;
	sscanf(sSlot.c_str(), "%d", &slot);
	if (slot < 0 || slot >= LOG_NFILES)
	{
		user->printf("%s: Please select slot between 0 and %d\n",
			sys->getDeviceName(), LOG_NFILES);
		return cmdOk;
	}
	log->open(fname, slot);

	user->printf("%s(%s): Opened log '%s' file\n",
		sys->getDeviceName(), args[0], fname);

	return cmdOk;
}

// Usage: registers <device>
CommandStatus SystemEngine::cmdRegisters(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	diState *devState = nullptr;
	if (!dev->hasInterface(devState))
	{
		user->printf("%s: No registers available\n", dev->getDeviceName());
		return cmdOk;
	}

	for (const auto *entry : devState->getStateEntries())
	{
		user->printf("%-10s %s\n", entry->getSymbol(), entry->getValueFormat());
	}

	return cmdOk;
}

CommandStatus SystemEngine::cmdReset(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	// Reset device back to initialized state
	dev->reset();

	return cmdOk;
}

CommandStatus SystemEngine::cmdSet(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	devCommand_t *cmdList = dev->getCommands();
	if (cmdList == nullptr) {
		user->printf("%s: do not handle device commands\n", dev->getDeviceName());
		return cmdOk;
	}

	Machine *sys = dialedMachine;
	args.next();
	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args.current()) {
			args.next();
			if (cmdList[idx].func != nullptr)
				return cmdList[idx].func(user, sys, dev, args);
		}
	}

	user->printf("%s: Unknown device command\n", dev->getDeviceName());

	return cmdOk;
}

CommandStatus SystemEngine::cmdShow(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return cmdOk;
	}

	// Machine *sys = dialedMachine;
	// gdevCommand_t *cmdList = mseShowDeviceCommands;
	// args.next();
	// for (int idx = 0; cmdList[idx].name; idx++) {
	// 	if (cmdList[idx].name == args.current()) {
	// 		args.next();
	// 		if (cmdList[idx].func != nullptr)
	// 			return cmdList[idx].*func(user, sys, dev, args);
	// 	}
	// }

	return cmdOk;
}

CommandStatus SystemEngine::cmdStart(Console *user, args_t &args)
{
//	Device *sysDevice = user->getDialedSystem();
//	if (sysDevice == nullptr)
//	{
//		fmt::printf("Please dial system first\n");
//		return CommandStatus::cmdOk;
//	}
//	Machine *sys = sysDevice->getMachine();

	string devName = args.getNext();
	Machine *sys = findSystem(devName);
	if (sys == nullptr) {
		user->printf("%s: system not found.\n", devName);
		return cmdOk;
	}

	// Start system device as final initialization
	sys->start(user);

	return cmdOk;
}

CommandStatus SystemEngine::cmdStep(Console *user, args_t &args)
{
	using namespace aspace;

	Device *dev = nullptr;

	if (!args.empty())
	{
		dev = findDevice(user, args.current());
		if (dev == nullptr) {
			user->printf("%s: unknown device\n", args.current());
			return cmdOk;
		}
	}
	else
		user->getLastAddress(dev);

	diExecute *exec;
	if (!dev->hasInterface(exec))
	{
		user->printf("%s: do not have execution interface\n", dev->getDeviceName());
		return cmdOk;
	}

	user->setLastAddress(dev, 0);
	exec->step(user);

	return cmdOk;
}

CommandStatus SystemEngine::cmdStop(Console *user, args_t &args)
{
	return cmdOk;
}

// show <suboption> commands

CommandStatus SystemEngine::cmdShowDevice(Console *user, args_t &args)
{
	Device *root = findDevice(user, args.current());
	if (root == nullptr) {
		user->printf("No such devices\n");
		return cmdOk;
	}

	for (Device &dev : DeviceIterator(*root))
	{
		user->printf("%s\n", dev.getDeviceName());
	}

	return cmdOk;
}

CommandStatus SystemEngine::cmdShowSystem(Console *user, args_t &args)
{
	SystemList sysList;
	sysList.list(cout);

	return cmdOk;
}

// Usage: set <system device> folder <path name>
CommandStatus SystemEngine::cmdSetFolder(Console *user, Machine *sys, Device *dev, args_t &args)
{
	return cmdOk;
}

// Usage: show <device> registers
CommandStatus SystemEngine::cmdShowRegisters(Console *user, Machine *sys, Device *dev, args_t &args)
{
	diState *devState = nullptr;

	if (!dev->hasInterface(devState))
	{
		user->printf("$s: No registers available\n", dev->getDeviceName());
		return cmdOk;
	}

	for (const auto *entry : devState->getStateEntries())
	{
		user->printf("%-10s %08X\n", entry->getSymbol(), entry->getValue());
	}

	return cmdOk;
}

// General command list
SystemEngine::command_t SystemEngine::mseCommands[] =
{
		{ "create",		SystemEngine::cmdCreate,	nullptr },
		{ "debug",		SystemEngine::cmdDebug,		nullptr },
		{ "dial",		SystemEngine::cmdDial,		nullptr },
		{ "dump",		SystemEngine::cmdDump,		nullptr },
		{ "dumpr",		SystemEngine::cmdDumpr,		nullptr },
		{ "execute",	SystemEngine::cmdExecute,	nullptr },
		{ "exit",		SystemEngine::cmdExit,		nullptr },
		{ "halt",		SystemEngine::cmdHalt,		nullptr },
		{ "list",		SystemEngine::cmdList,		nullptr },
		{ "load",		SystemEngine::cmdLoad,		nullptr },
		{ "log",		SystemEngine::cmdLog,		nullptr },
		{ "registers",	SystemEngine::cmdRegisters, nullptr },
		{ "reset",		SystemEngine::cmdReset,		nullptr },
		{ "run",		SystemEngine::cmdExecute,	nullptr },
		{ "set",		SystemEngine::cmdSet,		nullptr },
		{ "show",		SystemEngine::cmdShow,		nullptr },
		{ "start",		SystemEngine::cmdStart,		nullptr },
		{ "step",		SystemEngine::cmdStep,		nullptr },
		{ "stop",		SystemEngine::cmdStop,		nullptr },
		{ "quit",		SystemEngine::cmdExit,		nullptr },
		// Terminator
		nullptr
};

// General device command list
SystemEngine::gdevCommand_t SystemEngine::mseSetDeviceCommands[] =
{
		{ "folder",		SystemEngine::cmdSetFolder },
		// Terminator
		nullptr
};

// General device command list
SystemEngine::gdevCommand_t SystemEngine::mseShowDeviceCommands[] =
{
		{ "registers",		SystemEngine::cmdShowRegisters },
		// Terminator
		nullptr
};
