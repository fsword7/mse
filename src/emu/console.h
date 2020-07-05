/*
 * console.h
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/command.h"

class Console
{
public:
	Console() = default;
	~Console() = default;

	void prompt();

};
