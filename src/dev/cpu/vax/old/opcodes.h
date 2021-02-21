/*
 * opcodes.h
 *
 *  Created on: Mar 14, 2017
 *      Author: Timothy Stark
 */

#pragma once

// Instruction definitons

#define OPF_RSVD     0x80000000 // Reserved Opcode
#define OPF_FPD      0x40000000 // FPD
#define OPF_REGULAR  0x00000000 // Regular Opcode
#define OPF_EMULATE  0x80000000 // Instruction is emulatable

#define OPF_CASE     0x00000800 // CASE instructions
#define OPF_STRING   0x00000400 // String Instructions
#define OPF_PACKED   0x00000200 // Packed Instructions
#define OPF_VECTOR   0x00000100 // Vector Instructions

#define OPF_FLOAT    (OPF_SFLOAT|OPF_DFLOAT|OPF_GFLOAT|OPF_HFLOAT) // Floating Instructions
#define OPF_HFLOAT   0x00000080 // H_Floating Type
#define OPF_GFLOAT   0x00000040 // G_Floating Type
#define OPF_DFLOAT   0x00000020 // D_Floating Type
#define OPF_SFLOAT   0x00000010 // F_Floating Type

#define OPF_OCTA     0x00000008 // Octaword
#define OPF_QUAD     0x00000004 // Quadword
#define OPF_WORD     0x00000002 // Word
#define OPF_BYTE     0x00000001 // Byte

#define OPC_nEXTEND  0xFC // Extended opcode offset for opcode table


// Operand mode definitions
#define OPR_MMASK     0xF0
#define OPR_RMASK     0x0F
#define OPR_MEM       -1

// Operand Mode Mask Definitions
#define OPM_ACC         0x0300 // Access Mask
#define OPM_MODE        0x00F0 // Operand Mode Mask
#define OPM_SPF         0x0008 // Special Flag
#define OPM_LEN         0x0007 // Length Mask (1 << n)
// Operand Access Type Definitions
#define OPM_ADR         0x0000 // Address Operand
#define OPM_RD          0x0100 // Read Operand
#define OPM_WR          0x0200 // Write Operand
#define OPM_MD          0x0300 // Modify Operand
// Operand Macro Definitions
#define OPM_SCALE(spec) (spec & OPM_LEN)
#define OPM_SIZE(spec)  (1u << OPM_SCALE(spec))
#define OPM_TYPE(spec)  ((spec) & OPM_ACC)

// Address Modes for Operand Decoder
#define LIT0   0x00 // Short Literal (00-3F)
#define LIT1   0x10
#define LIT2   0x20
#define LIT3   0x30
#define IDX    0x40 // Indexed Registers
#define REG    0x50 // Register
#define REGD   0x60 // Register Deferred
#define ADEC   0x70 // Auto-Decrement Register
#define AINC   0x80 // Auto-Increment Register
#define AINCD  0x90 // Auto-Increment Register Deferred
#define BDP    0xA0 // Byte-Displacement
#define BDPD   0xB0 // Byte-Displacement Deferred
#define WDP    0xC0 // Word-Displacement
#define WDPD   0xD0 // Word-Displacement Deferred
#define LDP    0xE0 // Longword-Displacement
#define LDPD   0xF0 // Longword-Displacement Deferred

// Operand Types
#define RB     (OPM_RD|0)  // .rb - Read Byte
#define RW     (OPM_RD|1)  // .rw - Read Word
#define RL     (OPM_RD|2)  // .rl - Read Longword
#define RQ     (OPM_RD|3)  // .rq - Read Quadword
#define RO     (OPM_RD|4)  // .ro - Read Octaword

#define MB     (OPM_MD|0)  // .mb - Modify Byte
#define MW     (OPM_MD|1)  // .mw - Modify Word
#define ML     (OPM_MD|2)  // .ml - Modify Longword
#define MQ     (OPM_MD|3)  // .mq - Modify Quadword
#define MO     (OPM_MD|4)  // .mo - Modify Octaword

#define WB     (OPM_WR|0)  // .wb - Write Byte
#define WW     (OPM_WR|1)  // .ww - Write Word
#define WL     (OPM_WR|2)  // .wl - Write Longword
#define WQ     (OPM_WR|3)  // .wq - Write Quadword
#define WO     (OPM_WR|4)  // .wo - Write Octaword

