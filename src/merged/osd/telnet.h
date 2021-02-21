/*
 * telnet.h
 *
 *  Created on: May 7, 2018
 *      Author: Timothy Stark
 */

#pragma once

#define TLN_BINARY              0 // Binary Transmission
#define TLN_IS                  0 // Used by Terminal-Type Negotiation
#define TLN_SEND                1 // Used by Terminal-Type Negotiation
#define TLN_ECHO_OPTION         1 // Echo Option
#define TLN_ECHO                1 // Echo Option
#define TLN_SUPPRESS_GA         3 // Suppress Go-Ahead Option
#define TLN_SGA                 3 // Suppress Go-Ahead Option
#define TLN_TIMING_MARK         6 // Timing Mark
#define TLN_TERM_TYPE          24 // Terminal Type Option
#define TLN_EOR                25 // End of Record Marker
#define TLN_LINEMODE           34 // Line Mode
#define TLN_EOR_MARK          239 // End of Record Marker
#define TLN_SE                240 // End of Subnegotiation Parameters
#define TLN_NOP               241 // No Operation
#define TLN_DATA_MARK         242
#define TLN_BRK               243 // Break Character
#define TLN_IP                244 // Interrupt Process
#define TLN_AO                245 // Abort Output
#define TLN_AYT               246 // Are You There?
#define TLN_EC                247 // Erase Character
#define TLN_EL                248 // Erase Line
#define TLN_GA                249 // Go Ahead
#define TLN_SB                250 // Beginning of Subnegotiation
#define TLN_WILL              251 //
#define TLN_WONT              252 //
#define TLN_DO                253 //
#define TLN_DONT              254 //
#define TLN_IAC               255 // Interpret as Command
