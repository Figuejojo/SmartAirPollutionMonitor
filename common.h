/** @file Common.h
 *  @brief Header file intended to be call in all documents. It will contian elements that will be used across
 *          most of the files.
 *
 *  @authors Jose Jorge Figueroa Figueroa 
 */

#ifndef _COMMON_H_
#define _COMMON_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include <FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "pico/stdlib.h"


// Pico Peripherals/
#include "hardware/uart.h"

// Debug includes
#include "SAPMS/Debug_print.h"
#include "WIFI/process.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define SAPM_ID     "001"
    
/*  WIFI Elements  */
#define USE_WIRELESS (0)    /*!< On/Off Wireless Task. If wireless is on go to WIFI/IoT_conig.h */
#define WATCHDOG_ON  (0)    /*!< On/Off Watchdog for WIFI */                     

/* Sensor Elements */
#define USE_AM2320   (0)    /*!< On/Off AM2320  Task */
#define USE_ADA746   (0)    /*!< On/Off ADA746  Task */
#define USE_PMS5003  (0)    /*!< On/Off PMS5003 Task */
#define USE_SEN0515  (0)    /*!< On/Off ENS160  Task */

/* Develop Elements*/
#define ENABLE_DEBUG (1)    /*!< On/Off Debug prints */

/*******************************************************************************
* Type definitions
*******************************************************************************/

/**
 * @name ERR_t
 * @type enum
 * @brief Error typedef enumerator for function returns.
 */
typedef enum FUNC_ERRORS
{
/*@{*/
    NO_ERROR        = 1,    /*!< No Error           */
    ER_GEN          = 0,    /*!< Generic Error      */
    //Errors
    ER_CHECKSUM     = -1,   /*!< Checksum Error     */
    //  I2C Errors
    ER_I2C_ID       = -2,   /*!< I2C ID Error      */
    ER_I2C_GEN      = -3,   /*!< I2C Generic Error */
    // WIFI Errors
    ER_WIFI         =- 2,   /*!< Wifi Error*/
/*@}*/
}ERR_t;

#endif //_COMMON_H_
