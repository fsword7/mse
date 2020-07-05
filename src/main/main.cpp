/*
 * main.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"

int main(int argc, char **argv)
{
	Console cty;

	cout << fmt::sprintf("Multi-System Emulator System\n\n");

	cty.prompt();

	exit(0);
}
