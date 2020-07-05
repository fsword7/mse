/*
 * console.cpp
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#include "emu/core.h"
#include "emu/console.h"

void Console::prompt()
{
	string cmdLine;

	while(true)
	{
		cout << fmt::sprintf("MSE> ") << flush;

		getline(cin, cmdLine);

		// To be removed later...
		if (cmdLine == "exit")
			break;

		cout << fmt::sprintf("Command: %s\n\n", cmdLine) << flush;
	}
}
