# FreeRTOS TI C6X GCC port with TLS support

For thread-local storage to properly work, a few changes have been
applied to the core FreeRTOS code. Apart from that, the port in
FreeRTOS/Source/portable/GCC/C674X shows how to glue the toolchain
and the operating system, and the demo in
FreeRTOS/Demo/C674X_GCC provides some code that compiles but doesn't
support any specific hardware, leaving integration to the user according
to their needs.

From the demo directory:

1. Get the SBG toolchain generator [1] and build a tic6x toolchain
2. Set C6X_TOOLCHAIN_PATH in CMakeLists.txt as appropriate
3. cmake -B build -DCMAKE_BUILD_TYPE=Debug (or Release)
4. cmake --build build

The port was created to work with FreeRTOS V10.1.1 but should be
easy to port to any newer version.

[1] https://github.com/SBG-Systems/sbgToolchain
