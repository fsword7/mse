/*
 * sysconfig.cpp - system configuration package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/driver.h"
#include "emu/sysconfig.h"

SystemConfig::SystemConfig(const SystemDriver &driver)
: sysDriver(driver)
{

}
