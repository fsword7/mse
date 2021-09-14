/*
 * debug.h - debugger package for CPU processors
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once

namespace emu::debug
{
    class dbgDevice : public Device
    {

    };
}

using debug_t = emu::debug::dbgDevice;