/*
 * i8080.h - Intel 8080/8085 processor emulation package
 *
 *  Created on: Jun 24, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

class i8080_cpuDevice : public cpuDevice
{
    public:
        enum cpuType
        {
            cpuid_8080,
            cpuid_8080A,
            cpuid_8085A
        };

        inline bool is8080() { return idProcessorType == cpuid_8080 || idProcessorType == cpuid_8080A; }
        inline bool is8085() { return idProcessorType == cpuid_8085A; }

        void execute();

        pair16_t readArg16();
        void writeMem(offs_t addr, uint8_t data);
        
    protected:
        cpuType idProcessorType;

        // General-purpose registers
        pair16_t afReg; // AF register
        pair16_t bcReg; // BC register
        pair16_t deReg; // DE register
        pair16_t hlReg; // HL register
        pair16_t pcReg; // PC register
        pair16_t spReg; // SP register


        aspace::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;
        aspace::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;
};
