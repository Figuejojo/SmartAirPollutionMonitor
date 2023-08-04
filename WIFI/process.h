/** @file process.h
 *  @brief Header file for processing sensor information. 
 *
 *  @authors Jose Jorge Figueroa Figueroa 
 */

#ifndef _PROCESS_H_
#define _PROCESS_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define PROC_QUEUE_SZ (10)    /*!< SensorData Queue Element size */

//0,540,000,000
//4,294,967,295
//0,060,000,000

#define TIME2WIFI (60000000)/*!< Time to send wifi [us] - 60000000 is 1 min */ 
/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * @name SAPMS_e
 * @type enum
 * @brief This enumerator is to distinguished the incomming message from each
 *          sensor task.
 */
typedef enum
{
/*@{*/
    ENONESAPMS = -1,    /*!< Error State */
    EPMS = 0,           /*!< PMS sensor data enum */
    EENS,               /*!< SEN0515/ENS160 sensor data */
    EAM,                /*!< AM2320 sensor data */
    EGPS,               /*!< GPS sensor data */
    EWIFI,              /*!< Send format to WIFI thread. */
    SAPMS_END,          /*!< Enum end */
/*@}*/
}SAPMS_e;

/**
 * @name SAPMS_t
 * @type struct
 * @brief FreeRtos Queue messaging datatype for cache and peripherals.  
 */
typedef struct SAPMS
{
/*@{*/
    SAPMS_e eSRC;   /*!< Enum for collecting incomming Queue values */
    /** @name  SAPMS.sPM */
    struct
    {
    /*@{*/
        float fPM1; /*!< PM 1[um] float value   */
        float fPM25;/*!< PM 2.5[um] float value */
        float fPM10;/*!< PM 10[um] float value  */
    /*@}*/
    } sPM; 

    /** @name  SAPMS.sENS */
    struct 
    {
    /*@{*/
        float fCO2; /*!< ENS160 CO2 float value  */
        float fTVOC;/*!< ENS160 TVOC float value */
        //uint8_t AQI;
    /*@}*/
    }sENS;
    
    /** @name  SAPMS.sTEM */
    struct
    {
    /*@{*/
        float fTemp; /*!< AM2320 Temperature flaot value  */
        float fHum;  /*!< AM2320 Humidity flaot value  */
    /*@}*/
    }sAM;
    
    /** @todo Add other sensors as they are finisehd */
/*@}*/
}SAPMS_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name  vSetupWifi();
*   @brief Setup the configuration for the CYW43 Wifi module and MQTT
*
*   @param 	Void
*   @return Void
*/
void vSetupProcess(void);

/** @name   vTaskWireless
*   @brief  Wireless taks dedicated to aquire the sensor data and send it through wifi.
*               Runs everytime a incomming sgqSensorData queue arrive. 
*               Uses XXX_Queue to send sensor information to wifi.    
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskProcess(void * pvParameters);

/** @name   vTaskWireless
*   @brief  Collect data by sending it to the process task.    
*
*   @param 	ESAPMS  SAPM sensor enumerator.
*   @param  psSAPMS Pointer to SAPM sensor structure.
*   @return Void
*/
void vCollectData(SAPMS_t * psSAPMS, SAPMS_e ESAPMS);

#endif  //_PROCESS_H_
