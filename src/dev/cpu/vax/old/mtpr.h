/*
 * mtpr.h
 *
 *  Created on: Mar 12, 2017
 *      Author: Timothy Stark
 *
 *  MTPR  Move to Privileged Register
 *  MFPR  Move from Privileged Register
 *
 */

#pragma once

// Processor (Privileged) Registers
#define CPU_IPR(rn)   ipReg[rn] // Processor Registers
#define CPU_IPRN(rn)  ipName[rn]
#define DPY_IPR(rn)   (((rn) < CPU_nPREGS) ? CPU_IPR(rn) : 0)


#define IPR_nKSP     0   // Kernel Stack Pointer
#define IPR_nESP     1   // Executive Stack Pointer
#define IPR_nSSP     2   // Supervisor Stack Pointer
#define IPR_nUSP     3   // User Stack Pointer
#define IPR_nISP     4   // Interrupt Stack Pointer

#define IPR_nP0BR    8   // P0 Base Register
#define IPR_nP0LR    9   // P0 Length Register
#define IPR_nP1BR    10  // P1 Base Register
#define IPR_nP1LR    11  // P1 Length Register
#define IPR_nSBR     12  // System Base Register
#define IPR_nSLR     13  // System Length Register
#define IPR_nPCBB    16  // Process Control Block Base
#define IPR_nSCBB    17  // System Control Block Base
#define IPR_nIPL     18  // Interrupt Priority Level
#define IPR_nASTLVL  19  // AST Level
#define IPR_nSIRR    20  // Software Interrupt Request
#define IPR_nSISR    21  // Software Interrupt Summary
#define IPR_nIPIR    22  // KA820 Interprocessor Register
#define IPR_nMCSR    23  // Machine Check Status Register (VAX-11/750)
#define IPR_nICCS    24  // Interval Clock Control
#define IPR_nNICR    25  // Next Interval Count
#define IPR_nICR     26  // Interval Count
#define IPR_nTODR    27  // Time of Year (Optional)
#define iPR_nCSRS    28  // Console Storage Receiver Control Status
#define IPR_nCSRD    29  // Console Storage Receiver Data Buffer
#define IPR_nCSTS    30  // Console Storage Transmit Control Status
#define IPR_nCSTD    31  // Console Storage Transmit Data Buffer
#define IPR_nRXCS    32  // Console Terminal Receiver Control Status
#define IPR_nRXDB    33  // Console Terminal Receiver Data Buffer
#define IPR_nTXCS    34  // Console Terminal Transmit Control Status
#define IPR_nTXDB    35  // Console Terminal Transmit Data Buffer
#define IPR_nMSER    39  // Memory System Error Register
#define IPR_nACCS    40  // Accelerator Control Register
#define IPR_nSAVISP  41  // Console Saved ISP
#define IPR_nSAVPC   42  // Console Saved PC
#define IPR_nCONPC   42  // Console Saved PC
#define IPR_nSAVPSL  43  // Console Saved PSL
#define IPR_nCONPSL  43  // Console Saved PSL
#define IPR_nWCSA    44  // WCS Address
#define IPR_nWCSB    45  // WCS Data
#define iPR_nSBIFS   48  // SBI Fault/Status
#define IPR_nSBIS    49  // SBI Silo
#define IPR_nSBISC   50  // SBI Silo Comparator
#define IPR_nSBIMT   51  // SBI Silo Maintenance
#define IPR_nSBIER   52  // SBI Error Register
#define IPR_nSBITA   53  // SBI Timeout Address Register
#define IPR_nSBIQC   54  // SBI Quadword Clear
#define IPR_nIORESET 55  // Initialize Unibus Register
#define IPR_nMAPEN   56  // Memory Management Enable
#define IPR_nTBIA    57  // Translation Buffer Invalidate All
#define IPR_nTBIS    58  // Translation Buffer Invalidate Single
#define IPR_nTBDATA  59  // Translation Buffer Data
#define IPR_nMBRK    60  // Microprogram Break
#define IPR_nPME     61  // Performance Monitor Enable
#define IPR_nSID     62  // System ID Register
#define IPR_nTBCHK   63  // Translation Buffer Check