#define AB     (OPM_ADR|0) // .ab - Byte Address
#define AW     (OPM_ADR|1) // .aw - Word Address
#define AL     (OPM_ADR|2) // .al - Longword Address
#define AQ     (OPM_ADR|3) // .aq - Quadword Address
#define AO     (OPM_ADR|4) // .ao - Quadword Address

#define VB     (WB|OPM_SPF)   // .vb - Variable Bit Address
#define BB     (AB|OPM_SPF)   // .bb - Byte-Displacement Branch
#define BW     (AW|OPM_SPF)   // .bw - Word-Displacement Branch
#define IW     (MW|OPM_SPF)   // Bugcheck Immediate Word
#define IL     (ML|OPM_SPF)   // Bugcheck Immediate Longword

#define RF     (RL|OPM_SPF) // .rf - Read F_floating (Longword)
#define RD     (RQ|OPM_SPF) // .rd - Read D_floating (Quadword)
#define RG     (MQ|OPM_SPF) // .rg - Read G_floating (Quadword)
#define RH     (RO|OPM_SPF) // .rh - Read H_floating (Octaword)

#define MF     ML // .mf - Modify F_floating (Longword)
#define MD     MQ // .md - Modify D_floating (Quadword)
#define MG     MQ // .mg - Modify G_floating (Quadword)
#define MH     MO // .mh - Modify H_floating (Octaword)

#define WF     WL // .wf - Write F_floating (Longword)
#define WD     WQ // .wd - Write D_floating (Quadword)
#define WG     WQ // .wg - Write G_floating (Quadword)
#define WH     WO // .wh - Write H_floating (Octaword)

#define AF     AL // .af - F_floating Address (Longword)
#define AD     AQ // .ad - D_floating Address (Quadword)
#define AG     AQ // .ag - G_floating Address (Quadword)
#define AH     AO // .ah - H_floating Address (Octaword)

#define OP_ISREG(reg)         (ZXTL(reg) > -CPU_nGREGS)
#define OP_GETREG(reg)        (CPU_REGUL(~(reg)))
#define OP_GETREG1(reg)       (CPU_REGUL((~(reg)+1) & REG_MASK))
#define OP_SETREG(reg, data)  CPU_REGUL(~(reg)) = (data)
#define OP_SETREG1(reg, data) CPU_REGUL((~(reg)+1) & REG_MASK) = (data)

// Compatibility Mode: Opcode Format Definitions
#define OPCM_WIDTH  16
#define OPCM_SIZE   (1u << OPCM_WIDTH)
#define OPCM_INST   OPCM_SIZE

#define OPCM_TYPE   0377
#define OPCM_FLOAT  0400

// Compatibility Mode: Opcode Types
#define OPCM_AFOP  15
#define OPCM_ASMD  14
#define OPCM_ASOP  13
#define OPCM_FOP   12
#define OPCM_CCC   11  // Cxx Instructions
#define OPCM_SCC   10  // Sxx Instructions
#define OPCM_REG   9
#define OPCM_8BIT  8
#define OPCM_6BIT  7
#define OPCM_3BIT  6
#define OPCM_SOB   5   // SOB Instruction
#define OPCM_BR    4   // Branch Instructions
#define OPCM_RSOP  3
#define OPCM_DOP   2   // Instructions with two operands
#define OPCM_SOP   1   // Instructions with one operand
#define OPCM_NPN   0   // Instructions with no operands

#define OPCM_FOP_F   OPCM_FOP
#define OPCM_FOP_D   OPCM_FOP
#define OPCM_AFOP_F  OPCM_AFOP
#define OPCM_AFOP_D  OPCM_AFOP
#define OPCM_ASMD_F  OPCM_ASMD
#define OPCM_ASMD_D  OPCM_ASMD
#define OPCM_ASMD_FL OPCM_ASMD
#define OPCM_ASMD_DL OPCM_ASMD


// Instruction Data Structure Definitions
// Instruction Table for Assembler, Disassembler, and Execution

#define OPC_nUOPC		0xFFFF

