/*
 * opcode.h
 *
 *  Created on: Feb 12, 2017
 *      Author: fswor
 */

#pragma once

struct pdp10_Opcode {
	const char *opName;
	const char *opDesc;
	h10_t       opExtend;
	h10_t       opOcode;
};

#define OPC_nUUO		0000	// Unimplemented Instruction
#define OPC_nLUUO01		0001
#define OPC_nLUUO02		0002
#define OPC_nLUUO03		0003
#define OPC_nLUUO04		0004
#define OPC_nLUUO05		0005
#define OPC_nLUUO06		0006
#define OPC_nLUUO07		0007

#define OPC_nLUUO10		0010
#define OPC_nLUUO11		0011
#define OPC_nLUUO12		0012
#define OPC_nLUUO13		0013
#define OPC_nLUUO14		0014
#define OPC_nLUUO15		0015
#define OPC_nLUUO16		0016
#define OPC_nLUUO17		0017

#define OPC_nLUUO20		0020
#define OPC_nLUUO21		0021
#define OPC_nLUUO22		0022
#define OPC_nLUUO23		0023
#define OPC_nLUUO24		0024
#define OPC_nLUUO25		0025
#define OPC_nLUUO26		0026
#define OPC_nLUUO27		0027

#define OPC_nLUUO30		0030
#define OPC_nLUUO31		0031
#define OPC_nLUUO32		0032
#define OPC_nLUUO33		0033
#define OPC_nLUUO34		0034
#define OPC_nLUUO35		0035
#define OPC_nLUUO36		0036
#define OPC_nLUUO37		0037


#define OPC_nGFAD		0102	// Giant Floating Add
#define OPC_nGFSB		0103	// Giant Floating Subtract

#define OPC_nJSYS		0104
#define OPC_nADJSP		0105	// Adjust Stack Pointer
#define OPC_nGFMP		0106	// Giant Floating Multiply
#define OPC_nGFDV		0107	// Giant Floating Divide

#define OPC_nDFAD		0110	// Double Floating Add
#define OPC_nDFSB		0111	// Double Floating Subtract
#define OPC_nDFMP		0112	// Double Floating Multiply
#define OPC_nDFDV		0113	// Double Floating Divide

#define OPC_nDADD		0114	// Double Add
#define OPC_nDSUB		0115	// Double Subtract
#define OPC_nDMUL		0116	// Double Multiply
#define OPC_nDDIV		0117	// Double Divide

#define OPC_nDMOVE		0120	// Double Move
#define OPC_nDMOVN		0121	// Double Move Negative
#define OPC_nFIX		0122	// Fix
#define OPC_nEXTEND		0123	// Extended Instruction

#define OPC_nDMOVEM		0124	// Double Move to Memory
#define OPC_nDMOVNM		0125	// Double Move Negative to Memory
#define OPC_nFIXR		0126	// Fix and Round
#define OPC_nFLTR		0127	// Float and Round

#define OPC_nUFA		0130	// Unnormalized Floating Add
#define OPC_nDFN		0131	// Double Floating Negate
#define OPC_nFSC		0132	// Floating Scale
#define OPC_nADJBP		0133	// Adjust Byte Pointer
#define OPC_nIBP		0133	// Increment Byte Pointer

#define OPC_nILDB		0134	// Increment Pointer and Load Byte
#define OPC_nLDB		0135	// Load Byte
#define OPC_nIDPB		0136	// Increment Pointer and Deposit Byte
#define OPC_nDPB		0137	// Deposit Byte

#define OPC_nFAD		0140	// Floating Add
#define OPC_nFADL		0141	// Floating Add Long
#define OPC_nFADM		0142	// Floating Add to Memory
#define OPC_nFADB		0143	// Floating Add to Both

#define OPC_nFADR		0144	// Floating Add and Round
#define OPC_nFADRI		0145	// Floating Add and Round Immediate
#define OPC_nFADRM		0146	// Floating Add and Round to Memory
#define OPC_nFADRB		0147	// Floating Add and Round to Both

