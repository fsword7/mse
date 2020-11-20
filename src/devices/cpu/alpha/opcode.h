/*
 * opcode.h - Alpha CPU Processor (Opcode) package
 *
 *  Created on: Nov 20, 2020
 *      Author: Tim Stark
 */

#define OPC_nPAL     0x00
#define OPC_nLDA     0x08
#define OPC_nLDAH    0x09
#define OPC_nLDBU    0x0A
#define OPC_nLDQ_U   0x0B
#define OPC_nLDWU    0x0C
#define OPC_nSTW     0x0D
#define OPC_nSTB     0x0E
#define OPC_nSTQ_U   0x0F
#define OPC_nIALU    0x10 // ALU Operate instruction
#define OPC_nILOG    0x11
#define OPC_nISHFT   0x12
#define OPC_nIMUL    0x13
#define OPC_nIFLT    0x14
#define OPC_nVAX     0x15
#define OPC_nIEEE    0x16
#define OPC_nFP      0x17
#define OPC_nMISC    0x18
#define OPC_nPAL19   0x19
#define OPC_nJMP     0x1A
#define OPC_nPAL1B   0x1B
#define OPC_nFLTI    0x1C
#define OPC_nPAL1D   0x1D
#define OPC_nPAL1E   0x1E
#define OPC_nPAL1F   0x1F
#define OPC_nLDF     0x20
#define OPC_nLOG     0x21
#define OPC_nLDS     0x22
#define OPC_nLDT     0x23
#define OPC_nSTF     0x24
#define OPC_nSTG     0x25
#define OPC_nSTS     0x26
#define OPC_nSTT     0x27
#define OPC_nLDL     0x28
#define OPC_nLDQ     0x29
#define OPC_nLDL_L   0x2A
#define OPC_nLDQ_L   0x2B
#define OPC_nSTL     0x2C
#define OPC_nSTQ     0x2D
#define OPC_nSTL_C   0x2E
#define OPC_nSTQ_C   0x2F
#define OPC_nBR      0x30
#define OPC_nFBEQ    0x31
#define OPC_nFBLT    0x32
#define OPC_nFBLE    0x33
#define OPC_nBSR     0x34
#define OPC_nFBNE    0x35
#define OPC_nFBGE    0x36
#define OPC_nFBGT    0x37
#define OPC_nBLBC    0x38
#define OPC_nBEQ     0x39
#define OPC_nBLT     0x3A
#define OPC_nBLE     0x3B
#define OPC_nBLBS    0x3C
#define OPC_nBNE     0x3D
#define OPC_nBGE     0x3E
#define OPC_nBGT     0x3F

enum opCodes {
	OPC_PAL,  OPC_01,    OPC_02,    OPC_03,
	OPC_04,   OPC_05,    OPC_06,    OPC_07,
	OPC_LDA,  OPC_LDAH,  OPC_LDBU,  OPC_LDQ_U,
	OPC_LDWU, OPC_STW,   OPC_STB,   OPC_STQ_U,
	OPC_IALU, OPC_ILOG,  OPC_ISHFT, OPC_IMUL,
	OPC_IFLT, OPC_VAX,   OPC_IEEE,  OPC_FP,
	OPC_MISC, OPC_PAL19, OPC_JMP,   OPC_PAL1B,
	OPC_FLTI, OPC_PAL1D, OPC_PAL1E, OPC_PAL1F,
	OPC_LDF,  OPC_LOG,   OPC_LDS,   OPC_LDT,
	OPC_STF,  OPC_STG,   OPC_STS,   OPC_STT,
	OPC_LDL,  OPC_LDQ,   OPC_LDL_L, OPC_LDQ_L,
	OPC_STL,  OPC_STQ,   OPC_STL_C, OPC_STQ_C,
	OPC_BR,   OPC_FBEQ,  OPC_FBLT,  OPC_FBLE,
	OPC_BSR,  OPC_FBNE,  OPC_FBGE,  OPC_FBGT,
	OPC_BLBC, OPC_BEQ,   OPC_BLT,   OPC_BLE,
	OPC_BLBS, OPC_BNE,   OPC_BGE,   OPC_BGT
};
