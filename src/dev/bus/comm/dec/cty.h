/*
 * cty.h
 *
 *  Created on: May 4, 2018
 *      Author: fswor
 */

#pragma once

// RXCS - Console Receive Control and Status Register
#define RXCS_ACT	0x0800	// (R)   Receive Active
#define RXCS_RDY	0x0080	// (R)   Ready/Done
#define RXCS_IE		0x0040	// (R/W) Interrupt Enable

#define RXCS_RW		(RXCS_IE)

// RXDB - Console Receive Data Buffer
#define RXDB_ERR	0x8000	// (R)   Error
#define RXDB_OVR	0x4000	// (R)   Overrun
#define RXDB_FRM	0x2000	// (R)   Frame Error
#define RXDB_BRK	0x1000	// (R)   Receive Break
#define RXDB_CHAR	0x00FF	// (R)   Data Buffer

// TXCS - Console Transmit Control and Status Register
#define TXCS_RDY	0x0080	// (R)   Ready
#define TXCS_IE		0x0040	// (R/W) Interrupt Enable
#define TXCS_MAINT	0x0004	// (R/W) Maintenance Mode
#define TXCS_BRK	0x0001	// (R/W) Transmit Break

#define TXCS_RW    (TXCS_IE|TXCS_MAINT|TXCS_BRK)

// TXDB - Console Transmit Data Buffer
#define TXDB_CHAR	0x00FF	// (W)   Data Buffer
#define TXDB_SEL	0x0F00	// (W)   Mode Selection


namespace dec {
	class ctyDevice /* : public device_t */
	{
	public:
		ctyDevice();
		~ctyDevice();

//		static ctyDevice *create(sysDevice *sdev, std::string devName);

		uint16_t read16(uint32_t ioAddr);
		void     write16(uint32_t ioAddr, uint16_t data);

	};
}
