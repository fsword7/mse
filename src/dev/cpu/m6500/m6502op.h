/*
 * m6500.h - MOS 65xx processor - opcodes
 *
 *  Created on: Sep 3, 2021
 *      Author: Tim Stark
 */

#pragma once


#define DOPC_ADC
#define DOPC_AND
#define DOPC_ASL
#define DOPC_BIT

// Branch instructions
#define DOPC_BPL
#define DOPC_BMI
#define DOPC_BVC
#define DOPC_BVS
#define DOPC_BCC
#define DOPC_BNE
#define DOPC_BEQ

#define DOPC_CLC    pReg &= ~PSW_C;
#define DOPC_CLD    pReg &= ~PSW_D;
#define DOPC_CLI    pReg &= ~PSW_I;
#define DOPC_CLV    pReg &= ~PSW_V;
#define DOPC_CMP
#define DOPC_CPX
#define DOPC_CPY
#define DOPC_DEC
#define DOPC_DEX
#define DOPC_DEY
#define DOPC_EOR
#define DOPC_INC
#define DOPC_INX
#define DOPC_INY
#define DOPC_JMP
#define DOPC_JSR
#define DOPC_LDA
#define DOPC_LDX
#define DOPC_LDY
#define DOPC_LSR
#define DOPC_NOP
#define DOPC_ORA
#define DOPC_PHA
#define DOPC_PHP
#define DOPC_PLA
#define DOPC_PLP
#define DOPC_ROL
#define DOPC_ROR
#define DOPC_RTI
#define DOPC_RTS
#define DOPC_SBC
#define DOPC_SEC    pReg |= PSW_C;
#define DOPC_SED    pReg |= PSW_D;
#define DOPC_SEI    pReg |= PSW_I;
#define DOPC_STA
#define DOPC_STX
#define DOPC_STY
#define DOPC_TAX    xReg = aReg; setNZ(xReg);
#define DOPC_TAY    yReg = aReg; setNZ(yReg);
#define DOPC_TSX    xReg = spReg.ub.l;
#define DOPC_TXA    aReg = xReg; setNZ(aReg);
#define DOPC_TXS    spReg.ub.l = xReg;
#define DOPC_TYA    aReg = yReg; setNZ(aReg);


#define DO_OPC(opCode, opType)
