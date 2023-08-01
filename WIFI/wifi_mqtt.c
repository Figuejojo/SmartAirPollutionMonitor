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
#include "azure/az_core.h"
#include "azure/az_iot.h"
#include <mbedtls/base64.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>


/*******************************************************************************
* Static Global Variables
*******************************************************************************/
#define UDP_PORT 4444
#define BEACON_MSG_LEN_MAX 127
#define BEACON_TARGET "255.255.255.255"
#define BEACON_INTERVAL_MS 1000
#define ENDPOINT "global.azure-devices-provisioning.net"

                // Year since 1970 to the expiration date 
#define AUG_1_2023_630 (1690910990)
#define EXP_S(min) (AUG_1_2023_630+(min*60))


//az_span deviceId = AZ_SPAN_FROM_BUFFER(IOT_CONFIG_DEVICE_ID);
/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void svConnect();
static void svGeneratePassKeySas();
static void mbedtls_hmac_sha256(az_span key, az_span payload, az_span signed_payload);
static int decode_base64_bytes(az_span base64_encoded_bytes, az_span decoded_bytes, az_span* out_decoded_bytes);
static void base64_encode_bytes(az_span decoded_bytes, az_span base64_encoded_bytes, az_span* out_base64_encoded_bytes);

/*******************************************************************************
* Static Variables Declarations
*******************************************************************************/
static char sas_signature_buffer[128] = {0};    //Signed buffer
static char sasB64EncodedSignedSignature_buf[128] = {0};    //EncodedsasBuffer
static char mqttPwd_buf[256] = {0};            


static az_iot_hub_client hub_client;
static char my_mqtt_user_name[100];
static char my_mqtt_client_id[16];
static char telemetry_topic[128];

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
           
        }
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

/**
*	@name vSetupWifi
*   @type function
*/
static az_span const provisioning_global_endpoint = AZ_SPAN_LITERAL_FROM_STR(IOT_ENDPOINT);
static az_iot_provisioning_client provisioning_client;

