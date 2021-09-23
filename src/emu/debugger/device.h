/*
 * device.h - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/debugger/symbols.h"

class diExternalBus;
class diExecute;
class diState;
class diDebug;

namespace emu::debug
{
    class DeviceDebugger
    {
        public:
            DeviceDebugger(Device *device);
            ~DeviceDebugger();

            // hook function calls from device
            void hookInstruction(offs_t pcAddr);

        private:
            SymbolTable symTable;

            diExternalBus *devMemory = nullptr;
            diExecute *devExecute = nullptr;
            diState *devState = nullptr;
            diDebug *devDebug = nullptr;
    };
}

using debug_t = emu::debug::DeviceDebugger;