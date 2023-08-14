#ifndef _AZUREMQTT_H_
#define _AZUREMQTT_H_

#include "common.h"
#include "iot_configs.h"
#include "lwip/apps/mqtt.h"
#include "azure/az_core.h"
#include "azure/az_iot.h"
#include <mbedtls/base64.h>
#include <mbedtls/sha256.h>
#include <mbedtls/md.h>
#include "mbedtls/ctr_drbg.h"
#include "mbedtls/entropy.h"
#include "mbedtls/net_sockets.h"
#include "mbedtls/ssl.h"


typedef struct
{
  az_span hub_device_id;
  az_span hub_hostname;
  az_span hub_sas_key;
  az_span provisioning_id_scope;
  az_span provisioning_registration_id;
  az_span provisioning_sas_key;
  az_span x509_cert_pem_file_path;
  az_span x509_trust_pem_file_path;
  uint32_t sas_key_duration_minutes;
} iot_sample_environment_variables;

void azure_iot_init(struct mqtt_connect_client_info_t *ci);


#endif
