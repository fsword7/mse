/*
 * execute.cpp - Intel 8080/8085 processor emulation package
 *
 *  Created on: Aug 17, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/i8080/i8080.h"

pair16_t i8080_cpuDevice::readArg16()
{
    pair16_t p;

    p.ub.l = mapProgram.read8(pcReg.uw++, this);
    p.ub.h = mapProgram.read8(pcReg.uw++, this);
    
    return p;
}

void i8080_cpuDevice::writeMem(offs_t addr, uint8_t data)
{
    mapProgram.write8(addr, data, this);
}

void i8080_cpuDevice::execute()
{
    uint8_t opCode;

    switch (opCode)
    {
        case 0x00: // NOP instruction
            break;
        case 0x01: // LXI B,nnnn instruction
            bcReg = readArg16();
            break;
        case 0x02: // STAX B instruction 
            writeMem(bcReg.uw, afReg.ub.h);
            break;
        case 0x03: // INX B instruction
            bcReg.sw++;
            if (is8085())
            {
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= 0x20;
                else
                    afReg.sb.l &= ~0x20;
            }
            break;
    }
}