#define OPC_nHALT		0x0000
#define OPC_nNOP		0x0001
#define OPC_nREI		0x0002
#define OPC_nBPT		0x0003
#define OPC_nRET		0x0004
#define OPC_nRSB		0x0005
#define OPC_nLDPCTX		0x0006
#define OPC_nSVPCTX		0x0007

#define OPC_nCVTPS		0x0008
#define OPC_nCVTSP		0x0009
#define OPC_nINDEX		0x000A
#define OPC_nCRC		0x000B
#define OPC_nPROBER		0x000C
#define OPC_nPROBEW		0x000D
#define OPC_nINSQUE		0x000E
#define OPC_nREMQUE		0x000F

#define OPC_nBSBB		0x0010
#define OPC_nBRB		0x0011
#define OPC_nBNEQ		0x0012
#define OPC_nBEQL		0x0013
#define OPC_nBGTR		0x0014
#define OPC_nBLEQ		0x0015
#define OPC_nJSB		0x0016
#define OPC_nJMP		0x0017

#define OPC_nBGEQ		0x0018
#define OPC_nBLSS		0x0019
#define OPC_nBGTRU		0x001A
#define OPC_nBLEQU		0x001B
#define OPC_nBVC		0x001C
#define OPC_nBVS		0x001D
#define OPC_nBCC		0x001E
#define OPC_nBCS		0x001F

#define OPC_nADDP4		0x0020
#define OPC_nADDP6		0x0021
#define OPC_nSUBP4		0x0022
#define OPC_nSUBP6		0x0023
#define OPC_nCVTPT		0x0024
#define OPC_nMULP6		0x0025
#define OPC_nCVTTP		0x0026
#define OPC_nDIVP6		0x0027

#define OPC_nMOVC3		0x0028
#define OPC_nCMPC3		0x0029
#define OPC_nSCANC		0x002A
#define OPC_nSPANC		0x002B
#define OPC_nMOVC5		0x002C
#define OPC_nCMPC5		0x002D
#define OPC_nMOVTC		0x002E
#define OPC_nMOVTUC		0x002F

#define OPC_nBSBW		0x0030
#define OPC_nBRW		0x0031
#define OPC_nCVTWL		0x0032
#define OPC_nCVTWB		0x0033
#define OPC_nMOVP		0x0034
#define OPC_nCMPP3		0x0035
#define OPC_nCVTPL		0x0036
#define OPC_nCMPP4		0x0037

#define OPC_nEDITPC		0x0038
#define OPC_nMATCHC		0x0039
#define OPC_nLOCC		0x003A
#define OPC_nSKPC		0x003B
#define OPC_nMOVZWL		0x003C
#define OPC_nACBW		0x003D
#define OPC_nMOVAW		0x003E
#define OPC_nPUSHAW		0x003F

#define OPC_nADDF2		0x0040
#define OPC_nADDF3		0x0041
#define OPC_nSUBF2		0x0042
#define OPC_nSUBF3		0x0043
#define OPC_nMULF2		0x0044
#define OPC_nMULF3		0x0045
#define OPC_nDIVF2		0x0046
#define OPC_nDIVF3		0x0047

#define OPC_nCVTFB		0x0048
#define OPC_nCVTFW		0x0049
#define OPC_nCVTFL		0x004A
#define OPC_nCVTRFL		0x004B
#define OPC_nCVTBF		0x004C
#define OPC_nCVTWF		0x004D
#define OPC_nCVTLF		0x004E
#define OPC_nACBF		0x004F

#define OPC_nMOVF		0x0050
#define OPC_nCMPF		0x0051
#define OPC_nMNEGF		0x0052
#define OPC_nTSTF		0x0053
#define OPC_nEMODF		0x0054
#define OPC_nPOLYF		0x0055
#define OPC_nCVTFD		0x0056
// Reserved				0x0057

#define OPC_nADAWI		0x0058
// Reserved				0x0059
// Reserved				0x005A
// Reserved				0x005B
#define OPC_nINSQHI		0x005C
#define OPC_nINSQTI		0x005D
#define OPC_nREMQHI		0x005E
#define OPC_nREMQTI		0x005F

#define OPC_nADDD2		0x0060
#define OPC_nADDD3		0x0061
#define OPC_nSUBD2		0x0062
#define OPC_nSUBD3		0x0063
#define OPC_nMULD2		0x0064
#define OPC_nMULD3		0x0065
#define OPC_nDIVD2		0x0066
#define OPC_nDIVD3		0x0067

