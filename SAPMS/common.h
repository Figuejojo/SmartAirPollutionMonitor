/** @file Common.h
 *  @brief Header file intended to be call in all documents. It will contian elements that will be used across
 *          most of the files.
 *
 *  @author 
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
#include "pico/stdlib.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define ENABLE_DEBUG (1)
#define DEBUGMSG_SIZE (20)

#define USE_AM2320  (0)
#define USE_ADA746  (0)
#define USE_PMS5003 (0)

#endif //_COMMON_H_
