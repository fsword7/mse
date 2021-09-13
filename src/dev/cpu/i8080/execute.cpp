/*
 * execute.cpp - Intel 8080/8085 processor emulation package
 *
 *  Created on: Aug 17, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/i8080/i8080.h"
#include "dev/cpu/i8080/i8080op.h"
#include "dev/cpu/i8080/i8080dbg.h"

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
    uint16_t tval;
    int cFlag;

    switch (opCode)
    {

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


        case 0x00:
            // NOP instruction
            OP_EXEC(NOP, NOP, IMPL);
            break;

        case 0x76:
            // HLT instruction
            OP_EXEC(HLT, HLT, IMPL);
            break;

        case 0xF3:
            // EI instruction
            OP_EXEC(EI, EI, IMPL);
            break;

        case 0xFB:
            // DI instruction
            OP_EXEC(DI, DI, IMPL);
            break;

        case 0x37:
            // STC instruction
            OP_EXEC(STC, STC, IMPL);
            break;

        case 0x3F:
            // CMC instruction
            OP_EXEC(CMC, CMC, IMPL);
            break;

        case 0x2F:
            // CMA instruction
            OP_EXEC(CMA, CMA, IMPL);
            break;

        case 0xC7: case 0xCF: case 0xD7: case 0xDF:
        case 0xE7: case 0xEF: case 0xF7: case 0xFF:
            // RST n instruction
            OP_EXEC(RST, RST, RESET);
            break;



        case 0x0A: case 0x1A:
            // LDAX r instruction
            OP_EXEC(LDAX, LDAX, REGWSP);
            break;
        case 0x2A:
            // LHLD adr instruction
            tval = readArg16().uw;
            OP_EXEC(LHLD, LHLD, ADR);
            break;
        case 0x3A:
            // LDA adr instruction
            tval = readArg16().uw;
            OP_EXEC(LDA, LDA, ADR);
            break;

        case 0x01: case 0x11: case 0x21: case 0x31:
            // LXI r,d16 instruction
            tval = readArg16().uw;
            OP_EXEC(LXI, LXI, REGWSPI);
            break;

        case 0x02: case 0x12:
            // STAX r instruction
            OP_EXEC(STAX, STAX, REGWSP);
            break;
        case 0x22:
            // SHLD adr instruction
            tval = readArg16().uw;
            OP_EXEC(SHLD, SHLD, ADR);
            break;
        case 0x32:
            // STA adr instruction
            tval = readArg16().uw;
            OP_EXEC(STA, STA, ADR);
            break;

        case 0x40: case 0x41: case 0x42: case 0x43:
        case 0x44: case 0x45:            case 0x47:
        case 0x48: case 0x49: case 0x4A: case 0x4B:
        case 0x4C: case 0x4D:            case 0x4F:
        case 0x50: case 0x51: case 0x52: case 0x53:
        case 0x54: case 0x55:            case 0x57:
        case 0x58: case 0x59: case 0x5A: case 0x5B:
        case 0x5C: case 0x5D:            case 0x5F:
        case 0x60: case 0x61: case 0x62: case 0x63:
        case 0x64: case 0x65:            case 0x67:
        case 0x68: case 0x69: case 0x6A: case 0x6B:
        case 0x6C: case 0x6D:            case 0x6F:
        case 0x78: case 0x79: case 0x7A: case 0x7B:
        case 0x7C: case 0x7D:            case 0x7F:
            // MOV r,r instruction
            OP_EXEC(MOV, MOV, MOVE);
            break;

        case 0x46: case 0x4E: case 0x56: case 0x5E:
        case 0x66: case 0x6E:            case 0x7E:
            // MOV r,M instruction
            OP_EXEC(MOV, MOVrM, MOVErM)
            break;

        case 0x70: case 0x71: case 0x72: case 0x73:
        case 0x74: case 0x75:            case 0x77:
            // MOV M,r instruction
            OP_EXEC(MOV, MOVMr, MOVEMr);
            break;

        case 0x06: case 0x0E: case 0x16: case 0x1E:
        case 0x26: case 0x2E:            case 0x3E:
            // MVI r,d8 instruction
            tval = readArg8();
            OP_EXEC(MVI, MVI, REGI8);
            break;
        case 0x36:
            // MVI M,d8, instruction
            tval = readArg8();
            OP_EXEC(MVI, MVIM, REGMI8);
            break;

        case 0x04: case 0x0C: case 0x14: case 0x1C:
        case 0x24: case 0x2C:            case 0x3C:
            // INR r instruction
            OP_EXEC(INR, INR, REG);
            break;
        case 0x34:
            OP_EXEC(INR, INRM, REGM);
            break;

        case 0x05: case 0x0D: case 0x15: case 0x1D:
        case 0x25: case 0x2D:            case 0x3D:
            // DCR r instruction
            OP_EXEC(DCR, DCR, REG);
            break;
        case 0x35:
            OP_EXEC(DCR, DCRM, REGM);
            break;

        case 0x03: case 0x13: case 0x23: case 0x33:
            // INX r instruction
            OP_EXEC(INX, INX, REGWSP);
            break;

        case 0x0B: case 0x1B: case 0x2B: case 0x3B:
            // DCX r instruction
            OP_EXEC(DCX, DCX, REGWSP);
            break;

        case 0x07:
            // RLC instruction
            OP_EXEC(RLC, RLC, IMPL);
            break;
        case 0x0F:
            // RRC instruction
            OP_EXEC(RRC, RRC, IMPL);
            break;
        case 0x17:
            // RAL instruction
            OP_EXEC(RAL, RAL, IMPL);
            break;
        case 0x1F:
            // RAR instruction
            OP_EXEC(RAR, RAR, IMPL);
            break;

        case 0x09: case 0x19: case 0x29: case 0x39:
            // DAD r instruction
            OP_EXEC(DAD, DAD, REGWSP);
            break;

        case 0x80: case 0x81: case 0x82: case 0x83:
        case 0x84: case 0x85:            case 0x87:
            // ADD r instruction
            OP_EXEC(ADD, ADD, REG);
            break;
        case 0x86:
            // ADD M instruction
            OP_EXEC(ADD, ADDM, REG);
            break;
        case 0xC6:
            // ADI d8 instruction
            tval = readArg8();
            OP_EXEC(ADI, ADI, IMM8);
            break;

        case 0x88: case 0x89: case 0x8A: case 0x8B:
        case 0x8C: case 0x8D:            case 0x8F:
            // ADC r instruction
            OP_EXEC(ADC, ADC, REG);
            break;
        case 0x8E:
            // ADC M instruction
            OP_EXEC(ADC, ADCM, REG);
            break;
        case 0xCE:
            // ACI d8 instruction
            tval = readArg8();
            OP_EXEC(ACI, ACI, IMM8);
            break;

        case 0x90: case 0x91: case 0x92: case 0x93:
        case 0x94: case 0x95:            case 0x97:
            // SUB r instruction
            OP_EXEC(SUB, SUB, REG);
            break;
        case 0x96:
            // SUB M instruction
            OP_EXEC(SUB, SUBM, REG);
            break;
        case 0xD6:
            // SUI d8 instruction
            tval = readArg8();
            OP_EXEC(SUI, SUI, IMM8);
            break;

        case 0x98: case 0x99: case 0x9A: case 0x9B:
        case 0x9C: case 0x9D:            case 0x9F:
            // SBB r instruction
            OP_EXEC(SBB, SBB, REG);
            break;
        case 0x9E:
            // SBB M instruction
            OP_EXEC(SBB, SBBM, REG);
            break;
        case 0xDE:
            // SBI d8 instruction
            tval = readArg8();
            OP_EXEC(SBI, SBI, IMM8);
            break;

        case 0xA0: case 0xA1: case 0xA2: case 0xA3:
        case 0xA4: case 0xA5:            case 0xA7:
            // ANA r instruction
            OP_EXEC(ANA, ANA, REG);
            break;
        case 0xA6:
            // ANA M instruction
            OP_EXEC(ANA, ANAM, REG);
            break;
        case 0xE6:
            // ANI d8 instruction
            tval = readArg8();
            OP_EXEC(ANI, ANI, IMM8);
            break;

        case 0xA8: case 0xA9: case 0xAA: case 0xAB:
        case 0xAC: case 0xAD:            case 0xAF:
            // XRA r instruction
            OP_EXEC(XRA, XRA, REG);
            break;
        case 0xAE:
            // XRA M instruction
            OP_EXEC(XRA, XRAM, REG);
            break;
        case 0xEE:
            // XRI d8 instruction
            tval = readArg8();
            OP_EXEC(XRI, XRI, IMM8);
            break;
            
        case 0xB0: case 0xB1: case 0xB2: case 0xB3:
        case 0xB4: case 0xB5:            case 0xB7:
            // ORA r instruction
            OP_EXEC(ORA, ORA, REG);
            break;
        case 0xB6:
            // ORA M instruction
            OP_EXEC(ORA, ORAM, REG);
            break;
        case 0xF6:
            // ORI d8 instruction
            tval = readArg8();
            OP_EXEC(ORI, ORI, IMM8);
            break;

        case 0xB8: case 0xB9: case 0xBA: case 0xBB:
        case 0xBC: case 0xBD:            case 0xBF:
            // CMP r instruction
            OP_EXEC(CMP, CMP, REG);
            break;
        case 0xBE:
            // CMP M instruction
            OP_EXEC(CMP, CMPM, REG);
            break;
        case 0xFE:
            // CPI d8 instruction
            tval = readArg8();
            OP_EXEC(CPI, CPI, IMM8);
            break;




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

        case 0xC8: // RZ instruction
            opRET(afReg.ub.l & SR_ZF);
            break;
        case 0xC9: // RET instruction
            pcReg = opPOP();
            break;
        case 0xCA: // JZ instruction
            opJMP(afReg.ub.l & SR_ZF);
            break;
        case 0xCC: // CZ nnnn instruction
            opCALL(afReg.ub.l & SR_ZF);
            break;
        case 0xCD: // CALL nnnn instruction
            opCALL(true);
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

        case 0xF0: // RP instruction
            opRET(afReg.ub.l & SR_SF);
            break;
        case 0xF1: // POP A instruction
            afReg = opPOP();
            break;
        case 0xF2: // JP nnnn instruction
            opJMP(afReg.ub.l & SR_SF);
            break;
        case 0xF4: // CP nnnn instruction
            opCALL(afReg.ub.l & SR_SF);
            break;
        case 0xF5: // PUSH A instruction
            if (is8080())
                afReg.ub.l = (afReg.ub.l & ~(SR_X3F|SR_X5F)) | SR_VF;
            opPUSH(afReg);
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
        case 0xFC: // CM nnnn instruction
            opCALL(afReg.ub.l & SR_SF);
            break;
        case 0xFD: // JX nnnn instruction (8085 only)
            if (is8085())
                opJMP(afReg.ub.l & SR_X5F);
            else
                opCALL(true);
            break;

        // case 0x08: // DSUB instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;
       
        // case 0x10: // ASRH instruction (8085 only)
        //     if (is8080())
        //         break;
        //     afReg.ub.l = (afReg.ub.l & ~SR_CF) | (hlReg.ub.l & SR_CF);
        //     hlReg.uw = (hlReg.uw >> 1);
        //     break;
        
        // case 0x18: // RLDE instruction (8085 only)
        //     if (is8080())
        //         break;
        //     afReg.ub.l = (afReg.ub.l & ~(SR_CF | SR_VF)) | (deReg.ub.h >> 7);
        //     deReg.uw = (deReg.uw << 1) | (deReg.uw >> 15);
        //     if ((((deReg.uw >> 15) ^ afReg.ub.l) & SR_CF) != 0)
        //         afReg.ub.l |= SR_VF;
        //     break;

        // case 0x20: // RIM instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;

        // case 0x28: // LDEH nn instruction (8085 only)
        //     if (is8080())
        //         break;
        //     wzReg.uw = readArg8();
        //     deReg.uw = (hlReg.uw + wzReg.uw) & 0xFFFF;
        //     break;

        // case 0x30: // SIM instruction (8085 only)
        //     if (is8080())
        //         break;
        //     break;

        // case 0x38: // LDES nn instruction (8085 only)
        //     if (is8080())
        //         break;
        //     wzReg.uw = readArg8();
        //     deReg.uw = (spReg.uw + wzReg.uw) & 0xFFFF;
        //     break;

        // case 0xCB: // RST V instruction (8085 only)
        //     if (is8085())
        //     {
        //         if (afReg.ub.l & SR_VF)
        //             opRST(8);
        //     }
        //     else
        //         opJMP(true);
        //     break;
           
   }
}
