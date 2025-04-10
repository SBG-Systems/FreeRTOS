set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR tic6x)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(C6X_TOOLCHAIN_PATH /opt/sbg/sbg-gcc-tic6x-none-elf-20250314)

set(CMAKE_AR ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-ar)
set(CMAKE_ASM_COMPILER ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-gcc)
set(CMAKE_C_COMPILER ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-gcc)
set(CMAKE_CXX_COMPILER ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-g++)
set(CMAKE_LINKER ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-gcc)
set(CMAKE_OBJCOPY ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-objcopy)
set(CMAKE_RANLIB ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-ranlib)
set(CMAKE_SIZE ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-size)
set(CMAKE_STRIP ${C6X_TOOLCHAIN_PATH}/bin/tic6x-none-elf-strip)

cmake_minimum_required(VERSION 3.18)

project(Demo LANGUAGES C CXX ASM)

include(CheckIPOSupported)

add_compile_options("$<$<COMPILE_LANGUAGE:C>:-std=c18>")
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-std=c++17>")

add_compile_options(-static)

add_compile_options(-march=c674x)
add_compile_options(-mno-dsbt)
add_compile_options(-msdata=none)

# Enable exceptions for both C and C++ files. This is required to make
# sure exception handling is properly done when mixing C and C++ code.
add_compile_options(-fexceptions)

# XXX Work around a compiler bug.
add_compile_options(-fno-var-tracking)

add_compile_options(-Wall)
add_compile_options(-Wextra)
add_compile_options(-Wshadow)
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-Werror=implicit>")
add_compile_options(-Wpointer-arith)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-O2 -g)
	add_compile_options(-fstack-protector-strong)

	# Without LTOs (IPO in CMake jargon), the firmware can get very large,
	# requiring the garbage collection of unused sections.
	add_compile_options(-ffunction-sections)
	add_compile_options(-fdata-sections)
else()
	add_compile_definitions(NDEBUG)
	add_compile_options(-O3 -g)
	add_compile_options(-fno-stack-protector)
endif()

get_directory_property(COMPILE_OPTIONS COMPILE_OPTIONS)
add_link_options("${COMPILE_OPTIONS}")

add_link_options(-nostdlib)
add_link_options(LINKER:--nmagic)
add_link_options(LINKER:--print-memory-usage)
add_link_options(LINKER:--build-id=none)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_link_options(LINKER:--gc-sections)
endif()

add_link_options(LINKER:--require-defined=uxTopUsedPriority)

add_link_options(-T ${CMAKE_CURRENT_SOURCE_DIR}/Demo.ld)

set(INCLUDE_DIRS
	.
	../../Source/include
	../../Source/portable/GCC/C674X
)

set(SRCS
	bootAsm.S
	boot.c
	freertos.c
	main.c
	newlibLock.c
	ssp.c
	syscall.c
	../../Source/event_groups.c
	../../Source/list.c
	../../Source/queue.c
	../../Source/tasks.c
	../../Source/timers.c
	../../Source/portable/GCC/C674X/port.c
	../../Source/portable/GCC/C674X/portAsm.S
)

add_executable(Demo.elf)
target_sources(Demo.elf PRIVATE ${SRCS})
target_include_directories(Demo.elf PRIVATE ${INCLUDE_DIRS})
target_link_libraries(Demo.elf -Wl,--start-group -lstdc++ -lm -lc -lgcc -Wl,--end-group)

if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
	set_property(TARGET Demo.elf PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()