ERR_t vSetupWifi(void)
{
    volatile az_result res;
    /** MQTT setup **/
    #if 1   
    //Create EndPoint
    /*char mqttEndPoint_buff[256]; //This must be greater than the path
    az_span provisioning_mqtt_endpoint = az_span_create(mqttEndPoint_buff, (int32_t)sizeof(mqttEndPoint_buff));
    az_span_copy(provisioning_mqtt_endpoint, provisioning_global_endpoint);
    */
    //  Init client with provisioning global endpoint
    res = az_iot_provisioning_client_init(&provisioning_client,
                az_span_create_from_str(IOT_ENDPOINT),
                az_span_create_from_str(IOT_CONFIG_DEVICE_ID),
                az_span_create_from_str(IOT_REG_ID),
                NULL);
    if (az_result_failed(res))
    {
        printf("Client: az_result return code 0x%08x.", res);
    }
    // Get the MQTT client id used for the MQTT connection.
    char mqtt_client_id_buffer[128];
    res = az_iot_provisioning_client_get_client_id(&provisioning_client, 
            mqtt_client_id_buffer, 
            sizeof(mqtt_client_id_buffer), NULL);
    if (az_result_failed(res))
    {
        printf("Could not get the client id: az_result return code 0x%08x.", res);
    }
    #endif

    #if 0 // IoT Hub
    az_span hostname = az_span_create_from_str(HOSTNAME);
    az_span deviceID = az_span_create_from_str(IOT_CONFIG_DEVICE_ID);
    az_result res = az_iot_hub_client_init(&hub_client, hostname, deviceID, NULL);
    res = az_iot_hub_client_get_user_name(&hub_client, my_mqtt_user_name, sizeof(my_mqtt_user_name), NULL);
    res = az_iot_hub_client_get_client_id(&hub_client, my_mqtt_client_id,sizeof(my_mqtt_client_id), NULL);
    #endif
    

    printf("Endpoint Created\n");


    /*** @Todo: Create IOT endpoint (buffer) ***/
    /** Create the MQTT Client (LWIP) **/

     /** End **/

    svGeneratePassKeySas();

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


void svGeneratePassKeySas()
{
    volatile az_result res;
    uint64_t sasDuration = EXP_S(48);
    az_span az_sasKey = az_span_create_from_str(IOT_CONFIG_DEVICE_KEY);
    az_span az_signature = AZ_SPAN_FROM_BUFFER(sas_signature_buffer);
    az_span az_sasB64EncodedSignedSignature_buf = AZ_SPAN_FROM_BUFFER(sasB64EncodedSignedSignature_buf);
    
    // Get signature based on the required time set on EXP_S (SsinceAugust9+min)
    res = az_iot_hub_client_sas_get_signature(&hub_client,sasDuration,az_signature,&az_signature);
    if (az_result_failed(res))
    {
        printf("Could not get the client: az_result return code 0x%08x.", res);
    }
    // Generate sas base64 and encoded signed signature > decodebase64>signHMAC-SHA256 > 64encode
    //// 1.- Decode Base64
    char sasDecodedKey_buff[32];
    az_span az_sasDecodedKey = AZ_SPAN_FROM_BUFFER(sasDecodedKey_buff);
    res = decode_base64_bytes(az_sasKey, az_sasDecodedKey, &az_sasDecodedKey);
    if (az_result_failed(res))
    {
        printf("Could not decode: az_result return code 0x%08x.", res);
    }
    //// 2.- HMAC-SHA256 sign with decoded key.
    char sasHmac256Signature_buff[32];
    az_span az_sasHmac256Signature = AZ_SPAN_FROM_BUFFER(sasHmac256Signature_buff);
    mbedtls_hmac_sha256(az_sasDecodedKey, az_signature, az_sasHmac256Signature);

    //// 3.- BASE64 encoding
    base64_encode_bytes(az_sasHmac256Signature, az_sasB64EncodedSignedSignature_buf, &az_sasB64EncodedSignedSignature_buf);

    //// 4.- Get SAS password
    size_t mqttPwdLen = 0;
    res = az_iot_hub_client_sas_get_password(&hub_client, sasDuration, az_sasB64EncodedSignedSignature_buf, AZ_SPAN_EMPTY, mqttPwd_buf, sizeof(mqttPwd_buf), &mqttPwdLen);
    if (az_result_failed(res))
    {
        printf("Could not get the password: az_result return code 0x%08x.", res);
    }

}

static void mbedtls_hmac_sha256(az_span key, az_span payload, az_span signed_payload)
{
  mbedtls_md_context_t ctx;
  mbedtls_md_type_t md_type = MBEDTLS_MD_SHA256;

  mbedtls_md_init(&ctx);
  mbedtls_md_setup(&ctx, mbedtls_md_info_from_type(md_type), 1);
  mbedtls_md_hmac_starts(&ctx, (const unsigned char*)az_span_ptr(key), az_span_size(key));
  mbedtls_md_hmac_update(&ctx, (const unsigned char*)az_span_ptr(payload), az_span_size(payload));
  mbedtls_md_hmac_finish(&ctx, az_span_ptr(signed_payload));
  mbedtls_md_free(&ctx);
}

static int decode_base64_bytes(
    az_span base64_encoded_bytes,
    az_span decoded_bytes,
    az_span* out_decoded_bytes)
{
  memset(az_span_ptr(decoded_bytes), 0, (size_t)az_span_size(decoded_bytes));

  size_t len;
  int32_t mbedtls_ret;
  if ((mbedtls_ret = mbedtls_base64_decode(
          az_span_ptr(decoded_bytes),
          (size_t)az_span_size(decoded_bytes),
          &len,
          az_span_ptr(base64_encoded_bytes),
          (size_t)az_span_size(base64_encoded_bytes)))
      != 0)
  {
    return 1;
  }
  else
  {
    *out_decoded_bytes = az_span_create(az_span_ptr(decoded_bytes), (int32_t)len);
    return 0;
  }
}

static void base64_encode_bytes(
    az_span decoded_bytes,
    az_span base64_encoded_bytes,
    az_span* out_base64_encoded_bytes)
{
  size_t len;
  int32_t mbedtls_ret;
  if ((mbedtls_ret = mbedtls_base64_encode(
          az_span_ptr(base64_encoded_bytes),
          (size_t)az_span_size(base64_encoded_bytes),
          &len,
          az_span_ptr(decoded_bytes),
          (size_t)az_span_size(decoded_bytes)))
      != 0)
  {
    printf("mbedtls_base64_encode fail: mbedtls base64 encode ");
  }

  *out_base64_encoded_bytes = az_span_create(az_span_ptr(base64_encoded_bytes), (int32_t)len);
}
