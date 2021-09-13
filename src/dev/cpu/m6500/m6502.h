/*
 * m6500.h - MOS 6500 processor emulation package
 *
 *  Created on: Feb 14, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

#define PSW_N 0x80 // negative bit
#define PSW_V 0x40 // overflow bit
#define PSW_B 0x10 // break bit
#define PSW_D 0x08 // decinal bit
#define PSW_I 0x04 // 
#define PSW_Z 0x02 // zero bit
#define PSW_C 0x01 // carry bit

#define DO_OPC(opCode, opType)

class m6502_cpuDevice : public cpuDevice
{
    public:
        m6502_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	    virtual ~m6502_cpuDevice() = default;

        enum registerState
        {
            m6502_A, m6502_X, m6502_Y, m6502_P,
            m6502_S, m6502_PC, m6502_IR
        };
        
        mapConfigList getAddressConfigList() const;

        void step(Console *user) override;

        void run();
        void execute();
        void reset();

        void startDevice();

    protected:
        m6502_cpuDevice(const SystemConfig &config, const DeviceType &type,
	        const string &tagName, Device *owner, uint64_t clock, int addrWidth);
        
        inline void incSP() { spReg.ub.l++; }
        inline void decSP() { spReg.ub.l--; }

        inline void setNZ(int8_t val)
        {
            pReg &= ~(PSW_N|PSW_Z);
            if (val == 0)
                pReg |= PSW_Z;
            if (val < 0)
                pReg |= PSW_N;
        }

        // 6502 opcode function calls

    protected:
        uint8_t  aReg;  // A register (accumlator)
        uint8_t  xReg;  // X register (X index)
        uint8_t  yReg;  // Y register (Y index)
        uint8_t  pReg;  // P register (status)
        pair16_t spReg; // S register (stack - always 0100-01FF)
        uint16_t pcReg; // PC register

        mapAddressConfig mapProgramConfig;

        aspace::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;
};
