set(MSE_CPP_SRCS
	emu/command.cpp
	emu/console.cpp
	emu/delegate.cpp
	emu/device.cpp
	emu/sysconfig.cpp
	emu/syslist.cpp
	emu/systems.cpp
	main/main.cpp
)

set(MSE_H_SRCS
	emu/command.h
	emu/console.h
	emu/core.h
	emu/delegate.h
	emu/device.h
	emu/driver.h
	emu/sysconfig.h
	emu/syslist.h
)

add_executable(mse ${MSE_CPP_SRCS} ${MSE_H_SRCS})
target_link_libraries(mse
	fmt::fmt
)