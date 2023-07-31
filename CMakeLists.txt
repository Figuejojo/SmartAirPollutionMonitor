cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
set(PICO_SDK_PATH "..\\..\\pico-sdk") 
set(PICO_BOARD pico_w)
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(SAPM)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()


# Set Azure IoT SDK C settings
set(use_mqtt ON CACHE  BOOL "Set mqtt on" FORCE )
set(skip_samples ON CACHE  BOOL "Set slip_samples on" FORCE )
set(BUILD_TESTING OFF CACHE  BOOL "Set BUILD_TESTING off" FORCE )
set(AZ_SDK_C_NO_SAMPLES OFF)
set(AZ_PLATFORM_IMPL "CUSTOM")
set(AZ_CUSTOM_PLATFORM_IMPL_NAME "az_pico")

# Subdirectories
add_subdirectory(freertos)
add_subdirectory(SAPMS)
add_subdirectory(WIFI)
add_subdirectory(azure-sdk-for-c)

# rest of your project
add_executable(SAPM
    main.c
)

# Add all header files in the currect location
target_include_directories(SAPMS PUBLIC .)
target_include_directories(WIFI PUBLIC .)

#As the Azure sdk library is custom: the library is set as:
add_library(az_pico STATIC
    ${CMAKE_CURRENT_LIST_DIR}/libs/az_pico.c
  )

target_link_libraries(az_pico
    PRIVATE
      az_core
      pico_stdlib 
      freertos 
  )


# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(SAPM PUBLIC
    pico_cyw43_arch_lwip_sys_freertos
    pico_stdlib 
    freertos 
    SAPMS 
    WIFI 
    az_core
    az_iot_provisioning
    az_noplatform
    )

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(SAPM)