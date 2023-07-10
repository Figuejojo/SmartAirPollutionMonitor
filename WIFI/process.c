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
    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
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
