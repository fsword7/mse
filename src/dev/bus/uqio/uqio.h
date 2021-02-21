/*
 * uqio.h - UNIBUS/Q-bus device I/O interface
 *
 *  Created on: Jan 6, 2019
 *      Author: Tim Stark
 */

#pragma once

#define UQ_UNIT(reg) uqReg[reg]

#define UQID_UNIBUS  0 // Unibus Type (Old)
#define UQID_Q22BUS  1 // Q22 Bus Type (New)

#define UQID_SYSTEM   0 // System I/O devices
#define UQID_STANDARD 1 // Standard I/O devices
#define UQID_EXTERNAL 2 // External I/O devices

// Map I/O Flags
#define UQ_PIRQ   0x00000001 // Pending IRQ interrupt

#define UQ_NIPLS  8   // Up to 8 Interrupt Priority Levels
#define UQ_NVECS  32  // Up to 32 Interrupt Vectors

// Result Codes
#define UQ_OK      TS2_OK
#define UQ_ERROR   TS2_ERROR
#define UQ_NXM     128
#define UQ_RSVDSPC 129
#define UQ_RSVDVEC 130
#define UQ_UNSUP   131

// Interrupt Priority Levels
#define UIPL_HMAX   UQ_BR7
#define UIPL_HMIN   UQ_BR4

#define UQ_BR7   7
#define UQ_BR6   6
#define UQ_BR5   5
#define UQ_BR4   4

// I/O Device Space (in 16 bits) (Obslete)
#define UQIO_WIDTH   12
#define UQIO_SIZE    (1u << UQIO_WIDTH)
#define UQIO_MASK    ((UQIO_SIZE - 1) << 1)
#define UQIO_BMASK   (UQIO_MASK | 1)

#define UQIO_GETSPACE(uq, ioAddr) \
	UQ_UNIT(uq, ioSpace[(ioAddr &= UQIO_BMASK) >> 1])

// I/O Device Space (13-bit addressing in bytes)
#define IOP_WIDTH      13
#define IOP_SIZE       (1u << IOP_WIDTH)
#define IOP_MASK       (IOP_SIZE - 1)
#define IOP_BASE(size) ((size) - IOP_SIZE)

#define IOP_GETSPACE(uq, ioAddr) \
	UQ_UNIT(uq, ioSpace[(ioAddr & IOP_MASK) >> 1])

// I/O Memory Space (16-bit addressing in bytes)
#define UQ16_WIDTH    16
#define UQ16_SIZE     (1u << UQ16_WIDTH)
#define UQ16_MASK     (UQ16_SIZE - 1)
#define UQ16_IOPAGE   IOP_BASE(UQ16_SIZE)

// I/O Memory Space (18-bit addressing in bytes)
#define UQ18_WIDTH    18
#define UQ18_SIZE     (1u << UQ18_WIDTH)
#define UQ18_MASK     (UQ18_SIZE - 1)
#define UQ18_IOPAGE   IOP_BASE(UQ18_SIZE)

// I/O Memory Space (22-bit addressing in bytes)
#define UQ22_WIDTH    22
#define UQ22_SIZE     (1u << UQ22_WIDTH)
#define UQ22_MASK     (UQ22_SIZE - 1)
#define UQ22_IOPAGE   IOP_BASE(UQ22_SIZE)

#define ACC_BYTE    1 // Byte Access
#define ACC_WORD    2 // Word Access

#define UQ_QB    0x00000004 // QBus Mode
#define UQ_UB    0x00000002 // Unibus Mode
#define UQ_18B   0x00000001 // 18-bit Mode
#define UQ_16B   0x00000000 // 16-bit Mode


typedef struct ioQueue  uqQueue_t;
typedef struct ioMap    uqMap_t;
typedef struct uqDevice uqUnit_t;

typedef struct ioQueue  UQ_IRQ;
typedef struct ioMap    UQ_MAPIO;
typedef struct uqDevice UQ_DEVIO;

