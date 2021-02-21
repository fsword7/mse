/*
 * models.h
 *
 *  Created on: Apr 22, 2017
 *      Author: fswor
 */

#pragma once


// Processor type for MFPT instruction
#define MFPT_1144  1  // PDP-11/44 Processor
#define MFPT_F11   3  // F11 Processor
#define MFPT_T11   4  // T11 Processor
#define MFPT_J11   5  // J11 Processor

// Individual model-specific processor option flags
//#define CPUF_1103		(OPT_MxPS|OPT_SXS|OPT_MARK|OPT_RTT|OPT_JREG4)
#define CPUF_1104		(OPT_RTT|OPT_JREG4|OPT_CHGTBIT|OPT_IOPSW|OPT_STKLF| \
						 OPT_ODD)
#define CPUF_1105		(OPT_JREG4|OPT_CHGTBIT|OPT_IOPSW|OPT_STKLF|OPT_ODD)
#define CPUF_1120		(OPT_SDSD|OPT_JREG4|OPT_CHGTBIT|OPT_IOPSW|OPT_STKLF| \
						 OPT_ODD)
#define CPUF_1134		(OPT_MxPS|OPT_MxPy|OPT_SXS|OPT_MARK|OPT_RTT| \
						 OPT_JREG4|OPT_IOPSW|OPT_STKLF|OPT_ODD)
#define CPUF_1140		(OPT_SDSD|OPT_MxPy|OPT_SXS|OPT_MARK|OPT_RTT| \
						 OPT_JREG4|OPT_IOPSW|OPT_STKLF|OPT_ODD)
#define CPUF_1144		(OPT_MFPT|OPT_MxPS|OPT_MxPy|OPT_SXS|OPT_MARK| \
						 OPT_RTT|OPT_HALT4|OPT_IOPSW|OPT_STKLF|OPT_ODD| \
						 OPT_SPL|OPT_CSM)
#define CPUF_1145		(OPT_MxPy|OPT_SXS|OPT_MARK|OPT_RTT|OPT_HALT4| \
						 OPT_IOPSW|OPT_STKLR|OPT_ODD|OPT_SPL)
#define CPUF_1155		(0)
#define CPUF_1160		(OPT_SDSD|OPT_MxPy|OPT_SXS|OPT_MARK|OPT_RTT| \
						 OPT_IOPSW|OPT_STKLR|OPT_ODD)
#define CPUF_1170		(OPT_MxPy|OPT_SXS|OPT_MARK|OPT_RTT|OPT_HALT4| \
						 OPT_IOPSW|OPT_STKLR|OPT_ODD|OPT_SPL)

// Processor-specific option flags
#define CPUF_LSI11		(OPT_MxPS|OPT_SXS|OPT_MARK|OPT_RTT|OPT_JREG4)
#define CPUF_T11		(OPT_SDSD|OPT_MFPT|OPT_MxPS|OPT_SXS|OPT_RTT|OPT_JREG4)
#define CPUF_F11		(OPT_SDSD|OPT_MFPT|OPT_MxPS|OPT_MxPy|OPT_SXS| \
						 OPT_MARK|OPT_RTT|OPT_JREG4|OPT_IOPSW|OPT_STKLF)
#define CPUF_J11		(OPT_SDSD|OPT_MFPT|OPT_MxPS|OPT_MxPy|OPT_SXS| \
						 OPT_MARK|OPT_RTT|OPT_HALT4|OPT_IOPSW|OPT_STKLF| \
						 OPT_ODD|OPT_SPL|OPT_CSM|OPT_TSWLK)

// Model-specific LSI11 processor option flags
#define CPUF_1103		(CPUF_LSI11)

// Model-specific F11 processor option flags
#define CPUF_1123		(CPUF_F11)
#define CPUF_1123P		(CPUF_F11)
#define CPUF_1124		(CPUF_F11)

// Model-specific J11 processor option flags
#define CPUF_1153		(CPUF_J11)
#define CPUF_1173A		(CPUF_J11)
#define CPUF_1173B		(CPUF_J11)
#define CPUF_1183		(CPUF_J11)
#define CPUF_1184		(CPUF_J11)
#define CPUF_1193		(CPUF_J11)
#define CPUF_1194		(CPUF_J11)


// Model-specific system option flags
#define OPT_1103		(OPT_EIS|OPT_FIS)
#define OPT_1104		(0)
#define OPT_1105		(0)
#define OPT_1120		(0)
#define OPT_1123		(OPT_FPP|OPT_CIS)
#define OPT_1123P		(OPT_FPP|OPT_CIS)
#define OPT_1124		(OPT_FPP|OPT_CIS)
#define OPT_1134		(OPT_FPP)
#define OPT_1140		(OPT_FIS)
#define OPT_1144		(OPT_FPP|OPT_CIS)
#define OPT_1145		(OPT_FPP)
#define OPT_1153		(OPT_CIS)
#define OPT_1160		(0)
#define OPT_1170		(OPT_FPP|OPT_RH11)
#define OPT_1173A		(OPT_CIS)
#define OPT_1173B		(OPT_CIS)
#define OPT_1183		(OPT_CIS)
#define OPT_1184		(OPT_CIS)
#define OPT_1193		(OPT_CIS)
#define OPT_1194		(OPT_CIS)


#define PSW_F11   0170777
#define	MM0_F11   0160157
#define	MM3_F11   0000060
#define PAR_F11   0177777
#define PDR_F11   0077516

#define PSW_J11   0174777
#define MM0_J11   0160177
#define MM3_J11   0000077
#define PAR_J11   0177777
#define PDR_J11   0177516

