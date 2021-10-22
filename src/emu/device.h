/*
 * device.h - device handling package
 *
 *  Created on: Jul 5, 2020
 *      Author: Tim Stark
 */

#pragma once

#include <type_traits>

#include "emu/debugger/device.h"
// #include "emu/delegate.h"
#include "emu/list.h"

class SystemConfig;
class Device;
class DeviceInterface;

constexpr uint64_t CLK_NOEXECUTE = 0;
constexpr uint64_t CLK_FULLSPEED = ~0ull;

typedef uint32_t TimerDeviceID_t;

template <typename T> struct is_device
{
	static constexpr bool value = std::is_base_of<Device, T>::value;
};

template <typename T> struct is_interface
{
	static constexpr bool value = std::is_base_of<DeviceInterface, T>::value;
};


template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct system_tag_struct
{
	typedef SystemClass type;
};

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto system_tag_func()
{
	return system_tag_struct<SystemClass, shortName, fullName, fileName>{};
}

template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto systemCreator = &system_tag_func<SystemClass, shortName, fullName, fileName>;


template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
struct device_tag_struct
{
	typedef DeviceClass type;
};

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
auto device_tag_func()
{
	return device_tag_struct<DeviceClass, shortName, fullName, fileName>{};
}

template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
constexpr auto deviceCreator = &device_tag_func<DeviceClass, shortName, fullName, fileName>;

class DeviceType
{
private:
	typedef Device *(*create_func)(const SystemConfig &config, const DeviceType &type,
			cstag_t &tagName, Device *owner, uint64_t clock);

	template <typename SystemClass>
	static Device *createSystem(const SystemConfig &config, const DeviceType &type,
		cstag_t &tagName, Device *owner, uint64_t clock)
	{
		return new SystemClass(config, type, tagName, clock);
	}

	template <typename DeviceClass>
	static Device *createDevice(const SystemConfig &config, const DeviceType &type,
		cstag_t &tagName, Device *owner, uint64_t clock)
	{
		return new DeviceClass(config, tagName, owner, clock);
	}

public:
	DeviceType() = default;

	template <class SystemClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
	DeviceType(system_tag_struct<SystemClass, shortName, fullName, fileName>(*)())
	: idType(typeid(SystemClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createSystem<SystemClass>)
	{}

	template <class DeviceClass, ctag_t *shortName, ctag_t *fullName, ctag_t *fileName>
	DeviceType(device_tag_struct<DeviceClass, shortName, fullName, fileName>(*)())
	: idType(typeid(DeviceClass)),
	  shortName(shortName),
	  fullName(fullName),
	  fileName(fileName),
	  creator(&createDevice<DeviceClass>)
	{}

	Device *create(const SystemConfig &config, cstag_t &tagName, Device *owner, uint64_t clock) const
	{
		assert(creator != nullptr);
		return creator(config, *this, tagName, owner, clock);
	}

	// Getter function calls
	inline const type_info &getType() const  { return idType; }
	inline ctag_t *getFullName() const       { return fullName; }
	inline ctag_t *getShortName() const      { return shortName; }
	inline ctag_t *getSourceName() const     { return fileName; }

private:
	const type_info &idType  = typeid(nullptr_t);	// device type identification
	create_func      creator = nullptr;				// device creator

	ctag_t *shortName = nullptr;	// Short name
	ctag_t *fullName  = nullptr;	// full name/description
	ctag_t *fileName  = nullptr;	// Name of source file
};

//template <class DeviceClass>
//class SystemCreator : public DeviceType
//{
//public:
//	using DeviceType::DeviceType;
//	using DeviceType::create;
//
//};

template <class DeviceClass>
class DeviceCreator : public DeviceType
{
public:
	using DeviceType::DeviceType;
	using DeviceType::create;

	template <typename... Args>
	DeviceClass *create(SystemConfig &config, cstag_t &devName, Device *owner, Args &&... args) const
	{
		return new DeviceClass(config, devName, owner, std::forward<Args>(args)...);
	}

