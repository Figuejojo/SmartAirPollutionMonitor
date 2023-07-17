/** @file wifi_mqtt.c
 *  @brief  The file contains the definitions for the CYW43 wifi module 
 *              and MQTT
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "wifi_mqtt.h"
#include "pico/cyw43_arch.h"

#include "lwip/pbuf.h"
#include "lwip/udp.h"
/*******************************************************************************
* Static Global Variables
*******************************************************************************/
#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    struct udp_pcb * pcb = udp_new();
    ip_addr_t addr;
    ipaddr_aton(BEACON_TARGET, &addr);
    char msg[20];
    bool isConnected = false;
    cyw43_arch_enable_sta_mode();
    Print_debug("WirelessTask\n");

    while(1)
    {
		Print_debug("Connecting to Wi-Fi...\n");
        if(isConnected == true)
        {
            cyw43_arch_poll();
            Print_debug("Connected.\n");
        }
        else
        {
            if (cyw43_arch_wifi_connect_timeout_ms(SSID_WIFI, PSWD_WIFI, CYW43_AUTH_WPA2_AES_PSK, 2000)) {
                Print_debug("failed to connect.\n");
                isConnected == false;
            } else {
                Print_debug("Connected.\n");
                isConnected = true;
            }
        }
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}

/**
*	@name vSetupWifi
*   @type function
*/
ERR_t vSetupWifi(void)
{
#if (USE_WIRELESS == 1)
    if (cyw43_arch_init()) {
        printf("Wi-Fi init failed");
        return ER_WIFI;
    }
#endif
    return NO_ERROR;
}

