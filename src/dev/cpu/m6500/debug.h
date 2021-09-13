/*
 * debug.h - MOS 65xx processor - debugging/tracing
 *
 *  Created on: Sep 6, 2021
 *      Author: Tim Stark
 */

#define ONTRACE if(dbg.checkAnyFlags(DBG_TRACE))

#define PRE_IMPL(opcode)                            \
    {                                               \
        dbg.log("%04X  %-3s\n", pcReg.uw, opcode);  \
    }

#define POST_IMPL



#define OP_EXEC(opcode, format)     \
    ONTRACE PRE_##format(opcode);   \
    DO_##opcode;                    \
    POST_##format;

#define OP_FUNC(opcode, call, format)   \
    ONTRACE PRE_##format(opcode);       \
    call();                             \
    POST_##format;
