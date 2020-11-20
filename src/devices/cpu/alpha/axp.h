/*
 * alpha.h - DEC Alpha CPU processor package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#include "emu/devproc.h"

#define AXP_NIREGS 32  // 32 general purpose registers
#define AXP_NFREGS 32  // 32 floating registers

// Alpha Processor Instruction Field
//
// PALcode Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |                     Number                        |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Branch Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |              Displacement               |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Memory Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |          Displacement         |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// Operate Format
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// |   opcode  |    RA   |    RB   |       Function      |    RC   |
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//  3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0
//  1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0

#define OPP_CODE   26
#define OPM_CODE   0x3F
#define OPP_RA     21
#define OPM_RA     0x1F
#define OPP_RB     16
#define OPM_RB     0x1F
#define OPP_RC     0
#define OPM_RC     0x1F
#define OPP_MDSP   0
#define OPM_MDSP   0xFFFF
#define OPP_BDSP   0
#define OPM_BDSP   0x1FFFFF
#define OPP_LIT8   13
#define OPM_LIT8   0xFF
#define OPP_ILIT   12

#define OPP_IFNC   5
#define OPM_IFNC   0x3F
#define OPP_FFNC   5
#define OPM_FFNC   0x3F
#define OPP_FRND   11
#define OPM_FRND   0x3
#define OPP_PALCD  0
#define OPM_PALCD  0x3FFFFFF

#define OPP_FTRP   13
#define OPM_FTRP   0x7

#define OP_GETCD(x)   (((x) >> OPP_CODE) & OPM_CODE)
#define OP_GETRA(x)   (((x) >> OPP_RA) & OPM_RA)
#define OP_GETRB(x)   (((x) >> OPP_RB) & OPM_RB)
#define OP_GETRC(x)   (((x) >> OPP_RC) & OPM_RC)
#define OP_GETPAL(x)  (((x) >> OPP_PALCD) & OPM_PALCD)
#define OP_GETIFNC(x) (((x) >> OPP_IFNC) & OPM_IFNC)
#define OP_GETFFNC(x) (((x) >> OPP_FFNC) & OPM_FFNC)
#define OP_GETFRND(x) (((x) >> OPP_FRND) & OPM_FRND)
#define OP_GETLIT8(x) (((x) >> OPP_LIT8) & OPM_LIT8)
#define OP_GETMDSP(x) (((x) >> OPP_MDSP) & OPM_MDSP)
#define OP_GETBDSP(x) (((x) >> OPP_BDSP) & OPM_BDSP)

// PALcode Mode
#define PAL_UNDF   0  // Undefined
#define PAL_VMS    1  // VMS
#define PAL_UNIX   2  // UNIX
#define PAL_NT     3  // Windows NT

// Access Mode
#define ACC_K      0  // Kernel
#define ACC_E      1  // Executive
#define ACC_S      2  // Supervisor
#define ACC_U      3  // User


class AlphaProcessor : public ProcessorDevice
{
public:
	AlphaProcessor(const SystemConfig &config, const DeviceType &type);
	~AlphaProcessor() = default;

protected:
	// Register definitions
	uint64_t iRegs[AXP_NIREGS];	// General purpose registers
	uint64_t fRegs[AXP_NFREGS]; // Floating registers
	uint64_t pcReg;             // Program counter register

};
