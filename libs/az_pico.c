#include <azure/core/az_platform.h>
#include <azure/core/internal/az_precondition_internal.h>

#include "pico/stdlib.h"
#include <FreeRTOS.h>
#include <task.h>

az_result az_platform_clock_msec(int64_t* out_clock_msec)
{
  *out_clock_msec = xTaskGetTickCount();
  return AZ_OK;
}

az_result az_platform_sleep_msec(int32_t milliseconds)
{
  vTaskDelay(milliseconds);
  return AZ_OK;
}
