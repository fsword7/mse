/*
 * console.cpp
 *
 *  Created on: May 7, 2018
 *      Author: Timothy Stark
 */

#include "emu/emucore.h"
#include "dev/comm/dec/cty.h"
#include "dev/cpu/vax/mtpr.h"

using namespace dec;

ctyDevice::ctyDevice()
{
}

ctyDevice::~ctyDevice()
{
}

//ctyDevice *ctyDevice::create(sysDevice *sdev, std::string devName)
//{
//	ctyDevice *dev = new ctyDevice();
//
//	if (dev == nullptr)
//		return nullptr;
//
//	dev->devName = devName;
//	dev->devType = "CTY";
////	dev->devDesc = model->desc;
////	dev->driver  = model->driver;
//
//	// Assign system device for I/O access
//	dev->setSystemDevice(sdev);
//
//	// Initialize console device
////	dev->reset();
//
//	// Add CPU device to system device
////	sdev->addConsoleDevice(dev);
//
//	return dev;
//}

uint16_t ctyDevice::read16(uint32_t ioAddr)
{
	switch (ioAddr) {
	case IPR_nRXCS:
	case IPR_nRXDB:
	case IPR_nTXCS:
	case IPR_nTXDB:
		break;
	}

	// Return as default zero
	return 0;
}

void ctyDevice::write16(uint32_t ioAddr, uint16_t data)
{
	switch (ioAddr) {
	case IPR_nRXCS:
	case IPR_nRXDB:
	case IPR_nTXCS:
	case IPR_nTXDB:
		break;
	}
}
