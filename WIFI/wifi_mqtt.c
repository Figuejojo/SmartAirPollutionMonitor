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
#include "iot_configs.h"
#include "pico/cyw43_arch.h"
#include "lwip/dns.h"
#include "lwip/apps/mqtt.h"
#include "lwip/apps/mqtt_opts.h"
#include "lwip/apps/mqtt_priv.h"

/*******************************************************************************
* Macros
*******************************************************************************/
    // Year since 1970 to the expiration date 
#define AUG_1_2023_630 (1690910990)
#define EXP_S(min) (AUG_1_2023_630+(min*60))


//az_span deviceId = AZ_SPAN_FROM_BUFFER(IOT_CONFIG_DEVICE_ID);
/*******************************************************************************
* Static Variables Declarations
*******************************************************************************/
//MQTT - LWIP
static ip_addr_t dnsRespIP;
static ip_addr_t IPBackup; 
static mqtt_client_t * sgMQTT_client;

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();
static void svMQTTConnect();
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);

/*******************************************************************************
* Static Function Definitions
*******************************************************************************/
/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    svConnect();
    vTaskDelay(10000/portTICK_PERIOD_MS);
    int counter = 0;
    while(1)
    {
        if(0 > cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA))
        {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            printf("Disconnected\n"); 
        }
        else
        {
            svMQTTConnect();
        }
        vTaskDelay(10000/portTICK_PERIOD_MS);
    }
}

/**
*	@name vSetupWifi
*   @type function
*/
ERR_t vSetupWifi(void)
{
    sgMQTT_client = mqtt_client_new();
    return NO_ERROR;
}

/**
*	@name svConnect
*   @type static function
*/
void svConnect()
{
    if (cyw43_arch_init()) 
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
        printf("failed to initialise\n");
    }
    cyw43_arch_enable_sta_mode();

    printf("Connecting to Wi-Fi...\n");
    if (cyw43_arch_wifi_connect_timeout_ms(SSID_WIFI, PSWD_WIFI, CYW43_AUTH_WPA2_AES_PSK, 30000)) 
    {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
        printf("failed to connect.\n");
    } 
    else 
    {
        printf("Connected.\n");
        dns_dorequest(NULL);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    }
}

/****************************************************************************
 *                              DNS IP from name
*****************************************************************************/
static void dns_found(const char *name, const ip_addr_t *addr, void *arg)
{
  LWIP_UNUSED_ARG(arg);
  IPBackup = *addr;
  printf("%s: %s\n", name, addr ? ipaddr_ntoa(addr) : "<not found>");
}

static void dns_dorequest(void *arg)
{
  const char* dnsname = ENDPOINT;
  LWIP_UNUSED_ARG(arg);

  if (dns_gethostbyname(dnsname, &dnsRespIP, dns_found, NULL) == ERR_OK) 
  {
    dns_found(dnsname, &dnsRespIP, NULL);
  }
}

/****************************************************************************
*	                                    MQTT 
*****************************************************************************/
void svMQTTConnect()
{

}
