/*
 * i8080op.h - Intel 8080/8085 processor - opcodes
 *
 *  Created on: Sep 6, 2021
 *      Author: Tim Stark
 */


#define DO_NOP      // Do nothing

#define DO_LDAX     REG_A = readMem(REGW(opCode));
#define DO_LDA      REG_A = readMem(tval);
#define DO_LHLD     REG_HL = readMem(tval) | (readMem(tval+1) << 8);
#define DO_LXI      REGW(opCode) = tval;

#define DO_STAX     writeMem(REGW(opCode), REG_A);
#define DO_STA      writeMem(tval, REG_A);
#define DO_SHLD     writeMem(tval, REG_HL); writeMem(tval+1, REG_HL >> 8);

#define DO_MOV      REGB(opCode) = REGA(opCode);
#define DO_MOVMr    writeMem(REG_HL, REGA(opCode));
#define DO_MOVrM    REGB(opCode) = readMem(REG_HL);
#define DO_MVI      REGB(opCode) = tval;
#define DO_MVIM     writeMem(REG_HL, tval);

#define DO_INR      REGB(opCode) = opINR(REGB(opCode));
#define DO_DCR      REGB(opCode) = opDCR(REGB(opCode));
#define DO_INRM     writeMem(REG_HL, opINR(readMem(REG_HL)));
#define DO_DCRM     writeMem(REG_HL, opDCR(readMem(REG_HL)));
#define DO_INX      REGW(opCode)++;
#define DO_DCX      REGW(opCode)--;

            // if (is8085())
            // {
            //     afReg.sb.l &= ~SR_VF;
            //     if (bcReg.sw == 0x0000)
            //         afReg.ub.l |= SR_VF;
            // }

#define DO_RLC                                          \
            REG_A = (REG_A << 1) | (REG_A >> 7);        \
            REG_F = (REG_F & ~SR_CF) | (REG_A & SR_CF);

#define DO_RRC                                          \
            REG_F = (REG_F & ~SR_CF) | (REG_A & SR_CF); \
            REG_A = (REG_A >> 1) | (REG_A << 7);

#define DO_RAL                                                  \
            cFlag = REG_A & SR_CF;                              \
            REG_F = (REG_F & ~SR_CF) | ((REG_A >> 7) & SR_CF);  \
            REG_A = (REG_A << 1) | cFlag;

#define DO_RAR                                          \
            cFlag = (REG_A & SR_CF) << 7;               \
            REG_F = (REG_F & ~SR_CF) | (REG_A & SR_CF); \
            REG_A = (REG_A >> 1) | cFlag;

#define DO_DAD      opDAD(REGW(opCode));

#define DO_ADD      opADD(REGA(opCode));
#define DO_ADC      opADC(REGA(opCode));
#define DO_SUB      opSUB(REGA(opCode));
#define DO_SBB      opSBB(REGA(opCode));
#define DO_ANA      opANA(REGA(opCode));
#define DO_XRA      opXRA(REGA(opCode));
#define DO_ORA      opORA(REGA(opCode));
#define DO_CMP      opCMP(REGA(opCode));

#define DO_ADDM     opADD(readMem(REG_HL));
#define DO_ADCM     opADC(readMem(REG_HL));
#define DO_SUBM     opSUB(readMem(REG_HL));
#define DO_SBBM     opSBB(readMem(REG_HL));
#define DO_ANAM     opANA(readMem(REG_HL));
#define DO_XRAM     opXRA(readMem(REG_HL));
#define DO_ORAM     opORA(readMem(REG_HL));
#define DO_CMPM     opCMP(readMem(REG_HL));

#define DO_ADI      opADD(tval);
#define DO_ACI      opADC(tval);
#define DO_SUI      opSUB(tval);
#define DO_SBI      opSBB(tval);
#define DO_ANI      opANA(tval);
#define DO_XRI      opXRA(tval);
#define DO_ORI      opORA(tval);
#define DO_CPI      opCMP(tval);


#define DO_STC      REG_F |= SR_CF;
#define DO_CMC      REG_F ^= SR_CF;
#define DO_CMA      REG_A ^= 0xFF;

            // if (is8085())
            //     afReg.ub.l |= SR_HF|SR_VF;

#define DO_RST      opRST(REGBn(opCode));
#define DO_HLT
#define DO_EI       enableInterrupts(true);
#define DO_DI       enableInterrupts(false);