#define OPC_nCVTDB		0x0068
#define OPC_nCVTDW		0x0069
#define OPC_nCVTDL		0x006A
#define OPC_nCVTRDL		0x006B
#define OPC_nCVTBD		0x006C
#define OPC_nCVTWD		0x006D
#define OPC_nCVTLD		0x006E
#define OPC_nACBD		0x006F

#define OPC_nMOVD		0x0070
#define OPC_nCMPD		0x0071
#define OPC_nMNEGD		0x0072
#define OPC_nTSTD		0x0073
#define OPC_nEMODD		0x0074
#define OPC_nPOLYD		0x0075
#define OPC_nCVTDF		0x0076
// Reserved				0x0077

#define OPC_nASHL		0x0078
#define OPC_nASHQ		0x0079
#define OPC_nEMUL		0x007A
#define OPC_nEDIV		0x007B
#define OPC_nCLRQ		0x007C
#define OPC_nMOVQ		0x007D
#define OPC_nMOVAQ		0x007E
#define OPC_nPUSHAQ		0x007F

#define OPC_nADDB2		0x0080
#define OPC_nADDB3		0x0081
#define OPC_nSUBB2		0x0082
#define OPC_nSUBB3		0x0083
#define OPC_nMULB2		0x0084
#define OPC_nMULB3		0x0085
#define OPC_nDIVB2		0x0086
#define OPC_nDIVB3		0x0087

#define OPC_nBISB2		0x0088
#define OPC_nBISB3		0x0089
#define OPC_nBICB2		0x008A
#define OPC_nBICB3		0x008B
#define OPC_nXORB2		0x008C
#define OPC_nXORB3		0x008D
#define OPC_nMNEGB		0x008E
#define OPC_nCASEB		0x008F

#define OPC_nMOVB		0x0090
#define OPC_nCMPB		0x0091
#define OPC_nMCOMB		0x0092
#define OPC_nBITB		0x0093
#define OPC_nCLRB		0x0094
#define OPC_nTSTB		0x0095
#define OPC_nINCB		0x0096
#define OPC_nDECB		0x0097

#define OPC_nCVTBL		0x0098
#define OPC_nCVTBW		0x0099
#define OPC_nMOVZBL		0x009A
#define OPC_nMOVZBW		0x009B
#define OPC_nROTL		0x009C
#define OPC_nACBB		0x009D
#define OPC_nMOVAB		0x009E
#define OPC_nPUSHAB		0x009F

#define OPC_nADDW2		0x00A0
#define OPC_nADDW3		0x00A1
#define OPC_nSUBW2		0x00A2
#define OPC_nSUBW3		0x00A3
#define OPC_nMULW2		0x00A4
#define OPC_nMULW3		0x00A5
#define OPC_nDIVW2		0x00A6
#define OPC_nDIVW3		0x00A7

#define OPC_nBISW2		0x00A8
#define OPC_nBISW3		0x00A9
#define OPC_nBICW2		0x00AA
#define OPC_nBICW3		0x00AB
#define OPC_nXORW2		0x00AC
#define OPC_nXORW3		0x00AD
#define OPC_nMNEGW		0x00AE
#define OPC_nCASEW		0x00AF

#define OPC_nMOVW		0x00B0
#define OPC_nCMPW		0x00B1
#define OPC_nMCOMW		0x00B2
#define OPC_nBITW		0x00B3
#define OPC_nCLRW		0x00B4
#define OPC_nTSTW		0x00B5
#define OPC_nINCW		0x00B6
#define OPC_nDECW		0x00B7

#define OPC_nBISPSW		0x00B8
#define OPC_nBICPSW		0x00B9
#define OPC_nPOPR		0x00BA
#define OPC_nPUSHR		0x00BB
#define OPC_nCHMK		0x00BC
#define OPC_nCHME		0x00BD
#define OPC_nCHMS		0x00BE
#define OPC_nCHMU		0x00BF

#define OPC_nADDL2		0x00C0
#define OPC_nADDL3		0x00C1
#define OPC_nSUBL2		0x00C2
#define OPC_nSUBL3		0x00C3
#define OPC_nMULL2		0x00C4
#define OPC_nMULL3		0x00C5
#define OPC_nDIVL2		0x00C6
#define OPC_nDIVL3		0x00C7

