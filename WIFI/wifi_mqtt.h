/** @file wifimqtt.h
 *  @brief Header file for the WIFI and MQTT. 
 *
 *  @authors Jose Jorge Figueroa Figueroa 
 */

#ifndef _WIFI_MQTT_H_
#define _WIFI_MQTT_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "iot_configs.h"
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_opts.h"
#include "lwip/apps/mqtt_priv.h"
#include "hardware/watchdog.h"
//For SNTP consults:
#include <time.h>
#include "lwip/pbuf.h"
#include "lwip/udp.h"

#if(AZ_TS==1)
#include "AzureMQTT.h"
#endif

/*******************************************************************************
* Macro definitions
*******************************************************************************/
#define SSID_WIFI IOT_CONFIG_WIFI_SSID
#define PSWD_WIFI IOT_CONFIG_WIFI_PASSWORD

/*******************************************************************************
* Type definitions
*******************************************************************************/

typedef struct mqtt_data_client 
{
    mqtt_client_t *mqtt_client_inst;
    struct mqtt_connect_client_info_t mqtt_client_info;
    uint8_t data[MQTT_OUTPUT_RINGBUF_SIZE];
    uint8_t topic[100];
    uint32_t len;
} mqtt_data_client_t;

typedef struct ntp
{
    ip_addr_t ip;
    bool isIP;
    struct udp_pcb *ntp_pcb;
    absolute_time_t ntp_time;
}ntp_t;

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

err_t publish(char pub_payload[]);

#endif  //_WIFI_MQTT_H_