	template <typename... Args>
	DeviceClass *operator ()(SystemConfig &config, cstag_t &devName, Args&&... args) const;
};

#define DECLARE_DEVICE_TYPE(Type, Class) \
	extern DeviceCreator<Class> const &Type;

// #define DECLARE_DEVICE_TYPE(Type, Class)              \
// 	extern DeviceCreator<Class> const &Type;          \
// 	extern template class DeviceFinder<Class, false>; \
// 	extern template class DeviceFinder<Class, true>;

#define DECLARE_DEVICE_TYPE_NEW(Type, Class)          \
	class Class;                                      \
	extern template class DeviceFinder<Class, false>; \
	extern template class DeviceFinder<Class, true>;

#define DECLARE_DEVICE_TYPE_NS(Type, Namespace, Class)           \
	namespace Namepace { class Class; }                          \
	extern template class DeviceFinder<Namespace::Class, false>; \
	extern template class DeviceFinder<Namespace::Class, true>;

#define DEFINE_DEVICE_TYPE(Type, Class, ShortName, FullName)	\
	struct Class##_device_traits {								\
		static constexpr ctag_t shortName[] = ShortName;		\
		static constexpr ctag_t fullName[]  = FullName;			\
		static constexpr ctag_t fileName[]  = __FILE__;			\
	};															\
	constexpr ctag_t Class##_device_traits::shortName[];		\
	constexpr ctag_t Class##_device_traits::fullName[];			\
	constexpr ctag_t Class##_device_traits::fileName[];			\
	DeviceCreator<Class> const &Type = deviceCreator<Class, (Class##_device_traits::shortName), \
		(Class##_device_traits::fullName), (Class##_device_traits::fileName)>;

// *************************************************************

class Machine;
class DeviceInterface;
class diExternalBus;
class diExecute;
class diDebug;
class Timer;
// class mapMemoryRegion;
class ObjectFinder;

class Device
: public BindedDelegate,
  public List<Device>
{
public:
	using ifList_t = vector<DeviceInterface *>;

	virtual ~Device() = default;

	// Getter function calls
	inline cstag_t &getDeviceName() const   { return devName; }
	inline ctag_t  *getFullName() const     { return type.getFullName(); }
	inline ctag_t  *getShortName() const    { return type.getShortName(); }
	inline ctag_t  *getSourceName() const   { return type.getSourceName(); }
	inline uint64_t getClock() const		{ return clock; }

	inline const SystemConfig &getSystemConfig() const { return sysConfig; }

	inline Machine *getMachine() const { return ownMachine; }
	inline devCommand_t *getCommands() const { return devCommands; }

//	inline void setMachine(Machine *sys) { ownMachine = sys; }
	inline void setCommands(devCommand_t *cmds) { devCommands = cmds; }

	// Dynamic_cast safely converts references and pointers to up, down and sideways. 
	// If cast fails, return null pointer. For more information, check
	// https://en.cppreference.com/w/cpp/language/dynamic_cast.

	template <class DeviceClass>
	inline bool getInterface(DeviceClass *&iface)
	{
		return (iface = dynamic_cast<DeviceClass *>(this)) != nullptr;
	}

	template <class DeviceClass>
	inline bool getInterface(DeviceClass *&iface) const
	{
		return (iface = dynamic_cast<const DeviceClass *>(this)) != nullptr;
	}

	template <class DeviceClass>
	inline bool hasInterface(DeviceClass *&iface)
	{
		return (iface = dynamic_cast<DeviceClass *>(this)) != nullptr;
	}

	template <class DeviceClass>
	inline bool hasInterface(DeviceClass *&iface) const
	{
		return (iface = dynamic_cast<const DeviceClass *>(this)) != nullptr;
	}

	void addInterface(DeviceInterface *iface);
	void setMachine(Machine *sys);

	void updateClock();

	// Timer function calls
	Timer *allocateTimer(TimerDeviceID_t id = 0, void *data = nullptr);
	void setTimer(const attotime_t &duration, TimerDeviceID_t id = 0,
		int32_t param = 0, void *data = nullptr);

	inline ifList_t getInterfaces() { return ifList; }

	inline bool isStarted() const { return flagStarted; }
	inline Device *getOwner() const { return ownDevice; }

	// Memory management function calls
	const romEntry_t *mapGetROMEntries();

	mapMemoryRegion *findMemoryRegion(ctag_t *name);
	mapMemoryBank *findMemoryBank(ctag_t *name);
	mapMemoryShare *findMemoryShare(ctag_t *name);
	Device *findDevice(ctag_t *name);

	void registerObject(ObjectFinder *object);
	bool findObjects();
	void resolvePostMapping();

	// local device function calls
	void configure(SystemConfig &config);
	void start();
	void stop();
	void reset();

	// device-specific virtual device function calls
	//   do nothing by default
	virtual void devConfigure(SystemConfig &config) {}
	virtual void devStart() {}
	virtual void devStop() {}
	virtual void devReset() {}
	virtual void devUpdateClock() {}
	virtual void devExecuteTimer(Timer &timer, TimerDeviceID_t id, void *data, int32_t param) {}
	virtual cromEntry_t *devGetROMEntries() { return nullptr; }

	virtual void configureDevice(SystemConfig &config) {}
	virtual void startDevice() {}
	virtual void stopDevice() {}
	virtual void resetDevice() {}
	virtual void updateDeviceClock() {}
	virtual void executeTimerDevice(Timer &timer, TimerDeviceID_t id, void *data, int32_t param) {}
	virtual cromEntry_t *getDeviceFirmwareEntries() { return nullptr; }

	void completeConfig(); // complete final configuration

protected:
	Device(const SystemConfig &config, const DeviceType &type, cstag_t &tagName, Device *owner, uint64_t clock);

private:
	const DeviceType   &type;
	const SystemConfig &sysConfig;

	Machine *ownMachine = nullptr;
	Device *ownDevice = nullptr;
	bool flagStarted = false;

	cstag_t devName;
	uint64_t clock = 0; // clock frequency

	debug_t *debugger = nullptr;
	
	const romEntry_t *firmwareEntries = nullptr;

	// device interface section
	ifList_t ifList;

	vector<ObjectFinder *> objectList;

protected:
	friend class diExternalBus;
	friend class diExecute;
	friend class diDebug;

	diExternalBus *ifBus     = nullptr;
	diExecute     *ifExecute = nullptr;
	diDebug       *ifDebug   = nullptr;

	devCommand_t  *devCommands = nullptr;
};

typedef Device device_t;

// *************************************************************

class DeviceInterface
{
	friend class Device;

public:
	DeviceInterface(device_t *dev, tag_t *name);
	virtual ~DeviceInterface() = default;

	inline device_t *getDevice() const { return owner; }
	inline const tag_t *getName() const { return tagName; }

	// virtual function calls
	virtual void completeConfig() {}

	virtual void ifStart() {}
	virtual void ifUpdateClock() {}

private:
	DeviceInterface *next = nullptr;
	device_t *owner       = nullptr;
	const tag_t *tagName  = nullptr;
};

// *************************************************************

class DeviceIterator
{
public:
	DeviceIterator(Device &dev) : devRoot(dev) {}
//	DeviceIterator(Machine &system) : devRoot(machine.getSystemDevice()) {}

	class iterator
	{
	public:
		iterator(Device *device) : curDevice(device) {}

		// Required operator function calls
		bool operator == (const iterator &iter) { return curDevice == iter.curDevice; }
		bool operator != (const iterator &iter) { return curDevice != iter.curDevice; }
		iterator operator ++ () { advance(); return *this; }
		iterator operator ++ (int) { const iterator result(*this); ++*this; return result; }
		Device &operator * () { assert(curDevice != nullptr); return *curDevice; }
		Device *operator -> () { return curDevice; }

	protected:
		void advance()
		{
			if (curDevice == nullptr)
				return;

			if (curDevice->hasChildren())
			{
				ownDevice = curDevice;
				curDevice = ownDevice->getFirst();
				depth++;

//				fmt::printf("Iterator: Start of list - %d children\n", ownDevice->size());
//				fmt::printf("Iterator: Depth=%d Name=%s\n", depth, curDevice->getDeviceName());
//				cout << flush;

				return;
			}

			while (depth > 0 && ownDevice != nullptr)
			{
				curDevice = ownDevice->getNext();
				if (curDevice != nullptr) {
//					fmt::printf("Iterator: Depth=%d Name=%s\n", depth, curDevice->getDeviceName());
//					cout << flush;
					return;
				}

				curDevice = ownDevice;
				ownDevice = ownDevice->getOwner();
				depth--;

//				fmt::printf("Iterator: End of list - back to %s\n", curDevice->getDeviceName());
//				cout << flush;
			}

			curDevice = nullptr;
//			fmt::printf("Iterator: End of iterator - all done\n");
		}

		Device *curDevice = nullptr;
		Device *ownDevice = nullptr;
		int depth = 0;
	};

	iterator begin() { return iterator(&devRoot); }
	iterator end()   { return iterator(nullptr); }

private:
	Device &devRoot;
};

template<class InterfaceType>
class InterfaceIterator
{
public:
	InterfaceIterator(Device &dev) : devRoot(dev) {}

	class iterator : public DeviceIterator::iterator
	{
	public:
		iterator(Device *device) : DeviceIterator::iterator(device) { findInteface(); }

		// Required operator function calls
		bool operator == (const iterator &iter) { return iface == iter.iface; }
		bool operator != (const iterator &iter) { return iface != iter.iface; }
		iterator operator ++ () { advance(); findInteface(); return *this; }
		iterator operator ++ (int) { const iterator result(*this); ++*this; return result; }
		InterfaceType &operator * () { assert(iface != nullptr); return *iface; }
		InterfaceType *operator -> () { return iface; }

	private:
		void findInteface()
		{
			for (; curDevice != nullptr; advance()) {
				if (curDevice->hasInterface(iface))
					return;
			}
			iface = nullptr;
		}

		InterfaceType *iface = nullptr;
	};

	iterator begin() { return iterator(&devRoot); }
	iterator end()   { return iterator(nullptr); }

private:
	Device &devRoot;
};