#define OPC_nBISL2		0x00C8
#define OPC_nBISL3		0x00C9
#define OPC_nBICL2		0x00CA
#define OPC_nBICL3		0x00CB
#define OPC_nXORL2		0x00CC
#define OPC_nXORL3		0x00CD
#define OPC_nMNEGL		0x00CE
#define OPC_nCASEL		0x00CF

#define OPC_nMOVL		0x00D0
#define OPC_nCMPL		0x00D1
#define OPC_nMCOML		0x00D2
#define OPC_nBITL		0x00D3
#define OPC_nCLRL		0x00D4
#define OPC_nTSTL		0x00D5
#define OPC_nINCL		0x00D6
#define OPC_nDECL		0x00D7

#define OPC_nADWC		0x00D8
#define OPC_nSBWC		0x00D9
#define OPC_nMTPR		0x00DA
#define OPC_nMFPR		0x00DB
#define OPC_nMOVPSL		0x00DC
#define OPC_nPUSHL		0x00DD
#define OPC_nMOVAL		0x00DE
#define OPC_nPUSHAL		0x00DF

#define OPC_nBBS		0x00E0
#define OPC_nBBC		0x00E1
#define OPC_nBBSS		0x00E2
#define OPC_nBBCS		0x00E3
#define OPC_nBBSC		0x00E4
#define OPC_nBBCC		0x00E5
#define OPC_nBBSSI		0x00E6
#define OPC_nBBCCI		0x00E7

#define OPC_nBLBS		0x00E8
#define OPC_nBLBC		0x00E9
#define OPC_nFFS		0x00EA
#define OPC_nFFC		0x00EB
#define OPC_nCMPV		0x00EC
#define OPC_nCMPZV		0x00ED
#define OPC_nEXTV		0x00EE
#define OPC_nEXTZV		0x00EF

#define OPC_nINSV		0x00F0
#define OPC_nACBL		0x00F1
#define OPC_nAOBLSS		0x00F2
#define OPC_nAOBLEQ		0x00F3
#define OPC_nSOBGEQ		0x00F4
#define OPC_nSOBGTR		0x00F5
#define OPC_nCVTLB		0x00F6
#define OPC_nCVTLW		0x00F7

#define OPC_nASHP		0x00F8
#define OPC_nCVTLP		0x00F9
#define OPC_nCALLG		0x00FA
#define OPC_nCALLS		0x00FB
#define OPC_nXFC		0x00FC
// Two-byte Opcodes     0x00FD - 0x00FF


// xxFD - Two-Byte Opcode Table
// Reserved				0x0100
// Reserved				0x0101
#define OPC_nWAIT		0x0102
// Reserved				0x0103
// Reserved				0x0104
// Reserved				0x0105
// Reserved				0x0106
// Reserved				0x0107

// Reserved				0x0108
// Reserved				0x0109
// Reserved				0x010A
// Reserved				0x010B
// Reserved				0x010C
// Reserved				0x010D
// Reserved				0x010E
// Reserved				0x010F

// Reserved				0x0110
// Reserved				0x0111
// Reserved				0x0112
// Reserved				0x0113
// Reserved				0x0114
// Reserved				0x0115
// Reserved				0x0116
// Reserved				0x0117

// Reserved				0x0118
// Reserved				0x0119
// Reserved				0x011A
// Reserved				0x011B
// Reserved				0x011C
// Reserved				0x011D
// Reserved				0x011E
// Reserved				0x011F

// Reserved				0x0120
// Reserved				0x0121
// Reserved				0x0122
// Reserved				0x0123
// Reserved				0x0124
// Reserved				0x0125
// Reserved				0x0126
// Reserved				0x0127

// Reserved				0x0128
// Reserved				0x0129
// Reserved				0x012A
// Reserved				0x012B
// Reserved				0x012C
// Reserved				0x012D
// Reserved				0x012E
// Reserved				0x012F

// Reserved				0x0130
#define OPC_nMFVP		0x0131
#define OPC_nCVTDH		0x0132
#define OPC_nCVTGF		0x0133
#define OPC_nVLDL		0x0134
#define OPC_nVGATHL		0x0135
#define OPC_nVLDQ		0x0136
#define OPC_nVGATHQ		0x0137