#define OPC_nFSB		0150	// Floating Subtract
#define OPC_nFSBL		0151	// Floating Subtract Long
#define OPC_nFSBM		0152	// Floating Subtract to Memory
#define OPC_nFSBB		0153	// Floating Subtract to Both

#define OPC_nFSBR		0154	// Floating Subtract and Round
#define OPC_nFSBRI		0155	// Floating Subtract and Round Immediate
#define OPC_nFSBRM		0156	// Floating Subtract and Round to Memory
#define OPC_nFSBRB		0157	// Floating Subtract and Round to Both

#define OPC_nFMP		0160	// Floating Multiply
#define OPC_nFMPL		0161	// Floating Multiply Long
#define OPC_nFMPM		0162	// Floating Multiply to Memory
#define OPC_nFMPB		0163	// Floating Multiply to Both

#define OPC_nFMPR		0164	// Floating Multiply and Round
#define OPC_nFMPRI		0165	// Floating Multiply and Round Immediate
#define OPC_nFMPRM		0166	// Floating Multiply and Round to Memory
#define OPC_nFMPRB		0167	// Floating Multiply and Round to Both

#define OPC_nFDV		0170	// Floating Divide
#define OPC_nFDVL		0171	// Floating Divide Long
#define OPC_nFDVM		0172	// Floating Divide to Memory
#define OPC_nFDVB		0173	// Floating Divide to Both

#define OPC_nFDVR		0174	// Floating Divide and Round
#define OPC_nFDVRI		0175	// Floating Divide and Round Immediate
#define OPC_nFDVRM		0176	// Floating Divide and Round to Memory
#define OPC_nFDVRB		0177	// Floating Divide and Round to Both


#define OPC_nMOVE		0200	// Move
#define OPC_nMOVEI		0201	// Move Immediate
#define OPC_nMOVEM		0202	// Move to Memory
#define OPC_nMOVES		0203	// Move to Self

#define OPC_nMOVS		0204	// Move Swapped
#define OPC_nMOVSI		0205	// Move Swapped Immediate
#define OPC_nMOVSM		0206	// Move Swapped to Memory
#define OPC_nMOVSS		0207	// Move Swapped to Self

#define OPC_nMOVN		0210	// Move Negative
#define OPC_nMOVNI		0211	// Move Negative Immediate
#define OPC_nMOVNM		0212	// Move Negative to Memory
#define OPC_nMOVNS		0213	// Move Negative to Self

#define OPC_nMOVM		0214	// Move Magnitude
#define OPC_nMOVMI		0215	// Move Magnitude Immediate
#define OPC_nMOVMM		0216	// Move Magnitude to Memory
#define OPC_nMOVMS		0217	// Move Magnitude to Self

#define OPC_nIMUL		0220	// Integer Multiply
#define OPC_nIMULI		0221	// Integer Multiply Immediate
#define OPC_nIMULM		0222	// Integer Multiply to Memory
#define OPC_nIMULB		0223	// Integer Multiply to Both

#define OPC_nMUL		0224	// Multiply
#define OPC_nMULI		0225	// Multiply Immediate
#define OPC_nMULM		0226	// Multiply to Memory
#define OPC_nMULB		0227	// Multiply to Both

#define OPC_nIDIV		0230	// Integer Divide
#define OPC_nIDIVI		0231	// Integer Divide Immediate
#define OPC_nIDIVM		0232	// Integer Divide to Memory
#define OPC_nIDIVB		0233	// Integer Divide to Both

#define OPC_nDIV		0234	// Divide
#define OPC_nDIVI		0235	// Divide Immediate
#define OPC_nDIVM		0236	// Divide to Memory
#define OPC_nDIVB		0237	// Divide to Both

#define OPC_nASH		0240	// Arithmetic Shift
#define OPC_nROT		0241	// Rotate
#define OPC_nLSH		0242	// Logical Shift
#define OPC_nJFFO		0243	// Jump if Find First One

#define OPC_nASHC		0244	// Arithmetic Shift Combined
#define OPC_nROTC		0245	// Rotate Combined
#define OPC_nLSHC		0246	// Logical Shift Combined

