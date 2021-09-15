/*
 * symbols.cpp - debugger package - symbol tables
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/debugger/symbols.h"

using namespace emu::debug;

SymbolEntry::SymbolEntry(SymbolTable &table, ctag_t *name)
: table(table), name(name)
{

}

SymbolTable::SymbolTable(const Machine *system, Device *device)
: system(*system), device(device)
{

}

void SymbolTable::add(ctag_t *name, getFunc getter, setFunc setter, cstag_t &format)
{

}