//// IRQ Pending Queue
//struct ioQueue {
//	uqQueue_t  *ioNext;    // Next IRQ queue
//	ioUnit_t   *ioUnit;    // Unit I/O block
//	uqMap_t    *ioMap;     // Map I/O block
//	uint32      irqPrio;   // IRQ Priority
//};
//
//struct ioMap {
//	uqMap_t  *Next;
//	ioUnit_t *ioUnit;
//	uqUnit_t *ubaUnit;
//
//	const char *uqName;    // This name
//	uint32  ioFlags;   // I/O Flags
//	uint32  idClass;   // I/O Class ID
//	uqQueue_t  irqUnit;   // IRQ Pending Queue
//	uint32  irqPrio;   // IRQ Priority
//	uint32  irqVector; // IRQ Vector Address
//
//	// I/O Device Settings
//	uint32 idUnit;     // Unit ID
//	uint32 idDrive;    // Drive ID of unit.
//	uint32 Flags;      // I/O Flags
//	uint32 Priority;   // Device Priority
//	uint32 csrAddr;    // CSR Base Address
//	uint32 nRegs;	   // Number of I/O Registers
//	uint32 iplLevel;   // Interrupt Priority Level
////	uint32 intVector;
////	uint32 intLevel;
////	uint32 intMask;
//
//	// I/O Routine Functions
//	void (*ResetIO)(ioUnit_t *);
//	int  (*ReadIO)(ioUnit_t *, uint32, uint16 *, uint32);
//	int  (*WriteIO)(ioUnit_t *, uint32, uint16, uint32);
//	int  (*BootIO)(uqMap_t *, IO_BOOT *, USER *, int, char **);
//	void (*AcknowledgeIO)(uqMap_t *);
//	void (*SetVector)(uqMap_t *, uint16);
//	void (*SendInterrupt)(uqMap_t *);
//	void (*CancelInterrupt)(uqMap_t *);
//	// I/O Data Transfers
//	uint32 (*WriteBlock)(uqMap_t *, uint32, void *, uint32, uint32);
//	uint32 (*ReadBlock)(uqMap_t *, uint32, void *, uint32, uint32);
//	uint32 (*VerifyBlock)(uqMap_t *, uint32, void *, uint32, uint32);
//};
//
//struct uqDevice {
//	ioUnit_t   ioUnit;             // Unit Interface
//	uint32     idType;             // Unibus or QBus Type
//	uint32     idUnit;             // Unit #
//	ioUnit_t  *ioUnit2;            // I/O Interface Unit (itself)
//	ioUnit_t  *cpuDev;             // CPU/Processor Unit
//	// I/O Setup Routines
//	int  (*AddSpaceIO)(uqUnit_t *, uqMap_t *);
//
//	uqQueue_t *irqList[UQ_NIPLS];  // IRQ Pending Queue
//	uqMap_t   *ioSpace[UQIO_SIZE]; // 8 KB I/O Space
//	uqMap_t   *ioList;             // Listing of Mapping I/O Devices
//};
//
//
//int   uba_GetAddr(uint32 *, uint32 *, uint32 *, uint32 *, int, char **);
//int   uba_AddSpaceIO(uqUnit_t *, uqMap_t *);
//int32 uba_CheckIRQ(uqUnit_t *, int);
//void  uba_SetVector(uqMap_t *, uint32);
//void  uba_ResetIO(uqUnit_t *);
//void  uba_EnqueueIO(uqUnit_t *, uqMap_t *);
//void  uba_DequeueIO(uqUnit_t *, uqMap_t *);
//uqMap_t *uba_GetNextIO(uqUnit_t *, int);
//void  uba_ShowLevel(uqUnit_t *, USER *);
//void  uba_ShowList(uqUnit_t *, USER *);
//void  uba_ShowMap(uqUnit_t *, USER *);
//
//// External declarations
//extern COMMAND uba_Commands[];
//extern COMMAND uba_SetCommands[];
//extern COMMAND uba_ShowCommands[];
//
//// Listing of external Unibus/Q22 devices.
//extern DEVICE *uqba_Devices[];
