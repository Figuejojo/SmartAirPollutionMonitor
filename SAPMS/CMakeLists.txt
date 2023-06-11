cmake_minimum_required(VERSION 3.12)

set(SAPMS_SRCS
    bapi_I2C0.c
)

add_library(SAPMS ${SAPMS_SRCS})

target_include_directories(SAPMS PUBLIC .)

target_link_libraries(SAPMS PUBLIC
    pico_stdlib 
    freertos  
    hardware_i2c
)

