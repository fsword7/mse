/*
 * engine.cpp - System engine handler package
 *
 *  Created on: Jul 31, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/didebug.h"
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

Device *SystemEngine::findDevice(Console *cty, cstag_t name)
{
	Device *sysDevice = cty->getDialedSystem();
	if (sysDevice == nullptr)
	{
		fmt::printf("Please dial system first\n");
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

CommandStatus SystemEngine::create(Console *cty, args_t &args)
{
	SystemList list;
	string devName = args.getNext();
	string sysName = args.getNext();
	const SystemDriver *driver = nullptr;
	Machine *sys = nullptr;

	if (findSystem(devName) != nullptr) {
		fmt::fprintf(cout, "%s: system already created.\n", devName);
		return CommandStatus::cmdOk;
	}

	driver = list.find(sysName);
	if (driver == nullptr) {
		fmt::fprintf(cout, "%s: system '%s' not recognized.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	sys = Machine::create(cout, driver, devName);
	if (sys == nullptr)
	{
		fmt::fprintf(cout, "%s: system creation failure for %s - aborted.\n", devName, sysName);
		return CommandStatus::cmdOk;
	}

	// Add new system to machines list
	fmt::fprintf(cout, "%s: system %s created successfully\n", devName, sysName);
	machines.push_back(sys);

	// Dial system as default
	cty->setDialedSystem(sys->getSystemDevice());
	cty->setDialedDevice(nullptr);

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::dump(Console *cty, args_t &args)
{
	using namespace aspace;

	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	diExternalBus *sbus;
	if (!dev->hasInterface(sbus))
	{
		fmt::fprintf(cout, "%s: do not have external bus interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;

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

		fmt::printf("%s |%-16s|\n", line, lasc);
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::list(Console *cty, args_t &args)
{
	using namespace aspace;

	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	diExternalBus *sbus;
	diDebug *debug;
	if (!dev->hasInterface(sbus))
	{
		fmt::fprintf(cout, "%s: do not have external bus interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	if (!dev->hasInterface(debug))
	{
		fmt::fprintf(cout, "%s: do not have debug tools\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}

	uint32_t  sAddr, eAddr = -1;
	char     *strAddr;
	int       count = 20; // default 20 line count

	args.next();
	sscanf(args.current().c_str(), "%x", &sAddr);
	if ((strAddr = strchr(args.current().c_str(), '-')) != nullptr)
	{
		sscanf(strAddr+1, "%x", &eAddr);
		count = (eAddr - sAddr) / 4;
	}
	else if (args.size() > 3)
	{
		args.next();
		sscanf(args.current().c_str(), "%d", &count);
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
		addr += debug->list(cty, addr);
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::load(Console *cty, args_t &args)
{
	using namespace aspace;

	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	diExternalBus *sbus;
	if (!dev->hasInterface(sbus))
	{
		fmt::fprintf(cout, "%s: do not have external bus interface\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}
	AddressSpace *space = sbus->getAddressSpace();

	args.next();
	fs::path fname = args.current();

	args.next();
	offs_t off;
	sscanf(args.current().c_str(), "%llx", &off);
	offs_t soff = off;

	try {
		ifstream fin(fname, ios::binary);
		uint8_t blkData[512];
		while (!fin.eof())
		{
			fin.read((char *)blkData, sizeof(blkData));
			space->writeBlock(off, blkData, fin.gcount());
			off += fin.gcount();
		}
		fin.close();
		fmt::printf("%s: Loaded into %llX-%llX (length: %d bytes)\n", fname, soff, off, off - soff);
	}

	catch (system_error &e)
	{
		fmt::fprintf(cerr, "%s: file error: %s\n", fname, e.code().message());
		cout << flush;
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::set(Console *cty, args_t &args)
{
	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	devCommand_t *cmdList = dev->getCommands();
	if (cmdList == nullptr) {
		fmt::printf("%s: do not handle device commands\n", dev->getDeviceName());
		return CommandStatus::cmdOk;
	}

	args.next();
	for (int idx = 0; cmdList[idx].name; idx++) {
		if (cmdList[idx].name == args.current()) {
			args.next();
			if (cmdList[idx].func != nullptr)
				return cmdList[idx].func(cty, dev, args);
		}
	}

	fmt::printf("%s: Unknown device command\n", dev->getDeviceName());

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::show(Console *cty, args_t &args)
{
	Device *dev = findDevice(cty, args.current());
	if (dev == nullptr) {
		fmt::printf("%s: unknown device\n", args.current());
		return CommandStatus::cmdOk;
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::showDevices(Console *cty, args_t &args)
{
	Device *root = findDevice(cty, args.current());
	if (root == nullptr) {
		fmt::printf("No such devices\n");
		return CommandStatus::cmdOk;
	}

	for (Device &dev : DeviceIterator(*root))
	{
		fmt::printf("%s\n", dev.getDeviceName());
	}

	return CommandStatus::cmdOk;
}

CommandStatus SystemEngine::start(Console *cty, args_t &args)
{
//	Device *sysDevice = cty->getDialedSystem();
//	if (sysDevice == nullptr)
//	{
//		fmt::printf("Please dial system first\n");
//		return CommandStatus::cmdOk;
//	}
//	Machine *sys = sysDevice->getMachine();

	string devName = args.getNext();
	Machine *sys = findSystem(devName);
	if (sys == nullptr) {
		fmt::fprintf(cout, "%s: system not found.\n", devName);
		return CommandStatus::cmdOk;
	}

	// Start system device as final initialization
	sys->start(cty);

	return CommandStatus::cmdOk;
}
