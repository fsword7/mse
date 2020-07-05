set(MSE_CPP_SRCS
	emu/delegate.cpp
	emu/device.cpp
	main/console.cpp
	main/main.cpp
)

set(MSE_H_SRCS
	emu/delegate.h
	emu/device.h
	main/core.h
)

add_executable(mse ${MSE_CPP_SRCS} ${MSE_H_SRCS})