/*
 * symbols.h - debugger - symbols table
 *
 *  Created on: Sep 14, 2021
 *      Author: Tim Stark
 */

#pragma once


namespace emu::debug
{
    class SymbolTable;

    class SymbolEntry
    {
        public:
            SymbolEntry(SymbolTable &table, ctag_t *name);

        private:
            SymbolTable &table;
            ctag_t *name;
    };

    class SymbolTable
    {
        public:
            typedef std::function<uint64_t()>     getFunc;
            typedef std::function<void(uint64_t)> setFunc;

            SymbolTable(const Machine *system, Device *device = nullptr);
            ~SymbolTable() = default;

            void add(ctag_t *name, getFunc getter, setFunc setter = nullptr, cstag_t &format = "");

        private:
            const Machine &system;
            Device *device = nullptr;

            SymbolTable *parent = nullptr;

            std::unordered_map<string, SymbolEntry> symList;
    };
}