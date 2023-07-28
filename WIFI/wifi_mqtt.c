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
#include "lwip/dns.h"
#include "lwip/apps/mqtt.h"
/*******************************************************************************
* Static Global Variables
*******************************************************************************/
#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000
#define ENDPOINT "global.azure-devices-provisioning.net"

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();

/*******************************************************************************
* Static Variables Declarations
*******************************************************************************/
static mqtt_client_t* mqtt_client;

static const struct mqtt_connect_client_info_t mqtt_client_info =
{
  "test",
  NULL, /* user */
  NULL, /* pass */
  100,  /* keep alive */
  NULL, /* will_topic */
  NULL, /* will_msg */
  0,    /* will_qos */
  0     /* will_retain */
#if LWIP_ALTCP && LWIP_ALTCP_TLS
  , NULL
#endif
};

/*******************************************************************************
* Static Function Definitions
*******************************************************************************/
#if 1//LWIP_DNS_APP && LWIP_DNS
static void dns_found(const char *name, const ip_addr_t *addr, void *arg)
{
  LWIP_UNUSED_ARG(arg);
  printf("%s: %s\n", name, addr ? ipaddr_ntoa(addr) : "<not found>");
}

static void dns_dorequest(void *arg)
{
  const char* dnsname = ENDPOINT;
  ip_addr_t dnsresp;
  LWIP_UNUSED_ARG(arg);

  if (dns_gethostbyname(dnsname, &dnsresp, dns_found, NULL) == ERR_OK) {
    dns_found(dnsname, &dnsresp, NULL);
  }
}
#endif /* LWIP_DNS_APP && LWIP_DNS */


/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    svConnect();

    mqtt_client = mqtt_client_new();


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
            dns_dorequest(NULL);
        }
        vTaskDelay(5000/portTICK_PERIOD_MS);
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
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, true);
    }
}
