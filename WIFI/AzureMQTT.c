/** @file wifi_mqtt.c
 *  @brief  The file contains the definitions for the CYW43 wifi module 
 *              and MQTT
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
#if 1
/*******************************************************************************
* Includes
*******************************************************************************/
#include "AzureMQTT.h"
#include "azure/az_core.h"
#include "azure/az_iot.h"
#include <mbedtls/base64.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"


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

/*******************************************************************************
* Static Variables Declarations
*******************************************************************************/


/*******************************************************************************
* Static Function Definitions
*******************************************************************************/
/**
*	@name vTaskWireless
*   @type Task
*/
void svAzureMQTTConfig()
{
}

/**
*	@name vSetupWifi
*   @type function
*/
ERR_t vAzureSetup(void)
{
    return NO_ERROR;
}

void svGeneratePassKeySas()
{
    
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
#endif