// KA750 Processor - Privileged Registers Definition
#define IPR_nTBDR    36  // Translation Buffer Diasble Regiser
#define IPR_nCADR    37  // Cache Disable Register
#define IPR_nMCESR   38  // Machine Check Error Summary Register
#define IPR_nCAER    39  // Cache Error Register

// KA86 Processor - Privileged Registers Definition
#define IPR_nPAMACC  64  // Physical Address Memory Map Access
#define IPR_nPAMLOC  65  // Physical Address Memory Map Location
#define IPR_nCSWP    66  // Cache Sweep
#define IPR_nMDECC   67  // MBOX Data Ecc Register
#define IPR_nMENA    68  // MBOX Error Enable Register
#define IPR_nMDCTL   69  // MBOX Data Control Register
#define IPR_nMCCTL   70  // MBOX Mcc Control Register
#define IPR_nMERG    71  // MBOX Error Generator Register
#define IPR_nCRBT    72  // Console Reboot
#define IPR_nDFI     73  // Diagnostic Fault Insertion Register
#define IPR_nEHSR    74  // Error Handling Status Register
#define IPR_nSTXCS   76  // Console Storage C/S
#define IPR_nSTXDB   77  // Console Storage D/B
#define IPR_nESPA    78  // EBOX Scratchpad Address
#define IPR_nESPD    79  // EBOX Scratchpad Data

// KA820 Processor - Privileged Registers Definition
#define IPR_nRXCS1   80  // Serial-Line Unit 1 Receive CSR
#define IPR_nRXDB1   81  // Serial-Line Unit 1 Receive Data Buffer
#define IPR_nTXCS1   82  // Serial-Line Unit 1 Transmit CSR
#define IPR_nTXDB1   83  // Serial-Line Unit 1 Transmit Data Buffer
#define IPR_nRXCS2   84  // Serial-Line Unit 2 Receive CSR
#define IPR_nRXDB2   85  // Serial-Line Unit 2 Receive Data Buffer
#define IPR_nTXCS2   86  // Serial-Line Unit 2 Transmit CSR
#define IPR_nTXDB2   87  // Serial-Line Unit 2 Transmit Data Buffer
#define IPR_nRXCS3   88  // Serial-Line Unit 3 Receive CSR
#define IPR_nRXDB3   89  // Serial-Line Unit 3 Receive Data Buffer
#define IPR_nTXCS3   90  // Serial-Line Unit 3 Transmit CSR
#define IPR_nTXDB3   91  // Serial-Line Unit 3 Transmit Data Buffer
#define IPR_nRXCD    92  // Receive Console Data from another cpu
#define IPR_nCACHEX  93  // Cache invalidate Register
#define IPR_nBINID   94  // VAXBI node ID Register
#define IPR_nBISTOP  95  // VAXBI Stop Register

// KA670 Processor - Privileged Registers Definition
#define IPR_nBCBTS   113 // Backup Cache Tag Store
#define IPR_nBCP1TS  114 // Primary Tag Store 1st half
#define IPR_nBCP2TS  115 // Primary Tag Store 2st half
#define IPR_nBCRFR   116 // Refresh Register
#define IPR_nBCIDX   117 // Index Register
#define IPR_nBCSTS   118 // Status
#define IPR_nBCCTL   119 // Control Register
#define IPR_nBCERR   120 // Error Address
#define IPR_nBCFBTS  121 // Flush backup tag store
#define IPR_nBCFPTS  122 // Flush primary tag store

// KA43/KA46 Processor - Privileged Registers Definition
#define IPR_nVINTSR  123 // vector i/f error status
#define IPR_nPCTAG   124 // primary cache tag store
#define IPR_nPCIDX   125 // primary cache index
#define IPR_nPCERR   126 // primary cache error address
#define IPR_nPCSTS   127 // primary cache status


