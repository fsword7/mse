/*
 * device.h - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace emu::debug
{
    class DeviceDebugger
    {
        public:
            DeviceDebugger(const Device &device);
            ~DeviceDebugger();
    };
}

using debug_t = emu::debug::DeviceDebugger;