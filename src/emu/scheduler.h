/*
 *  scheduler.h - device scheduler package
 *
 *  Created on: Sep 22, 2021
 *      Author: Tim Stark
 */

#pragma once

class Machine;

class DeviceScheduler
{
    public:
        DeviceScheduler(Machine &system)
        : system(system)
        {

        }


    private:
        Machine &system;
};