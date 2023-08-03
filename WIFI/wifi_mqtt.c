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
#include "lwip/tcp.h"
#include "lwip/apps/httpd.h"
#include "lwip/dns.h"
/*******************************************************************************
* Static Global Variables
*******************************************************************************/
#define UDP_PORT 80//5005 //4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();
static void tcp_setup(void);
static void dns_dorequest(void *arg);
static uint32_t tcp_send_packet(void);
static err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err);
static err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err);

ip_addr_t dnsRespIP;
ip_addr_t IPBackup;
struct tcp_pcb *testpcb;
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
    vTaskDelay(500);
    dns_dorequest(NULL);
    vTaskDelay(500);
    tcp_setup();
    vTaskDelay(500);
    
    int counter = 0;
    volatile err_t err = 0;
    while(1)
    {
        if(0 > cyw43_wifi_link_status(&cyw43_state, CYW43_ITF_STA))
        {
            cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, false);
            printf("Disconnected\n"); 
        }
        else
        {
            Print_debug("Openning->");
            ip_addr_t ip = IPBackup;
            tcp_connect(testpcb, &ip, 80, connectCallback);

        }
        vTaskDelay(60000*5/portTICK_PERIOD_MS);
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

err_t connectCallback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
    printf("Connection Established -> ");
    tcp_send_packet();
    return 0;
}

uint32_t tcp_send_packet(void)
{
    printf("Now sending a packet\n");
    err_t error;

    static uint32_t sensorData = 40;
    char string[130] = {0};
    sprintf(string, "GET /update?api_key=VD7UI0ZJ7DPOEHGC&field1=%d HTTP/1.0\r\nConnection: Close\r\nHost: api.thingspeak.com\r\n\r\n ",sensorData++);

    /* push to buffer */
    error = tcp_write(testpcb, string, strlen(string), TCP_WRITE_FLAG_COPY);

    if (error) {
        printf("ERROR: Code: %d (tcp_send_packet :: tcp_write)\n", error);
        return 1;
    }

    /* now send */
    error = tcp_output(testpcb);
    if (error) {
        printf("ERROR: Code: %d (tcp_send_packet :: tcp_output)\n", error);
        return 1;
    }
    return 0;
}

err_t tcpRecvCallback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
    printf("Data recieved.\n");
    if (p == NULL) {
        printf("The remote host closed the connection.\n");
        printf("Now I'm closing the connection.\n");
        tcp_close(testpcb);
        return ERR_ABRT;
    } else {
        printf("Number of pbufs %d\n", pbuf_clen(p));
        printf("Contents of pbuf %s\n", (char *)p->payload);
    }
    Print_debug("Closing tcp");
    if(ERR_OK == tcp_close(testpcb))
    {
        Print_debug("Closed tcp");
    }
    return 0;
}

void tcp_setup(void)
{
    uint32_t data = 0xdeadbeef;

    /* create an ip */
    ip_addr_t ip = IPBackup; //Change for thinkspeak??//Thinkspeak 54.81.196.77

    /* create the control block */
    testpcb = tcp_new();    

    /* dummy data to pass to callbacks*/
    tcp_arg(testpcb, &data);

    /* register callbacks with the pcb */
    tcp_err(testpcb, NULL);
    tcp_recv(testpcb, tcpRecvCallback);
    tcp_sent(testpcb, NULL);

    /* now connect */
    //tcp_connect(testpcb, &ip, 80, connectCallback);
    
}
