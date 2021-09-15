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

    typedef std::function<uint64_t()>     getFunc;
    typedef std::function<void(uint64_t)> setFunc;

    class SymbolEntry
    {
        public:
            SymbolEntry(SymbolTable &table, ctag_t *name, cstag_t &format);
            virtual ~SymbolEntry() = default;

        protected:
            SymbolTable &table;
            cstag_t name;
            cstag_t format;
    };

    class IntegerSymbolEntry : public SymbolEntry
    {
        public:
            IntegerSymbolEntry(SymbolTable &table, ctag_t *name, getFunc getter, setFunc setter, cstag_t &format);
            IntegerSymbolEntry(SymbolTable &table, ctag_t *name, uint64_t cval);

            uint64_t getValue() const { return getter(); }
            void setValue(uint64_t val) { if (setter != nullptr) setter(val); }

        private:
            getFunc  getter = nullptr;
            setFunc  setter = nullptr;
            uint64_t value = 0;
    };

    class SymbolTable
    {
        public:
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