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
	Device *sysDevice = user->getDialedSystem();
	if (sysDevice == nullptr)
	{
		user->printf("Please dial system first\n");
		return nullptr;
	}

	return sysDevice->getMachine();
}

Device *SystemEngine::findDevice(Console *user, cstag_t name)
{
	Device *sysDevice = user->getDialedSystem();
	if (sysDevice == nullptr)
	{
		user->printf("Please dial system first\n");
		return nullptr;
	}

	for (Device &dev : DeviceIterator(*sysDevice))
		if (dev.getDeviceName() == name)
			return &dev;

	return nullptr;
}

Device *SystemEngine::findDevice(cstag_t name)
{
	return nullptr;
}

// K = kilobyte - 1024 bytes
// M = megabyte - 1024*1024 bytes
// G = gigabyte - 1024*1024*1024 bytes
// T = terabyte - 1024*1024*1024*1024 bytes

ctag_t *byteScale = " kmgt";

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

	sys = Machine::create(cout, driver, devName);
	if (sys == nullptr)
	{
		user->printf("%s: system creation failure for %s - aborted.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	// Add new system to machines list
//	user->printf("%s: system %s created successfully\n", devName, sysName);
	machines.push_back(sys);

	// Dial system as default
	user->setDialedSystem(sys->getSystemDevice());
	user->setDialedDevice(nullptr);

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdDial(Console *user, args_t &args)
{
	string devName = args.getNext();
	Machine *sys = nullptr;

	if (devName == "none") {
		dialedMachine = nullptr;
		dialedSystem = nullptr;
		dialedDevice = nullptr;
		user->printf("(%s): Dialed system to none\n", args[0]);
	} else {
		Machine *sys = findSystem(devName);
		if (sys != nullptr) {
			dialedMachine = sys;
			dialedSystem = sys->getSystemDevice();
			dialedDevice = nullptr;
		} else
			user->printf("(%s): system %s not found\n",
				args[0], devName);
	}

	return cmdOk;
}

// debug <device> <option|all> <on|off>
// debug <device> log <slot|all|console> <on|off>
CommandStatus SystemEngine::cmdDebug(Console *user, args_t &args)
{
	Device *dev = nullptr;

	dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("(%s): Unknown device: %s\n",
			args[0], args.current());
		return CommandStatus::cmdOk;
	}

	diDebug *devDebug = nullptr;
	if (!dev->hasInterface(devDebug))
	{
		user->printf("%s(%s): do not have debugging interface\n",
			dev->getDeviceName(), args[0]);
		return CommandStatus::cmdOk;
	}
	Debug *dbg = devDebug->getDebugSetting();


	return CommandStatus::cmdOk;
}

//// Usage: debug <device> <option|all> <on|off>
////        debug <device> log <slot|all|cty> <on|off>
//static int cmdDebug(Console *con, Device *cdev, args_t &args)
//{
//	Device *sdev, *dev;
//	Debug  *dbg;
//	bool    onFlag = false;
//	int     slot = 0;
//	int     ok;
//
//	// Check number of arguments
//	if (args.size() < 3) {
//		std::cout << "Usage: " << args[0] << " <device> <option|all> <on|off>" << std::endl;
//		std::cout << "       " << args[0] << " <device> log <slot|all|cty> <on|off>" << std::endl;
//		return CMD_OK;
//	}
//
//	// Check current system device
//	sdev = con->getSystemDevice();
//	if (sdev == nullptr) {
//		std::cerr << cdev->getName() << ": Please select system device first" << std::endl;
//		return CMD_OK;
//	}
//
//	// check existing device by using name
//	dev = sdev->findDevice(args[1]);
//	if (dev == nullptr) {
//		std::cout << args[1] << ": device not found." << std::endl;
//		return CMD_OK;
//	}
//
//
//	dbg = dev->getDebug();
//	if (boost::iequals(args[2], "log")) {
//		// Parse on/off argument
//		if (boost::iequals(args[4], "on"))
//			onFlag = true;
//		else if (boost::iequals(args[4], "off"))
//			onFlag = false;
//		else {
//			std::cout << "Invalid on/off option" << std::endl;
//			return CMD_OK;
//		}
//
//		// Parse cty, all or slot argument
//		if (boost::iequals(args[3], "cty")) {
//			if (onFlag == true)
//				dbg->enableLog(LOG_CTYSLOT);
//			else
//				dbg->disableLog(LOG_CTYSLOT);
//		} else if (boost::iequals(args[3], "all")) {
//			if (onFlag == true)
//				dbg->enableLog(LOG_ALLSLOTS);
//			else
//				dbg->disableLog(LOG_ALLSLOTS);
//		} else {
//			if (sscanf(args[3].c_str(), "%d", &slot) != 1) {
//				std::cout << "Invalid slot argument" << std::endl;
//				return CMD_OK;
//			} else if ((slot < 0) || (slot > LOG_NFILES)) {
//				std::cout << "Out of valid range (0 - " << LOG_NFILES << ")" << std::endl;
//				return CMD_OK;
//			}
//			if (onFlag == true)
//				dbg->enableLog(slot);
//			else
//				dbg->disableLog(slot);
//		}
//	} else  {
//		// Parse on/off argument
//		if (boost::iequals(args[3], "on"))
//			onFlag = true;
//		else if (boost::iequals(args[3], "off"))
//			onFlag = false;
//		else {
//			std::cout << "Invalid on/off option" << std::endl;
//			return CMD_OK;
//		}
//
//		// Parse debug option argument
//		if (onFlag == true)
//			ok = dbg->enableFlag(args[2]);
//		else
//			ok = dbg->disableFlag(args[2]);
//		if (ok == CMD_NOTFOUND) {
//			std::cout << "Invalid debug option: " << args[2] << std::endl;
//			return CMD_OK;
//		}
//	}
//
//	return CMD_OK;
//}

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
			return CommandStatus::cmdOk;
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
		return CommandStatus::cmdOk;
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
	return CommandStatus::cmdOk;
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
			return CommandStatus::cmdOk;
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
		return CommandStatus::cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	if (!dev->hasInterface(debug))
	{
		user->printf("%s: do not have debug tools\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
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
	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdLoad(Console *user, args_t &args)
{
	using namespace aspace;

	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	diExternalBus *sbus;
	if (!dev->hasInterface(sbus))
	{
		user->printf("%s: do not have external bus interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	diExecute *exec;
	if (!dev->hasInterface(exec)) {
		user->printf("%s: do not have execution interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
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

		exec->load(fin);
		fin.close();
	}

	catch (system_error &e)
	{
		user->printf("%s: file error: %s\n", fname, e.code().message());
		cout << flush;

		return CommandStatus::cmdOk;
	}

	// Assigning starting address at execution
	// if device has execution interface
//	diExecute *exec;
//	if (dev->hasInterface(exec)) {
//		exec->setPCAddress(soff);
//		user->printf("%s: Set starting execution at address %llX\n", dev->getDeviceName(), soff);
//	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdLog(Console *user, args_t &args)
{
	Machine *sys = nullptr;
	LogFile *log = nullptr;
	string   fname = args.getNext();
	string   sSlot = args.getNext();

	if ((sys = findSystem(user)) == nullptr)
		return CommandStatus::cmdOk;

	log = sys->getLogFile();

	int slot;
	sscanf(sSlot.c_str(), "%d", &slot);
	if (slot < 0 || slot >= LOG_NFILES)
	{
		user->printf("%s: Please select slot between 0 and %d\n",
			sys->getDeviceName(), LOG_NFILES);
		return CommandStatus::cmdOk;
	}
	log->open(fname, slot);

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdReset(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	// Reset device back to initialized state
	dev->reset();

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdRun(Console *user, args_t &args)
{
	return cmdOk;
}

CommandStatus SystemEngine::cmdSet(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	devCommand_t *cmdList = dev->getCommands();
	if (cmdList == nullptr) {
		user->printf("%s: do not handle device commands\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}

	args.next();
	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args.current()) {
			args.next();
			if (cmdList[idx].func != nullptr)
				return cmdList[idx].func(user, dev, args);
		}
	}

	user->printf("%s: Unknown device command\n", dev->getDeviceName());

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdShow(Console *user, args_t &args)
{
	Device *dev = findDevice(user, args.current());
	if (dev == nullptr) {
		user->printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdShowDevices(Console *user, args_t &args)
{
	Device *root = findDevice(user, args.current());
	if (root == nullptr) {
		user->printf("No such devices\n");
		return CommandStatus::cmdOk;
	}

	for (Device &dev : DeviceIterator(*root))
	{
		user->printf("%s\n", dev.getDeviceName());
	}

	return CommandStatus::cmdOk;
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
		return CommandStatus::cmdOk;
	}

	// Start system device as final initialization
	sys->start(user);

	return CommandStatus::cmdOk;
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
			return CommandStatus::cmdOk;
		}
	}
	else
		user->getLastAddress(dev);

	diExecute *exec;
	if (!dev->hasInterface(exec))
	{
		user->printf("%s: do not have execution interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}

	user->setLastAddress(dev, 0);
	exec->step(user);

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::cmdStop(Console *user, args_t &args)
{
	return cmdOk;
}

// General command list
SystemEngine::command_t SystemEngine::mseCommands[] =
{
		{ "create",		SystemEngine::cmdCreate,	nullptr },
		{ "debug",		SystemEngine::cmdDebug,		nullptr },
		{ "dial",		SystemEngine::cmdDial,		nullptr },
		{ "dump",		SystemEngine::cmdDump,		nullptr },
		{ "exit",		SystemEngine::cmdExit,		nullptr },
		{ "halt",		SystemEngine::cmdHalt,		nullptr },
		{ "list",		SystemEngine::cmdList,		nullptr },
		{ "load",		SystemEngine::cmdLoad,		nullptr },
		{ "log",		SystemEngine::cmdLog,		nullptr },
		{ "reset",		SystemEngine::cmdReset,		nullptr },
		{ "run",		SystemEngine::cmdRun,		nullptr },
		{ "set",		SystemEngine::cmdSet,		nullptr },
		{ "show",		SystemEngine::cmdShow,		nullptr },
		{ "start",		SystemEngine::cmdStart,		nullptr },
		{ "step",		SystemEngine::cmdStep,		nullptr },
		{ "stop",		SystemEngine::cmdStop,		nullptr },
		{ "quit",		SystemEngine::cmdExit,		nullptr },
		// Terminator
		nullptr
};

