/*
 * device.h - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/debugger/symbols.h"

namespace emu::debug
{
    class DeviceDebugger
    {
        public:
            DeviceDebugger(Device *device);
            ~DeviceDebugger();


        private:
            SymbolTable symTable;
    };
}

using debug_t = emu::debug::DeviceDebugger;