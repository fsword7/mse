set(MSE_CPP_SRCS
	emu/map/addrmap.cpp
	emu/command.cpp
	emu/console.cpp
	emu/delegate.cpp
	emu/device.cpp
	emu/devsys.cpp
	emu/engine.cpp
	emu/sysconfig.cpp
	emu/syslist.cpp
	emu/systems.cpp
	main/main.cpp
	system/dec/vax/ka650.cpp
	system/dec/vax/ka655.cpp
)

set(MSE_H_SRCS
	emu/map/addrmap.h
	emu/command.h
	emu/console.h
	emu/core.h
	emu/delegate.h
	emu/device.h
	emu/devsys.h
	emu/driver.h
	emu/engine.h
	emu/sysconfig.h
	emu/syslist.h
	system/dec/vax/ka650.h
	system/dec/vax/ka655.h
)

add_executable(mse ${MSE_CPP_SRCS} ${MSE_H_SRCS})
target_link_libraries(mse
	fmt::fmt
)