#define PSW_1103  0000377
#define PSW_1104  0000377
#define PSW_1105  0000377
#define PSW_1120  0000377

#define PSW_1134  0170377
#define MM0_1134  0160557
#define PAR_1134  0007777
#define PDR_1134  0077516

#define PSW_1140  0170377
#define MM0_1140  0160557
#define PAR_1140  0007777
#define PDR_1140  0077516

#define PSW_1144  0170777
#define MM0_1144  0160557
#define MM3_1144  0000077
#define PAR_1144  0177777
#define PDR_1144  0077516

#define PSW_1145  0174377
#define MM0_1145  0171777
#define MM3_1145  0000007
#define PAR_1145  0007777
#define PDR_1145  0077717

#define PSW_1160  0170377
#define MM0_1160  0160557
#define PAR_1160  0007777
#define PDR_1160  0077516

#define PSW_1170  0174377
#define MM0_1170  0171777
#define MM3_1170  0000067
#define PAR_1170  0177777
#define PDR_1170  0077717

#if 0
// Model Type (exclusively bit selection)
#define CPU_94    0x00100000 // PDP-11/94  J11E UBA
#define CPU_93    0x00080000 // PDP-11/93  J11E QBA
#define CPU_84    0x00040000 // PDP-11/84  J11B UBA
#define CPU_83    0x00020000 // PDP-11/83  J11B QBA
#define CPU_73B   0x00010000 // PDP-11/73B J11B
#define CPU_73A   0x00008000 // PDP-11/73A J11A
#define CPU_70    0x00004000 // PDP-11/70
#define CPU_60    0x00002000 // PDP-11/60
#define CPU_55    0x00001000 // PDP-11/55
#define CPU_53    0x00000800 // PDP-11/53  J11D
#define CPU_45    0x00000400 // PDP-11/45
#define CPU_44    0x00000200 // PDP-11/44
#define CPU_40    0x00000100 // PDP-11/40
#define CPU_34    0x00000080 // PDP-11/34
#define CPU_24    0x00000040 // PDP-11/24  F11 UBA
#define CPU_23P   0x00000020 // PDP-11/23P F11 QBA
#define CPU_23    0x00000010 // PDP-11/23  F11 QBA
#define CPU_20    0x00000008 // PDP-11/20
#define CPU_05    0x00000004 // PDP-11/05
#define CPU_04    0x00000002 // PDP-11/04
#define CPU_03    0x00000001 // PDP-11/03 or LSI-11

#define CPU_LSI11 (CPU_03)
#define CPU_T11   0x00800000 // T-11 Processor
#define CPU_F11   (CPU_23|CPU_23P|CPU_24)
#define CPU_J11   (CPU_53|CPU_73A|CPU_73B|CPU_83|CPU_84|CPU_93|CPU_94)
#define CPU_J11A  (CPU_73A)
#define CPU_J11B  (CPU_73B|CPU_83|CPU_84)
#define CPU_J11E  (CPU_93|CPU_94)
#define CPU_J11U  (CPU_84|CPU_94) // KTJ11B UBA
#define CPU_ALL   0x00FFFFFF

// Specific-Model CPU Features
#define HAS_IOPSW (CPU_04|CPU_05|CPU_20|CPU_34|CPU_40|CPU_44| \
                   CPU_45|CPU_60|CPU_70|CPU_F11|CPU_J11)
#define CHG_TBIT  (CPU_04|CPU_05|CPU_20)
#define HAS_STKLF (CPU_04|CPU_05|CPU_20|CPU_34|CPU_40|CPU_44| \
                   CPU_F11|CPU_J11)
#define HAS_STKLR (CPU_45|CPU_60|CPU_70)
#define HAS_ODD   (CPU_04|CPU_05|CPU_20|CPU_34|CPU_40|CPU_44| \
                   CPU_45|CPU_60|CPU_70|CPU_J11)
//#define OPT_M9312 (CPU_04|CPU_05|CPU_20|CPU_34|CPU_40|CPU_45| \
//                   CPU_55|CPU_60|CPU_70)

// Specific-Model Instructions
#define IS_SDSD   (CPU_20|CPU_40|CPU_60|CPU_T11|CPU_F11|CPU_J11)
#define HAS_MFPT  (CPU_44|CPU_T11|CPU_F11|CPU_J11)
#define HAS_MxPS  (CPU_03|CPU_34|CPU_44|CPU_T11|CPU_F11|CPU_J11)
#define HAS_MxPy  (CPU_34|CPU_40|CPU_44|CPU_45|CPU_60|CPU_70| \
                   CPU_F11|CPU_J11)
#define HAS_SXS   (CPU_03|CPU_34|CPU_40|CPU_44|CPU_45|CPU_60|CPU_70| \
                   CPU_T11|CPU_F11|CPU_J11)
#define HAS_MARK  (CPU_03|CPU_34|CPU_40|CPU_44|CPU_45|CPU_60|CPU_70| \
                   CPU_F11|CPU_J11)
#define HAS_RTT   (CPU_03|CPU_04|CPU_34|CPU_40|CPU_44|CPU_45|CPU_60| \
                   CPU_70|CPU_T11|CPU_F11|CPU_J11)
#define HAS_HALT4 (CPU_44|CPU_45|CPU_70|CPU_J11)
#define HAS_JREG4 (CPU_03|CPU_04|CPU_05|CPU_20|CPU_34|CPU_40| \
		   CPU_T11|CPU_F11)
#endif
