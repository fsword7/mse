/*
 * devcall.h - device callback delegate package
 *
 *  Created on: Oct 20, 2021
 *      Author: Tim Stark
 */

#pragma once

class DeviceCallbackBase
{
public:
    DeviceCallbackBase(Device &owner) : owner(owner) {}
    virtual ~DeviceCallbackBase();

private:
    Device &owner;
};

class DeviceCallbackReadBase : public DeviceCallbackBase
{

};

class DeviceCallbackWriteBase : public DeviceCallbackBase
{

};