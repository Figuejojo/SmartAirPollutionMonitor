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
/*******************************************************************************
* Static Global Variables
*******************************************************************************/

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static int scan_result(void *env, const cyw43_ev_scan_result_t *result) {
    if (result) {
        taskENTER_CRITICAL();

        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
            result->ssid, result->rssi, result->channel,
            result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
            result->auth_mode);

        taskEXIT_CRITICAL();    
    }
    return 0;
}

/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    bool scan_in_progress = false;
    int err = 0;
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    cyw43_arch_enable_sta_mode();
    while(1)
    {
        if(!scan_in_progress)
        {
            cyw43_wifi_scan_options_t scan_options = {0};
            int err = cyw43_wifi_scan(&cyw43_state, &scan_options, NULL, scan_result);
            if(0 == err)
            {
                Print_debug("Scanning Wifi...");
            }
            else
            {
                Print_debug("Failed to start scan");
                vTaskDelay(10000/portTICK_PERIOD_MS);
                cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
            }
        }
        else if (!cyw43_wifi_scan_active(&cyw43_state))
        {
            vTaskDelay(10000/portTICK_PERIOD_MS);
            scan_in_progress = false;
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
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

