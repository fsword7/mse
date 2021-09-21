/*
 * i8080.h - Intel 8080/8085 processor emulation package
 *
 *  Created on: Jun 24, 2021
 *      Author: Tim Stark
 */

#pragma once

#include "emu/devproc.h"

// Status register definitions

#define SR_SF   0x80
#define SR_ZF   0x40
#define SR_X5F  0x20
#define SR_HF   0x10
#define SR_X3F  0x08
#define SR_PF   0x04
#define SR_VF   0x02
#define SR_CF   0x01

// 8/16-bit register defintions
//
// +-----+-----+
// |  A  |  F  |
// +-----+-----+
// |  B  |  C  |
// +-----+-----+
// |  D  |  E  |
// +-----+-----+
// |  H  |  L  |
// +-----+-----+
// |     SP    |
// +-----------+
// |     PC    |
// +-----------+

#define REGn_A   0
#define REGn_F   1 // F - flags, M - indirect HL register
#define REGn_B   2
#define REGn_C   3
#define REGn_D   4
#define REGn_E   5
#define REGn_H   6
#define REGn_L   7

#define REGn_AF  0
#define REGn_BC  1
#define REGn_DE  2
#define REGn_HL  3
#define REGn_SP  4
#define REGn_PC  5

#define REGAn(opCode)   (opCode & 0x7)
#define REGBn(opCode)   ((opCode >> 3) & 0x7)
#define REGWn(opCode)   ((opCode >> 4) & 0x3)
#define REGA(opCode)    state.bRegs[REGAn(opCode)+2]
#define REGB(opCode)    state.bRegs[REGBn(opCode)+2]
#define REGW(opCode)    state.wRegs[REGWn(opCode)+1]

#define REG_A           state.bRegs[REGn_A]
#define REG_F           state.bRegs[REGn_F]
#define REG_HL          state.wRegs[REGn_HL]

// #define REGn_A   0
// #define REGn_F   1 // F - flags, M - indirect HL register
// #define REGn_B   2
// #define REGn_C   3
// #define REGn_D   4
// #define REGn_E   5
// #define REGn_H   6
// #define REGn_L   7

// #define REG_A           state2.iRegs[REGn_AF].ub.h
// #define REG_F           state2.iRegs[REGn_AF].ub.l
// #define REG_B           state2.iRegs[REGn_BC].ub.h
// #define REG_C           state2.iRegs[REGn_BC].ub.l
// #define REG_D           state2.iRegs[REGn_DE].ub.h
// #define REG_E           state2.iRegs[REGn_DE].ub.l
// #define REG_H           state2.iRegs[REGn_DE].ub.h
// #define REG_L           state2.iRegs[REGn_DE].ub.l

// #define REG_BC          state2.iRegs[REGn_BC].uw;
// #define REG_DE          state2.iRegs[REGn_DE].uw;
// #define REG_HL          state2.iRegs[REGn_HL].uw;
// #define REG_SP          state2.iRegs[REGn_SP].uw;
// #define REG_PC          state2.iRegs[REGn_PC].uw;

// Intel 8080/8085 series processor
class i8080_cpuDevice : public cpuDevice
{
    public:
        enum cpuType
        {
            cpuid_8080,
            cpuid_8080A,
            cpuid_8085A
        };

        enum stateRegisters
        {
            i8080_PC, i8080_SP, i8080_A,  i8080_B,
            i8080_C,  i8080_D,  i8080_E,  i8080_F,
            i8080_H,  i8080_L,  i8080_AF, i8080_BC,
            i8080_DE, i8080_HL
        };

	    i8080_cpuDevice(const SystemConfig &config, const DeviceType &type, const string &tagName, Device *owner, uint64_t clock, cpuType idType);
        i8080_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	    virtual ~i8080_cpuDevice() = default;

	    // Virtual function calls from execution interface
	    void step(Console *user) override;
	    // void setPCAddress(offs_t addr) override;
	    // bool load(ifstream &fin, offs_t off) override;
	    int list(Console *cty, offs_t vAddr) override;

        mapConfigList getAddressConfigList() const;
        
        void run();
        void execute();

        void startDevice() override;

        // Device state register function calls
        void exportString(const DeviceStateEntry &entry, string &out) const override;
        
    protected:
        void init();

        inline bool is8080() { return idProcessorType == cpuid_8080 || idProcessorType == cpuid_8080A; }
        inline bool is8085() { return idProcessorType == cpuid_8085A; }

        void enableInterrupts(bool sw);

        uint8_t readArg8();
        pair16_t readArg16();
        uint8_t readMem(offs_t addr);
        void writeMem(offs_t addr, uint8_t data);
       
        uint8_t opINR(uint8_t val);
        uint8_t opDCR(uint8_t val);
        void opDAD(uint16_t val);

        void opADD(uint8_t val);
        void opADC(uint8_t val);
        void opSUB(uint8_t val);
        void opSBB(uint8_t val);
        void opANA(uint8_t val);
        void opORA(uint8_t val);
        void opXRA(uint8_t val);
        void opCMP(uint8_t val);
        void opRST(uint8_t val);
        void opCALL(bool flag);
        void opRET(bool flag);
        void opJMP(bool flag);
        void opPUSH(pair16_t val);
        pair16_t opPOP();

    protected:
        cpuType idProcessorType;

        // General-purpose registers
        pair16_t afReg; // AF register
        pair16_t bcReg; // BC register
        pair16_t deReg; // DE register
        pair16_t hlReg; // HL register
        pair16_t pcReg; // PC register
        pair16_t spReg; // SP register
        pair16_t wzReg; 
        uint16_t pcBase; // base PC address (current instruction)

        union {
            uint8_t  bRegs[8];
            uint16_t wRegs[4];
        } state;

        // struct {
        //     pair16_t iRegs[6];
        // } state2;

        uint8_t zspFlags[256]; // Z/P flags settting

        mapAddressConfig mapProgramConfig;
        mapAddressConfig mapIOPortConfig;

        aspace::MemoryAccess<16, 0, 0, LittleEndian>::specific mapProgram;
        aspace::MemoryAccess<8, 0, 0, LittleEndian>::specific mapIOPort;
};

class i8080a_cpuDevice : public i8080_cpuDevice
{
public:
	i8080a_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8080a_cpuDevice() = default;

};

class i8085a_cpuDevice : public i8080_cpuDevice
{
public:
	i8085a_cpuDevice(const SystemConfig &config, const string &tagName, Device *owner, uint64_t clock);
	virtual ~i8085a_cpuDevice() = default;

};

DECLARE_DEVICE_TYPE(i8080, i8080_cpuDevice);
DECLARE_DEVICE_TYPE(i8080A, i8080a_cpuDevice);
DECLARE_DEVICE_TYPE(i8805A, i8085a_cpuDevice);