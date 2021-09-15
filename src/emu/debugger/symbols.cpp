/*
 * symbols.cpp - debugger package - symbol tables
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/debugger/symbols.h"

using namespace emu::debug;

SymbolEntry::SymbolEntry(SymbolTable &table, ctag_t *name, cstag_t &format)
: table(table), name(name), format(format)
{

}

IntegerSymbolEntry::IntegerSymbolEntry(SymbolTable &table, ctag_t *name,
    getFunc getter, setFunc setter, cstag_t &format)
: SymbolEntry(table, name, format),
  getter(std::move(getter)),
  setter(std::move(setter)),
  value(0)
{
}

IntegerSymbolEntry::IntegerSymbolEntry(SymbolTable &table, ctag_t *name, uint64_t cval)
: SymbolEntry(table, name, ""),
  getter([this] () { return value; }),
  setter(nullptr),
  value(cval)
{
}

SymbolTable::SymbolTable(const Machine *system, Device *device)
: system(*system), device(device)
{

}

void SymbolTable::add(ctag_t *name, getFunc getter, setFunc setter, cstag_t &format)
{
    // Replace that symbol entry with new one.
    symList.erase(name);
    symList.emplace(name, IntegerSymbolEntry(*this, name, getter, setter, format));
}