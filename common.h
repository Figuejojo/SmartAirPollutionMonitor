/** @file Common.h
 *  @brief Header file intended to be call in all documents. It will contian elements that will be used across
 *          most of the files.
 *
 *  @authors Y3913624 
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include <FreeRTOS.h>
#include <FreeRTOS-Kernel/include/queue.h>
#include <task.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"

// Pico Peripherals
#include "hardware/uart.h"

// Debug includes
#include "SAPMS/Debug_print.h"
#include "WIFI/process.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define ENABLE_DEBUG (1) // Enable the Debug prints

#define USE_WIRELESS (0)

#define USE_AM2320   (0)
#define USE_ADA746   (0)
#define USE_PMS5003  (0)
#define USE_SEN0515  (0)

typedef enum FUNC_ERRORS
{
    NO_ERROR        = 1,
    ER_GEN          = 0,
    ER_CHECKSUM     = -1,
    ER_END          = ER_CHECKSUM-1,
}ERR_t;

#endif //_COMMON_H_
