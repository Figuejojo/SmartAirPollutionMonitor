// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT
#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_

#define MQTTT  (1)
#define TSPEAK (0)
#define Azure  (0)

// Wifi
#define IOT_CONFIG_WIFI_SSID "<WIFI>"    //Write the Wifi username  
#define IOT_CONFIG_WIFI_PASSWORD "<PWD>" //Write the Wifi password

//Thinkspeak
#if (Thinkspeak == 1)
    //GET https://api.thingspeak.com/update?api_key=A2R0P2SYBI9FOTP8&field1=0
    #define URL_HTTP "https://"
    #define HTTP_URL "api.thingspeak.com"
    #define HTTP_OPT "GET "
    #define HTTP_CMD "/update?"
    #define HTTP_KEY "<Key>"    //Write the API Key.
    #define HTTP_FIELD "&field1="
#endif 

#if (Azure == 1)
    #define IOT_CONFIG_IOTHUB_FQDN ""        //!< IoTHub Name 
    #define IOT_CONFIG_DPS_ID_SCOPE ""   //!< Scope ID
    #define IOT_CONFIG_DEVICE_ID "<DEV-ID>"         //APMIOT_002" //!< Device ID
    #define IOT_CONFIG_DEVICE_KEY "<PRIM-KEY>"      //"DRtgC6kb0mNrs+SICxHbJ9wPTMnAE37gG/stbDnByzY="        
    #define IOT_REG_ID ""
    #define ENDPOINT ""
#endif

#if (MQTTT == 1)
    #define ENDPOINT "broker.emqx.io"
#endif 


// Publish 1 message every 2 seconds
#define TELEMETRY_FREQUENCY_MILLISECS 2000

#endif //_IOT_CONFIG_H_
