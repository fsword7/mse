/*
 * pdp10_opc.cpp
 *
 *  Created on: Feb 14, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/pdp10/kx10.h"
#include "dev/cpu/pdp10/opcode.h"

#define PDP6
#define KA
#define KI
#define KL
#define KS
#define XKL

#define FMEM	OPF_FMEM
#define FSMEM	OPF_FSMEM
#define SMEM	OPF_SMEM
#define FAC		OPF_FAC
#define FAC2	OPF_FAC2
#define FBR		OPF_FBR
#define SAC		OPF_SAC
#define SAC2	OPF_SAC2
#define SACZ	OPF_SACZ
#define SWAR	OPF_SWAR

// Operand flags for each instruction
uint32_t pdp10_cpu_base::oprFlags[01000] =
{
		// UUO Instructions 000-077
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// UUO Instructions 100-177
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// Full-word Instructions 200-277
		SAC|FMEM,			SAC,			FAC|SMEM,			SACZ|FSMEM,			// MOVE, MOVEI, MOVEM, MOVES
		SAC|FMEM|SWAR,		SAC|SWAR,		FAC|SMEM|SWAR,		SACZ|FSMEM|SWAR,	// MOVS, MOVSI, MOVSM, MOVSS
		SAC|FMEM,			SAC,			FAC|SMEM,			SACZ|FSMEM,			// MOVN, MOVNI, MOVNM, MOVNS
		SAC|FMEM,			SAC,			FAC|SMEM,			SACZ|FSMEM,			// MOVM, MOVMI, MOVMM, MOVMS


		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		FBR|SAC|FMEM,		FBR|SAC,		FBR|FSMEM,			FBR|SAC|FSMEM,		// ADD, ADDI, ADDM, ADDB
		FBR|SAC|FMEM,		FBR|SAC,		FBR|FSMEM,			FBR|SAC|FSMEM,		// SUB, SUBI, SUBM, SUBB


		// UUO Instructions 300-377
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// UUO Instructions 400-477
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// UUO Instructions 500-577
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// UUO Instructions 600-677
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,

		// IOT instructions 700-777
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0,
		0, 0, 0, 0,  0, 0, 0, 0
};

#define OPC(name, opcode, flags) { name, nullptr, 0, opcode }

const pdp10_Opcode pdp10_Opcodes[] =
{
	// Opcode 000 - 077
	OPC("UUO",		OPC_nUUO, 0),

	// Opcode 100 - 177

	// Opcode 200 - 277

	// Opcode 300 - 377

	// Opcode 400 - 477

	// Opcode 500 - 577

	// Opcode 600 - 677

	OPC("AOBJN",	OPC_nAOBJN, 0),
	OPC("AOBJP",	OPC_nAOBJP, 0),

	OPC("BLT",		OPC_nBLT, 0),
};

const pdp10_Opcode pdp10_extOpcodes[] =
{
	OPC("CMPSL",	OPC_nCMPSL, 0),
	OPC("CMPSE",	OPC_nCMPSE, 0),
	OPC("CMPSLE",	OPC_nCMPSLE, 0),
	OPC("CMPSGE",	OPC_nCMPSGE, 0),
	OPC("CMPSN",	OPC_nCMPSN, 0),
	OPC("CMPSG",	OPC_nCMPSG, 0),

	OPC("EDIT",		OPC_nEDIT, 0),

	OPC("CVTBDO",	OPC_nCVTBDO, 0),
	OPC("CVTBDT",	OPC_nCVTBDT, 0),
	OPC("CVTDBO",	OPC_nCVTDBO, 0),
	OPC("CVTDBT",	OPC_nCVTDBT, 0),

	OPC("MOVSO",	OPC_nMOVSO, 0),
	OPC("MOVST",	OPC_nMOVST, 0),
	OPC("MOVSLJ",	OPC_nMOVSLJ, 0),
	OPC("MOVSRJ",	OPC_nMOVSRJ, 0),

	OPC("XBLT",		OPC_nXBLT, 0),

	OPC("GSNGL",	OPC_nGSNGL, 0),
	OPC("GDBLE",	OPC_nGDBLE, 0),
	OPC("GFSC",		OPC_nGFSC, 0),
	OPC("GFIX",		OPC_nGFIX, 0),
	OPC("GFIXR",	OPC_nGFIXR, 0),
	OPC("GFLTR",	OPC_nGFLTR, 0),
	OPC("DGFIX",	OPC_nDGFIX, 0),
	OPC("DGFIXR",	OPC_nDGFIXR, 0),
	OPC("DGFLTR",	OPC_nDGFLTR, 0),

	// null terminator
	{ nullptr }
};


#if 0


	INST("AOBJN", OPC_nAOBJN, 0, 0, 0),
	INST("AOBJP", OPC_nAOBJP, 0, 0, 0),


	INST("BLT",   OPC_nBLT,  0, 0, 0),








	INST("EXCH",   OPC_nEXCH,  0, 0, 0),
	INST("EXTEND", OPC_nEXTEND, 0, 0, 0),


	INST("MOVE",  OPC_nMOVE,  0, 0, 0),
	INST("MOVEI", OPC_nMOVEI, 0, 0, 0),
	INST("MOVEM", OPC_nMOVEM, 0, 0, 0),
	INST("MOVES", OPC_nMOVES, 0, 0, 0),

	INST("MOVM",  OPC_nMOVM,  0, 0, 0),
	INST("MOVMI", OPC_nMOVMI, 0, 0, 0),
	INST("MOVMM", OPC_nMOVMM, 0, 0, 0),
	INST("MOVMS", OPC_nMOVMS, 0, 0, 0),

	INST("MOVN",  OPC_nMOVN,  0, 0, 0),
	INST("MOVNI", OPC_nMOVNI, 0, 0, 0),
	INST("MOVNM", OPC_nMOVNM, 0, 0, 0),
	INST("MOVNS", OPC_nMOVNS, 0, 0, 0),

	INST("MOVS",  OPC_nMOVS,  0, 0, 0),
	INST("MOVSI", OPC_nMOVSI, 0, 0, 0),
	INST("MOVSM", OPC_nMOVSM, 0, 0, 0),
	INST("MOVSS", OPC_nMOVSS, 0, 0, 0),

	INST("DMOVE",   OPC_nDMOVE, 0, 0, 0),
	INST("DMOVEM",  OPC_nDMOVEM, 0, 0, 0),
	INST("DMOVN",   OPC_nDMOVN, 0, 0, 0),
	INST("DMOVNM",  OPC_nDMOVNM, 0, 0, 0),


	INST("JFCL",   OPC_nJFCL,  0, 0, 0),
	INST("JFFO",   OPC_nJFFO,  0, 0, 0),
	INST("JRA",    OPC_nJRA,   0, 0, 0),
	INST("JRST",   OPC_nJRST,  0, 0, 0),
	INST("JSA",    OPC_nJSA,   0, 0, 0),
	INST("JSP",    OPC_nJSP,   0, 0, 0),
	INST("JSR",    OPC_nJSR,   0, 0, 0),


	INST("XCT",   OPC_nXCT,  0, 0, 0),

	// Arithmetic Instructions
	INST("ADD",   OPC_nADD,  0, 0, 0),
	INST("ADDB",  OPC_nADDB, 0, 0, 0),
	INST("ADDI",  OPC_nADDI, 0, 0, 0),
	INST("ADDM",  OPC_nADDM, 0, 0, 0),

	INST("SUB",   OPC_nSUB,  0, 0, 0),
	INST("SUBB",  OPC_nSUBB, 0, 0, 0),
	INST("SUBI",  OPC_nSUBI, 0, 0, 0),
	INST("SUBM",  OPC_nSUBM, 0, 0, 0),

	INST("MUL",   OPC_nMUL,  0, 0, 0),
	INST("MULB",  OPC_nMULB, 0, 0, 0),
	INST("MULI",  OPC_nMULI, 0, 0, 0),
	INST("MULM",  OPC_nMULM, 0, 0, 0),

	INST("IMUL",   OPC_nIMUL,  0, 0, 0),
	INST("IMULB",  OPC_nIMULB, 0, 0, 0),
	INST("IMULI",  OPC_nIMULI, 0, 0, 0),
	INST("IMULM",  OPC_nIMULM, 0, 0, 0),

	INST("DIV",   OPC_nDIV,  0, 0, 0),
	INST("DIVB",  OPC_nDIVB, 0, 0, 0),
	INST("DIVI",  OPC_nDIVI, 0, 0, 0),
	INST("DIVM",  OPC_nDIVM, 0, 0, 0),

	INST("IDIV",   OPC_nIDIV,  0, 0, 0),
	INST("IDIVB",  OPC_nIDIVB, 0, 0, 0),
	INST("IDIVI",  OPC_nIDIVI, 0, 0, 0),
	INST("IDIVM",  OPC_nIDIVM, 0, 0, 0),

	// Double Arithmetic Instructions
	INST("DADD",   OPC_nDADD,  0, 0, 0),
	INST("DSUB",   OPC_nDSUB,  0, 0, 0),
	INST("DMUL",   OPC_nDMUL,  0, 0, 0),
	INST("DDIV",   OPC_nDDIV,  0, 0, 0),

	// Shift Instructions
	INST("ASH",   OPC_nASH,  0, 0, 0),
	INST("ASHC",  OPC_nASHC, 0, 0, 0),
	INST("LSH",   OPC_nLSH,  0, 0, 0),
	INST("LSHC",  OPC_nLSHC, 0, 0, 0),
	INST("ROT",   OPC_nROT,  0, 0, 0),
	INST("ROTC",  OPC_nROTC, 0, 0, 0),

	// Byte Instructions
	INST("ADJBP",  OPC_nADJBP, 0, 0, 0),
	INST("IBP",    OPC_nIBP,   0, 0, 0),
	INST("LDB",    OPC_nLDB,   0, 0, 0),
	INST("DPB",    OPC_nDPB,   0, 0, 0),
	INST("ILDB",   OPC_nILDB,  0, 0, 0),
	INST("IDPB",   OPC_nIDPB,  0, 0, 0),

	// Stack Instructions
	INST("ADJSP", OPC_nADJSP, 0, 0, 0),
	INST("POP",   OPC_nPOP,   0, 0, 0),
	INST("POPJ",  OPC_nPOPJ,  0, 0, 0),
	INST("PUSH",  OPC_nPUSH,  0, 0, 0),
	INST("PUSHJ", OPC_nPUSHJ, 0, 0, 0),

	// Floating Instructions
	INST("FAD",     OPC_nFAD,    0, 0, 0),
	INST("FADL",    OPC_nFADL,   0, 0, 0),
	INST("FADM",    OPC_nFADM,   0, 0, 0),
	INST("FADB",    OPC_nFADB,   0, 0, 0),
	INST("FADR",    OPC_nFADR,   0, 0, 0),
	INST("FADRL",   OPC_nFADRL,  0, 0, 0),
	INST("FADRM",   OPC_nFADRM,  0, 0, 0),
	INST("FADRB",   OPC_nFADRB,  0, 0, 0),

	INST("FSB",     OPC_nFSB,    0, 0, 0),
	INST("FSBL",    OPC_nFSBL,   0, 0, 0),
	INST("FSBM",    OPC_nFSBM,   0, 0, 0),
	INST("FSBB",    OPC_nFSBB,   0, 0, 0),
	INST("FSBR",    OPC_nFSBR,   0, 0, 0),
	INST("FSBRL",   OPC_nFSBRL,  0, 0, 0),
	INST("FSBRM",   OPC_nFSBRM,  0, 0, 0),
	INST("FSBRB",   OPC_nFSBRB,  0, 0, 0),

	INST("FMP",     OPC_nFMP,    0, 0, 0),
	INST("FMPL",    OPC_nFMPL,   0, 0, 0),
	INST("FMPM",    OPC_nFMPM,   0, 0, 0),
	INST("FMPB",    OPC_nFMPB,   0, 0, 0),
	INST("FMPR",    OPC_nFMPR,   0, 0, 0),
	INST("FMPRL",   OPC_nFMPRL,  0, 0, 0),
	INST("FMPRM",   OPC_nFMPRM,  0, 0, 0),
	INST("FMPRB",   OPC_nFMPRB,  0, 0, 0),

	INST("FDV",     OPC_nFDV,    0, 0, 0),
	INST("FDVL",    OPC_nFDVL,   0, 0, 0),
	INST("FDVM",    OPC_nFDVM,   0, 0, 0),
	INST("FDVB",    OPC_nFDVB,   0, 0, 0),
	INST("FDVR",    OPC_nFDVR,   0, 0, 0),
	INST("FDVRL",   OPC_nFDVRL,  0, 0, 0),
	INST("FDVRM",   OPC_nFDVRM,  0, 0, 0),
	INST("FDVRB",   OPC_nFDVRB,  0, 0, 0),

	INST("FIX",    OPC_nFIX,   0, 0, 0),
	INST("FIXR",   OPC_nFIXR,  0, 0, 0),
	INST("FLTR",   OPC_nFLTR,  0, 0, 0),
	INST("FSC",    OPC_nFSC,   0, 0, 0),

	// Double Floating Instructions
	INST("DFAD",   OPC_nDFAD,  0, 0, 0),
	INST("DFDV",   OPC_nDFDV,  0, 0, 0),
	INST("DFMP",   OPC_nDFMP,  0, 0, 0),
	INST("DFSB",   OPC_nDFSB,  0, 0, 0),

	// Giant Floating Instructions
	INST("GFAD",   OPC_nGFAD,  0, 0, 0),
	INST("GFDV",   OPC_nGFDV,  0, 0, 0),
	INST("GFMP",   OPC_nGFMP,  0, 0, 0),
	INST("GFSB",   OPC_nGFSB,  0, 0, 0),

	INST("DFN",    OPC_nDFN,   0, 0, 0),
	INST("UFA",    OPC_nUFA,   0, 0, 0),

	INST("MAP",   OPC_nMAP,   0, 0, 0),

	INST("CAI",    OPC_nCAI,   0, 0, 0),
	INST("CAIL",   OPC_nCAIL,  0, 0, 0),
	INST("CAIE",   OPC_nCAIE,  0, 0, 0),
	INST("CAILE",  OPC_nCAILE, 0, 0, 0),
	INST("CAIA",   OPC_nCAIA,  0, 0, 0),
	INST("CAIGE",  OPC_nCAIGE, 0, 0, 0),
	INST("CAIN",   OPC_nCAIN,  0, 0, 0),
	INST("CAIG",   OPC_nCAIG,  0, 0, 0),

	INST("CAM",    OPC_nCAM,   0, 0, 0),
	INST("CAML",   OPC_nCAML,  0, 0, 0),
	INST("CAME",   OPC_nCAME,  0, 0, 0),
	INST("CAMLE",  OPC_nCAMLE, 0, 0, 0),
	INST("CAMA",   OPC_nCAMA,  0, 0, 0),
	INST("CAMGE",  OPC_nCAMGE, 0, 0, 0),
	INST("CAMN",   OPC_nCAMN,  0, 0, 0),
	INST("CAMG",   OPC_nCAMG,  0, 0, 0),

	INST("JUMP",    OPC_nJUMP,   0, 0, 0),
	INST("JUMPL",   OPC_nJUMPL,  0, 0, 0),
	INST("JUMPE",   OPC_nJUMPE,  0, 0, 0),
	INST("JUMPLE",  OPC_nJUMPLE, 0, 0, 0),
	INST("JUMPA",   OPC_nJUMPA,  0, 0, 0),
	INST("JUMPGE",  OPC_nJUMPGE, 0, 0, 0),
	INST("JUMPN",   OPC_nJUMPN,  0, 0, 0),
	INST("JUMPG",   OPC_nJUMPG,  0, 0, 0),

	INST("SKIP",    OPC_nSKIP,   0, 0, 0),
	INST("SKIPL",   OPC_nSKIPL,  0, 0, 0),
	INST("SKIPE",   OPC_nSKIPE,  0, 0, 0),
	INST("SKIPLE",  OPC_nSKIPLE, 0, 0, 0),
	INST("SKIPA",   OPC_nSKIPA,  0, 0, 0),
	INST("SKIPGE",  OPC_nSKIPGE, 0, 0, 0),
	INST("SKIPN",   OPC_nSKIPN,  0, 0, 0),
	INST("SKIPG",   OPC_nSKIPG,  0, 0, 0),

	INST("AOJ",    OPC_nAOJ,   0, 0, 0),
	INST("AOJL",   OPC_nAOJL,  0, 0, 0),
	INST("AOJE",   OPC_nAOJE,  0, 0, 0),
	INST("AOJLE",  OPC_nAOJLE, 0, 0, 0),
	INST("AOJA",   OPC_nAOJA,  0, 0, 0),
	INST("AOJGE",  OPC_nAOJGE, 0, 0, 0),
	INST("AOJN",   OPC_nAOJN,  0, 0, 0),
	INST("AOJG",   OPC_nAOJG,  0, 0, 0),

	INST("AOS",    OPC_nAOS,   0, 0, 0),
	INST("AOSL",   OPC_nAOSL,  0, 0, 0),
	INST("AOSE",   OPC_nAOSE,  0, 0, 0),
	INST("AOSLE",  OPC_nAOSLE, 0, 0, 0),
	INST("AOSA",   OPC_nAOSA,  0, 0, 0),
	INST("AOSGE",  OPC_nAOSGE, 0, 0, 0),
	INST("AOSN",   OPC_nAOSN,  0, 0, 0),
	INST("AOSG",   OPC_nAOSG,  0, 0, 0),

	INST("SOJ",    OPC_nSOJ,   0, 0, 0),
	INST("SOJL",   OPC_nSOJL,  0, 0, 0),
	INST("SOJE",   OPC_nSOJE,  0, 0, 0),
	INST("SOJLE",  OPC_nSOJLE, 0, 0, 0),
	INST("SOJA",   OPC_nSOJA,  0, 0, 0),
	INST("SOJGE",  OPC_nSOJGE, 0, 0, 0),
	INST("SOJN",   OPC_nSOJN,  0, 0, 0),
	INST("SOJG",   OPC_nSOJG,  0, 0, 0),

	INST("SOS",    OPC_nSOS,   0, 0, 0),
	INST("SOSL",   OPC_nSOSL,  0, 0, 0),
	INST("SOSE",   OPC_nSOSE,  0, 0, 0),
	INST("SOSLE",  OPC_nSOSLE, 0, 0, 0),
	INST("SOSA",   OPC_nSOSA,  0, 0, 0),
	INST("SOSGE",  OPC_nSOSGE, 0, 0, 0),
	INST("SOSN",   OPC_nSOSN,  0, 0, 0),
	INST("SOSG",   OPC_nSOSG,  0, 0, 0),


	INST("SETZ",   OPC_nSETZ,  0, 0, 0),
	INST("SETZI",  OPC_nSETZI, 0, 0, 0),
	INST("SETZM",  OPC_nSETZM, 0, 0, 0),
	INST("SETZB",  OPC_nSETZB, 0, 0, 0),

	INST("SETO",   OPC_nSETO,  0, 0, 0),
	INST("SETOI",  OPC_nSETOI, 0, 0, 0),
	INST("SETOM",  OPC_nSETOM, 0, 0, 0),
	INST("SETOB",  OPC_nSETOB, 0, 0, 0),

	INST("SETM",   OPC_nSETM,  0, 0, 0),
	INST("SETMI",  OPC_nSETMI, 0, 0, 0),
	INST("SETMM",  OPC_nSETMM, 0, 0, 0),
	INST("SETMB",  OPC_nSETMB, 0, 0, 0),

	INST("SETA",   OPC_nSETA,  0, 0, 0),
	INST("SETAI",  OPC_nSETAI, 0, 0, 0),
	INST("SETAM",  OPC_nSETAM, 0, 0, 0),
	INST("SETAB",  OPC_nSETAB, 0, 0, 0),

	INST("SETCA",   OPC_nSETCA,  0, 0, 0),
	INST("SETCAI",  OPC_nSETCAI, 0, 0, 0),
	INST("SETCAM",  OPC_nSETCAM, 0, 0, 0),
	INST("SETCAB",  OPC_nSETCAB, 0, 0, 0),

	INST("SETCM",   OPC_nSETCM,  0, 0, 0),
	INST("SETCMI",  OPC_nSETCMI, 0, 0, 0),
	INST("SETCMM",  OPC_nSETCMM, 0, 0, 0),
	INST("SETCMB",  OPC_nSETCMB, 0, 0, 0),

	INST("AND",     OPC_nAND,    0, 0, 0),
	INST("ANDI",    OPC_nANDI,   0, 0, 0),
	INST("ANDM",    OPC_nANDM,   0, 0, 0),
	INST("ANDB",    OPC_nANDB,   0, 0, 0),

	INST("ANDCA",   OPC_nANDCA,  0, 0, 0),
	INST("ANDCAI",  OPC_nANDCAI, 0, 0, 0),
	INST("ANDCAM",  OPC_nANDCAM, 0, 0, 0),
	INST("ANDCAB",  OPC_nANDCAB, 0, 0, 0),

	INST("ANDCM",   OPC_nANDCM,  0, 0, 0),
	INST("ANDCMI",  OPC_nANDCMI, 0, 0, 0),
	INST("ANDCMM",  OPC_nANDCMM, 0, 0, 0),
	INST("ANDCMB",  OPC_nANDCMB, 0, 0, 0),

	INST("ANDCB",   OPC_nANDCB,  0, 0, 0),
	INST("ANDCBI",  OPC_nANDCBI, 0, 0, 0),
	INST("ANDCBM",  OPC_nANDCBM, 0, 0, 0),
	INST("ANDCBB",  OPC_nANDCBB, 0, 0, 0),

	INST("IOR",     OPC_nIOR,    0, 0, 0),
	INST("IORI",    OPC_nIORI,   0, 0, 0),
	INST("IORM",    OPC_nIORM,   0, 0, 0),
	INST("IORB",    OPC_nIORB,   0, 0, 0),

	INST("ORA",     OPC_nORA,    0, 0, 0),
	INST("ORAI",    OPC_nORAI,   0, 0, 0),
	INST("ORAM",    OPC_nORAM,   0, 0, 0),
	INST("ORAB",    OPC_nORAB,   0, 0, 0),

	INST("ORCM",     OPC_nORCM,    0, 0, 0),
	INST("ORCMI",    OPC_nORCMI,   0, 0, 0),
	INST("ORCMM",    OPC_nORCMM,   0, 0, 0),
	INST("ORCMB",    OPC_nORCMB,   0, 0, 0),

	INST("ORCB",     OPC_nORCB,    0, 0, 0),
	INST("ORCBI",    OPC_nORCBI,   0, 0, 0),
	INST("ORCBM",    OPC_nORCBM,   0, 0, 0),
	INST("ORCBB",    OPC_nORCBB,   0, 0, 0),

	INST("XOR",     OPC_nXOR,    0, 0, 0),
	INST("XORI",    OPC_nXORI,   0, 0, 0),
	INST("XORM",    OPC_nXORM,   0, 0, 0),
	INST("XORB",    OPC_nXORB,   0, 0, 0),

	INST("EQV",     OPC_nXOR,    0, 0, 0),
	INST("EQVI",    OPC_nXORI,   0, 0, 0),
	INST("EQVM",    OPC_nXORM,   0, 0, 0),
	INST("EQVB",    OPC_nXORB,   0, 0, 0),


	INST("HLL",      OPC_nHLL,    0, 0, 0),
	INST("HLLI",     OPC_nHLLI,   0, 0, 0),
	INST("HLLM",     OPC_nHLLM,   0, 0, 0),
	INST("HLLS",     OPC_nHLLS,   0, 0, 0),

	INST("HLLZ",     OPC_nHLLZ,    0, 0, 0),
	INST("HLLZI",    OPC_nHLLZI,   0, 0, 0),
	INST("HLLZM",    OPC_nHLLZM,   0, 0, 0),
	INST("HLLZS",    OPC_nHLLZS,   0, 0, 0),

	INST("HLLO",     OPC_nHLLO,    0, 0, 0),
	INST("HLLOI",    OPC_nHLLOI,   0, 0, 0),
	INST("HLLOM",    OPC_nHLLOM,   0, 0, 0),
	INST("HLLOS",    OPC_nHLLOS,   0, 0, 0),

	INST("HLLE",     OPC_nHLLE,    0, 0, 0),
	INST("HLLEI",    OPC_nHLLEI,   0, 0, 0),
	INST("HLLEM",    OPC_nHLLEM,   0, 0, 0),
	INST("HLLES",    OPC_nHLLES,   0, 0, 0),


	INST("HLR",      OPC_nHLR,    0, 0, 0),
	INST("HLRI",     OPC_nHLRI,   0, 0, 0),
	INST("HLRM",     OPC_nHLRM,   0, 0, 0),
	INST("HLRS",     OPC_nHLRS,   0, 0, 0),

	INST("HLRZ",     OPC_nHLRZ,    0, 0, 0),
	INST("HLRZI",    OPC_nHLRZI,   0, 0, 0),
	INST("HLRZM",    OPC_nHLRZM,   0, 0, 0),
	INST("HLRZS",    OPC_nHLRZS,   0, 0, 0),

	INST("HLRO",     OPC_nHLRO,    0, 0, 0),
	INST("HLROI",    OPC_nHLROI,   0, 0, 0),
	INST("HLROM",    OPC_nHLROM,   0, 0, 0),
	INST("HLROS",    OPC_nHLROS,   0, 0, 0),

	INST("HLRE",     OPC_nHLRE,    0, 0, 0),
	INST("HLREI",    OPC_nHLREI,   0, 0, 0),
	INST("HLREM",    OPC_nHLREM,   0, 0, 0),
	INST("HLRES",    OPC_nHLRES,   0, 0, 0),


	INST("HRL",      OPC_nHRL,    0, 0, 0),
	INST("HRLI",     OPC_nHRLI,   0, 0, 0),
	INST("HRLM",     OPC_nHRLM,   0, 0, 0),
	INST("HRLS",     OPC_nHRLS,   0, 0, 0),

	INST("HRLZ",     OPC_nHRLZ,    0, 0, 0),
	INST("HRLZI",    OPC_nHRLZI,   0, 0, 0),
	INST("HRLZM",    OPC_nHRLZM,   0, 0, 0),
	INST("HRLZS",    OPC_nHRLZS,   0, 0, 0),

	INST("HRLO",     OPC_nHRLO,    0, 0, 0),
	INST("HRLOI",    OPC_nHRLOI,   0, 0, 0),
	INST("HRLOM",    OPC_nHRLOM,   0, 0, 0),
	INST("HRLOS",    OPC_nHRLOS,   0, 0, 0),

	INST("HRLE",     OPC_nHRLE,    0, 0, 0),
	INST("HRLEI",    OPC_nHRLEI,   0, 0, 0),
	INST("HRLEM",    OPC_nHRLEM,   0, 0, 0),
	INST("HRLES",    OPC_nHRLES,   0, 0, 0),


	INST("HRR",      OPC_nHRR,    0, 0, 0),
	INST("HRRI",     OPC_nHRRI,   0, 0, 0),
	INST("HRRM",     OPC_nHRRM,   0, 0, 0),
	INST("HRRS",     OPC_nHRRS,   0, 0, 0),

	INST("HRRZ",     OPC_nHRRZ,    0, 0, 0),
	INST("HRRZI",    OPC_nHRRZI,   0, 0, 0),
	INST("HRRZM",    OPC_nHRRZM,   0, 0, 0),
	INST("HRRZS",    OPC_nHRRZS,   0, 0, 0),

	INST("HRRO",     OPC_nHRRO,    0, 0, 0),
	INST("HRROI",    OPC_nHRROI,   0, 0, 0),
	INST("HRROM",    OPC_nHRROM,   0, 0, 0),
	INST("HRROS",    OPC_nHRROS,   0, 0, 0),

	INST("HRRE",     OPC_nHRRE,    0, 0, 0),
	INST("HRREI",    OPC_nHRREI,   0, 0, 0),
	INST("HRREM",    OPC_nHRREM,   0, 0, 0),
	INST("HRRES",    OPC_nHRRES,   0, 0, 0),



	INST("TLN",      OPC_nTLN,     0, 0, 0),
	INST("TLNE",     OPC_nTLNE,    0, 0, 0),
	INST("TLNA",     OPC_nTLNA,    0, 0, 0),
	INST("TLNN",     OPC_nTLNN,    0, 0, 0),

	INST("TLZ",      OPC_nTLZ,     0, 0, 0),
	INST("TLZE",     OPC_nTLZE,    0, 0, 0),
	INST("TLZA",     OPC_nTLZA,    0, 0, 0),
	INST("TLZN",     OPC_nTLZN,    0, 0, 0),

	INST("TLC",      OPC_nTLC,     0, 0, 0),
	INST("TLCE",     OPC_nTLCE,    0, 0, 0),
	INST("TLCA",     OPC_nTLCA,    0, 0, 0),
	INST("TLCN",     OPC_nTLCN,    0, 0, 0),

	INST("TLO",      OPC_nTLO,     0, 0, 0),
	INST("TLOE",     OPC_nTLOE,    0, 0, 0),
	INST("TLOA",     OPC_nTLOA,    0, 0, 0),
	INST("TLON",     OPC_nTLON,    0, 0, 0),


	INST("TRN",      OPC_nTRN,     0, 0, 0),
	INST("TRNE",     OPC_nTRNE,    0, 0, 0),
	INST("TRNA",     OPC_nTRNA,    0, 0, 0),
	INST("TRNN",     OPC_nTRNN,    0, 0, 0),

	INST("TRZ",      OPC_nTRZ,     0, 0, 0),
	INST("TRZE",     OPC_nTRZE,    0, 0, 0),
	INST("TRZA",     OPC_nTRZA,    0, 0, 0),
	INST("TRZN",     OPC_nTRZN,    0, 0, 0),

	INST("TRC",      OPC_nTRC,     0, 0, 0),
	INST("TRCE",     OPC_nTRCE,    0, 0, 0),
	INST("TRCA",     OPC_nTRCA,    0, 0, 0),
	INST("TRCN",     OPC_nTRCN,    0, 0, 0),

	INST("TRO",      OPC_nTRO,     0, 0, 0),
	INST("TROE",     OPC_nTROE,    0, 0, 0),
	INST("TROA",     OPC_nTROA,    0, 0, 0),
	INST("TRON",     OPC_nTRON,    0, 0, 0),


	INST("TDN",      OPC_nTDN,     0, 0, 0),
	INST("TDNE",     OPC_nTDNE,    0, 0, 0),
	INST("TDNA",     OPC_nTDNA,    0, 0, 0),
	INST("TDNN",     OPC_nTDNN,    0, 0, 0),

	INST("TDZ",      OPC_nTDZ,     0, 0, 0),
	INST("TDZE",     OPC_nTDZE,    0, 0, 0),
	INST("TDZA",     OPC_nTDZA,    0, 0, 0),
	INST("TDZN",     OPC_nTDZN,    0, 0, 0),

	INST("TDC",      OPC_nTDC,     0, 0, 0),
	INST("TDCE",     OPC_nTDCE,    0, 0, 0),
	INST("TDCA",     OPC_nTDCA,    0, 0, 0),
	INST("TDCN",     OPC_nTDCN,    0, 0, 0),

	INST("TDO",      OPC_nTDO,     0, 0, 0),
	INST("TDOE",     OPC_nTDOE,    0, 0, 0),
	INST("TDOA",     OPC_nTDOA,    0, 0, 0),
	INST("TDON",     OPC_nTDON,    0, 0, 0),


	INST("TSN",      OPC_nTSN,     0, 0, 0),
	INST("TSNE",     OPC_nTSNE,    0, 0, 0),
	INST("TSNA",     OPC_nTSNA,    0, 0, 0),
	INST("TSNN",     OPC_nTSNN,    0, 0, 0),

	INST("TSZ",      OPC_nTSZ,     0, 0, 0),
	INST("TSZE",     OPC_nTSZE,    0, 0, 0),
	INST("TSZA",     OPC_nTSZA,    0, 0, 0),
	INST("TSZN",     OPC_nTSZN,    0, 0, 0),

	INST("TSC",      OPC_nTSC,     0, 0, 0),
	INST("TSCE",     OPC_nTSCE,    0, 0, 0),
	INST("TSCA",     OPC_nTSCA,    0, 0, 0),
	INST("TSCN",     OPC_nTSCN,    0, 0, 0),

	INST("TSO",      OPC_nTSO,     0, 0, 0),
	INST("TSOE",     OPC_nTSOE,    0, 0, 0),
	INST("TSOA",     OPC_nTSOA,    0, 0, 0),
	INST("TSON",     OPC_nTSON,    0, 0, 0),

	// EXTEND Instructions
	// Compare String Instructions
	INST("CMPSL",      OPC_nCMPSL,    0, 0, 0),
	INST("CMPSE",      OPC_nCMPSE,    0, 0, 0),
	INST("CMPSLE",     OPC_nCMPSLE,   0, 0, 0),
	INST("CMPSGE",     OPC_nCMPSGE,   0, 0, 0),
	INST("CMPSN",      OPC_nCMPSN,    0, 0, 0),
	INST("CMPSG",      OPC_nCMPSG,    0, 0, 0),

	INST("EDIT",       OPC_nEDIT,     0, 0, 0),

	INST("CVTBDO",     OPC_nCVTBDO,   0, 0, 0),
	INST("CVTBDT",     OPC_nCVTBDT,   0, 0, 0),
	INST("CVTDBO",     OPC_nCVTDBO,   0, 0, 0),
	INST("CVTDBT",     OPC_nCVTDBT,   0, 0, 0),

	INST("MOVSO",      OPC_nMOVSO,    0, 0, 0),
	INST("MOVST",      OPC_nMOVST,    0, 0, 0),
	INST("MOVSLJ",     OPC_nMOVSLJ,   0, 0, 0),
	INST("MOVSRJ",     OPC_nMOVSRJ,   0, 0, 0),

	INST("XBLT",       OPC_nXBLT,     0, 0, 0),

	INST("GSNGL",      OPC_nGSNGL,    0, 0, 0),
	INST("GDBLE",      OPC_nGDBLE,    0, 0, 0),
	INST("GFSC",       OPC_nGFSC,     0, 0, 0),
	INST("GFIX",       OPC_nGFIX,     0, 0, 0),
	INST("GFIXR",      OPC_nGFIXR,    0, 0, 0),
	INST("GFLTR",      OPC_nGFLTR,    0, 0, 0),
	INST("DGFIX",      OPC_nDGFIX,    0, 0, 0),
	INST("DGFIXR",     OPC_nDGFIXR,   0, 0, 0),
	INST("DGFLTR",     OPC_nDGFLTR,   0, 0, 0)

#endif

