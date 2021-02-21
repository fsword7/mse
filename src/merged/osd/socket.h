/*
 * socket.h
 *
 *  Created on: Feb 19, 2017
 *      Author: Timothy Stark
 */

#pragma once

#if __WIN32__
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif
