cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico2 CACHE STRING "Board type")

# Writes build/compile_commands.json so the editor knows the SDK include paths
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Must come before project()
include(pico_sdk_import.cmake)

project(pico2_bridge C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

pico_sdk_init()

# Project sources. Add each new module here.
set(FW_SOURCES
    src/main.c
    src/drivers/tmp36.c
    src/drivers/tmp36_convert.c
)

add_executable(pico2_bridge ${FW_SOURCES})

target_include_directories(pico2_bridge PRIVATE src)

# Warnings apply to our sources only, not to the SDK sources linked in.
set_source_files_properties(${FW_SOURCES} PROPERTIES
    COMPILE_OPTIONS "-Wall;-Wextra"
)

target_link_libraries(pico2_bridge PRIVATE
    pico_stdlib
    hardware_adc
)

# UART0 belongs to RS-485; debug printf goes over USB only.
pico_enable_stdio_usb(pico2_bridge 1)
pico_enable_stdio_uart(pico2_bridge 0)

pico_add_extra_outputs(pico2_bridge)