#define OPC_nEXCH		0250	// Exchange
#define OPC_nBLT		0251	// Block Transfer
#define OPC_nAOBJP		0252	// Add One to Both Halves of AC and Jump if Positive
#define OPC_nAOBJN		0253	// Add One to Both Halves of AC and Jump if Negative

#define OPC_nJRST		0254	// Jump and Restore
#define OPC_nJFCL		0255	// Jump on Flag and Clear
#define OPC_nXCT		0256	// Execute
#define OPC_nMAP		0257	// Map an Address

#define OPC_nPUSHJ		0260	// Push and Jump
#define OPC_nPUSH		0261	// Push
#define OPC_nPOP		0262	// Pop
#define OPC_nPOPJ		0263	// Pop and Jump

#define OPC_nJSR		0264	// Jump to Subroutine
#define OPC_nJSP		0265	// Jump and Save PC
#define OPC_nJSA		0266	// Jump and Save AC
#define OPC_nJRA		0267	// Jump and Restore AC

#define OPC_nADD		0270	// Add
#define OPC_nADDI		0271	// Add Immediate
#define OPC_nADDM		0272	// Add to Memory
#define OPC_nADDB		0273	// Add to Both

#define OPC_nSUB		0274	// Subtract
#define OPC_nSUBI		0275	// Subtract Immediate
#define OPC_nSUBM		0276	// Subtract to Memory
#define OPC_nSUBB		0277	// Subtract to Both


#define OPC_nCAI		0300	// Compare AC Immediate and Do Not Skip
#define OPC_nCAIL		0301	// Compare AC Immediate and Skip if AC less than E
#define OPC_nCAIE		0302	// Compare AC immediate and Skip if Equal
#define OPC_nCAILE		0303	// Compare AC Immediate and Skip if AC less than or Equal to E
#define OPC_nCAIA		0304	// Compare AC Immediate and Always Skip
#define OPC_nCAIGE		0305	// Compare AC Immediate and Skip if AC Greater than or Equal to E
#define OPC_nCAIN		0306	// Compare AC Immediate and Skip if Not Equal
#define OPC_nCAIG		0307	// Compare AC Immediate and Skip if AC Greater than E

#define OPC_nCAM		0310	// Compare AC with Memory but Do Not Skip
#define OPC_nCAML		0311	// Compare AC with Memory and Skip if AC Less
#define OPC_nCAME		0312	// Compare AC with Memory and Skip if Equal
#define OPC_nCAMLE		0313	// Compare AC with Memory and Skip if AC Less or Equal
#define OPC_nCAMA		0314	// Compare AC with Memory but Always Skip
#define OPC_nCAMGE		0315	// Compare AC with Memory and Skip if AC Greater or Equal
#define OPC_nCAMN		0316	// Compare AC with Memory and Skip if Not Equal
#define OPC_nCAMG		0317	// Compare AC with Memory and Skip if AC Greater

#define OPC_nJUMP		0320	// Do Not Jump
#define OPC_nJUMPL		0321	// Jump if AC Less than Zero
#define OPC_nJUMPE		0322	// Jump if AC Equal to Zero
#define OPC_nJUMPLE		0323	// Jump if AC Less than or Equal to Zero
#define OPC_nJUMPA		0324	// Jump Always
#define OPC_nJUMPGE		0325	// Jump if AC Greater than or Equal to Zero
#define OPC_nJUMPN		0326	// Jump if AC Not Equal to Zero
#define OPC_nJUMPG		0327	// Jump if AC Greater than Zero

#define OPC_nSKIP		0330	// Do Not Skip, but read Memory
#define OPC_nSKIPL		0331	// Skip if Memory Less than Zero
#define OPC_nSKIPE		0332	// Skip if Memory Equal to Zero
#define OPC_nSKIPLE		0333	// Skip if Memory Less than or Equal to Zero
#define OPC_nSKIPA		0334	// Skip Always
#define OPC_nSKIPGE		0335	// Skip if Memory Greater than or Equal to Zero
#define OPC_nSKIPN		0336	// Skip if Memory Not Equal to Zero
#define OPC_nSKIPG		0337	// Skip if Memory Greater than Zero

