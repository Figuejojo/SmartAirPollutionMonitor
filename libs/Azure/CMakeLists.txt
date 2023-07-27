# Azure Core
set(SDK_AZURE_LIBS azure-iot-middleware-freertos/libraries)

# Middle Ware iot FreeRTOS
#libs\FreeRTOS\FreeRTOS\FreeRTOS\Source
set(FREERTOS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../FreeRTOS/FreeRTOS)
set(FREERTOS_PORT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../FreeRTOS/FreeRTOS/FreeRTOS/Source/portable/ThirdParty/GCC/RP2040/Include)
set(CONFIG_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/..)
message("$(FREERTOS_PATH)")

add_subdirectory(${SDK_AZURE_LIBS}/azure-sdk-for-c azure-sdk-for-c)

include_directories("${FreeRTOSPlus_PATH}/Source/Utilities/logging")
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/..")

#include_directories(${CONFIG_DIRECTORY})
#include_directories(${FREERTOS_PORT_DIRECTORY})
#include_directories(${FREERTOS_DIRECTORY}/FreeRTOS/Source/include)
#add_subdirectory(source/)
