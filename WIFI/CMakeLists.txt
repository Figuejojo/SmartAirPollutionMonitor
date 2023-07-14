cmake_minimum_required(VERSION 3.12)

set(WIFI_SRCS
    wifi_mqtt.c
    process.c
)

add_library(WIFI ${WIFI_SRCS})

target_include_directories(WIFI PUBLIC .)

target_link_libraries(WIFI PUBLIC
    pico_cyw43_arch_lwip_poll
    pico_stdlib 
    freertos  
)