#define OPC_nAOJ		0340	// Add One to AC but Do Not Jump
#define OPC_nAOJL		0341	// Add One to AC and Jump if Less than Zero
#define OPC_nAOJE		0342	// Add One to AC and Jump if Equal to Zero
#define OPC_nAOJLE		0343	// Add One to AC and Jump if Less than or Equal to Zero
#define OPC_nAOJA		0344	// Add One to AC and Jump Always
#define OPC_nAOJGE		0345	// Add One to AC and Jump if Greater than or Equal to Zero
#define OPC_nAOJN		0346	// Add One to AC and Jump if Not Equal to Zero
#define OPC_nAOJG		0347	// Add One to AC and Jump if Greater than Zero

#define OPC_nAOS		0350	// Add One to Memory but Do Not Skip
#define OPC_nAOSL		0351	// Add One to Memory and Skip if Less than Zero
#define OPC_nAOSE		0352	// Add One to Memory and Skip if Equal to Zero
#define OPC_nAOSLE		0353	// Add One to Memory and Skip if Less than or Equal to Zero
#define OPC_nAOSA		0354	// Add One to Memory and Skip Always
#define OPC_nAOSGE		0355	// Add One to Memory and Skip if Greater than or Equal to Zero
#define OPC_nAOSN		0356	// Add One to Memory and Skip if Not Equal to Zero
#define OPC_nAOSG		0357	// Add One to Memory and Skip if Greater than Zero

#define OPC_nSOJ		0360	// Subtract One from AC but Do Not Jump
#define OPC_nSOJL		0361	// Subtract One from AC and Jump if Less than Zero
#define OPC_nSOJE		0362	// Subtract One from AC and Jump if Equal to Zero
#define OPC_nSOJLE		0363	// Subtract One from AC and Jump if Less than or Equal to Zero
#define OPC_nSOJA		0364	// Subtract One from AC and Jump Always
#define OPC_nSOJGE		0365	// Subtract One from AC and Jump if Greater than or Equal to Zero
#define OPC_nSOJN		0366	// Subtract One from AC and Jump if Not Equal to Zero
#define OPC_nSOJG		0367	// Subtract One from AC and Jump if Greater than Zero

#define OPC_nSOS		0370	// Subtract One from Memory but Do Not Skip
#define OPC_nSOSL		0371	// Subtract One from Memory and Skip if Less than Zero
#define OPC_nSOSE		0372	// Subtract One from Memory and Skip if Equal to Zero
#define OPC_nSOSLE		0373	// Subtract One from Memory and Skip if Less than or Equal to Zero
#define OPC_nSOSA		0374	// Subtract One from Memory and Skip Always
#define OPC_nSOSGE		0375	// Subtract One from Memory and if Greater than or Equal to Zero
#define OPC_nSOSN		0376	// Subtract One from Memory and if Not Equal to Zero
#define OPC_nSOSG		0377	// Subtract One from Memory and if Greater than Zero


#define OPC_nSETZ		0400	// Set to Zeros
#define OPC_nSETZI		0401	// Set to Zeros Immediate
#define OPC_nSETZM		0402	// Set to Zeros Memory
#define OPC_nSETZB		0403	// Set to Zeros Both

#define OPC_nAND		0404	// And
#define OPC_nANDI		0405	// And Immediate
#define OPC_nANDM		0406	// And to Memory
#define OPC_nANDB		0407	// And to Both

#define OPC_nANDCA		0410	// And with Complement of AC
#define OPC_nANDCAI		0411	// And with Complement of AC Immediate
#define OPC_nANDCAM		0412	// And with Complement of AC to Memory
#define OPC_nANDCAB		0413	// And with Complement of AC to Both

#define OPC_nSETM		0414	// Set to Memory
#define OPC_nSETMI		0415	// Set to Memory Immediate
#define OPC_nXMOVEI		0415	// Extended Move Immediate
#define OPC_nSETMM		0416	// Set to Memory Memory
#define OPC_nSETMB		0417	// Set to Memory Both

