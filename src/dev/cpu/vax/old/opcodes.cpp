/*
 * vax_opc.cpp
 *
 *  Created on: Mar 14, 2017
 *      Author: Timothy Stark
 *
 *  VAX instruction table
 *
 */

#include "emu/emucore.h"
#include "dev/cpu/vax/vax.h"
#include "dev/cpu/vax/opcodes.h"

// New VAX opcode table (working in progress)
#define UOPC     OPF_RSVD
#define OPC_REG  OPF_REGULAR
#define OPC_EMU  OPF_EMULATE
#define OPC_VEC  OPF_VECTOR
#define OPC_SFL  OPF_SFLOAT
#define OPC_DFL  OPF_DFLOAT
#define OPC_GFL  OPF_GFLOAT
#define OPC_HFL  OPF_HFLOAT

#define OPC_SDF  OPF_SFLOAT|OPF_DFLOAT
#define OPC_SGF  OPF_SFLOAT|OPF_GFLOAT
#define OPC_SHF  OPF_SFLOAT|OPF_HFLOAT
#define OPC_DSF  OPF_DFLOAT|OPF_SFLOAT
#define OPC_DHF  OPF_DFLOAT|OPF_HFLOAT
#define OPC_GHF  OPF_GFLOAT|OPF_HFLOAT
#define OPC_HSF  OPF_HFLOAT|OPF_SFLOAT
#define OPC_HDF  OPF_HFLOAT|OPF_DFLOAT
#define OPC_HGF  OPF_HFLOAT|OPF_GFLOAT


#define OPR(...) __VA_ARGS__
#define OPC(name, opcode, nopr, opr, flags) \
	{ name, flags, opcode, nopr, opr }

