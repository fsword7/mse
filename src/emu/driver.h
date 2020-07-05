/*
 * driver.h - system driver package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

struct SystemDriver
{
	const char   *name;
	const char   *parent;
	const char   *description;
};
