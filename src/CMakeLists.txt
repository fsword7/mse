set(MSE_CPP_SRCS
	emu/console.cpp
	emu/delegate.cpp
	emu/device.cpp
	main/main.cpp
)

set(MSE_H_SRCS
	emu/console.h
	emu/core.h
	emu/delegate.h
	emu/device.h
)

add_executable(mse ${MSE_CPP_SRCS} ${MSE_H_SRCS})
target_link_libraries(mse
	fmt::fmt
)