// Reserved				0x0138
// Reserved				0x0139
// Reserved				0x013A
// Reserved				0x013B
// Reserved				0x013C
// Reserved				0x013D
// Reserved				0x013E
// Reserved				0x013F

#define OPC_nADDG2		0x0140
#define OPC_nADDG3		0x0141
#define OPC_nSUBG2		0x0142
#define OPC_nSUBG3		0x0143
#define OPC_nMULG2		0x0144
#define OPC_nMULG3		0x0145
#define OPC_nDIVG2		0x0146
#define OPC_nDIVG3		0x0147

#define OPC_nCVTGB		0x0148
#define OPC_nCVTGW		0x0149
#define OPC_nCVTGL		0x014A
#define OPC_nCVTRGL		0x014B
#define OPC_nCVTBG		0x014C
#define OPC_nCVTWG		0x014D
#define OPC_nCVTLG		0x014E
#define OPC_nACBG		0x014F

#define OPC_nMOVG		0x0150
#define OPC_nCMPG		0x0151
#define OPC_nMNEGG		0x0152
#define OPC_nTSTG		0x0153
#define OPC_nEMODG		0x0154
#define OPC_nPOLYG		0x0155
#define OPC_nCVTGH		0x0156
// Reserved				0x0157

// Reserved				0x0158
// Reserved				0x0159
// Reserved				0x015A
// Reserved				0x015B
// Reserved				0x015C
// Reserved				0x015D
// Reserved				0x015E
// Reserved				0x015F

#define OPC_nADDH2		0x0160
#define OPC_nADDH3		0x0161
#define OPC_nSUBH2		0x0162
#define OPC_nSUBH3		0x0163
#define OPC_nMULH2		0x0164
#define OPC_nMULH3		0x0165
#define OPC_nDIVH2		0x0166
#define OPC_nDIVH3		0x0167

#define OPC_nCVTHB		0x0168
#define OPC_nCVTHW		0x0169
#define OPC_nCVTHL		0x016A
#define OPC_nCVTRHL		0x016B
#define OPC_nCVTBH		0x016C
#define OPC_nCVTWH		0x016D
#define OPC_nCVTLH		0x016E
#define OPC_nACBH		0x016F

#define OPC_nMOVH		0x0170
#define OPC_nCMPH		0x0171
#define OPC_nMNEGH		0x0172
#define OPC_nTSTH		0x0173
#define OPC_nEMODH		0x0174
#define OPC_nPOLYH		0x0175
#define OPC_nCVTHG		0x0176
// Reserved				0x0177

// Reserved				0x0178
// Reserved				0x0179
// Reserved				0x017A
// Reserved				0x017B
#define OPC_nCLRO		0x017C
#define OPC_nMOVO		0x017D
#define OPC_nMOVAO		0x017E
#define OPC_nPUSHAO		0x017F

#define OPC_nVVADDL		0x0180
#define OPC_nVSADDL		0x0181
#define OPC_nVVADDG		0x0182
#define OPC_nVSADDG		0x0183
#define OPC_nVVADDF		0x0184
#define OPC_nVSADDF		0x0185
#define OPC_nVVADDD		0x0186
#define OPC_nVSADDD		0x0187

#define OPC_nVVSUBL		0x0188
#define OPC_nVSSUBL		0x0189
#define OPC_nVVSUBG		0x018A
#define OPC_nVSSUBG		0x018B
#define OPC_nVVSUBF		0x018C
#define OPC_nVSSUBF		0x018D
#define OPC_nVVSUBD		0x018E
#define OPC_nVSSUBD		0x018F

// Reserved				0x0190
// Reserved				0x0191
// Reserved				0x0192
// Reserved				0x0193
// Reserved				0x0194
// Reserved				0x0195
// Reserved				0x0196
// Reserved				0x0197

#define OPC_nCVTFH		0x0198
#define OPC_nCVTFG		0x0199
#define OPC_nPROBEVMR	0x019A
#define OPC_nPROBEVMW	0x019B
#define OPC_nVSTL		0x019C
#define OPC_nVSCATL		0x019D
#define OPC_nVSTQ		0x019E
#define OPC_nVSCATQ		0x019F

