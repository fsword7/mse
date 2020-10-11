/*
 * syslist.h - system list handler package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/driver.h"

class SystemList
{
public:
	SystemList() = default;
	~SystemList() = default;

	const SystemDriver *find(const string name);

	void list(ostream &out);

private:
	static const SystemDriver *sysList[];
};