const vaxOpcode vaxOpcodes[] = {
	OPC("UOPC",     OPC_nUOPC,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	// Opcode 0000 - 00FF (00 - FF)
	OPC("HALT",     OPC_nHALT,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("NOP",      OPC_nNOP,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("REI",      OPC_nREI,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BPT",      OPC_nBPT,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("RET",      OPC_nRET,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("RSB",      OPC_nRSB,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("LDPCTX",   OPC_nLDPCTX,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),
	OPC("SVPCTX",   OPC_nSVPCTX,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	OPC("CVTPS",    OPC_nCVTPS,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("CVTSP",    OPC_nCVTSP,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("INDEX",    OPC_nINDEX,    6, OPR({ RL, RL, RL, RL, RL, WL }), OPC_REG),
	OPC("CRC",      OPC_nCRC,      4, OPR({ AB, RL, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("PROBER",   OPC_nPROBER,   3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG),
	OPC("PROBEW",   OPC_nPROBEW,   3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG),
	OPC("INSQUE",   OPC_nINSQUE,   2, OPR({ AB, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQUE",   OPC_nREMQUE,   2, OPR({ AB, WL, 0,  0,  0,  0  }), OPC_REG),

	OPC("BSBB",     OPC_nBSBB,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BRB",      OPC_nBRB,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BNEQ",     OPC_nBNEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BNEQU",    OPC_nBNEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BEQL",     OPC_nBEQL,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BEQLU",    OPC_nBEQL,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGTR",     OPC_nBGTR,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLEQ",     OPC_nBLEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("JSB",      OPC_nJSB,      1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("JMP",      OPC_nJMP,      1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BGEQ",     OPC_nBGEQ,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLSS",     OPC_nBLSS,     1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGTRU",    OPC_nBGTRU,    1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLEQU",    OPC_nBLEQU,    1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BVC",      OPC_nBVC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BVS",      OPC_nBVS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BCC",      OPC_nBCC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BGEQU",    OPC_nBCC,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BCS",      OPC_nBCS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BLSSU",    OPC_nBCS,      1, OPR({ BB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDP4",    OPC_nADDP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("ADDP6",    OPC_nADDP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("SUBP4",    OPC_nSUBP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("SUBP6",    OPC_nSUBP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTPT",    OPC_nCVTPT,    5, OPR({ RW, AB, AB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("MULP",     OPC_nMULP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTTP",    OPC_nCVTTP,    5, OPR({ RW, AB, AB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("DIVP",     OPC_nDIVP6,    6, OPR({ RW, AB, RW, AB, RW, AB }), OPC_REG|OPF_FPD),

	OPC("MOVC3",    OPC_nMOVC3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPC3",    OPC_nCMPC3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("SCANC",    OPC_nSCANC,    4, OPR({ RW, AB, AB, RB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("SPANC",    OPC_nSPANC,    4, OPR({ RW, AB, AB, RB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("MOVC5",    OPC_nMOVC5,    5, OPR({ RW, AB, RB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("CMPC5",    OPC_nCMPC5,    5, OPR({ RW, AB, RB, RW, AB, 0  }), OPC_REG|OPF_FPD),
	OPC("MOVTC",    OPC_nMOVTC,    6, OPR({ RW, AB, RB, AB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("MOVTUC",   OPC_nMOVTUC,   6, OPR({ RW, AB, RB, AB, RW, AB }), OPC_REG|OPF_FPD),

	OPC("BSBW",     OPC_nBSBW,     1, OPR({ BW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BRW",      OPC_nBRW,      1, OPR({ BW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CVTWL",    OPC_nCVTWL,    2, OPR({ RW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTWB",    OPC_nCVTWB,    2, OPR({ RW, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVP",     OPC_nMOVP,     3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPP3",    OPC_nCMPP3,    3, OPR({ RW, AB, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CVTPL",    OPC_nCVTPL,    3, OPR({ RW, AB, WL, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("CMPP4",    OPC_nCMPP4,    4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),

	OPC("EDITPC",   OPC_nEDITPC,   4, OPR({ RW, AB, AB, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("MATCGC",   OPC_nMATCHC,   4, OPR({ RW, AB, RW, AB, 0,  0  }), OPC_REG|OPF_FPD),
	OPC("LOCC",     OPC_nLOCC,     3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("SKPC",     OPC_nSKPC,     3, OPR({ RB, RW, AB, 0,  0,  0  }), OPC_REG|OPF_FPD),
	OPC("MOVZWL",   OPC_nMOVZWL,   2, OPR({ RW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("ACBW",     OPC_nACBW,     4, OPR({ RW, RW, MW, BW, 0,  0  }), OPC_REG),
	OPC("MOVAW",    OPC_nMOVAW,    2, OPR({ AW, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAW",   OPC_nPUSHAW,   1, OPR({ AW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDF2",    OPC_nADDF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("ADDF3",    OPC_nADDF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_SFL),
	OPC("SUBF2",    OPC_nSUBF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("SUBF3",    OPC_nSUBF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_SFL),
	OPC("MULF2",    OPC_nMULF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("MULF3",    OPC_nMULF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_SFL),
	OPC("DIVF2",    OPC_nDIVF2,    2, OPR({ RF, MF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("DIVF3",    OPC_nDIVF3,    3, OPR({ RF, RF, WF, 0,  0,  0  }), OPC_SFL),

	OPC("CVTFB",    OPC_nCVTFB,    2, OPR({ RF, WB, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTFW",    OPC_nCVTFW,    2, OPR({ RF, WW, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTFL",    OPC_nCVTFL,    2, OPR({ RF, WL, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTRFL",   OPC_nCVTRFL,   2, OPR({ RF, WL, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTBF",    OPC_nCVTBF,    2, OPR({ RB, WF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTWF",    OPC_nCVTWF,    2, OPR({ RW, WF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CVTLF",    OPC_nCVTLF,    2, OPR({ RL, WF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("ACBF",     OPC_nACBF,     4, OPR({ RF, RF, MF, BW, 0,  0  }), OPC_SFL),

	OPC("MOVF",     OPC_nMOVF,     2, OPR({ RF, WF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("CMPF",     OPC_nCMPF,     2, OPR({ RF, RF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("MNEGF",    OPC_nMNEGF,    2, OPR({ RF, WF, 0,  0,  0,  0  }), OPC_SFL),
	OPC("TSTF",     OPC_nTSTF,     1, OPR({ RF, 0,  0,  0,  0,  0  }), OPC_SFL),
	OPC("EMODF",    OPC_nEMODF,    5, OPR({ RF, RB, RF, WL, WF, 0  }), OPC_SFL|UOPC),
	OPC("POLYF",    OPC_nPOLYF,    3, OPR({ RF, RW, AB, 0,  0,  0  }), OPC_SFL|UOPC),
	OPC("CVTFD",    OPC_nCVTFD,    2, OPR({ RF, WD, 0,  0,  0,  0  }), OPC_SDF),

	OPC("ADAWI",    OPC_nADAWI,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("INSQHI",   OPC_nINSQHI,   2, OPR({ AB, AQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("INSQTI",   OPC_nINSQTI,   2, OPR({ AB, AQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQHI",   OPC_nREMQHI,   2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("REMQTI",   OPC_nREMQTI,   2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),

	OPC("ADDD2",    OPC_nADDD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("ADDD3",    OPC_nADDD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_DFL),
	OPC("SUBD2",    OPC_nSUBD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("SUBD3",    OPC_nSUBD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_DFL),
	OPC("MULD2",    OPC_nMULD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("MULD3",    OPC_nMULD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_DFL),
	OPC("DIVD2",    OPC_nDIVD2,    2, OPR({ RD, MD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("DIVD3",    OPC_nDIVD3,    3, OPR({ RD, RD, WD, 0,  0,  0  }), OPC_DFL),

	OPC("CVTDB",    OPC_nCVTDB,    2, OPR({ RD, WB, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTDW",    OPC_nCVTDW,    2, OPR({ RD, WW, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTDL",    OPC_nCVTDL,    2, OPR({ RD, WL, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTRDL",   OPC_nCVTRDL,   2, OPR({ RD, WL, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTBD",    OPC_nCVTBD,    2, OPR({ RB, WD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTWD",    OPC_nCVTWD,    2, OPR({ RW, WD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CVTLD",    OPC_nCVTLD,    2, OPR({ RL, WD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("ACBD",     OPC_nACBD,     4, OPR({ RD, RD, MD, BW, 0,  0  }), OPC_DFL),

	OPC("MOVD",     OPC_nMOVD,     2, OPR({ RD, WD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("CMPD",     OPC_nCMPD,     2, OPR({ RD, RD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("MNEGD",    OPC_nMNEGD,    2, OPR({ RD, WD, 0,  0,  0,  0  }), OPC_DFL),
	OPC("TSTD",     OPC_nTSTD,     1, OPR({ RD, 0,  0,  0,  0,  0  }), OPC_DFL),
	OPC("EMODD",    OPC_nEMODD,    5, OPR({ RD, RB, RD, WL, WD, 0  }), OPC_DFL|UOPC),
	OPC("POLYD",    OPC_nPOLYD,    3, OPR({ RD, RW, AB, 0,  0,  0  }), OPC_DFL|UOPC),
	OPC("CVTDF",    OPC_nCVTDF,    2, OPR({ RD, WF, 0,  0,  0,  0  }), OPC_DSF),

	OPC("ASHL",     OPC_nASHL,     3, OPR({ RB, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("ASHQ",     OPC_nASHQ,     3, OPR({ RB, RQ, WQ, 0,  0,  0  }), OPC_REG),
	OPC("EMUL",     OPC_nEMUL,     4, OPR({ RL, RL, RL, WQ, 0,  0  }), OPC_REG),
	OPC("EDIV",     OPC_nEDIV,     4, OPR({ RL, RQ, WL, WL, 0,  0  }), OPC_REG),
	OPC("CLRQ",     OPC_nCLRQ,     1, OPR({ WQ, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRD",     OPC_nCLRQ,     1, OPR({ WD, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRG",     OPC_nCLRQ,     1, OPR({ WG, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("MOVQ",     OPC_nMOVQ,     2, OPR({ RQ, WQ, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAQ",    OPC_nMOVAQ,    2, OPR({ AQ, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAD",    OPC_nMOVAQ,    2, OPR({ AD, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAG",    OPC_nMOVAQ,    2, OPR({ AG, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAQ",   OPC_nPUSHAQ,   1, OPR({ AQ, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAD",   OPC_nPUSHAQ,   1, OPR({ AD, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAG",   OPC_nPUSHAQ,   1, OPR({ AG, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDB2",    OPC_nADDB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDB3",    OPC_nADDB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("SUBB2",    OPC_nSUBB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBB3",    OPC_nSUBB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("MULB2",    OPC_nMULB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULB3",    OPC_nMULB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("DIVB2",    OPC_nDIVB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVB3",    OPC_nDIVB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),

	OPC("BISB2",    OPC_nBISB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISB3",    OPC_nBISB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("BICB2",    OPC_nBICB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICB3",    OPC_nBICB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("XORB2",    OPC_nXORB2,    2, OPR({ RB, MB, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORB3",    OPC_nXORB3,    3, OPR({ RB, RB, WB, 0,  0,  0  }), OPC_REG),
	OPC("MNEGB",    OPC_nMNEGB,    2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEB",    OPC_nCASEB,    3, OPR({ RB, RB, RB, 0,  0,  0  }), OPC_REG|OPF_CASE),

	OPC("MOVB",     OPC_nMOVB,     2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPB",     OPC_nCMPB,     2, OPR({ RB, RB, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOMB",    OPC_nMCOMB,    2, OPR({ RB, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITB",     OPC_nBITB,     2, OPR({ RB, RB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRB",     OPC_nCLRB,     1, OPR({ WB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTB",     OPC_nTSTB,     1, OPR({ RB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCB",     OPC_nINCB,     1, OPR({ MB, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECB",     OPC_nDECB,     1, OPR({ MB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("CVTBL",    OPC_nCVTBL,    2, OPR({ RB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTBW",    OPC_nCVTBW,    2, OPR({ RB, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVZBL",   OPC_nMOVZBL,   2, OPR({ RB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVZBW",   OPC_nMOVZBW,   2, OPR({ RB, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("ROTL",     OPC_nROTL,     3, OPR({ RB, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("ACBB",     OPC_nACBB,     4, OPR({ RB, RB, MB, BW, 0,  0  }), OPC_REG),
	OPC("MOVAB",    OPC_nMOVAB,    2, OPR({ AB, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAB",   OPC_nPUSHAB,   1, OPR({ AB, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDW2",    OPC_nADDW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDW3",    OPC_nADDW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("SUBW2",    OPC_nSUBW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBW3",    OPC_nSUBW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("MULW2",    OPC_nMULW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULW3",    OPC_nMULW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("DIVW2",    OPC_nDIVW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVW3",    OPC_nDIVW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),

	OPC("BISW2",    OPC_nBISW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISW3",    OPC_nBISW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("BICW2",    OPC_nBICW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICW3",    OPC_nBICW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("XORW2",    OPC_nXORW2,    2, OPR({ RW, MW, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORW3",    OPC_nXORW3,    3, OPR({ RW, RW, WW, 0,  0,  0  }), OPC_REG),
	OPC("MNEGW",    OPC_nMNEGW,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEW",    OPC_nCASEW,    3, OPR({ RW, RW, RW, 0,  0,  0  }), OPC_REG|OPF_CASE),

	OPC("MOVW",     OPC_nMOVW,     2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPW",     OPC_nCMPW,     2, OPR({ RW, RW, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOMW",    OPC_nMCOMW,    2, OPR({ RW, WW, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITW",     OPC_nBITW,     2, OPR({ RW, RW, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRW",     OPC_nCLRW,     1, OPR({ WW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTW",     OPC_nTSTW,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCW",     OPC_nINCW,     1, OPR({ MW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECW",     OPC_nDECW,     1, OPR({ MW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BISPSW",   OPC_nBISPSW,   1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("BICPSW",   OPC_nBICPSW,   1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("POPR",     OPC_nPOPR,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHR",    OPC_nPUSHR,    1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMK",     OPC_nCHMK,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHME",     OPC_nCHME,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMS",     OPC_nCHMS,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CHMU",     OPC_nCHMU,     1, OPR({ RW, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADDL2",    OPC_nADDL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("ADDL3",    OPC_nADDL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("SUBL2",    OPC_nSUBL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("SUBL3",    OPC_nSUBL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("MULL2",    OPC_nMULL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("MULL3",    OPC_nMULL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("DIVL2",    OPC_nDIVL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("DIVL3",    OPC_nDIVL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),

	OPC("BISL2",    OPC_nBISL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("BISL3",    OPC_nBISL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("BICL2",    OPC_nBICL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("BICL3",    OPC_nBICL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("XORL2",    OPC_nXORL2,    2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("XORL3",    OPC_nXORL3,    3, OPR({ RL, RL, WL, 0,  0,  0  }), OPC_REG),
	OPC("MNEGL",    OPC_nMNEGL,    2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CASEL",    OPC_nCASEL,    3, OPR({ RL, RL, RL, 0,  0,  0  }), OPC_REG|OPF_CASE),

	OPC("MOVL",     OPC_nMOVL,     2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CMPL",     OPC_nCMPL,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MCOML",    OPC_nMCOML,    2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("BITL",     OPC_nBITL,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("CLRL",     OPC_nCLRL,     1, OPR({ WL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("CLRF",     OPC_nCLRL,     1, OPR({ WF, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("TSTL",     OPC_nTSTL,     1, OPR({ RL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("INCL",     OPC_nINCL,     1, OPR({ ML, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("DECL",     OPC_nDECL,     1, OPR({ ML, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("ADWC",     OPC_nADWC,     2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("SBWC",     OPC_nSBWC,     2, OPR({ RL, ML, 0,  0,  0,  0  }), OPC_REG),
	OPC("MTPR",     OPC_nMTPR,     2, OPR({ RL, RL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MFPR",     OPC_nMFPR,     2, OPR({ RL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVPSL",   OPC_nMOVPSL,   1, OPR({ WL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHL",    OPC_nPUSHL,    1, OPR({ RL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAL",    OPC_nMOVAL,    2, OPR({ AL, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("MOVAF",    OPC_nMOVAL,    2, OPR({ AF, WL, 0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAL",   OPC_nPUSHAL,   1, OPR({ AL, 0,  0,  0,  0,  0  }), OPC_REG),
	OPC("PUSHAF",   OPC_nPUSHAL,   1, OPR({ AF, 0,  0,  0,  0,  0  }), OPC_REG),

	OPC("BBS",      OPC_nBBS,      3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBC",      OPC_nBBC,      3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSS",     OPC_nBBSS,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCS",     OPC_nBBCS,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSC",     OPC_nBBSC,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCC",     OPC_nBBCC,     3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBSSI",    OPC_nBBSSI,    3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),
	OPC("BBCCI",    OPC_nBBCCI,    3, OPR({ RL, VB, BB, 0,  0,  0  }), OPC_REG),

	OPC("BLBS",     OPC_nBLBS,     2, OPR({ RL, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("BLBC",     OPC_nBLBC,     2, OPR({ RL, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("FFS",      OPC_nFFS,      4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("FFC",      OPC_nFFC,      4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("CMPV",     OPC_nCMPV,     4, OPR({ RL, RB, VB, RL, 0,  0  }), OPC_REG),
	OPC("CMPZV",    OPC_nCMPZV,    4, OPR({ RL, RB, VB, RL, 0,  0  }), OPC_REG),
	OPC("EXTV",     OPC_nEXTV,     4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),
	OPC("EXTZV",    OPC_nEXTZV,    4, OPR({ RL, RB, VB, WL, 0,  0  }), OPC_REG),

	OPC("INSV",     OPC_nINSV,     4, OPR({ RL, RL, RB, VB, 0 , 0  }), OPC_REG),
	OPC("ACBL",     OPC_nACBL,     4, OPR({ RL, RL, ML, BW, 0 , 0  }), OPC_REG),
	OPC("AOBLSS",   OPC_nAOBLSS,   3, OPR({ RL, ML, BB, 0,  0,  0  }), OPC_REG),
	OPC("AOBLEQ",   OPC_nAOBLEQ,   3, OPR({ RL, ML, BB, 0,  0,  0  }), OPC_REG),
	OPC("SOBGEQ",   OPC_nSOBGEQ,   2, OPR({ ML, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("SOBGTR",   OPC_nSOBGTR,   2, OPR({ ML, BB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTLB",    OPC_nCVTLB,    2, OPR({ RL, WB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CVTLW",    OPC_nCVTLW,    2, OPR({ RL, WW, 0,  0,  0,  0  }), OPC_REG),

	OPC("ASHP",     OPC_nASHP,     6, OPR({ RB, RW, AB, RB, RW, AB }), OPC_REG|OPF_FPD),
	OPC("CVTLP",    OPC_nCVTLP,    3, OPR({ RL, RW, AB, 0 , 0 , 0  }), OPC_REG|OPF_FPD),
	OPC("CALLG",    OPC_nCALLG,    2, OPR({ AB, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("CALLS",    OPC_nCALLS,    2, OPR({ RL, AB, 0,  0,  0,  0  }), OPC_REG),
	OPC("XFC",      OPC_nXFC,      0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG),

	// Opcode 0100 - 01FF (00FD - FFFD)
	OPC("WAIT",     OPC_nWAIT,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("MFVP",     OPC_nMFVP,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("CVTDH",    OPC_nCVTDH,    2, OPR({ RD, WH, 0,  0,  0,  0  }), OPC_DHF|UOPC),
	OPC("CVTGF",    OPC_nCVTGF,    2, OPR({ RG, WF, 0,  0,  0,  0  }), OPC_SGF),
	OPC("VLDL",     OPC_nVLDL,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VGATHL",   OPC_nVGATHL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VLDQ",     OPC_nVLDQ,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VGATHQ",   OPC_nVGATHQ,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("ADDG2",    OPC_nADDG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("ADDG3",    OPC_nADDG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_GFL),
	OPC("SUBG2",    OPC_nSUBG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("SUBG3",    OPC_nSUBG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_GFL),
	OPC("MULG2",    OPC_nMULG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("MULG3",    OPC_nMULG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_GFL),
	OPC("DIVG2",    OPC_nDIVG2,    2, OPR({ RG, MG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("DIVG3",    OPC_nDIVG3,    3, OPR({ RG, RG, WG, 0,  0,  0  }), OPC_GFL),

	OPC("CVTGB",    OPC_nCVTGB,    2, OPR({ RG, WB, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTGW",    OPC_nCVTGW,    2, OPR({ RG, WW, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTGL",    OPC_nCVTGL,    2, OPR({ RG, WL, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTRGL",   OPC_nCVTRGL,   2, OPR({ RG, WL, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTBG",    OPC_nCVTBG,    2, OPR({ RB, WG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTWG",    OPC_nCVTWG,    2, OPR({ RW, WG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CVTLG",    OPC_nCVTLG,    2, OPR({ RL, WG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("ACBG",     OPC_nACBG,     4, OPR({ RG, RG, MG, BW, 0,  0  }), OPC_GFL),

	OPC("MOVG",     OPC_nMOVG,     2, OPR({ RG, WG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("CMPG",     OPC_nCMPG,     2, OPR({ RG, RG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("MNEGG",    OPC_nMNEGG,    2, OPR({ RG, WG, 0,  0,  0,  0  }), OPC_GFL),
	OPC("TSTG",     OPC_nTSTG,     1, OPR({ RG, 0,  0,  0,  0,  0  }), OPC_GFL),
	OPC("EMODG",    OPC_nEMODG,    5, OPR({ RG, RB, RG, WL, WG, 0  }), OPC_GFL|UOPC),
	OPC("POLYG",    OPC_nPOLYG,    3, OPR({ RG, RW, AB, 0,  0,  0  }), OPC_GFL|UOPC),
	OPC("CVTGH",    OPC_nCVTGH,    2, OPR({ RG, WH, 0,  0,  0,  0  }), OPC_GHF|UOPC),

	OPC("ADDH2",    OPC_nADDH2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("ADDH3",    OPC_nADDH3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_HFL|UOPC),
	OPC("SUBH2",    OPC_nSUBH2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("SUBH3",    OPC_nSUBH3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_HFL|UOPC),
	OPC("MULH2",    OPC_nMULH2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("MULH3",    OPC_nMULH3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_HFL|UOPC),
	OPC("DIVH2",    OPC_nDIVH2,    2, OPR({ RH, MH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("DIVH3",    OPC_nDIVH3,    3, OPR({ RH, RH, WH, 0,  0,  0  }), OPC_HFL|UOPC),

	OPC("CVTHB",    OPC_nCVTHB,    2, OPR({ RH, WB, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTHW",    OPC_nCVTHW,    2, OPR({ RH, WW, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTHL",    OPC_nCVTHL,    2, OPR({ RH, WL, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTRHL",   OPC_nCVTRHL,   2, OPR({ RH, WL, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTBH",    OPC_nCVTBH,    2, OPR({ RB, WH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTWH",    OPC_nCVTWH,    2, OPR({ RW, WH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTLH",    OPC_nCVTLH,    2, OPR({ RL, WH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("ACBH",     OPC_nACBH,     4, OPR({ RH, RH, MH, BW, 0,  0  }), OPC_HFL|UOPC),

	OPC("MOVH",     OPC_nMOVH,     2, OPR({ RH, WH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CMPH",     OPC_nCMPH,     2, OPR({ RH, RG, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("MNEGH",    OPC_nMNEGH,    2, OPR({ RH, WH, 0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("TSTH",     OPC_nTSTH,     1, OPR({ RH, 0,  0,  0,  0,  0  }), OPC_HFL|UOPC),
	OPC("EMODH",    OPC_nEMODH,    5, OPR({ RH, RB, RH, WL, WH, 0  }), OPC_HFL|UOPC),
	OPC("POLYH",    OPC_nPOLYH,    3, OPR({ RH, RW, AB, 0,  0,  0  }), OPC_HFL|UOPC),
	OPC("CVTHG",    OPC_nCVTHG,    2, OPR({ RH, WG, 0,  0,  0,  0  }), OPC_HGF|UOPC),

	OPC("CLRO",     OPC_nCLRO,     1, OPR({ WO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("CLRH",     OPC_nCLRO,     1, OPR({ WO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVO",     OPC_nMOVO,     2, OPR({ RO, WO, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVAO",    OPC_nMOVAO,    2, OPR({ AO, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("MOVAH",    OPC_nMOVAO,    2, OPR({ AH, WL, 0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PUSHAO",   OPC_nPUSHAO,   1, OPR({ AO, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PUSHAH",   OPC_nPUSHAO,   1, OPR({ AH, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	OPC("VVADDL",   OPC_nVVADDL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDL",   OPC_nVSADDL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDG",   OPC_nVVADDG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDG",   OPC_nVSADDG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDF",   OPC_nVVADDF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDF",   OPC_nVSADDF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVADDD",   OPC_nVVADDD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSADDD",   OPC_nVSADDD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVSUBL",   OPC_nVVSUBL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBL",   OPC_nVVSUBL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBG",   OPC_nVVSUBG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBG",   OPC_nVSSUBG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBF",   OPC_nVVSUBF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBF",   OPC_nVSSUBF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSUBD",   OPC_nVVSUBD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSUBD",   OPC_nVSSUBD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("CVTFH",    OPC_nCVTFH,    2, OPR({ RF, WH, 0,  0,  0,  0  }), OPC_SHF|UOPC),
	OPC("CVTFG",    OPC_nCVTFG,    2, OPR({ RF, WG, 0,  0,  0,  0  }), OPC_SGF),
	OPC("PROBEVMR", OPC_nPROBEVMR, 0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("PROBEVMW", OPC_nPROBEVMW, 0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("VSTL",     OPC_nVSTL,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCATL",   OPC_nVSCATL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSTQ",     OPC_nVSTQ,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCATQ",   OPC_nVSCATQ,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVMULL",   OPC_nVVMULL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULL",   OPC_nVSMULL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULG",   OPC_nVVMULG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULG",   OPC_nVSMULG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULF",   OPC_nVVMULF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULF",   OPC_nVSMULF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVMULD",   OPC_nVVMULD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSMULD",   OPC_nVSMULD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VSYNC",    OPC_nVSYNC,    0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("MTVP",     OPC_nMTVP,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVG",   OPC_nVVDIVG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVG",   OPC_nVSDIVG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVF",   OPC_nVVDIVF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVF",   OPC_nVSDIVF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVDIVD",   OPC_nVVDIVD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSDIVD",   OPC_nVSDIVD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVCMPL",   OPC_nVVCMPL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPL",   OPC_nVSCMPL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPG",   OPC_nVVCMPG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPG",   OPC_nVSCMPG,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPF",   OPC_nVVCMPF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPF",   OPC_nVSCMPF,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCMPD",   OPC_nVVCMPD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSCMPD",   OPC_nVSCMPD,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVBISL",   OPC_nVVBISL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSBISL",   OPC_nVSBISL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVBICL",   OPC_nVVBICL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSBICL",   OPC_nVSBICL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVSRLL",   OPC_nVVSRLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSRLL",   OPC_nVSSRLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVSLLL",   OPC_nVVSLLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSSLLL",   OPC_nVSSLLL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("VVXORL",   OPC_nVVXORL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VSXORL",   OPC_nVSXORL,   0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("VVCVT",    OPC_nVVCVT,    0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),
	OPC("IOTA",     OPC_nIOTA,     0, OPR({ 0,  0,  0,  0,  0,  0  }), OPC_VEC|UOPC),

	OPC("CVTHF",    OPC_nCVTHF,    2, OPR({ RH, WF, 0,  0,  0,  0  }), OPC_HSF|UOPC),
	OPC("CVTHD",    OPC_nCVTHD,    2, OPR({ RH, WD, 0,  0,  0,  0  }), OPC_HDF|UOPC),

	// Opcode 9200 - 02FF (00FE - FFFE)

	// Opcode 0300 - 03FF (00FF - FFFF)
	OPC("BUGL",     OPC_nBUGL,     1, OPR({ IL, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),
	OPC("BUGW",     OPC_nBUGW,     1, OPR({ IL, 0,  0,  0,  0,  0  }), OPC_REG|UOPC),

	// null terminator
	{ nullptr }
};

void vax_cpu_base::buildOpcodes()
{
	const vaxOpcode *opc;
	dopc_t    *dopc;
	uint16_t   opCode;

	// Initialize all opcode table as default unimplemented opcode
	for (int idx = 0; idx < CPU_nOPCTBL; idx++)
		opCodes[idx] = &vaxOpcodes[0];

	// Initialize opcode table
	for (int idx = 1; vaxOpcodes[idx].opName; idx++) {
		opCode = vaxOpcodes[idx].opCode;
		if (opCodes[opCode] == &vaxOpcodes[0])
			opCodes[opCode] = &vaxOpcodes[idx];
	}

	// Initialize decode opcode table with default illegal instruction
	// Set flag to issue RSVD_INSTRUCTION_FAULT as default.
	for (int idx = 0; idx < CPU_nOPCTBL; idx++) {
		dopCode[idx].opCode = nullptr;
		dopCode[idx].flag   = OPF_RSVD;
		dopCode[idx].nModes = 0;
		for (int oidx = 0; oidx < 6; oidx++)
			dopCode[idx].opMode[oidx] = 0;
	}

	// Initialize decode opcode table for each valid instruction
	for (int idx = 1; vaxOpcodes[idx].opName; idx++) {
		opc    = &vaxOpcodes[idx];
		opCode = opc->opCode;
		dopc   = &dopCode[opCode];

		dopc->opCode = opc;
		dopc->flag   = opc->flags;
		dopc->nModes = opc->nOperands;
		for (int oidx = 0; oidx < dopc->nModes; oidx++)
			dopc->opMode[oidx] = opc->oprMode[oidx];
	}
}
