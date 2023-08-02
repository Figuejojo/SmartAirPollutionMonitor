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
#include "lwip/apps/httpd.h"
#include "lwip/dns.h"
/*******************************************************************************
* Static Global Variables
*******************************************************************************/
#define UDP_PORT 5005 //4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();
static void dns_dorequest(void *arg);
ip_addr_t dnsRespIP;
ip_addr_t IPBackup;
/*******************************************************************************
* Static global variables
*******************************************************************************/

/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskWireless(void * pvParameters)
{
    svConnect();


    
    struct tcp_pcb * tcp_new(void);
    //struct udp_pcb* pcb = udp_new();

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
            printf("WIFIStuff\n");
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
    //httpd_init();
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

static void dns_found(const char *name, const ip_addr_t *addr, void *arg)
{
  LWIP_UNUSED_ARG(arg);
  IPBackup = *addr;
  printf("%s: %s\n", name, addr ? ipaddr_ntoa(addr) : "<not found>");
}

void dns_dorequest(void *arg)
{
  const char* dnsname = HTTP_URL;
  LWIP_UNUSED_ARG(arg);

  if (dns_gethostbyname(dnsname, &dnsRespIP, dns_found, NULL) == ERR_OK) 
  {
    dns_found(dnsname, &dnsRespIP, NULL);
  }
}

void httpd_post_finished(void *connection, char *response_uri, u16_t response_uri_len)
{
    
}

err_t httpd_post_receive_data(void *connection, struct pbuf *p)
{
    return 0;
}

err_t httpd_post_begin(void *connection, const char *uri, const char *http_request,
                       u16_t http_request_len, int content_len, char *response_uri,
                       u16_t response_uri_len, u8_t *post_auto_wnd)
{
    return 0;    
}

void vSendUdp()
{
    #if 0 
    struct pbuf *p = pbuf_alloc(PBUF_TRANSPORT, BEACON_MSG_LEN_MAX+1, PBUF_RAM);
    char *req = (char *)p->payload;
    memset(req, 0, BEACON_MSG_LEN_MAX+1);
    snprintf(req, BEACON_MSG_LEN_MAX, "%d\n", counter);
    err_t er = udp_sendto(pcb, p, &addr, UDP_PORT);
    pbuf_free(p);
    if (er != ERR_OK) {
        printf("Failed to send UDP packet! error=%d", er);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
    } else {
        printf("Sent packet %d\n", counter);
        counter++;
    }
    #endif
}