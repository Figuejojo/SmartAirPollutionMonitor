/** @file SEN0515.h
 *  @brief TVOC and CO2 sensor
 *          Containts the definitions for SEN0515/ENS160
 *
 *  @author "Exam Number"
 */

#ifndef _SEN0515_H_
#define _SEN0515_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define SEN0_TX (0)  // Raspberry Pi Pico Pin for TXD
#define SEN0_RX (0)  // Raspberry Pi Pico Pin for RXD

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	setupSEN0515
*   @brief  To setup the Pin modes for the GPS
*
*   @param 	Void
*   @return Void
*/
void setupSEN0515(void);

/** @name 	vTaskSEN0515
*   @brief //Brief description Todo
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskSEN0515(void * pvParameters);



#endif //_SEN0515_H_
