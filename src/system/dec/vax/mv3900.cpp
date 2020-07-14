/*
 * mv3900.cpp - MicroVAX 3900 system driver package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/sysconfig.h"
#include "emu/device.h"
#include "emu/driver.h"


COMP(mv3900,  nullptr, vax, KA650, ka650_device, mv3900,  mv3900_init,  "DEC", "MicroVAX 3900")
COMP(mv3900x, nullptr, vax, KA650, ka650_device, mv3900x, mv3900x_init, "DEC", "MicroVAX 3900x")
