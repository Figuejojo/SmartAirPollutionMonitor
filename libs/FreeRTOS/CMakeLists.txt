#FreeRTOS SetUp
set(PICO_SDK_FREERTOS_SOURCE FreeRTOS)

add_library(freertos
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/event_groups.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/list.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/queue.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/stream_buffer.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/tasks.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/timers.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/portable/MemMang/heap_3.c
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/port.c
)

target_include_directories(freertos PUBLIC
    .
    ./..
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/include
    ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/include
)
target_link_libraries(freertos PUBLIC
    pico_stdlib 
    hardware_exception
)

#freeRTOS Plus
set(FreeRTOSPlus_PATH ${PICO_SDK_FREERTOS_SOURCE}/FreeRTOS-Plus)