#define OPC_nANDCM		0420	// And Complement of Memory
#define OPC_nANDCMI		0421	// And Complement of Memory Immediate
#define OPC_nANDCMM		0422	// And Complement of Memory to Memory
#define OPC_nANDCMB		0423	// And Complement of Memory to Both

#define OPC_nSETA		0424	// Set to AC
#define OPC_nSETAI		0425	// Set to AC Immediate
#define OPC_nSETAM		0426	// Set to AC Memory
#define OPC_nSETAB		0427	// Set to AC Both

#define OPC_nXOR		0430	// Exclusive Or
#define OPC_nXORI		0431	// Exclusive Or Immediate
#define OPC_nXORM		0432	// Exclusive Or to Memory
#define OPC_nXORB		0433	// Exclusive Or to Both

#define OPC_nIOR		0434	// Inclusive Or
#define OPC_nIORI		0435	// Inclusive Or Immediate
#define OPC_nIORM		0436	// Inclusive Or to Memory
#define OPC_nIORB		0437	// Inclusive Or to Both

#define OPC_nANDCB		0440	// And Complements of Both
#define OPC_nANDCBI		0441	// And Complements of Both Immediate
#define OPC_nANDCBM		0442	// And Complements of Both to Memory
#define OPC_nANDCBB		0443	// And Complements of Both to Both

#define OPC_nEQV		0444	// Equivalence
#define OPC_nEQVI		0445	// Equivalence Immediate
#define OPC_nEQVM		0446	// Equivalence to Memory
#define OPC_nEQVB		0447	// Equivalence to Both

#define OPC_nSETCA		0450	// Set to Complement of AC
#define OPC_nSETCAI		0451	// Set to Complement of AC Immediate
#define OPC_nSETCAM		0452	// Set to Complement of AC Memory
#define OPC_nSETCAB		0453	// Set to Complement of AC Both

#define OPC_nORA		0454	// Or with Complement of AC
#define OPC_nORAI		0455	// Or with Complement of AC Immediate
#define OPC_nORAM		0456	// Or with Complement of AC to Memory
#define OPC_nORAB		0457	// Or with Complement of AC to Both

#define OPC_nSETCM		0460	// Set to Complement of Memory
#define OPC_nSETCMI		0461	// Set to Complement of Memory Immediate
#define OPC_nSETCMM		0462	// Set to Complement of Memory Memory
#define OPC_nSETCMB		0463	// Set to Complement of Memory Both

#define OPC_nORCM		0464	// Or Complement of Memory
#define OPC_nORCMI		0465	// Or Complement of Memory Immediate
#define OPC_nORCMM		0466	// Or Complement of Memory to Memory
#define OPC_nORCMB		0467	// Or Complement of Memory to Both

#define OPC_nORCB		0470	// Or Complement of Both
#define OPC_nORCBI		0471	// Or Complement of Both Immediate
#define OPC_nORCBM		0472	// Or Complement of Both to Memory
#define OPC_nORCBB		0473	// Or Complement of Both to Both

#define OPC_nSETO		0474	// Set to Ones
#define OPC_nSETOI		0475	// Set to Ones Immediate
#define OPC_nSETOM		0476	// Set to Ones Memory
#define OPC_nSETOB		0477	// Set to Ones Both


#define OPC_nHLL		0500	// Half Left to Left
#define OPC_nHLLI		0501	// Half Left to Left Immediate
#define OPC_nXHLLI		0501	// Extended Half Left to Left Immediate
#define OPC_nHLLM		0502	// Half Left to Left Memory
#define OPC_nHLLS		0503	// Half Left to Left Self

#define OPC_nHRL		0504	// Half Right to Left
#define OPC_nHRLI		0505	// Half Right to Left Immediate
#define OPC_nHRLM		0506	// Half Right to Left Memory
#define OPC_nHRLS		0507	// Half Right to Left Self

#define OPC_nHLLZ		0510	// Half Left to Left, Zeros
#define OPC_nHLLZI		0511	// Half Left to Left, Zeros, Immediate
#define OPC_nHLLZM		0512	// Half Left to Left, Zeros, Memory
#define OPC_nHLLZS		0513	// Half Left to Left, Zeros, Self

