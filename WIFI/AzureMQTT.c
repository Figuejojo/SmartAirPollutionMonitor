/** @file wifi_mqtt.c
 *  @brief  The file contains the definitions for the CYW43 wifi module 
 *              and MQTT
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "AzureMQTT.h"



/*******************************************************************************
* Macros
*******************************************************************************/
    // Year since 1970 to the expiration date 
#define AUG_1_2023_630 (1690910990)
#define EXP_S(min) (AUG_1_2023_630+(min*60))


//az_span deviceId = AZ_SPAN_FROM_BUFFER(IOT_CONFIG_DEVICE_ID);
/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void mbedtls_hmac_sha256(az_span key, az_span payload, az_span signed_payload);
static int decode_base64_bytes(az_span base64_encoded_bytes, az_span decoded_bytes, az_span* out_decoded_bytes);
static void base64_encode_bytes(az_span decoded_bytes, az_span base64_encoded_bytes, az_span* out_base64_encoded_bytes);
static void svGeneratePassKeySas();
/*******************************************************************************
* Static Variables Declarations
*******************************************************************************/
iot_sample_environment_variables env_vars;
static az_iot_hub_client hub_client;
extern time_t epoch;

/*******************************************************************************
* Static Function Definitions
*******************************************************************************/
static char iot_sample_hub_hostname_buffer[128];
static char iot_sample_hub_device_id_buffer[64];
static char iot_sample_hub_sas_key_buffer[128];
static char iot_sample_x509_trust_pem_file_path_buffer[256];
static char mqtt_password_buffer[256];
static char mqtt_client_username_buffer[128];

void azure_iot_init(struct mqtt_connect_client_info_t *ci)
{
  // Get HostName from configurations
  env_vars.hub_hostname = AZ_SPAN_FROM_BUFFER(iot_sample_hub_hostname_buffer);
  env_vars.hub_hostname = az_span_create_from_str(IOT_CONFIG_IOTHUB_FQDN);
  // Get Device Id from configurations
  env_vars.hub_device_id= AZ_SPAN_FROM_BUFFER(iot_sample_hub_device_id_buffer);
  env_vars.hub_device_id= az_span_create_from_str(IOT_CONFIG_DEVICE_ID);
  // Get Sas Key from configurations
  env_vars.hub_sas_key  = AZ_SPAN_FROM_BUFFER(iot_sample_hub_sas_key_buffer);
  env_vars.hub_sas_key  = az_span_create_from_str(IOT_CONFIG_DEVICE_KEY);
  // Get Scope
  //env_vars.provisioning_id_scope = 
  //IOT_CONFIG_DPS_ID_SCOPE;
  // Get Sas Key from configurations
  env_vars.sas_key_duration_minutes = 120;
  //x509 PEM file path
  env_vars.x509_trust_pem_file_path = AZ_SPAN_FROM_BUFFER(iot_sample_x509_trust_pem_file_path_buffer);
  env_vars.x509_trust_pem_file_path = az_span_create_from_str("");

  //Endpoint??

  //Client Init
  int rc = az_iot_hub_client_init(&hub_client, env_vars.hub_hostname, env_vars.hub_device_id, NULL);
  if (az_result_failed(rc))
  {
    printf("Failed to initialize hub client: az_result return code 0x%08x.", rc);
  }

  char mqtt_client_id_buffer[128];
  rc = az_iot_hub_client_get_client_id(&hub_client, mqtt_client_id_buffer, sizeof(mqtt_client_id_buffer), NULL);
  if (az_result_failed(rc))
  {
    printf("Failed to get MQTT client id: az_result return code 0x%08x.", rc);
  }

  //SAS Key Generation
  svGeneratePassKeySas();

  // UsernameBuffer
  rc = az_iot_hub_client_get_user_name(&hub_client, mqtt_client_username_buffer,sizeof(mqtt_client_username_buffer),NULL);
  if(az_result_failed(rc))
  {
    printf("Failed to get MQTT client username: az_result return code 0x%08x.", rc);  
  }

  // MQTT Client LWIP
  
  ci->client_id = mqtt_client_username_buffer;
  ci->client_user = mqtt_client_username_buffer;
  ci->client_pass = mqtt_password_buffer;
  ci->keep_alive = 50;
  ci->will_topic = NULL;
  ci->will_msg = NULL;
  ci->will_qos = 0;
  ci->will_retain = 0;   
  
}

/**
*	@name vTaskWireless
*   @type Task
*/
static char sas_signature_buffer[128];

void svGeneratePassKeySas()
{
  //Singing
  uint64_t sas_duration = epoch + (env_vars.sas_key_duration_minutes*60);
  az_span sas_signature = AZ_SPAN_FROM_BUFFER(sas_signature_buffer);
  int rc = az_iot_hub_client_sas_get_signature(&hub_client, sas_duration, sas_signature, &sas_signature);
  if (az_result_failed(rc))
  {
    printf("Could not get the signature for SAS key: az_result return code 0x%08x.", rc);
  }
  //Decode-HMAC-Encode
  char sas_base64_encoded_signed_signature_buffer[128];
  az_span sas_base64_encoded_signed_signature = AZ_SPAN_FROM_BUFFER(sas_base64_encoded_signed_signature_buffer);
  
  //    Decode SasKey
  char sas_decoded_key_buff[32];//char sas_decoded_key_buffer[32];
  az_span sas_decoded_key = AZ_SPAN_FROM_BUFFER(sas_decoded_key_buff);
  decode_base64_bytes(env_vars.hub_sas_key,sas_decoded_key,&sas_decoded_key);
  //    HMAC 
  char sas_hmac256_buffer[32];//char sas_hmac256_signed_signature_buffer[32];
  az_span sas_hmac256_signed_signature = AZ_SPAN_FROM_BUFFER(sas_hmac256_buffer);
  mbedtls_hmac_sha256(sas_decoded_key,sas_signature,sas_hmac256_signed_signature);

  //    Encoded
  base64_encode_bytes(sas_hmac256_signed_signature,sas_base64_encoded_signed_signature,&sas_base64_encoded_signed_signature);

  // Get Password
  size_t mqtt_password_length;
  rc = az_iot_hub_client_sas_get_password(&hub_client, sas_duration, sas_base64_encoded_signed_signature, AZ_SPAN_EMPTY, mqtt_password_buffer, sizeof(mqtt_password_buffer), &mqtt_password_length);
  if (az_result_failed(rc))
  {
    printf("Could not get the password: az_result return code 0x%08x.", rc);
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
