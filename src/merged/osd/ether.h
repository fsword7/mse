/*
 * ether.h
 *
 *  Created on: Mar 18, 2017
 *      Author: Timothy Stark
 *
 *  Virtual Ethernet interface
 *
 */

#pragma once

#if defined(HAVE_PCAP)
#define USE_BPF 1
#include <pcap.h>
#endif
