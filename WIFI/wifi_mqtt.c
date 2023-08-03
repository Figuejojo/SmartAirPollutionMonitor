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
static mqtt_client_t sgMQTT_client;

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

void svMQTTConnect()
{
    // Get the MQTT client id used for the MQTT connection.

    struct mqtt_connect_client_info_t ci = {0};
    //ci.client_id = mqtt_client_id_buffer;
    //ci.client_pass = mqttPwd_buf;
    //ci.client_user = mqtt_client_username_buffer;
    //ci.keep_alive = 240;
    Print_debug("MQTT_Client_Connect");
    ip_addr_t temp = IPBackup;
    printf("Ip: %s\n",ipaddr_ntoa(&temp));
    vTaskDelay(100);
    err_t err = mqtt_client_connect(&sgMQTT_client, &IPBackup, LWIP_IANA_PORT_SECURE_MQTT, mqtt_connection_cb, 0, &ci);
}

static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status)
{
    if(status == MQTT_CONNECT_ACCEPTED)
    {
       Print_debug("mqtt_connection_cb: Successfully connected");
    }
    else
    {
        Print_debug("mqtt_connection_cb: Disconnected, reason:");
    }

}

static void mqtt_sub_request_cb(void *arg, err_t result)
{
  /* Just print the result code here for simplicity, 
     normal behaviour would be to take some action if subscribe fails like 
     notifying user, retry subscribe or disconnect from server */
  printf("Subscribe result: %d\n", result);
}

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

/**
*	@name vSetupWifi
*   @type function
*/

ERR_t vSetupWifi(void)
{
    return NO_ERROR;
}

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
