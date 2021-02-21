/*
 * executes.h - PDP10 family processor - execute routine
 *
 *  Created on: Feb 12, 2017
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/cpu/pdp10/kx10.h"
#include "opcode.h"


void pdp10_cpu_base::execute()
{
	int 		irCode;
	int			acReg;
	bool		pcInhibit = false;
	bool        sacInhibit = false;
	bool		indFlag;
	int         ixReg;
	uint32_t    opFlags;
	int         flag1, flag3;

	while(1) {
		// Fetch instruction from current PC address

		// Decode instruction address

		// Fetch operands from Memory
		abReg = pcReg;
		if (readv(0, 0))
			goto page_fault;

		// Decoding instruction code
		irCode  = IW_GETOP(mbReg);
		acReg   = IW_GETAC(mbReg);
//		iaCode  = abReg;
		opFlags = oprFlags[irCode];

		// Decoding recursively direct/indirect address
		do {
			indFlag = IW_GETI(mbReg) != 0;
			ixReg   = IW_GETIX(mbReg);
			abReg   = IW_GETY(mbReg);
			arReg   = mbReg;

			if (ixReg > 0) {
				mbReg = (abReg + curReg[ixReg]) & W10_FMASK;
				abReg = mbReg & H10_MASK;
				arReg = mbReg;
			}

			if (indFlag == true)
				readv(0, 0);

		} while (indFlag);

		// Fetching operands
		if (opFlags & (OPF_FMEM|OPF_FSMEM)) {
			if (readv(0, 0))
				goto page_fault;
			arReg = mbReg;
		}

		if (opFlags & OPF_FAC) {
			brReg = arReg;
			arReg = curReg[acReg];
		}

		if (opFlags & OPF_FAC2) {
			mqReg = curReg[(acReg+1) & AC_MASK];
		}

		if (opFlags & OPF_FBR) {
			brReg = curReg[acReg];
		}

		if (opFlags & OPF_SWAR)
			arReg = ((arReg >> H10_WIDTH) | (arReg << H10_WIDTH)) & W10_FMASK;

		// Execute instruction
		switch (irCode) {


		// UUO - illegal unimplemented opcode
		muuo:
		defualt:

		// MUUO - monitor unimplemented opcode
		case 0000:
		case 0040: case 0041: case 0042: case 0043:
		case 0044: case 0045: case 0046: case 0047:
		case 0050: case 0051: case 0052: case 0053:
		case 0054: case 0055: case 0056: case 0057:
		case 0060: case 0061: case 0062: case 0063:
		case 0064: case 0065: case 0066: case 0067:
		case 0070: case 0071: case 0072: case 0073:
		case 0074: case 0075: case 0076: case 0077:
#if CPU_KI10|CPU_KL10
		case 0100: case 0101: case 0102: case 0103:
		case 0104: case 0105: case 0106: case 0107:
#endif
			break;

		// LUUO - local unimplemented opcode (user mode)
		case 0001: case 0002: case 0003:
		case 0004: case 0005: case 0006: case 0007:
		case 0010: case 0011: case 0012: case 0013:
		case 0014: case 0015: case 0016: case 0017:
		case 0020: case 0021: case 0022: case 0023:
		case 0024: case 0025: case 0026: case 0027:
		case 0030: case 0031: case 0032: case 0033:
		case 0034: case 0035: case 0036: case 0037:
			break;

		case 0200: // MOVE
		case 0201: // MOVEI
		case 0202: // MOVEM
		case 0203: // MOVES
		case 0204: // MOVS
		case 0205: // MOVSI
		case 0206: // MOVSM
		case 0207: // MOVSS
			// Do nothing - just move data
			break;

		case 0214: // MOVM
		case 0215: // MOVMI
		case 0216: // MOVMM
		case 0217: // MOVMS
			if ((arReg & W10_SIGN) == 0)
				break;
			// fall through

		case 0210: // MOVN
		case 0211: // MOVNI
		case 0212: // MOVNM
		case 0213: // MOVNS
			flag1 = flag3 = 0;
			adReg = (arReg ^ W10_FMASK) + 1;

			if ((((arReg & W10_CMASK) ^ W10_CMASK) + 1) & W10_SIGN) {
				pcFlags |= PCF_CR1;
				flag1 = 1;
			}
			if (adReg & W10_C1MASK) {
				pcFlags |= PCF_CR0;
				flag3 = 1;
			}

			if (flag1 != flag3) {
				pcFlags |= PCF_AOV|PCF_TR1;
			}

			if (arReg == W10_SIGN)
				pcFlags |= PCF_TR1;

			arReg = adReg & W10_FMASK;
			break;


		case 0270: // ADD
		case 0271: // ADDI
		case 0272: // ADDM
		case 0273: // ADDB
			flag1 = flag3 = 0;
			if (((arReg & W10_CMASK) + (brReg & W10_CMASK)) & W10_SIGN) {
				pcFlags |= PCF_CR1;
				flag1 = 1;
			}
			arReg = arReg + brReg;
			if (arReg & W10_C1MASK) {
				pcFlags |= PCF_CR0;
				flag3 = 1;
			}
			if (flag1 |= flag3) {
				pcFlags |= PCF_AOV|PCF_TR1;
			}
			break;

		case 0274: // SUB
		case 0275: // SUBI
		case 0276: // SUBM
		case 0277: // SUBB
			flag1 = flag3 = 0;
			if ((((arReg ^ W10_CMASK) & W10_CMASK) + (brReg & W10_CMASK) + 1) & W10_SIGN) {
				pcFlags |= PCF_CR1;
				flag1 = 1;
			}
			arReg = (arReg ^ W10_FMASK) + brReg + 1;
			if (arReg & W10_C1MASK) {
				pcFlags |= PCF_CR0;
				flag3 = 1;
			}
			if (flag1 |= flag3) {
				pcFlags |= PCF_AOV|PCF_TR1;
			}
			break;

		case 0220: // IMUL
		case 0221: // IMULI
		case 0222: // IMULM
		case 0223: // IMULB
		case 0224: // MUL
		case 0225: // MULI
		case 0226: // MULM
		case 0227: // MULB
			flag3 = 0;
			if (arReg & W10_SIGN) {
				arReg = (arReg ^ W10_FMASK) + 1;
				flag3 = 1;
			}
			if (brReg & W10_SIGN) {
				brReg = (brReg ^ W10_FMASK) + 1;
				flag3 = !flag3;
			}

			if ((arReg == 0) && (brReg == 0)) {
				arReg = mqReg = 0;
				break;
			}
			mqReg   = arReg * (brReg & W10_RMASK);
			arReg   = arReg * ((brReg >> H10_WIDTH) & W10_RMASK);
			mqReg  += (arReg << H10_WIDTH) & W10_LMASK;
			arReg >>= H10_WIDTH;
			arReg   = (arReg << 1) + (mqReg >> 35);
			mqReg  &= W10_CMASK;

			if ((irCode & 4) == 0) {
				if (arReg > flag3) {
					pcFlags |= PCF_AOV|PCF_TR1;
				}
				if (flag3) {
					mqReg ^= W10_CMASK;
					mqReg++;
					mqReg |= W10_SIGN;
				}
				arReg = mqReg;
				break;
			} else {
				if ((arReg & W10_SIGN) != 0) {
					pcFlags |= PCF_AOV|PCF_TR1;
				}
				if (flag3) {
					arReg ^= W10_FMASK;
					mqReg ^= W10_CMASK;
					mqReg++;
					if ((mqReg & W10_SIGN) != 0) {
						arReg++;
						mqReg &= W10_CMASK;
					}
					mqReg |= W10_SIGN;
				}
				arReg &= W10_FMASK;
				mqReg = (mqReg & W10_CMASK) | (arReg & W10_SIGN);
			}
			break;

		case 0230: // IDIV
		case 0231: // IDIVI
		case 0232: // IDIVM
		case 0233: // IDIVB
			flag1 = flag3 = 0;
			if (brReg & W10_SIGN) {
				brReg = ((brReg ^ W10_CMASK) + 1) & W10_FMASK;
				flag1 = 1;
			}

			if (brReg == 0) {
				pcFlags |= PCF_AOV|PCF_DCX;
				sacInhibit = true;
				break;
			}

			if ((arReg == W10_SIGN) && (brReg == 1)) {
				pcFlags |= PCF_AOV|PCF_DCX;
				sacInhibit = true;
				break;
			}

			if (arReg & W10_SIGN) {
				arReg = ((arReg ^ W10_CMASK) + 1) & W10_FMASK;
				flag1 = !flag1;
				flag3 = 1;
			}

			mqReg = arReg % brReg;
			arReg = arReg / brReg;
			if (flag1)
				arReg = ((arReg ^ W10_CMASK) + 1) & W10_FMASK;
			if (flag3)
				mqReg = ((mqReg ^ W10_CMASK) + 1) & W10_FMASK;
			break;

		case 0234: // DIV
		case 0235: // DIVI
		case 0236: // DIVM
		case 0237: // DIVB
			break;

		// IOT instructions
//		case 0700: case 0701: case 0702: case 0703: case 0704: case 0705: case 0706: case 0707:
//		case 0710: case 0711: case 0712: case 0713: case 0714: case 0715: case 0716: case 0717:
//		case 0720: case 0721: case 0722: case 0723: case 0724: case 0725: case 0726: case 0727:
//		case 0730: case 0731: case 0732: case 0733: case 0734: case 0735: case 0736: case 0737:
//		case 0740: case 0741: case 0742: case 0743: case 0744: case 0745: case 0746: case 0747:
//		case 0750: case 0751: case 0752: case 0753: case 0754: case 0755: case 0756: case 0757:
//		case 0760: case 0761: case 0762: case 0763: case 0764: case 0765: case 0766: case 0767:
//		case 0770: case 0771: case 0772: case 0773: case 0774: case 0775: case 0776: case 0777:

		}

		// Save operands to Memory

		if (sacInhibit == false) {
			arReg &= W10_FMASK;
			if (opFlags & (OPF_SMEM|OPF_FSMEM)) {
				mbReg = arReg;
				if (writev(0, 0))
					goto page_fault;
			}

			// Save results to AC register
			if ((opFlags & OPF_SAC) || ((opFlags & OPF_SACZ) && acReg > 0))
				curReg[acReg] = arReg;
			if (opFlags & OPF_SAC2)
				curReg[(acReg+1) & AC_MASK] = mqReg;
		}

		// Page fault exception
	page_fault:

		// All done, increment PC register if inhibit is not set
		if (pcInhibit == false)
			pcReg = (pcReg + 1) & H10_MASK;
		pcInhibit = false;
	}
}