#define OPC_nHRLZ		0514	// Half Right to Left, Zeros
#define OPC_nHRLZI		0515	// Half Right to Left, Zeros, Immediate
#define OPC_nHRLZM		0516	// Half Right to Left, Zeros, Memory
#define OPC_nHRLZS		0517	// half Right to Left, Zeros, Self

#define OPC_nHLLO		0520	// Half Left to Left, Ones
#define OPC_nHLLOI		0521	// Half Left to Left, Ones, Immediate
#define OPC_nHLLOM		0522	// Half Left to Left, Ones, Memory
#define OPC_nHLLOS		0523	// Half Left to Left, Ones, Self

#define OPC_nHRLO		0524	// Half Right to Left, Ones
#define OPC_nHRLOI		0525	// Half Right to Left, Ones, Immediate
#define OPC_nHRLOM		0526	// Half Right to Left, Ones, Memory
#define OPC_nHRLOS		0527	// Half Right to Left, Ones, Self

#define OPC_nHLLE		0530	// Half Left to Left, Extend
#define OPC_nHLLEI		0531	// Half Left to Left, Extend, Immediate
#define OPC_nHLLEM		0532	// Half Left to Left, Extend, Memory
#define OPC_nHLLES		0533	// Half Left to Left, Extend, Self

#define OPC_nHRLE		0534	// Half Right to Left, Extend
#define OPC_nHRLEI		0535	// Half Right to Left, Extend, Immediate
#define OPC_nHRLEM		0536	// Half Right to Left, Extend, Memory
#define OPC_nHRLES		0537	// Half Right to Left, Extend, Self

#define OPC_nHRR		0540	// Half Right to Right
#define OPC_nHRRI		0541	// Half Right to Right Immediate
#define OPC_nHRRM		0542	// Half Right to Right Memory
#define OPC_nHRRS		0543	// Half Right to Right Self

#define OPC_nHLR		0544	// Half Left to Right
#define OPC_nHLRI		0545	// Half Left to Right Immediate
#define OPC_nHLRM		0546	// Half Left to Right Memory
#define OPC_nHLRS		0547	// Half Left to Right Self

#define OPC_nHRRZ		0550	// Half Right to Right, Zeros
#define OPC_nHRRZI		0551	// Half Right to Right, Zeros, Immediate
#define OPC_nHRRZM		0552	// Half Right to Right, Zeros, Memory
#define OPC_nHRRZS		0553	// Half Right to Right, Zeros, Self

#define OPC_nHLRZ		0554	// Half Left to Right, Zeros
#define OPC_nHLRZI		0555	// Half Left to Right, Zeros, Immediate
#define OPC_nHLRZM		0556	// Half Left to Right, Zeros, Memory
#define OPC_nHLRZS		0557	// Half Left to Right, Zeros, Self

#define OPC_nHRRO		0560	// Half Right to Right, Ones
#define OPC_nHRROI		0561	// Half Right to Right, Ones, Immediate
#define OPC_nHRROM		0562	// Half Right to Right, Ones, Memory
#define OPC_nHRROS		0563	// Half Right to Right, Ones, Self

#define OPC_nHLRO		0564	// Half Left to Right, Ones
#define OPC_nHLROI		0565	// Half Left to Right, Ones, Immediate
#define OPC_nHLROM		0566	// Half Left to Right, Ones, Memory
#define OPC_nHLROS		0567	// Half Left to Right, Ones, Self

#define OPC_nHRRE		0570	// Half Right to Right, Extend
#define OPC_nHRREI		0571	// Half Right to Right, Extend, Immediate
#define OPC_nHRREM		0572	// Half Right to Right, Extend, Memory
#define OPC_nHRRES		0573	// Half Right to Right, Extend, Self

#define OPC_nHLRE		0574	// Half Left to Right, Extend
#define OPC_nHLREI		0575	// Half Left to Right, Extend, Immediate
#define OPC_nHLREM		0576	// Half Left to Right, Extend, Memory
#define OPC_nHLRES		0577	// Half Left to Right, Extend, Self


