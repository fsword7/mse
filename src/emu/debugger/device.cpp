/*
 * device.cpp - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/dibus.h"
#include "emu/diexec.h"
#include "emu/distate.h"
#include "emu/didebug.h"
#include "emu/debugger/symbols.h"
#include "emu/debugger/device.h"

using namespace emu::debug;

DeviceDebugger::DeviceDebugger(Device *device)
: symTable(device->getMachine(), device)
{

    // Get interfaces from device
    device->hasInterface(devMemory);
    device->hasInterface(devExecute);
    device->hasInterface(devState);
    device->hasInterface(devDebug);

    if (devState != nullptr)
    {
        // Add all device state registers to symbol table
        for (const auto entry : devState->getStateEntries())
        {
            using namespace std::placeholders;

            symTable.add(entry->getSymbol().c_str(), 
                std::bind(&DeviceStateEntry::getValue, entry),
                entry->isWritable() ? std::bind(&DeviceStateEntry::setValue, entry, _1) : setFunc(nullptr),
                entry->getFormat());
        }
    }
}

DeviceDebugger::~DeviceDebugger()
{

}