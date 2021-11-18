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
    : sysDevice(config, type, tagName, clock),
      ramData(*this, "ram"),
      cpu(*this, "i8080"),
      crt(*this, "VT100_Video")
    {

    }

    ~vt100_vtDevice() = default;
    
    void vt100(SystemConfig &config);
    
    void vt100_init();

    void vt100_mem(aspace::AddressList &map);
    void vt100_iomem(aspace::AddressList &map);

    uint32_t vt100_updateScreen(ScreenDevice &screen, bitmap16_t &bitmap, const rect_t &clip);
    uint8_t readData(offs_t addr);
    
private:
    RequiredDevice<i8080_cpuDevice> cpu;
    RequiredDevice<vt100video_t> crt;

    RequiredSharedPointer<uint8_t> ramData;
};