#define OPC_nTRN		0600	// Test Right, No Modification, but Do Not Skip
#define OPC_nTLN		0601	// Test Left, No Modification, but Do Not Skip
#define OPC_nTRNE		0602	// Test Right, No Modification, and Skip if All Masked Bits Equal Zero
#define OPC_nTLNE		0603	// Test Left, No Modification, and Skip if All Masked Bits Equal Zero
#define OPC_nTRNA		0604	// Test Right, No Modification, but Always Skip
#define OPC_nTLNA		0605	// Test Left, No Modification, but Always Skip
#define OPC_nTRNN		0606	// Test Right, No Modification, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTLNN		0607	// Test Left, No Modification, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTDN		0610	// Test Direct, No Modification, but Do Not Skip
#define OPC_nTSN		0611	// Test Swapped, No Modification, but Do Not Skip
#define OPC_nTDNE		0612	// Test Direct, No Modification, and Skip if All Masked Bits Equal Zero
#define OPC_nTSNE		0613	// Test Swapped, No Modification, and Skip if All Masked Bits Equal Zero
#define OPC_nTDNA		0614	// Test Direct, No Modification, but Skip Always
#define OPC_nTSNA		0615	// Test Swapped, No Modification, but Skip Always
#define OPC_nTDNN		0616	// Test Direct, No Modification, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTSNN		0617	// Test Swapped, No Modification, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTRZ		0620	// Test Right, Zeros, but Do Not Skip
#define OPC_nTLZ		0621	// Test Left, Zeros, but Do Not Skip
#define OPC_nTRZE		0622	// Test Right, Zeros, and Skip if All Masked Bits Equal Zero
#define OPC_nTLZE		0623	// Test Left, Zeros, and Skip if All Masked Bits Equal Zero
#define OPC_nTRZA		0624	// Test Right, Zeros, but Skip Always
#define OPC_nTLZA		0625	// Test Left, Zeros, but Skip Always
#define OPC_nTRZN		0626	// Test Right, Zeros, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTLZN		0627	// Test Left, Zeros, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTDZ		0630	// Test Direct, Zeros, but Do Not Skip
#define OPC_nTSZ		0631	// Test Swapped, Zeros, but Do Not Skip
#define OPC_nTDZE		0632	// Test Direct, Zeros, and Skip if All Masked Bits Equal Zero
#define OPC_nTSZE		0633	// Test Swapped, Zeros, and Skip if All Masked Bits Equal Zero
#define OPC_nTDZA		0634	// Test Direct, Zeros, but Skip Always
#define OPC_nTSZA		0635	// Test Swapped, Zeros, but Skip Always
#define OPC_nTDZN		0636	// Test Direct, Zeros, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTSZN		0637	// Test Swapped, Zeros, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTRC		0640	// Test Right, Complements, but Do Not Skip
#define OPC_nTLC		0641	// Test Left, Complements, but Do Not Skip
#define OPC_nTRCE		0642	// Test Right, Complements, and Skip if All Masked Bits Equal Zero
#define OPC_nTLCE		0643	// Test Left, Complements, and Skip if All Masked Bits Equal Zero
#define OPC_nTRCA		0644	// Test Right, Complements, but Skip Always
#define OPC_nTLCA		0645	// Test Left, Complements, but Skip Always
#define OPC_nTRCN		0646	// Test Right, Complements, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTLCN		0647	// Test Left, Complements, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTDC		0650	// Test Direct, Complements, but Do Not Skip
#define OPC_nTSC		0651	// Test Swapped, Complements, but Do Not Skip
#define OPC_nTDCE		0652	// Test Direct, Complements, and Skip if All Masked Bits Equal Zero
#define OPC_nTSCE		0653	// Test Swapped, Complements, and Skip if All Masked Bits Equal Zero
#define OPC_nTDCA		0654	// Test Direct, Complements, but Skip Always
#define OPC_nTSCA		0655	// Test Swapped, Complements, but Skip Always
#define OPC_nTDCN		0656	// Test Direct, Complements, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTSCN		0657	// Test Swapped, Complements, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTRO		0660	// Test Right, Ones, but Do Not Skip
#define OPC_nTLO		0661	// Test Left, Ones but Do Not Skip
#define OPC_nTROE		0662	// Test Right, Ones, and Skip if All Masked Bits Equal Zero
#define OPC_nTLOE		0663	// Test Left, Ones, and Skip if All Masked Bits Equal Zero
#define OPC_nTROA		0664	// Test Right, Ones, but Skip Always
#define OPC_nTLOA		0665	// Test Left, Ones, but Skip Always
#define OPC_nTRON		0666	// Test Right, Ones, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTLON		0667	// Test Left, Ones, and Skip if Not All Masked Bits Equal Zero

