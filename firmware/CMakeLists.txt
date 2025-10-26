cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_FETCH_FROM_GIT ON)
include(pico_sdk_import.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PICO_PLATFORM rp2040)
set(PICO_BOARD pico)

# include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
# include($ENV{PICO_SDK_PATH}/tools/CMakeLists.txt)

project(pico_start C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

add_subdirectory(main)
add_subdirectory(lib)

pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 0)
