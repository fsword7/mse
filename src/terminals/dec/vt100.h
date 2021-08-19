/*
 * vt100.h - DEC VT100 terminal
 *
 *  Created on: Aug 19, 2021
 *      Author: Tim Stark
 */

#pragma once

class vt100_vtDevice : public sysDevice
{
    public:
        vt100_vtDevice(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, uint64_t clock)
        : sysDevice(config, type, tagName, clock)
        {

        }
        ~vt100_vtDevice() = default;

        void vt100(SystemConfig &config);
        
        void vt100_init();

        void vt100_mem(aspace::AddressList &map);
        void vt100_iomem(aspace::AddressList &map);

    private:
        i8080_cpuDevice *cpu = nullptr;
};
