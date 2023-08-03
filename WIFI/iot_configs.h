/** @file iotconfig.h
 *  @brief Header file for the MQTT configuration parameters and WIFI  
 *
 *  @authors Jose Jorge Figueroa Figueroa 
 */

#ifndef _IOT_CONFIG_H_
#define _IOT_CONFIG_H_

#define MQTTT  (0)
#define TSPEAK (0)
#define Azure  (0)

// Wifi

//Thinkspeak
#if(TSPEAK == 1)
    #define URL_HTTP "https://"
    #define HTTP_URL "api.thingspeak.com"
    #define HTTP_OPT "GET "
    #define HTTP_CMD "/update?"
    #define HTTP_KEY "<Key>"    //Write the API Key.
    #define HTTP_FIELD "&field1="
#endif 

#if(Azure == 1)
    #define IOT_CONFIG_IOTHUB_FQDN  "<HUB-Name>"        //!< IoTHub Name 
    #define IOT_CONFIG_DPS_ID_SCOPE "<Scope-ID>"   //!< Scope ID
    #define IOT_CONFIG_DEVICE_ID    "<DEV-ID>"         //APMIOT_002" //!< Device ID
    #define IOT_CONFIG_DEVICE_KEY   "<PRIM-KEY>"      //"DRtgC6kb0mNrs+SICxHbJ9wPTMnAE37gG/stbDnByzY="        
    #define ENDPOINT ""
#endif

#if(MQTTT == 1)
    #define ENDPOINT   "<URL>"     //!< URL 
    #define IOT_CLIENT "<Client>"  //!< Client ID
    #define IOT_USER   "<USD>"     //!< User ID
    #define IOT_PWD    "<PWD>"     //!< Password
    #define IOT_TOPIC  "<Topic>"   //!< Topic
#endif 

// Publish 1 message every 2 seconds
#define TELEMETRY_FREQUENCY_MILLISECS 2000

#endif //_IOT_CONFIG_H_
