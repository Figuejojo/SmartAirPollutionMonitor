/** @file process.h
 *  @brief Header file for processing sensor information. 
 *
 *  @authors Y3913624 
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/


/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name  vSetupWifi();
*   @brief Setup the configuration for the CYW43 Wifi module and MQTT
*
*   @param 	Void
*   @return Void
*/
void vSetupProcess(void);

/** @name   vTaskWireless
*   @brief  Wireless taks dedicated to aquire the sensor data and send it through wifi.
*               Runs Every XXXXs
*               Uses XXX_Queue to send sensor information.    
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskProcess(void * pvParameters);

#endif  //_PROCESS_H_