#define IPR_KSP    CPU_IPR(IPR_nKSP)     // (R/W) Kernel Stack Pointer
#define IPR_ESP    CPU_IPR(IPR_nESP)     // (R/W) Executive Stack Pointer
#define IPR_SSP    CPU_IPR(IPR_nSSP)     // (R/W) Supervisor Stack Pointer
#define IPR_USP    CPU_IPR(IPR_nUSP)     // (R/W) User Stack Pointer
#define IPR_ISP    CPU_IPR(IPR_nISP)     // (R/W) Interrupt Stack Pointer

#define IPR_P0BR   CPU_IPR(IPR_nP0BR)    // (R/W) P0 Base Register
#define IPR_P0LR   CPU_IPR(IPR_nP0LR)    // (R/W) P0 Length Register
#define IPR_P1BR   CPU_IPR(IPR_nP1BR)    // (R/W) P1 Base Register
#define IPR_P1LR   CPU_IPR(IPR_nP1LR)    // (R/W) P1 Length Register
#define IPR_SBR    CPU_IPR(IPR_nSBR)     // (R/W) System Base Register
#define IPR_SLR    CPU_IPR(IPR_nSLR)     // (R/W) System Limit Register

#define IPR_PCBB   CPU_IPR(IPR_nPCBB)    // (R/W) Process Control Block Base
#define IPR_SCBB   CPU_IPR(IPR_nSCBB)    // (R/W) System Control Block Base
#define IPR_IPL    CPU_IPR(IPR_nIPL)     // (R/W) Interrupt Priority Level
#define IPR_ASTLVL CPU_IPR(IPR_nASTLVL)  // (R/W) AST Level
#define IPR_SIRR   CPU_IPR(IPR_nSIRR)    // (W)   Software Interrupt Request Register
#define IPR_SISR   CPU_IPR(IPR_nSISR)    // (R/W) Software Interrupt Summary Register

#define IPR_ICCS   CPU_IPR(IPR_nICCS)    // (R/W) Internal Clock Control Status
#define IPR_NICR   CPU_IPR(IPR_nNICR)    // (W)   Next Interval Count Register
#define IPR_ICR    CPU_IPR(IPR_nICR)     // (R)   Interval Count Register
#define IPR_TODR   CPU_IPR(IPR_nTODR)    // (R/W) Time of Year Register

#define IPR_RXCS   CPU_IPR(IPR_nRXCS)    // (R/W) Console Receiver Status
#define IPR_RXDB   CPU_IPR(IPR_nRXDB)    // (R)   Console Receiver Data Buffer
#define IPR_TXCS   CPU_IPR(IPR_nTXCS)    // (R/W) Console Transmit Status
#define IPR_TXDB   CPU_IPR(IPR_nTXDB)    // (W)   Console Transmit Data Buffer

#define IPR_MAPEN  CPU_IPR(IPR_nMAPEN)   // (R/W) Map Enable
#define IPR_TBIA   CPU_IPR(IPR_nTBIA)    // (W)   Translation Buffer Invalidate All
#define IPR_TBIS   CPU_IPR(IPR_nTBIS)    // (W)   Trabslation Buffer Invalidate Single

#define IPR_PME    CPU_IPR(IPR_nPME)     // (R/W) Performance Monitor Enable
#define IPR_SID    CPU_IPR(IPR_nSID)     // (R)   System Identification
#define IPR_TBCHK  CPU_IPR(IPR_nTBCHK)   // (W)   Translation Buffer Check

#define IPR_CONPC  CPU_IPR(IPR_nCONPC)   // (R/W) Console Saved PC
#define IPR_CONPSL CPU_IPR(IPR_nCONPSL)  // (R/W) Console Saved PSL
#define IPR_CADR   CPU_IPR(IPR_nCADR)    // (R/W) Cache Disable Register
#define IPR_MSER   CPU_IPR(IPR_nMSER)    // (R/W) Memory System Error Register
