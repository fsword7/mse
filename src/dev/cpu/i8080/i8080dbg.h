/*
 * i8080dbg.h - MOS 65xx processor - debugging/tracing
 *
 *  Created on: Sep 6, 2021
 *      Author: Tim Stark
 */

static ctag_t *regList8[]   = { "B", "C", "D", "E", "H", "L", "M", "A" };
static ctag_t *regList16s[] = { "B", "D", "H", "SP" };
static ctag_t *regList16p[] = { "B", "D", "H", "PSW" };

#define ONTRACE if(dbg.checkAnyFlags(DBG_TRACE))

#define POST_X8(val)    ONTRACE dbg.log(" ==> %02X\n", val)
#define POST_X16(val)   ONTRACE dbg.log(" ==> %04X\n", val)

#define PRE_IMPL(name)                              \
    {                                               \
        dbg.log("%04X  %-4s\n", pcReg.uw, #name);   \
    }

#define POST_IMPL

#define PRE_REG(name)                                   \
    {                                                   \
        dbg.log("%04X  %-4s  %s\n", pcReg.uw, #name,    \
            regList8[REGAn(opCode)]);                   \
    }

#define POST_REG POST_X8(REG_A)
//#define POST_REG ONTRACE dbg.log("[ %02X ] ==> %02X\n", REGAn(opCode), REG_A);

#define PRE_REGM(name)                                  \
    {                                                   \
        dbg.log("%04X  %-4s  M\n", pcReg.uw, #name);    \
    }

#define POST_REGM
//#define POST_REG ONTRACE dbg.log("[ %02X ] ==> %02X\n", REGAn(opCode), REG_A);

#define PRE_IMM8(name)                                          \
    {                                                           \
        dbg.log("%04X  %-4s  $%02X\n", pcReg.uw, #name, tval);  \
    }

#define POST_IMM8
//#define POST_REG ONTRACE dbg.log("[ %02X ] ==> %02X\n", REGAn(opCode), REG_A);

#define PRE_REGI8(name)                                     \
    {                                                       \
        dbg.log("%04X  %-4s  %s,$%02X\n", pcReg.uw, #name,  \
            regList8[REGBn(opCode)], tval);                 \
    }

#define POST_REGI8
//#define POST_REG ONTRACE dbg.log("[ %02X ] ==> %02X\n", REGAn(opCode), REG_A);

#define PRE_REGMI8(name)                                            \
    {                                                               \
        dbg.log("%04X  %-4s  M,$%02X\n", pcReg.uw, #name, tval);    \
    }

#define POST_REGMI8
//#define POST_REG ONTRACE dbg.log("[ %02X ] ==> %02X\n", REGAn(opCode), REG_A);

#define PRE_ADR(name)                                           \
    {                                                           \
        dbg.log("%04X  %-4s  %04X\n", pcReg.uw, #name, tval);   \
    }

#define POST_ADR


#define PRE_REGWSP(name)                                \
    {                                                   \
        dbg.log("%04X  %-4s  %s\n", pcReg.uw, #name,    \
            regList16s[REGWn(opCode)]);                 \
    }

#define POST_REGWSP POST_X16(REGW(opCode))

#define PRE_REGWSPI(name)                                   \
    {                                                       \
        dbg.log("%04X  %-4s  %s,$%04X\n", pcReg.uw, #name,  \
            regList16s[REGWn(opCode)], tval);               \
    }

#define POST_REGWSPI POST_X16(REGW(opCode))


#define PRE_MOVE(name)                                          \
    {                                                           \
        dbg.log("%04X  %-4s  %s,%s", pcReg.uw, #name,           \
            regList8[REGBn(opCode)], regList8[REGAn(opCode)]);  \
    }

#define POST_MOVE POST_X8(REGB(opCode))

#define PRE_MOVErM(name)                                \
    {                                                   \
        dbg.log("%04X  %-4s  %s,M", pcReg.uw, #name,    \
            regList8[REGBn(opCode)]);                   \
    }

#define POST_MOVErM ONTRACE dbg.log(" (%04X) ==> %02X\n", REG_HL, REGB(opCode))

#define PRE_MOVEMr(name)                                \
    {                                                   \
        dbg.log("%04X  %-4s  M,%s", pcReg.uw, #name,    \
            regList8[REGAn(opCode)]);                   \
    }

#define POST_MOVEMr ONTRACE dbg.log(" (%04X) <== %02X\n", REG_HL, REGA(opCode))


#define PRE_RESET(name)                             \
    {                                               \
        dbg.log("%04X  %-4s  %d", pcReg.uw, #name,  \
            REGBn(opCode));                         \
    }

#define POST_RESET POST_X16(0)

#define OP_EXEC(name, opcode, format)    \
    ONTRACE PRE_##format(name);          \
    DO_##opcode;                         \
    POST_##format;

#define OP_FUNC(opcode, call, format)   \
    ONTRACE PRE_##format(opcode);       \
    call();                             \
    POST_##format;
