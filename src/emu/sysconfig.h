/*
 * sysconfig.h - system configuration package
 *
 *  Created on: Jul 14, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/driver.h"

class SystemConfig
{
public:
	SystemConfig(const SystemDriver &driver);

private:
	const SystemDriver &sysDriver;
};
