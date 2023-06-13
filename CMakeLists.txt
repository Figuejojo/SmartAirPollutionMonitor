cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
set(PICO_SDK_PATH "..\\..\\pico-sdk") 
set(PICO_BOARD "pico_w")
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(SAPM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# Subdirectories
add_subdirectory(freertos)

# rest of your project
add_executable(SAPM
    common.h
    main.c
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(SAPM pico_stdlib freertos)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(SAPM)