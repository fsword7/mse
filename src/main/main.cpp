/*
 * main.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"
#include "emu/engine.h"

int main(int argc, char **argv)
{
	Console cty;
	SystemEngine engine;

	cout << fmt::sprintf("Multi-System Emulator System\n\n");

	// System initialization
	engine.ginit();

	cty.prompt();

	// System shut down and cleanup
	engine.gexit();
	exit(0);
}
