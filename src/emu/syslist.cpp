/*
 * syslist.cpp - system list package (handler)
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/syslist.h"

void SystemList::list(ostream &out)
{
	const SystemDriver *model;

	out << fmt::sprintf("Name       Description\n");
	out << fmt::sprintf("---------- -----------\n");
	for (int idx = 0; sysList[idx] != nullptr; idx++)
	{
		model = sysList[idx];
		out << fmt::sprintf("%-10s %s\n", model->name, model->description);
	}
	out << flush;
}
