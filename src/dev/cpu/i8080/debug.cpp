/*
 * debug.cpp - Intel 8080/8085 processor emulation package
 *
 *  Created on: Aug 18, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "dev/cpu/i8080/i8080.h"

int i8080_cpuDevice::list(Console *cty, offs_t vAddr)
{
    offs_t sAddr = vAddr;
//    uint8_t opCode = mapProgram.read8(vAddr++, this);
    uint8_t opCode = getAddressSpace(AS_PROGRAM)->read8(vAddr++, this);
    string line;

    line = fmt::sprintf("%04X %02X ", sAddr, opCode);
    switch (opCode)
    {
        case 0x00: line += fmt::sprintf("NOP"); break;
        case 0x01: line += fmt::sprintf("LXI  B,$%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x02: line += fmt::sprintf("STAX B"); break;
        case 0x03: line += fmt::sprintf("INX  B"); break;
        case 0x04: line += fmt::sprintf("INR  B"); break;
        case 0x05: line += fmt::sprintf("DCR  B"); break;
        case 0x06: line += fmt::sprintf("MVI  B,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x07: line += fmt::sprintf("RLC"); break;
        case 0x08: line += fmt::sprintf("DSUB (*)"); break;
        case 0x09: line += fmt::sprintf("DAD  B"); break;
        case 0x0A: line += fmt::sprintf("LDAX B"); break;
        case 0x0B: line += fmt::sprintf("DCX  B"); break;
        case 0x0C: line += fmt::sprintf("INR  C"); break;
        case 0x0D: line += fmt::sprintf("DCR  C"); break;
        case 0x0E: line += fmt::sprintf("MVI  C,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x0F: line += fmt::sprintf("RRC"); break;

        case 0x10: line += fmt::sprintf("ASRH (*)"); break;
        case 0x11: line += fmt::sprintf("LXI  D,$%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x12: line += fmt::sprintf("STAX D"); break;
        case 0x13: line += fmt::sprintf("INX  D"); break;
        case 0x14: line += fmt::sprintf("INR  D"); break;
        case 0x15: line += fmt::sprintf("DCR  D"); break;
        case 0x16: line += fmt::sprintf("MVI  D,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x17: line += fmt::sprintf("RAL"); break;
        case 0x18: line += fmt::sprintf("RLDE (*)"); break;
        case 0x19: line += fmt::sprintf("DAD  D"); break;
        case 0x1A: line += fmt::sprintf("LDAX D"); break;
        case 0x1B: line += fmt::sprintf("DCX  D"); break;
        case 0x1C: line += fmt::sprintf("INR  E"); break;
        case 0x1D: line += fmt::sprintf("DCR  E"); break;
        case 0x1E: line += fmt::sprintf("MVI  E,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x1F: line += fmt::sprintf("RAR"); break;

        case 0x20: line += fmt::sprintf("RIM"); break;
        case 0x21: line += fmt::sprintf("LXI  H,$%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x22: line += fmt::sprintf("SHLD $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x23: line += fmt::sprintf("INX  H"); break;
        case 0x24: line += fmt::sprintf("INR  H"); break;
        case 0x25: line += fmt::sprintf("DCR  H"); break;
        case 0x26: line += fmt::sprintf("MVI  H,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x27: line += fmt::sprintf("DAA"); break;
        case 0x28: line += fmt::sprintf("LDEH $%02X (*)", mapProgram.read8(vAddr++, this)); break;
        case 0x29: line += fmt::sprintf("DAD  H"); break;
        case 0x2A: line += fmt::sprintf("LHLD $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x2B: line += fmt::sprintf("DCX  H"); break;
        case 0x2C: line += fmt::sprintf("INR  L"); break;
        case 0x2D: line += fmt::sprintf("DCR  L"); break;
        case 0x2E: line += fmt::sprintf("MVI  L,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x2F: line += fmt::sprintf("CAM"); break;

        case 0x30: line += fmt::sprintf("SIM"); break;
        case 0x31: line += fmt::sprintf("LXI  SP,$%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x32: line += fmt::sprintf("STA  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x33: line += fmt::sprintf("INX  SP"); break;
        case 0x34: line += fmt::sprintf("INR  M"); break;
        case 0x35: line += fmt::sprintf("DCR  M"); break;
        case 0x36: line += fmt::sprintf("MVI  M,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x37: line += fmt::sprintf("DAA"); break;
        case 0x38: line += fmt::sprintf("LDES $%02X (*)", mapProgram.read8(vAddr++, this)); break;
        case 0x39: line += fmt::sprintf("DAD  SP"); break;
        case 0x3A: line += fmt::sprintf("LHA  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0x3B: line += fmt::sprintf("DCX  SP"); break;
        case 0x3C: line += fmt::sprintf("INR  A"); break;
        case 0x3D: line += fmt::sprintf("DCR  A"); break;
        case 0x3E: line += fmt::sprintf("MVI  A,$%02X", mapProgram.read8(vAddr++, this)); break;
        case 0x3F: line += fmt::sprintf("CMC"); break;

        case 0x40: line += fmt::sprintf("MOV  B,B"); break;
        case 0x41: line += fmt::sprintf("MOV  B,C"); break;
        case 0x42: line += fmt::sprintf("MOV  B,D"); break;
        case 0x43: line += fmt::sprintf("MOV  B,E"); break;
        case 0x44: line += fmt::sprintf("MOV  B,H"); break;
        case 0x45: line += fmt::sprintf("MOV  B,L"); break;
        case 0x46: line += fmt::sprintf("MOV  B,M"); break;
        case 0x47: line += fmt::sprintf("MOV  B,A"); break;

        case 0x48: line += fmt::sprintf("MOV  C,B"); break;
        case 0x49: line += fmt::sprintf("MOV  C,C"); break;
        case 0x4A: line += fmt::sprintf("MOV  C,D"); break;
        case 0x4B: line += fmt::sprintf("MOV  C,E"); break;
        case 0x4C: line += fmt::sprintf("MOV  C,H"); break;
        case 0x4D: line += fmt::sprintf("MOV  C,L"); break;
        case 0x4E: line += fmt::sprintf("MOV  C,M"); break;
        case 0x4F: line += fmt::sprintf("MOV  C,A"); break;

        case 0x50: line += fmt::sprintf("MOV  D,B"); break;
        case 0x51: line += fmt::sprintf("MOV  D,C"); break;
        case 0x52: line += fmt::sprintf("MOV  D,D"); break;
        case 0x53: line += fmt::sprintf("MOV  D,E"); break;
        case 0x54: line += fmt::sprintf("MOV  D,H"); break;
        case 0x55: line += fmt::sprintf("MOV  D,L"); break;
        case 0x56: line += fmt::sprintf("MOV  D,M"); break;
        case 0x57: line += fmt::sprintf("MOV  D,A"); break;

        case 0x58: line += fmt::sprintf("MOV  E,B"); break;
        case 0x59: line += fmt::sprintf("MOV  E,C"); break;
        case 0x5A: line += fmt::sprintf("MOV  E,D"); break;
        case 0x5B: line += fmt::sprintf("MOV  E,E"); break;
        case 0x5C: line += fmt::sprintf("MOV  E,H"); break;
        case 0x5D: line += fmt::sprintf("MOV  E,L"); break;
        case 0x5E: line += fmt::sprintf("MOV  E,M"); break;
        case 0x5F: line += fmt::sprintf("MOV  E,A"); break;

        case 0x60: line += fmt::sprintf("MOV  H,B"); break;
        case 0x61: line += fmt::sprintf("MOV  H,C"); break;
        case 0x62: line += fmt::sprintf("MOV  H,D"); break;
        case 0x63: line += fmt::sprintf("MOV  H,E"); break;
        case 0x64: line += fmt::sprintf("MOV  H,H"); break;
        case 0x65: line += fmt::sprintf("MOV  H,L"); break;
        case 0x66: line += fmt::sprintf("MOV  H,M"); break;
        case 0x67: line += fmt::sprintf("MOV  H,A"); break;

        case 0x68: line += fmt::sprintf("MOV  L,B"); break;
        case 0x69: line += fmt::sprintf("MOV  L,C"); break;
        case 0x6A: line += fmt::sprintf("MOV  L,D"); break;
        case 0x6B: line += fmt::sprintf("MOV  L,E"); break;
        case 0x6C: line += fmt::sprintf("MOV  L,H"); break;
        case 0x6D: line += fmt::sprintf("MOV  L,L"); break;
        case 0x6E: line += fmt::sprintf("MOV  L,M"); break;
        case 0x6F: line += fmt::sprintf("MOV  L,A"); break;

        case 0x70: line += fmt::sprintf("MOV  M,B"); break;
        case 0x71: line += fmt::sprintf("MOV  M,C"); break;
        case 0x72: line += fmt::sprintf("MOV  M,D"); break;
        case 0x73: line += fmt::sprintf("MOV  M,E"); break;
        case 0x74: line += fmt::sprintf("MOV  M,H"); break;
        case 0x75: line += fmt::sprintf("MOV  M,L"); break;
        case 0x76: line += fmt::sprintf("HLT"); break;
        case 0x77: line += fmt::sprintf("MOV  M,A"); break;

        case 0x78: line += fmt::sprintf("MOV  A,B"); break;
        case 0x79: line += fmt::sprintf("MOV  A,C"); break;
        case 0x7A: line += fmt::sprintf("MOV  A,D"); break;
        case 0x7B: line += fmt::sprintf("MOV  A,E"); break;
        case 0x7C: line += fmt::sprintf("MOV  A,H"); break;
        case 0x7D: line += fmt::sprintf("MOV  A,L"); break;
        case 0x7E: line += fmt::sprintf("MOV  A,M"); break;
        case 0x7F: line += fmt::sprintf("MOV  A,A"); break;

        case 0x80: line += fmt::sprintf("ADD  B"); break;
        case 0x81: line += fmt::sprintf("ADD  C"); break;
        case 0x82: line += fmt::sprintf("ADD  D"); break;
        case 0x83: line += fmt::sprintf("ADD  E"); break;
        case 0x84: line += fmt::sprintf("ADD  H"); break;
        case 0x85: line += fmt::sprintf("ADD  L"); break;
        case 0x86: line += fmt::sprintf("ADD  M"); break;
        case 0x87: line += fmt::sprintf("ADD  A"); break;

        case 0x88: line += fmt::sprintf("ADC  B"); break;
        case 0x89: line += fmt::sprintf("ADC  C"); break;
        case 0x8A: line += fmt::sprintf("ADC  D"); break;
        case 0x8B: line += fmt::sprintf("ADC  E"); break;
        case 0x8C: line += fmt::sprintf("ADC  H"); break;
        case 0x8D: line += fmt::sprintf("ADC  L"); break;
        case 0x8E: line += fmt::sprintf("ADC  M"); break;
        case 0x8F: line += fmt::sprintf("ADC  A"); break;

        case 0x90: line += fmt::sprintf("SUB  B"); break;
        case 0x91: line += fmt::sprintf("SUB  C"); break;
        case 0x92: line += fmt::sprintf("SUB  D"); break;
        case 0x93: line += fmt::sprintf("SUB  E"); break;
        case 0x94: line += fmt::sprintf("SUB  H"); break;
        case 0x95: line += fmt::sprintf("SUB  L"); break;
        case 0x96: line += fmt::sprintf("SUB  M"); break;
        case 0x97: line += fmt::sprintf("SUB  A"); break;

        case 0x98: line += fmt::sprintf("SBB  B"); break;
        case 0x99: line += fmt::sprintf("SBB  C"); break;
        case 0x9A: line += fmt::sprintf("SBB  D"); break;
        case 0x9B: line += fmt::sprintf("SBB  E"); break;
        case 0x9C: line += fmt::sprintf("SBB  H"); break;
        case 0x9D: line += fmt::sprintf("SBB  L"); break;
        case 0x9E: line += fmt::sprintf("SBB  M"); break;
        case 0x9F: line += fmt::sprintf("SBB  A"); break;

        case 0xA0: line += fmt::sprintf("ANA  B"); break;
        case 0xA1: line += fmt::sprintf("ANA  C"); break;
        case 0xA2: line += fmt::sprintf("ANA  D"); break;
        case 0xA3: line += fmt::sprintf("ANA  E"); break;
        case 0xA4: line += fmt::sprintf("ANA  H"); break;
        case 0xA5: line += fmt::sprintf("ANA  L"); break;
        case 0xA6: line += fmt::sprintf("ANA  M"); break;
        case 0xA7: line += fmt::sprintf("ANA  A"); break;

        case 0xA8: line += fmt::sprintf("ORA  B"); break;
        case 0xA9: line += fmt::sprintf("ORA  C"); break;
        case 0xAA: line += fmt::sprintf("ORA  D"); break;
        case 0xAB: line += fmt::sprintf("ORA  E"); break;
        case 0xAC: line += fmt::sprintf("ORA  H"); break;
        case 0xAD: line += fmt::sprintf("ORA  L"); break;
        case 0xAE: line += fmt::sprintf("ORA  M"); break;
        case 0xAF: line += fmt::sprintf("ORA  A"); break;

        case 0xB0: line += fmt::sprintf("XRA  B"); break;
        case 0xB1: line += fmt::sprintf("XRA  C"); break;
        case 0xB2: line += fmt::sprintf("XRA  D"); break;
        case 0xB3: line += fmt::sprintf("XRA  E"); break;
        case 0xB4: line += fmt::sprintf("XRA  H"); break;
        case 0xB5: line += fmt::sprintf("XRA  L"); break;
        case 0xB6: line += fmt::sprintf("XRA  M"); break;
        case 0xB7: line += fmt::sprintf("XRA  A"); break;

        case 0xB8: line += fmt::sprintf("CMP  B"); break;
        case 0xB9: line += fmt::sprintf("CMP  C"); break;
        case 0xBA: line += fmt::sprintf("CMP  D"); break;
        case 0xBB: line += fmt::sprintf("CMP  E"); break;
        case 0xBC: line += fmt::sprintf("CMP  H"); break;
        case 0xBD: line += fmt::sprintf("CMP  L"); break;
        case 0xBE: line += fmt::sprintf("CMP  M"); break;
        case 0xBF: line += fmt::sprintf("CMP  A"); break;

        case 0xC0: line += fmt::sprintf("RNZ"); break;
        case 0xC1: line += fmt::sprintf("POP  B"); break;
        case 0xC2: line += fmt::sprintf("JNZ  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xC3: line += fmt::sprintf("JMP  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xC4: line += fmt::sprintf("CNZ  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xC5: line += fmt::sprintf("PUSH B"); break;
        case 0xC6: line += fmt::sprintf("ADI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xC7: line += fmt::sprintf("RST  0"); break;

        case 0xC8: line += fmt::sprintf("RZ"); break;
        case 0xC9: line += fmt::sprintf("RET"); break;
        case 0xCA: line += fmt::sprintf("JZ   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xCB: line += fmt::sprintf("RSTV 8 (*)");break;
        case 0xCC: line += fmt::sprintf("CZ   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xCD: line += fmt::sprintf("CALL $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xCE: line += fmt::sprintf("ACI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xCF: line += fmt::sprintf("RST  1"); break;

        case 0xD0: line += fmt::sprintf("RNC"); break;
        case 0xD1: line += fmt::sprintf("POP  D"); break;
        case 0xD2: line += fmt::sprintf("JNC  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xD3: line += fmt::sprintf("OUT  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xD4: line += fmt::sprintf("CNC  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xD5: line += fmt::sprintf("PUSH D"); break;
        case 0xD6: line += fmt::sprintf("SUI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xD7: line += fmt::sprintf("RST  2"); break;

        case 0xD8: line += fmt::sprintf("RC"); break;
        case 0xD9: line += fmt::sprintf("SHLX D (*)"); break;
        case 0xDA: line += fmt::sprintf("JC   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xDB: line += fmt::sprintf("IN   $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xDC: line += fmt::sprintf("CC   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xDD: line += fmt::sprintf("JNX  $%04X (*)", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xDE: line += fmt::sprintf("ABI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xDF: line += fmt::sprintf("RST  3"); break;

        case 0xE0: line += fmt::sprintf("RPO"); break;
        case 0xE1: line += fmt::sprintf("POP  H"); break;
        case 0xE2: line += fmt::sprintf("JPO  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xE3: line += fmt::sprintf("XTHL"); break;
        case 0xE4: line += fmt::sprintf("CPO  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xE5: line += fmt::sprintf("PUSH H"); break;
        case 0xE6: line += fmt::sprintf("ANI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xE7: line += fmt::sprintf("RST  4"); break;

        case 0xE8: line += fmt::sprintf("RPE"); break;
        case 0xE9: line += fmt::sprintf("PCHL"); break;
        case 0xEA: line += fmt::sprintf("JPE  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xEB: line += fmt::sprintf("XCHG"); break;
        case 0xEC: line += fmt::sprintf("CPE  $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xED: line += fmt::sprintf("LHLX D (*)"); break;
        case 0xEE: line += fmt::sprintf("XRI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xEF: line += fmt::sprintf("RST  5"); break;

        case 0xF0: line += fmt::sprintf("RP"); break;
        case 0xF1: line += fmt::sprintf("POP  PSW"); break;
        case 0xF2: line += fmt::sprintf("JP   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xF3: line += fmt::sprintf("DI"); break;
        case 0xF4: line += fmt::sprintf("CP   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xF5: line += fmt::sprintf("PUSH PSW"); break;
        case 0xF6: line += fmt::sprintf("ORI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xF7: line += fmt::sprintf("RST  6"); break;

        case 0xF8: line += fmt::sprintf("RM"); break;
        case 0xF9: line += fmt::sprintf("SPHL"); break;
        case 0xFA: line += fmt::sprintf("JM   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xFB: line += fmt::sprintf("EI"); break;
        case 0xFC: line += fmt::sprintf("CM   $%04X", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xFD: line += fmt::sprintf("JX   $%04X (*)", mapProgram.read16(vAddr, this)); vAddr += 2; break;
        case 0xFE: line += fmt::sprintf("CPI  $%02X", mapProgram.read8(vAddr++, this)); break;
        case 0xFF: line += fmt::sprintf("RST  7"); break;
    }
    
    fmt::printf("%s\n", line);
    
    return vAddr - sAddr;
}
