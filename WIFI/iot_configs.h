/** @file iotconfig.h
 *  @brief Header file for the MQTT configuration parameters and WIFI  
 *
 *  @authors Jose Jorge Figueroa Figueroa 
 */

#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_

#define MQTT_TS  (0)
#define HTTP_TS  (0)
#define AZ_TS    (0)

// Wifi
#define IOT_CONFIG_WIFI_SSID "<SSID>"    //Write the Wifi username  
#define IOT_CONFIG_WIFI_PASSWORD "<PWD>" //Write the Wifi password

//Thinkspeak
#if(HTTP_TS == 1)
    #define URL_HTTP "https://"
    #define HTTP_URL "api.thingspeak.com"
    #define HTTP_OPT "GET "
    #define HTTP_CMD "/update?"
    #define HTTP_KEY "<Key>"    //Write the API Key.
    #define HTTP_FIELD "&field1="
#elif(AZ_TS == 1)
    #define IOT_CONFIG_IOTHUB_FQDN  "<HUB-Name>"        //!< IoTHub Name 
    #define IOT_CONFIG_DPS_ID_SCOPE "<Scope-ID>"   //!< Scope ID
    #define IOT_CONFIG_DEVICE_ID    "<DEV-ID>"         //APMIOT_002" //!< Device ID
    #define IOT_CONFIG_DEVICE_KEY   "<PRIM-KEY>"      //"DRtgC6kb0mNrs+SICxHbJ9wPTMnAE37gG/stbDnByzY="        
    #define ENDPOINT ""
#elif(MQTT_TS == 1)
    #define ENDPOINT   "mqtt3.thingspeak.com" 
    #define IOT_CLIENT "<Client ID>"
    #define IOT_USER   "<User ID(Same as Client ID)>"
    #define IOT_PWD    "<Password>"
    #define IOT_TOPIC  "<Topic/Subtopic>"
#else
    #define IOT_TOPIC  "<Topic/PlaceHold>"
    #define IOT_CLIENT "<PlaceHolder>"
    #define IOT_USER   "<PlaceHolder>"
    #define IOT_PWD    "<PlaceHolder>"  
    #define ENDPOINT "<PlaceHolder>"
#endif

// Publish 1 message every 2 seconds
#define TELEMETRY_FREQUENCY_MILLISECS 2000

#endif //_IOT_CONFIG_H_
