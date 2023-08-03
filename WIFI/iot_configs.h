// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT
#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_


// Wifi
#define IOT_CONFIG_WIFI_SSID "<WIFI>"    //Write the Wifi username  
#define IOT_CONFIG_WIFI_PASSWORD "<PWD>" //Write the Wifi password

//Thinkspeak
//GET https://api.thingspeak.com/update?api_key=A2R0P2SYBI9FOTP8&field1=0
#define URL_HTTP "https://"
#define HTTP_URL "api.thingspeak.com"
#define HTTP_OPT "GET "
#define HTTP_CMD "/update?"
#define HTTP_KEY "<Key>"    //Write the API Key.
#define HTTP_FIELD "&field1="

// Publish 1 message every 2 seconds
#define TELEMETRY_FREQUENCY_MILLISECS 2000

#endif //_IOT_CONFIG_H_
