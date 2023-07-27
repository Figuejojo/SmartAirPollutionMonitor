/** @file wifimqtt.h
 *  @brief Header file for the WIFI and MQTT. 
 *
 *  @authors Y3913624 
 */

#ifndef _WIFI_MQTT_H_
#define _WIFI_MQTT_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
//#include "pico/cyw43_arch.h"
/*******************************************************************************
* Type definitions
*******************************************************************************/
#define SSID_WIFI "JJRAZER"     //Uncommit and insert your WIFI Name as "Name"
#define PSWD_WIFI "#JJRZ12345"  //Uncommit and insert yout Wifi PSWD as "Pswd"

#define TIME2CONNECT (30000)
/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name  vSetupWifi();
*   @brief Setup the configuration for the CYW43 Wifi module and MQTT
*
*   @param 	Void
*   @return Void
*/
ERR_t vSetupWifi(void);

/** @name   vTaskWireless
*   @brief  Wireless taks dedicated to aquire the sensor data and send it through wifi.
*               Runs Every XXXXs
*               Uses XXX_Queue to send sensor information.    
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskWireless(void * pvParameters);

#endif  //_WIFI_MQTT_H_
