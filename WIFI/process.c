/** @file process.c
 *  @brief  The file contains the definitions for processing the sensors data
 *              and sending the information to the WIFI thread.
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "process.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
static QueueHandle_t sgqSensorData; /*!< SensorData FreeRTOS Queue */

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskProcess(void * pvParameters)
{
    SAPMS_t xQdata = {0};
    SAPMS_t cacheData = {0};
    uint8_t msg[30] = {0};
    while(1)
    {
        xQueueReceive(sgqSensorData,&xQdata,portMAX_DELAY);
        switch (xQdata.eSRC)
        {
            case EPMS:
                cacheData.sPM.fPM1 =  (cacheData.sPM.fPM1 + xQdata.sPM.fPM1)/2;
                cacheData.sPM.fPM25 = (cacheData.sPM.fPM25 + xQdata.sPM.fPM25)/2;
                cacheData.sPM.fPM10 = (cacheData.sPM.fPM10 + xQdata.sPM.fPM10)/2;
                sprintf(msg,"PM1 %0.1f ,PM2.5 %0.1f , PM5 %0.1f",cacheData.sPM.fPM1,cacheData.sPM.fPM25,cacheData.sPM.fPM10);
                break;
            
            case EENS:
                cacheData.sENS.fCO2 = (cacheData.sENS.fCO2 + xQdata.sENS.fCO2)/2;
                cacheData.sENS.fTVOC = (cacheData.sENS.fTVOC + xQdata.sENS.fTVOC)/2;
                sprintf(msg,"CO2 %0.1f ,TVOC %0.1f",cacheData.sENS.fCO2, cacheData.sENS.fTVOC);
                break;

            case EAM:
                cacheData.sAM.fHum = (cacheData.sAM.fHum + xQdata.sAM.fHum)/2;
                cacheData.sAM.fTemp = (cacheData.sAM.fTemp + xQdata.sAM.fTemp)/2;
                sprintf(msg,"Hum %0.1f ,Temp %0.1f",cacheData.sAM.fHum, cacheData.sAM.fTemp);
                break;

            default:
                sprintf(msg,"No New Data\n");
                break;
        }
        Print_debug(msg);
    }
}

/**
*	@name vSetupWifi
*   @type function
*/
void vSetupProcess(void)
{
    sgqSensorData = xQueueCreate(PROC_QUEUE_SZ, sizeof(SAPMS_t));
}

/**
*	@name vCollectData
*   @type function
*/
void vCollectData(SAPMS_t * psSAPMS, SAPMS_e ESAPMS)
{
    psSAPMS->eSRC = ESAPMS;
    xQueueSendToBack(sgqSensorData,psSAPMS,0);
}
