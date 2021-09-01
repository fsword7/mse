/*
 * execute.cpp - Intel 8080/8085 processor emulation package
 *
 *  Created on: Aug 17, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/i8080/i8080.h"

uint8_t i8080_cpuCycles[256] =
{
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, // 00-0F
    4, 10,  7,  5,  5,  5,  7,  4,  4, 10,  7,  5,  5,  5,  7,  4, // 10-1F
    4, 10, 16,  5,  5,  5,  7,  4,  4, 10, 16,  5,  5,  5,  7,  4, // 20-2F
    4, 10, 13,  5, 10, 10, 10,  4,  4, 10, 13,  5,  5,  5,  7,  4, // 30-3F
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 40-4F 
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 50-5F 
    5,  5,  5,  5,  5,  5,  7,  5,  5,  5,  5,  5,  5,  5,  7,  5, // 60-6F 
    7,  7,  7,  7,  7,  7,  7,  7,  5,  5,  5,  5,  5,  5,  7,  5, // 70-7F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 80-8F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 90-9F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // A0-AF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // B0-BF
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 11,  7, 11, // C0-CF
    5, 10, 10, 10, 11, 11,  7, 11,  5, 10, 10, 10, 11, 11,  7, 11, // D0-DF
    5, 10, 10, 10, 11, 11,  7, 11,  5,  5, 10,  5, 11, 11,  7, 11, // E0-EF
    5, 10, 10,  4, 11, 11,  7, 11,  5,  5, 10,  4, 11, 11,  7, 11  // F0-FF
};

uint8_t i8085_cpuCycles[256] =
{
    4, 10,  7,  6,  4,  4,  7,  4, 10, 10,  7,  6,  4,  4,  7,  4, // 00-0F
    7, 10,  7,  6,  4,  4,  7,  4, 10, 10,  7,  6,  4,  4,  7,  4, // 10-1F
    7, 10, 16,  6,  4,  4,  7,  4, 10, 10, 16,  6,  4,  4,  7,  4, // 20-2F
    7, 10, 13,  6, 10, 10, 10,  4, 10, 10, 13,  6,  4,  4,  7,  4, // 30-3F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 40-4F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 50-5F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 60-6F 
    7,  7,  7,  7,  7,  7,  5,  7,  4,  4,  4,  4,  4,  4,  7,  4, // 70-7F 
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 80-8F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // 90-9F
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // A0-AF
    4,  4,  4,  4,  4,  4,  7,  4,  4,  4,  4,  4,  4,  4,  7,  4, // B0-BF
    6, 10, 10, 10, 11, 12,  7, 12,  6, 10, 10, 12, 11, 11,  7, 12, // C0-CF
    6, 10, 10, 10, 11, 12,  7, 12,  6, 10, 10, 10, 11, 10,  7, 12, // D0-DF
    6, 10, 10, 16, 11, 12,  7, 12,  6,  6, 10,  5, 11, 10,  7, 12, // E0-EF
    6, 10, 10,  4, 11, 12,  7, 12,  6,  6, 10,  4, 11, 10,  7, 12  // F0-FF
};

void i8080_cpuDevice::init()
{


    for (int val = 0; val < 256; val++)
    {
        int p = 0;
        if (val & 1) p++;
        if (val & 2) p++;
        if (val & 4) p++;
        if (val & 8) p++;
        if (val & 16) p++;
        if (val & 32) p++;
        if (val & 64) p++;
        if (val & 128) p++;

        if (val == 0)
            zspFlags[val] |= SR_ZF;
        if (p & 1)
            zspFlags[val] |= SR_PF;
    }
}

uint8_t i8080_cpuDevice::readArg8()
{
    return mapProgram.read8(pcReg.uw++, this);
}

pair16_t i8080_cpuDevice::readArg16()
{
    pair16_t p;

    p.ub.l = mapProgram.read8(pcReg.uw++, this);
    p.ub.h = mapProgram.read8(pcReg.uw++, this);
    
    return p;
}

uint8_t i8080_cpuDevice::readMem(offs_t addr)
{
    return mapProgram.read8(addr, this);
}

void i8080_cpuDevice::writeMem(offs_t addr, uint8_t data)
{
    mapProgram.write8(addr, data, this);
}

uint8_t i8080_cpuDevice::opINR(uint8_t val)
{
    uint8_t hc = ((val & 0x0F) == 0x0F) ? SR_HF : 0;
    afReg.ub.l = (afReg.ub.l & SR_CF) | zspFlags[++val] | hc;
    return val;
}

uint8_t i8080_cpuDevice::opDCR(uint8_t val)
{
    uint8_t hc = ((val & 0x0F) == 0x0F) ? SR_HF : 0;
    afReg.ub.l = (afReg.ub.l & SR_CF) | zspFlags[--val] | hc | SR_VF;
    return val;
}

void i8080_cpuDevice::opDAD(uint16_t val)
{
    int add = hlReg.uw + val;
    afReg.ub.l = (afReg.ub.l & ~SR_CF) | ((add >> 16) & SR_CF);
    hlReg.uw = add;
}

void i8080_cpuDevice::opADD(uint8_t val)
{
    int add = afReg.ub.h + val;
    afReg.ub.l = zspFlags[add & 0xFF] | ((add >> 8) & SR_CF) |
                 (~(afReg.ub.h ^ add ^ val) & SR_HF) | SR_VF;
    afReg.ub.h = add;
}

void i8080_cpuDevice::opADC(uint8_t val)
{
    int add = afReg.ub.h + val + (afReg.ub.h & SR_CF);
    afReg.ub.l = zspFlags[add & 0xFF] | ((add >> 8) & SR_CF) |
                 (~(afReg.ub.h ^ add ^ val) & SR_HF) | SR_VF;
    afReg.ub.h = add;
}

void i8080_cpuDevice::opSUB(uint8_t val)
{
    int sub = afReg.ub.h - val;
    afReg.ub.l = zspFlags[sub & 0xFF] | ((sub >> 8) & SR_CF) |
                 (~(afReg.ub.h ^ sub ^ val) & SR_HF) | SR_VF;
    afReg.ub.h = sub;
}

void i8080_cpuDevice::opSBB(uint8_t val)
{
    int sub = afReg.ub.h - val - (afReg.ub.l & SR_CF);
    afReg.ub.l = zspFlags[sub & 0xFF] | ((sub >> 8) & SR_CF) |
                 (~(afReg.ub.h ^ sub ^ val) & SR_HF) | SR_VF;
    afReg.ub.h = sub;
}

void i8080_cpuDevice::opCMP(uint8_t val)
{
    int diff = afReg.ub.h - val;
    afReg.ub.l = zspFlags[diff & 0xFF] | ((diff >> 8) & SR_CF) |
                 (~(afReg.ub.h ^ diff ^ val) & SR_HF) | SR_VF;
}

void i8080_cpuDevice::opANA(uint8_t val)
{
    uint8_t hc = ((afReg.ub.h | val) << 1) & SR_HF;
    afReg.ub.h &= val;
    afReg.ub.l = zspFlags[afReg.ub.h];
    afReg.ub.l |= is8085() ? SR_HF : hc;
}

void i8080_cpuDevice::opORA(uint8_t val)
{
    afReg.ub.h |= val;
    afReg.ub.l = zspFlags[afReg.ub.h];
}

void i8080_cpuDevice::opXRA(uint8_t val)
{
    afReg.ub.h ^= val;
    afReg.ub.l = zspFlags[afReg.ub.h];
}

void i8080_cpuDevice::opRST(uint8_t val)
{
    opPUSH(pcReg);
    pcReg.uw = 8 * val;
}

void i8080_cpuDevice::opCALL(bool flag)
{
    if (flag == true)
    {
        pair16_t addr = readArg16();
        opPUSH(pcReg);
        pcReg = addr;
    }
    else
        pcReg.uw += 2;
}

void i8080_cpuDevice::opRET(bool flag)
{
    if (flag == true)
        pcReg = opPOP();
}

void i8080_cpuDevice::opJMP(bool flag)
{
    if (flag == true)
        pcReg = readArg16();
    else
        pcReg.uw += 2;
}

void i8080_cpuDevice::opPUSH(pair16_t val)
{
    mapProgram.write8(--spReg.uw, val.ub.h, this);
    mapProgram.write8(--spReg.uw, val.ub.l, this);
}

pair16_t i8080_cpuDevice::opPOP()
{
    pair16_t val;

    val.ub.l = mapProgram.read8(spReg.uw++, this);
    val.ub.h = mapProgram.read8(spReg.uw++, this);
    return val;
}

void i8080_cpuDevice::step(Console *user)
{
}

void i8080_cpuDevice::run()
{
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
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x04: // INR B instruction
            bcReg.ub.h = opINR(bcReg.ub.h);
            break;
        case 0x05: // DCR B instruction
            bcReg.ub.h = opDCR(bcReg.ub.h);
            break;
        case 0x06: // MVI B,nn instruction
            bcReg.ub.h = readArg8();
            break;
        case 0x07: // RLC instruction
            afReg.ub.h = (afReg.ub.h << 1) | (afReg.ub.h >> 7);
            afReg.ub.l = (afReg.ub.l & 0xFE) | (afReg.ub.h & SR_CF);
            break;
        case 0x08: // DSUB instruction (8085 only)
            if (is8080())
                break;
            break;
        case 0x09: // DAD B instruction
            opDAD(bcReg.uw);
            break;
        case 0x0A: // LDAX B instruction
            afReg.ub.h = readMem(bcReg.uw);
            break;
        case 0x0B: // DCX B instruction
            bcReg.uw--;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x0C: // INR C instruction
            bcReg.ub.l = opINR(bcReg.ub.l);
            break;
        case 0x0D: // DCR C instruction
            bcReg.ub.l = opDCR(bcReg.ub.l);
            break;
        case 0x0E: // MVI C,nn instruction
            bcReg.ub.l = readArg8();
            break;
        case 0x0F: // RRC instruction
            afReg.ub.l = (afReg.ub.l & 0xFE) | (afReg.ub.h & SR_CF);
            afReg.ub.h = (afReg.ub.h >> 1) | (afReg.ub.h << 7);
            break;
       
        case 0x10: // ASRH instruction (8085 only)
            if (is8080())
                break;
            afReg.ub.l = (afReg.ub.l & ~SR_CF) | (hlReg.ub.l & SR_CF);
            hlReg.uw = (hlReg.uw >> 1);
            break;
        case 0x11: // LXI D,nnnn instruction
            deReg = readArg16();
            break;
        case 0x12: // STAX D instruction
            writeMem(deReg.uw, afReg.ub.h);
            break;
        case 0x13: // INX D instruction
            deReg.uw++;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x14: // INR D instruction
            deReg.ub.h = opINR(deReg.ub.h);
            break;
        case 0x15: // DCR D instruction
            deReg.ub.h = opDCR(deReg.ub.h);
            break;
        case 0x16: // MVI D,nn
            deReg.ub.h = readArg8();
            break;
        case 0x17: // RAL instruction
        {
            int c = afReg.ub.l & SR_CF;
            afReg.ub.l = (afReg.ub.l & 0xFE) | (afReg.ub.h >> 7);
            afReg.ub.h = (afReg.ub.h << 1) | c;
            break;
        }
        case 0x18: // RLDE instruction (8085 only)
            if (is8080())
                break;
            afReg.ub.l = (afReg.ub.l & ~(SR_CF | SR_VF)) | (deReg.ub.h >> 7);
            deReg.uw = (deReg.uw << 1) | (deReg.uw >> 15);
            if ((((deReg.uw >> 15) ^ afReg.ub.l) & SR_CF) != 0)
                afReg.ub.l |= SR_VF;
            break;
        case 0x19: // DAD D instruction
            opDAD(deReg.uw);
            break;
        case 0x1A: // LDAX D instruction
            afReg.ub.h = readMem(deReg.uw);
            break;
        case 0x1B: // DCX D instruction
            deReg.uw--;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x1C: // INR E instruction
            deReg.ub.l = opINR(deReg.ub.l);
            break;
        case 0x1D: // DCR E instruction
            deReg.ub.l = opDCR(deReg.ub.l);
            break;
        case 0x1E: // MVI E,nn instruction
            deReg.ub.l = readArg8();
            break;
        case 0x1F: // RAR instruction
        {
            int c = (afReg.ub.l & 0x01) << 7;
            afReg.ub.l = (afReg.ub.l & 0xFE) | (afReg.ub.h & SR_CF);
            afReg.ub.h = (afReg.ub.h >> 1) | c;
            break;
        }
        case 0x20: // RIM instruction (8085 only)
            if (is8080())
                break;
            break;
        case 0x21: // LXI H,nnnn instruction
            hlReg = readArg16();
            break;
        case 0x22: // SHLD nnnn instruction
            wzReg = readArg16();
            writeMem(wzReg.uw,   hlReg.ub.l);
            writeMem(wzReg.uw+1, hlReg.ub.h);
            break;
        case 0x23: // INX H instruction
            hlReg.uw++;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x24: // INR H instruction
            hlReg.ub.h = opINR(hlReg.ub.h);
            break;
        case 0x25: // DCR H instruction
            hlReg.ub.h = opDCR(hlReg.ub.h);
            break;
        case 0x26: // MVI H,nn
            hlReg.ub.h = readArg8();
            break;
        case 0x27: // DAA instruction
            wzReg.ub.h = afReg.ub.h;

            if (is8085() && afReg.ub.l & SR_VF)
            {
                if ((afReg.ub.l & SR_HF) || ((afReg.ub.h & 0x0F) > 9))
                    wzReg.ub.h -= 6;
                if ((afReg.ub.l & SR_CF) || (afReg.ub.h > 0x99))
                    wzReg.ub.h -= 0x60;
            }
            else
            {
                if ((afReg.ub.l & SR_HF) || ((afReg.ub.h & 0x0F) > 9))
                    wzReg.ub.h += 6;
                if ((afReg.ub.l & SR_CF) || (afReg.ub.h > 0x99))
                    wzReg.ub.h += 0x60;
            }

            afReg.ub.l = (afReg.ub.l & 3) | (afReg.ub.h & 0x28) |
                ((afReg.ub.h > 0x99) ? 1 : 0) | ((afReg.ub.h ^ wzReg.ub.h) & 0x10) |
                zspFlags[wzReg.ub.h];
            afReg.ub.h = wzReg.ub.h;
            break;
        case 0x28: // LDEH nn instruction (8085 only)
            if (is8080())
                break;
            wzReg.uw = readArg8();
            deReg.uw = (hlReg.uw + wzReg.uw) & 0xFFFF;
            break;

        case 0x29: // DAD H instruction
            opDAD(hlReg.uw);
            break;
        case 0x2A: // LHLD nnnn instruction
            wzReg = readArg16();
            hlReg.ub.l = readMem(wzReg.uw);
            hlReg.ub.h = readMem(wzReg.uw+1);
            break;
        case 0x2B: // DCX H instruction
            hlReg.uw--;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x2C: // INR L instruction
            hlReg.ub.l = opINR(hlReg.ub.l);
            break;
        case 0x2D: // DCR L instruction
            hlReg.ub.l = opDCR(hlReg.ub.l);
            break;
        case 0x2E: // MVI L,nn instruction
            hlReg.ub.l = readArg8();
            break;
        case 0x2F: // CMA instruction
            afReg.ub.h ^= 0xFF;
            if (is8085())
                afReg.ub.l |= SR_HF|SR_VF;
            break;

        case 0x30: // SIM instruction (8085 only)
            if (is8080())
                break;
            break;
        case 0x31: // LXI SP,nnnn instruction
            spReg = readArg16();
            break;
        case 0x32: // STAX nnnn instruction
            wzReg = readArg16();
            writeMem(wzReg.uw, afReg.ub.h);
            break;
        case 0x33: // INX SP instruction
            spReg.uw++;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        case 0x34: // INR M instruction
            wzReg.ub.l = opINR(readMem(hlReg.uw));
            writeMem(hlReg.uw, wzReg.ub.l);
            break;
        case 0x35: // DCR M instruction
            wzReg.ub.l = opDCR(readMem(hlReg.uw));
            writeMem(hlReg.uw, wzReg.ub.l);
            break;
        case 0x36: // MVI M,nn instruction
            wzReg.ub.l = readArg8();
            writeMem(hlReg.uw, wzReg.ub.l);
            break;
        case 0x37: // STC instruction
            afReg.ub.l = (afReg.ub.l & 0xFE) | SR_CF;
            break;
        case 0x38: // LDES nn instruction (8085 only)
            if (is8080())
                break;
            wzReg.uw = readArg8();
            deReg.uw = (spReg.uw + wzReg.uw) & 0xFFFF;
            break;
        case 0x39: // DAD SP instruction
            opDAD(spReg.uw);
            break;
        case 0x3A: // LDAX nnnn instruction
            wzReg = readArg16();
            afReg.ub.h = readMem(wzReg.uw);
            break;
        case 0x3B: // DCX SP instruction
            spReg.uw--;
            if (is8085())
            {
                afReg.sb.l &= ~SR_VF;
                if (bcReg.sw == 0x0000)
                    afReg.ub.l |= SR_VF;
            }
            break;
        
        case 0x3C: // INR A instruction
            afReg.ub.h = opINR(afReg.ub.h);
            break;
        case 0x3D: // DCR A instruction
            afReg.ub.h = opDCR(afReg.ub.h);
            break;
        case 0x3E: // MVI A,nn instruction
            afReg.ub.h = readArg8();
            break;
        case 0x3F: // CMC instruction
            afReg.ub.l = (afReg.ub.l & 0xFE) | (~afReg.ub.l & SR_CF);
            break;

        case 0x40: bcReg.ub.h = bcReg.ub.h; break;
        case 0x41: bcReg.ub.h = bcReg.ub.l; break;
        case 0x42: bcReg.ub.h = deReg.ub.h; break;
        case 0x43: bcReg.ub.h = deReg.ub.l; break;
        case 0x44: bcReg.ub.h = hlReg.ub.h; break;
        case 0x45: bcReg.ub.h = hlReg.ub.l; break;
        case 0x46: bcReg.ub.h = readMem(hlReg.uw); break;
        case 0x47: bcReg.ub.h = afReg.ub.h; break;

        case 0x48: bcReg.ub.l = bcReg.ub.h; break;
        case 0x49: bcReg.ub.l = bcReg.ub.l; break;
        case 0x4A: bcReg.ub.l = deReg.ub.h; break;
        case 0x4B: bcReg.ub.l = deReg.ub.l; break;
        case 0x4C: bcReg.ub.l = hlReg.ub.h; break;
        case 0x4D: bcReg.ub.l = hlReg.ub.l; break;
        case 0x4E: bcReg.ub.l = readMem(hlReg.uw); break;
        case 0x4F: bcReg.ub.l = afReg.ub.h; break;

        case 0x50: deReg.ub.h = bcReg.ub.h; break;
        case 0x51: deReg.ub.h = bcReg.ub.l; break;
        case 0x52: deReg.ub.h = deReg.ub.h; break;
        case 0x53: deReg.ub.h = deReg.ub.l; break;
        case 0x54: deReg.ub.h = hlReg.ub.h; break;
        case 0x55: deReg.ub.h = hlReg.ub.l; break;
        case 0x56: deReg.ub.h = readMem(hlReg.uw); break;
        case 0x57: deReg.ub.h = afReg.ub.h; break;

        case 0x58: deReg.ub.l = bcReg.ub.h; break;
        case 0x59: deReg.ub.l = bcReg.ub.l; break;
        case 0x5A: deReg.ub.l = deReg.ub.h; break;
        case 0x5B: deReg.ub.l = deReg.ub.l; break;
        case 0x5C: deReg.ub.l = hlReg.ub.h; break;
        case 0x5D: deReg.ub.l = hlReg.ub.l; break;
        case 0x5E: deReg.ub.l = readMem(hlReg.uw); break;
        case 0x5F: deReg.ub.l = afReg.ub.h; break;

        case 0x60: hlReg.ub.h = bcReg.ub.h; break;
        case 0x61: hlReg.ub.h = bcReg.ub.l; break;
        case 0x62: hlReg.ub.h = deReg.ub.h; break;
        case 0x63: hlReg.ub.h = deReg.ub.l; break;
        case 0x64: hlReg.ub.h = hlReg.ub.h; break;
        case 0x65: hlReg.ub.h = hlReg.ub.l; break;
        case 0x66: hlReg.ub.h = readMem(hlReg.uw); break;
        case 0x67: hlReg.ub.h = afReg.ub.h; break;

        case 0x68: hlReg.ub.l = bcReg.ub.h; break;
        case 0x69: hlReg.ub.l = bcReg.ub.l; break;
        case 0x6A: hlReg.ub.l = deReg.ub.h; break;
        case 0x6B: hlReg.ub.l = deReg.ub.l; break;
        case 0x6C: hlReg.ub.l = hlReg.ub.h; break;
        case 0x6D: hlReg.ub.l = hlReg.ub.l; break;
        case 0x6E: hlReg.ub.l = readMem(hlReg.uw); break;
        case 0x6F: hlReg.ub.l = afReg.ub.h; break;

        case 0x70: writeMem(hlReg.uw, bcReg.ub.h); break;
        case 0x71: writeMem(hlReg.uw, bcReg.ub.l); break;
        case 0x72: writeMem(hlReg.uw, deReg.ub.h); break;
        case 0x73: writeMem(hlReg.uw, deReg.ub.l); break;
        case 0x74: writeMem(hlReg.uw, hlReg.ub.h); break;
        case 0x75: writeMem(hlReg.uw, hlReg.ub.l); break;
        case 0x76:                                 break; // HLT instruction
        case 0x77: writeMem(hlReg.uw, afReg.ub.h); break;

        case 0x78: afReg.ub.h = bcReg.ub.h; break;
        case 0x79: afReg.ub.h = bcReg.ub.l; break;
        case 0x7A: afReg.ub.h = deReg.ub.h; break;
        case 0x7B: afReg.ub.h = deReg.ub.l; break;
        case 0x7C: afReg.ub.h = hlReg.ub.h; break;
        case 0x7D: afReg.ub.h = hlReg.ub.l; break;
        case 0x7E: afReg.ub.h = readMem(hlReg.uw); break;
        case 0x7F: afReg.ub.h = afReg.ub.h; break;

        case 0x80: opADD(bcReg.ub.h); break;
        case 0x81: opADD(bcReg.ub.l); break;
        case 0x82: opADD(deReg.ub.h); break;
        case 0x83: opADD(deReg.ub.l); break;
        case 0x84: opADD(hlReg.ub.h); break;
        case 0x85: opADD(hlReg.ub.l); break;
        case 0x86: wzReg.ub.l = readMem(hlReg.uw); opADD(wzReg.ub.l); break;
        case 0x87: opADD(afReg.ub.h); break;
     
        case 0x88: opADC(bcReg.ub.h); break;
        case 0x89: opADC(bcReg.ub.l); break;
        case 0x8A: opADC(deReg.ub.h); break;
        case 0x8B: opADC(deReg.ub.l); break;
        case 0x8C: opADC(hlReg.ub.h); break;
        case 0x8D: opADC(hlReg.ub.l); break;
        case 0x8E: wzReg.ub.l = readMem(hlReg.uw); opADC(wzReg.ub.l); break;
        case 0x8F: opADC(afReg.ub.h); break;
     
        case 0x90: opSUB(bcReg.ub.h); break;
        case 0x91: opSUB(bcReg.ub.l); break;
        case 0x92: opSUB(deReg.ub.h); break;
        case 0x93: opSUB(deReg.ub.l); break;
        case 0x94: opSUB(hlReg.ub.h); break;
        case 0x95: opSUB(hlReg.ub.l); break;
        case 0x96: wzReg.ub.l = readMem(hlReg.uw); opSUB(wzReg.ub.l); break;
        case 0x97: opSUB(afReg.ub.h); break;
     
        case 0x98: opSBB(bcReg.ub.h); break;
        case 0x99: opSBB(bcReg.ub.l); break;
        case 0x9A: opSBB(deReg.ub.h); break;
        case 0x9B: opSBB(deReg.ub.l); break;
        case 0x9C: opSBB(hlReg.ub.h); break;
        case 0x9D: opSBB(hlReg.ub.l); break;
        case 0x9E: wzReg.ub.l = readMem(hlReg.uw); opSBB(wzReg.ub.l); break;
        case 0x9F: opSBB(afReg.ub.h); break;
     
        case 0xA0: opANA(bcReg.ub.h); break;
        case 0xA1: opANA(bcReg.ub.l); break;
        case 0xA2: opANA(deReg.ub.h); break;
        case 0xA3: opANA(deReg.ub.l); break;
        case 0xA4: opANA(hlReg.ub.h); break;
        case 0xA5: opANA(hlReg.ub.l); break;
        case 0xA6: wzReg.ub.l = readMem(hlReg.uw); opANA(wzReg.ub.l); break;
        case 0xA7: opANA(afReg.ub.h); break;
     
        case 0xA8: opXRA(bcReg.ub.h); break;
        case 0xA9: opXRA(bcReg.ub.l); break;
        case 0xAA: opXRA(deReg.ub.h); break;
        case 0xAB: opXRA(deReg.ub.l); break;
        case 0xAC: opXRA(hlReg.ub.h); break;
        case 0xAD: opXRA(hlReg.ub.l); break;
        case 0xAE: wzReg.ub.l = readMem(hlReg.uw); opXRA(wzReg.ub.l); break;
        case 0xAF: opXRA(afReg.ub.h); break;
     
        case 0xB0: opORA(bcReg.ub.h); break;
        case 0xB1: opORA(bcReg.ub.l); break;
        case 0xB2: opORA(deReg.ub.h); break;
        case 0xB3: opORA(deReg.ub.l); break;
        case 0xB4: opORA(hlReg.ub.h); break;
        case 0xB5: opORA(hlReg.ub.l); break;
        case 0xB6: wzReg.ub.l = readMem(hlReg.uw); opORA(wzReg.ub.l); break;
        case 0xB7: opORA(afReg.ub.h); break;
     
        case 0xB8: opCMP(bcReg.ub.h); break;
        case 0xB9: opCMP(bcReg.ub.l); break;
        case 0xBA: opCMP(deReg.ub.h); break;
        case 0xBB: opCMP(deReg.ub.l); break;
        case 0xBC: opCMP(hlReg.ub.h); break;
        case 0xBD: opCMP(hlReg.ub.l); break;
        case 0xBE: wzReg.ub.l = readMem(hlReg.uw); opCMP(wzReg.ub.l); break;
        case 0xBF: opCMP(afReg.ub.h); break;

        case 0xC0: // RNZ instruction
            opRET((afReg.ub.l & SR_ZF) == 0);
            break;
        case 0xC1: // POP B instruction
            bcReg = opPOP();
            break;
        case 0xC2: // JNZ nnnn instruction
            opJMP((afReg.ub.l & SR_ZF) == 0);
            break;
        case 0xC3: // JMP nnnn instruction
            opJMP(true);
            break;
        case 0xC4: // CNZ nnnn instruction
            opCALL((afReg.ub.l & SR_ZF) == 0);
            break;
        case 0xC5: // PUSH B instruction
            opPUSH(bcReg);
            break;
        case 0xC6: // ADI nn instruction
            wzReg.ub.l = readArg8();
            opADD(wzReg.ub.l);
            break;
        case 0xC7: // RST 0 instruction
            opRST(0);
            break;

        case 0xC8: // RZ instruction
            opRET(afReg.ub.l & SR_ZF);
            break;
        case 0xC9: // RET instruction
            pcReg = opPOP();
            break;
        case 0xCA: // JZ instruction
            opJMP(afReg.ub.l & SR_ZF);
            break;
        case 0xCB: // RST V instruction (8085 only)
            if (is8085())
            {
                if (afReg.ub.l & SR_VF)
                    opRST(8);
            }
            else
                opJMP(true);
            break;
        case 0xCC: // CZ nnnn instruction
            opCALL(afReg.ub.l & SR_ZF);
            break;
        case 0xCD: // CALL nnnn instruction
            opCALL(true);
            break;
        case 0xCE: // ACI nn instruction
            wzReg.ub.l = readArg8();
            opADC(wzReg.ub.l);
            break;
        case 0xCF: // RST 1 instruction
            opRST(1);
            break;

        case 0xD0: // RNC instruction
            opRET((afReg.ub.l & SR_CF) == 0);
            break;
        case 0xD1: // POP D
            deReg = opPOP();
            break;
        case 0xD2: // JNC nnnn
            opJMP((afReg.ub.l & SR_CF) == 0);
            break;
        case 0xD3: // OUT nn
            wzReg.uw = readArg8();
            mapIOPort.write8(wzReg.uw, afReg.ub.h, this);
            break;
        case 0xD4: // CNC nnnn
            opCALL((afReg.ub.l & SR_CF) == 0);
            break;
        case 0xD5: // PUSH D
            opPUSH(deReg);
            break;
        case 0xD6: // SUI nn
            wzReg.ub.l = readArg8();
            opSUB(wzReg.ub.l);
            break;
        case 0xD7: // RST 2 instruction
            opRST(2);
            break;

        case 0xD8: // RC instruction
            opRET(afReg.ub.l & SR_CF);
            break;
        case 0xD9: // SHLX (8085 only)
            if (is8085())
            {
                wzReg.uw = deReg.uw;
                writeMem(wzReg.uw, hlReg.ub.l);
                writeMem(wzReg.uw+1, hlReg.ub.h);
            }
            else
                pcReg = opPOP();
            break;
        case 0xDA: // JC nnnn
            opJMP(afReg.ub.l & SR_CF);
            break;
        case 0xDB: // IN nn
            wzReg.uw = readArg8();
            afReg.ub.h = mapIOPort.read8(wzReg.uw, this);
            break;
        case 0xDC: // CC nnnn
            opCALL(afReg.ub.l & SR_CF);
            break;
        case 0xDD: // JNX nnnn (8085 only)
            if (is8085())
                opJMP((afReg.ub.l & SR_X5F) == 0);
            else
                opCALL(true);
            break;
        case 0xDE: // SBI nn
            wzReg.ub.l = readArg8();
            opSBB(wzReg.ub.l);
            break;
        case 0xDF: // RST 3
            opRST(3);
            break;

        case 0xE0: // RPO instruction
            opRET((afReg.ub.l & SR_PF) == 0);
            break;
        case 0xE1: // POP H instruction
            hlReg = opPOP();
            break;
        case 0xE2: // JPO nnnn instruction
            opJMP((afReg.ub.l & SR_PF) == 0);
            break;
        case 0xE3: // XTHL instruction
            wzReg = opPOP();
            opPUSH(hlReg);
            hlReg.uw = wzReg.uw;
            break;
        case 0xE4: // CPO nnnn instruction
            opCALL((afReg.ub.l & SR_PF) == 0);
            break;
        case 0xE5: // PUSH H instruction
            opPUSH(hlReg);
            break;
        case 0xE6: // ANI nn instruction
            wzReg.ub.l = readArg8();
            opANA(wzReg.ub.l);
            break;
        case 0xE7: // RST 4 instruction
            opRST(4);
            break;
        
        case 0xE8: // RPE instruction
            opRET(afReg.ub.l & SR_PF);
            break;
        case 0xE9: // PCHL instruction
            pcReg = hlReg;
            break;
        case 0xEA: // JPE instruction
            opJMP(afReg.ub.l & SR_PF);
            break;
        case 0xEB: // XCHG instruction
            wzReg = deReg;
            deReg = hlReg;
            hlReg = wzReg;
            break;
        case 0xEC: // CPE nnnn instruction
            opCALL(afReg.ub.l & SR_PF);
            break;
        case 0xED: // LHLX instruction
            if (is8085())
            {
                wzReg = deReg;
                hlReg.ub.l = readMem(wzReg.uw);
                hlReg.ub.h = readMem(wzReg.uw+1);
            }
            else
                opCALL(true);
            break;
        case 0xEE: // XRI nn instruction
            wzReg.ub.l = readArg8();
            opXRA(wzReg.ub.l);
            break;
        case 0xEF: // RST 5 instruction
            opRST(5);
            break;

        case 0xF0: // RP instruction
            opRET(afReg.ub.l & SR_SF);
            break;
        case 0xF1: // POP A instruction
            afReg = opPOP();
            break;
        case 0xF2: // JP nnnn instruction
            opJMP(afReg.ub.l & SR_SF);
            break;
        case 0xF3: // DI instruction
            enableInterrupts(false);
            break;
        case 0xF4: // CP nnnn instruction
            opCALL(afReg.ub.l & SR_SF);
            break;
        case 0xF5: // PUSH A instruction
            if (is8080())
                afReg.ub.l = (afReg.ub.l & ~(SR_X3F|SR_X5F)) | SR_VF;
            opPUSH(afReg);
            break;
        case 0xF6: // ORI nn instruction
            wzReg.ub.l = readArg8();
            opORA(wzReg.ub.l);
            break;
        case 0xF7: // RST 6 instruction
            opRST(6);
            break;
        
        case 0xF8: // RM instruction
            opRET(afReg.ub.l & SR_SF);
            break;
        case 0xF9: // SPHL instruction
            spReg = hlReg;
            break;
        case 0xFA: // JM nnnn instruction
            opJMP(afReg.ub.l & SR_SF);
            break;
        case 0xFB: // EI instruction
            enableInterrupts(true);
            break;
        case 0xFC: // CM nnnn instruction
            opCALL(afReg.ub.l & SR_SF);
            break;
        case 0xFD: // JX nnnn instruction (8085 only)
            if (is8085())
                opJMP(afReg.ub.l & SR_X5F);
            else
                opCALL(true);
            break;
        case 0xFE: // CPI nn instruction
            wzReg.ub.l = readArg8();
            opCMP(wzReg.ub.l);
            break;
        case 0xFF: // RST 7
            opRST(7);
            break;
   }
}
