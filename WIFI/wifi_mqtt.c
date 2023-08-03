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
static mqtt_data_client_t mqtt;

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();
static void svMQTTConnect();
static void dns_dorequest(void *arg);
//MQTT Specifics
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len);
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags);
static void mqtt_pub_request_cb(void *arg, err_t result);
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status);
static void mqtt_request_cb(void *arg, err_t err);
static void mqtt_pub_request_cb(void *arg, err_t result);
static err_t publish(mqtt_client_t *client, void *arg);

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
    svMQTTConnect();

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
            publish(mqtt.mqtt_client_inst, &mqtt);
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

void dns_dorequest(void *arg)
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
    mqtt.mqtt_client_inst = mqtt_client_new();
    mqtt.mqtt_client_info.client_id = IOT_CLIENT;
    mqtt.mqtt_client_info.client_user = IOT_USER;
    mqtt.mqtt_client_info.client_pass = IOT_PWD;
    mqtt.mqtt_client_info.keep_alive = 50;
    mqtt.mqtt_client_info.will_topic = NULL;
    mqtt.mqtt_client_info.will_msg = NULL;
    mqtt.mqtt_client_info.will_qos = 0;
    mqtt.mqtt_client_info.will_retain = 0;   
    #if LWIP_ALTCP && LWIP_ALTCP_TLS
    mqtt.mqtt_client_info.tls_config = NULL;
    #endif

    mqtt_set_inpub_callback(mqtt.mqtt_client_inst, mqtt_incoming_publish_cb, mqtt_incoming_data_cb, &mqtt);

    err_t err = mqtt_client_connect(mqtt.mqtt_client_inst, &IPBackup, 1883, &mqtt_connection_cb, &mqtt, &mqtt.mqtt_client_info);
    if(err != ERR_OK)
    {
        printf("connect error\n");
    }
}

void mqtt_incoming_publish_cb(void *arg, const char *topic, u32_t tot_len) 
{
  mqtt_data_client_t* mqtt_client = (mqtt_data_client_t*)arg;
  strcpy((char*)mqtt_client->topic, (char*)topic);

  if (strcmp((const char*)mqtt.topic, IOT_TOPIC) == 0)
  {
    printf("Published successfully\n");
  }
}

void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) 
{
    printf("mqtt_incoming_data_cb\n");
    mqtt_data_client_t* mqtt_client = (mqtt_data_client_t*)arg;
    LWIP_UNUSED_ARG(data);
    strncpy((char*)mqtt_client->data, (char*)data, len);

    mqtt_client->len=len;
    mqtt_client->data[len]='\0';
}

void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) 
{
  mqtt_data_client_t* mqtt_client = (mqtt_data_client_t*)arg;
  LWIP_UNUSED_ARG(client);

  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" connection cb: status %d\n", mqtt_client->mqtt_client_info.client_id, (int)status));

  if (status == MQTT_CONNECT_ACCEPTED)
  {
    printf("MQTT_CONNECT_ACCEPTED\n");
    mqtt_sub_unsub(client,IOT_TOPIC, 0,mqtt_request_cb, arg,1);
        
  }
}

void mqtt_request_cb(void *arg, err_t err)
{
  mqtt_data_client_t* mqtt_client = ( mqtt_data_client_t*)arg;
  LWIP_PLATFORM_DIAG(("MQTT client \"%s\" request cb: err %d\n", mqtt_client->mqtt_client_info.client_id, (int)err));
}


/* Called when publish is complete either with sucess or failure */
void mqtt_pub_request_cb(void *arg, err_t result)
{
  if(result != ERR_OK) {
    printf("Publish result: %d\n", result);
  }
}

err_t publish(mqtt_client_t *client, void *arg)
{
  char pub_payload[50] = {0};
  static uint8_t temp = 20;
  sprintf(pub_payload,"field1=%d",temp);
  err_t err;
  u8_t qos = 0; /* 0 1 or 2, see MQTT specification */
  u8_t retain = 0; /* No don't retain such crappy payload... */
  err = mqtt_publish(client, IOT_TOPIC, pub_payload, strlen(pub_payload), qos, retain, mqtt_pub_request_cb, arg);
  if(err != ERR_OK) {
    printf("Publish err: %d\n", err);
  }
  return err;
}