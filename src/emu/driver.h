/*
 * driver.h - system driver package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include "emu/device.h"

#define SYSTEM_NOT_WORKING   0x00000010

struct SystemFlag
{
	enum type : uint32_t
	{
		// System flags
		CLASS_MASK  = 0x0000000F,
		ORIENT_MASK = 0x000000F0,
		NOT_WORKING = 0x00000100,

		// Orientation flags
		FLIP_X      = 0x00000010,
		FLIP_Y		= 0x00000020,
		SWAP_XY		= 0x00000040,

		// Machine class types
		CLASS_OTHER    = 0,
		CLASS_CONSOLE  = 1,
		CLASS_COMPUTER = 2,
		CLASS_TERMINAL = 3,
		CLASS_PRINTER  = 4,
	};
};

//constexpr uint32_t SYSTEM_NOT_WORKING = SystemFlag::NOT_WORKING;

struct SystemDriver
{
	typedef void (*sysConfigure)(SystemConfig &config, Device &owner);

	const char			*name;
	const char			*parent;
	const char			*section;
	const DeviceType	&type;
	sysConfigure	     configure;
	const romEntry_t	*romEntries;

	const char   *description;
	const char   *source;		// source file name
};

#define SYSTEM_NAME(Name) Name##_driver
#define SYSTEM_EXTERN(Name) extern const SystemDriver SYSTEM_NAME(Name)
#define SYSTEM_TRAITS_NAME(Name) Name##_system_traits

#define SYSTEM_TRAITS(Name, FullName)							\
	struct SYSTEM_TRAITS_NAME(Name) {							\
		static constexpr tag_t shortName[]  = #Name;			\
		static constexpr tag_t fullName[]   = FullName;			\
		static constexpr tag_t fileName[]   = __FILE__;			\
	};															\
	constexpr tag_t SYSTEM_TRAITS_NAME(Name)::shortName[];		\
	constexpr tag_t SYSTEM_TRAITS_NAME(Name)::fullName[];		\
	constexpr tag_t SYSTEM_TRAITS_NAME(Name)::fileName[];

#define SYSTEM_TYPE(Name, Class)					\
	systemCreator<Class,							\
		(SYSTEM_TRAITS_NAME(Name)::shortName),		\
		(SYSTEM_TRAITS_NAME(Name)::fullName),		\
		(SYSTEM_TRAITS_NAME(Name)::fileName)>


#define CONSOLE(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
	SYSTEM_TRAITS(Name, Description) \
	extern const SystemDriver SYSTEM_NAME(Name) = \
	{ \
		#Name, \
		#Parent, \
		#Section, \
		SYSTEM_TYPE(Name, Class), \
		[] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
		ROM_NAME(Name), \
		Description, \
		__FILE__ \
	};

#define COMPUTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
	SYSTEM_TRAITS(Name, Description) \
	extern const SystemDriver SYSTEM_NAME(Name) = \
	{ \
		#Name, \
		#Parent, \
		#Section, \
		SYSTEM_TYPE(Name, Class), \
		[] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
		ROM_NAME(Name), \
		Description, \
		__FILE__ \
	};

#define TERMINAL(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
	SYSTEM_TRAITS(Name, Description) \
	extern const SystemDriver SYSTEM_NAME(Name) = \
	{ \
		#Name, \
		#Parent, \
		#Section, \
		SYSTEM_TYPE(Name, Class), \
		[] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
		ROM_NAME(Name), \
		Description, \
		__FILE__ \
	};

#define PRINTER(Name, Parent, Section, Type, Class, Configure, Reset, Company, Description, Flags) \
	SYSTEM_TRAITS(Name, Description) \
	extern const SystemDriver SYSTEM_NAME(Name) = \
	{ \
		#Name, \
		#Parent, \
		#Section, \
		SYSTEM_TYPE(Name, Class), \
		[] (SystemConfig &config, Device &owner) { static_cast<Class &>(owner).Configure(config); }, \
		ROM_NAME(Name), \
		Description, \
		__FILE__ \
	};

//class system_driver
//{
//public:
//	typedef void (*system_creator)(system_config &, device_t &);
//	typedef void (*driver_init)(device_t &);
//
//	const char				*name;			// machine name
//	const char 				*parent;		// parent of machine name
//	const char				*section;		// section area
//	const device_type_base	&type;
//	system_creator			configure;		// system creator/configuration callback
//	driver_init				init;			// system initialize callback
//	const romEntry_t		*rom;			// ROM entries for firmware
//	const char				*description;	//Description/full name
//	const char				*source;		// source file name
//};


//#define COMP(Name, Parent, Section, Type, Class, Create, Init, Company, Description)	\
//	SYSTEM_TRAITS(Name, Description)					\
//	extern const system_driver SYSTEM_NAME(Name) =		\
//	{													\
//		#Name,											\
//		#Parent,										\
//		#Section,										\
//		SYSTEM_TYPE(Name, Class),						\
//		[] (system_config &config, device_t &owner) { static_cast<Class &>(owner).Create(config); }, \
//		[] (device_t &owner) { static_cast<Class &>(owner).Init(); },	\
//		ROM_NAME(Name),									\
//		Description,									\
//		__FILE__										\
//	};