#define OPC_nTDO		0670	// Test Direct, Ones, but Do Not Skip
#define OPC_nTSO		0671	// Test Swapped, Ones, but Do Not Skip
#define OPC_nTDOE		0672	// Test Direct, Ones, and Skip if All Masked Bits Equal Zero
#define OPC_nTSOE		0673	// Test Swapped, Ones, and Skip if All Masked Bits Equal Zero
#define OPC_nTDOA		0674	// Test Direct, Ones, but Skip Always
#define OPC_nTSOA		0675	// Test Swapped, Ones, but Skip Always
#define OPC_nTDON		0676	// Test Direct, Ones, and Skip if Not All Masked Bits Equal Zero
#define OPC_nTSON		0677	// Test Swapped, Ones, and Skip if Not All Masked Bits Equal Zero


#define OPC_nAPR0		0700
#define OPC_nAPR1		0701
#define OPC_nAPR2		0702
#define OPC_nAPR3		0703

#define OPC_nPMOVE		0704
#define OPC_nPMOVEM		0705
#define OPC_nNMOVE		0706
#define OPC_nNMOVEM		0707

#define OPC_nLDCFG		0710
#define OPC_nRDCFG		0711

#define OPC_nAMOVE		0714
#define OPC_nAMOVEM		0715
#define OPC_nUMOVE		0716
#define OPC_nUMOVEM		0717

// EXTEND instructions
#define OPC_nCMPSL		0001	// Compare Strings and Skip if Less Than
#define OPC_nCMPSE		0002	// Compare Strings and Skip if Equal To
#define OPC_nCMPSLE		0003	// Compare Strings and Skip if Less Than or Equal To

#define OPC_nEDIT		0004	// Edit String
#define OPC_nCMPSGE		0005	// Compare Strings and Skip if Greater Than or Equal To
#define OPC_nCMPSN		0006	// Compare Strings and Skip if Not Equal
#define OPC_nCMPSG		0007	// Compare Strings and Skip if Greater Than

#define OPC_nCVTDBO		0010	// Convert Decimal to Binary Offset
#define OPC_nCVTDBT		0011	// Convert Decimal to Binary Translated
#define OPC_nCVTBDO		0012	// Convert Binary to Decimal Offset
#define OPC_nCVTBDT		0013	// Convert Binary to Decimal Translated

#define OPC_nMOVSO		0014	// Move String Offset
#define OPC_nMOVST		0015	// Move String Translated
#define OPC_nMOVSLJ		0016	// Move String Left Justified
#define OPC_nMOVSRJ		0017	// Move String Right Justified

#define OPC_nXBLT		0020	// Extended Block Transfer
#define OPC_nGSNGL		0021	// Giant Floating to Single Floating
#define OPC_nGDBLE		0022	// Single Floating to Giant Floating
#define OPC_nDGFIX		0023	// Giant Floating to Double Precision Integer

#define OPC_nGFIX		0024	// Giant Floating to Integer
#define OPC_nDGFIXR		0025	// Giant Floating to Double and Round
#define OPC_nGFIXR		0026	// Giant Floating Fix and Round
#define OPC_nDGFLTR		0027	// Giant Float Double and Round

#define OPC_nGFLTR		0030	// Giant Float and Round
#define OPC_nGFSC		0031	// Giant Floating Scale

// Operand flags
#define OPR_SWAR		000001 // Swap AR operand
#define OPR_FEMB		000002 // Fetch from Memory
#define OPR_SVAR		000004 // Save AR operand to Memory