#define OPC_nVVMULL		0x01A0
#define OPC_nVSMULL		0x01A1
#define OPC_nVVMULG		0x01A2
#define OPC_nVSMULG		0x01A3
#define OPC_nVVMULF		0x01A4
#define OPC_nVSMULF		0x01A5
#define OPC_nVVMULD		0x01A6
#define OPC_nVSMULD		0x01A7

#define OPC_nVSYNC		0x01A8
#define OPC_nMTVP		0x01A9
#define OPC_nVVDIVG		0x01AA
#define OPC_nVSDIVG		0x01AB
#define OPC_nVVDIVF		0x01AC
#define OPC_nVSDIVF		0x01AD
#define OPC_nVVDIVD		0x01AE
#define OPC_nVSDIVD		0x01AF

// Reserved				0x01B0
// Reserved				0x01B1
// Reserved				0x01B2
// Reserved				0x01B3
// Reserved				0x01B4
// Reserved				0x01B5
// Reserved				0x01B6
// Reserved				0x01B7

// Reserved				0x01B8
// Reserved				0x01B9
// Reserved				0x01BA
// Reserved				0x01BB
// Reserved				0x01BC
// Reserved				0x01BD
// Reserved				0x01BE
// Reserved				0x01BF

#define OPC_nVVCMPL		0x01C0
#define OPC_nVSCMPL		0x01C1
#define OPC_nVVCMPG		0x01C2
#define OPC_nVSCMPG		0x01C3
#define OPC_nVVCMPF		0x01C4
#define OPC_nVSCMPF		0x01C5
#define OPC_nVVCMPD		0x01C6
#define OPC_nVSCMPD		0x01C7

#define OPC_nVVBISL		0x01C8
#define OPC_nVSBISL		0x01C9
// Illegal Vector		0x01CA
// Illegal Vector		0x01CB
#define OPC_nVVBICL		0x01CC
#define OPC_nVSBICL		0x01CD
// Illegal Vector		0x01CE
// Illegal Vector		0x01CF

// Reserved				0x01D0
// Reserved				0x01D1
// Reserved				0x01D2
// Reserved				0x01D3
// Reserved				0x01D4
// Reserved				0x01D5
// Reserved				0x01D6
// Reserved				0x01D7

// Reserved				0x01D8
// Reserved				0x01D9
// Reserved				0x01DA
// Reserved				0x01DB
// Reserved				0x01DC
// Reserved				0x01DD
// Reserved				0x01DE
// Reserved				0x01DF

#define OPC_nVVSRLL		0x01E0
#define OPC_nVSSRLL		0x01E1
// Illegal Vector		0x01E2
// Illegal Vector		0x01E3
#define OPC_nVVSLLL		0x01E4
#define OPC_nVSSLLL		0x01E5
// Illegal Vector		0x01E6
// Illegal Vector		0x01E7

#define OPC_nVVXORL		0x01E8
#define OPC_nVSXORL		0x01E9
// Illegal Vector		0x01EA
// Illegal Vector		0x01EB
#define OPC_nVVCVT		0x01EC
#define OPC_nIOTA		0x01ED
#define OPC_nVVMERGE	0x01EE
#define OPC_nVSMERGE	0x01EF

// Reserved				0x01F0
// Reserved				0x01F1
// Reserved				0x01F2
// Reserved				0x01F3
// Reserved				0x01F4
// Reserved				0x01F5
#define OPC_nCVTHF		0x01F6
#define OPC_nCVTHD		0x01F7

// Reserved				0x01F8
// Reserved				0x01F9
// Reserved				0x01FA
// Reserved				0x01FB
// Reserved				0x01FC
// Reserved				0x01FD
// Reserved				0x01FE
// Reserved				0x01FF

// xxFE - Two-Byte Opcode Table
// Reserved				0x0200 - 0x02FF

// xxFF - Two Byte Opcode Table
// Reserved				0x0300 - 0x03F7

// Reserved				0x03F8
// Reserved				0x03F9
// Reserved				0x03FA
// Reserved				0x03FB
#define OPC_nBUGL		0x03FD
#define OPC_nBUGW		0x03FE
// Reserved all time 	0x03FF
