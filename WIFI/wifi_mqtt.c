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
/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    while(1)
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        vTaskDelay(250/portTICK_PERIOD_MS);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        vTaskDelay(250/portTICK_PERIOD_MS);
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

