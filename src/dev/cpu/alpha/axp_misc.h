/*
 * axp_misc.h
 *
 *  Created on: Jan 25, 2021
 *      Author: Tim Stark
 */

#pragma once

#define DOPC_IMPLVER	RAV = getArchType();
#define DOPC_AMASK		RAV = RBVL & ~getArchFlags();

#define DOPC_RPCC	RAV = (ccOffset << 32) | (ccReg & 0xFFFFFFFF)

#define DOPC_RC_	RAV = bIntrFlag ? 1 : 0; bIntrFlag = false;
#define DOPC_RS		RAV = bIntrFlag ? 1 : 0; bIntrFlag = true;

// Do nothing at this time until multi-processing are supported
#define DOPC_MB
#define DOPC_WMB
#define DOPC_TRAPB
#define DOPC_EXCB
#define DOPC_FETCH
#define DOPC_FETCH_M
#define DOPC_ECB
#define DOPC_WH64
#define DOPC_WH64EN
