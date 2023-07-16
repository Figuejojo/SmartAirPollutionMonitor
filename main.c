/** @file Main.c
 *  @brief This is the main file of the Smart Air Pollution Monitor.
 *
 *  @author Y3913624
 */

/** @todo: assigned task priorities. */ 

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "wifi_mqtt.h"
#include "AM2320.h"
#include "ADA746.h"
#include "PMS5003.h"
#include "SEN0515.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Declaration
*******************************************************************************/
/** @name  TaskLEDBlinkvoid
*   @brief Using the Pin3 with a LED as Heartbeat for knowing if the program is 
*           still executing. Works as an example for other tasks.
*
*   @param 	Void * pvParameters
*/
void TaskLEDBlinkvoid(void * pvParameters);

/*******************************************************************************
* Function Definition
*******************************************************************************/

/**
	*	@name main
	*   @Type Main Function
*/
int main()
{
    //*** Hardware Initializations (If needed) ***/
    
    //*** Queues Creation and setup  
    debug_queue_setup();
    GPS_setup();
    vSetupPMS5003();
    setupSEN0515();
    vSetupProcess();
    ERR_t errWifi = vSetupWifi();

    
    /*****************
    * FreeRTOS tASKS *
    ******************/

   //These tasks are always created.
    xTaskCreate(TaskLEDBlinkvoid,"Ledblink",256,NULL,1,NULL);
    xTaskCreate(vTaskProcess,"DataProcess",256,NULL,1,NULL);

    // These tasks can be turn on/off by using the macros on common.h file.
#if USE_WIRELESS == 1
    if(NO_ERROR == errWifi)
    {
        xTaskCreate(vTaskWireless,"Wireless",256,NULL,1,NULL);
    }
#endif
#if USE_ADA746 == 1
    xTaskCreate(vTaskGPS,"GPSTask",256,NULL,1,NULL);
#endif
#if USE_AM2320 == 1
    xTaskCreate(vTask_AM2320,"TempAndHumd",256,NULL,1,NULL);
#endif
#if USE_PMS5003 == 1
    xTaskCreate(vTaskPMS5003,"PM2and10um",256,NULL,1,NULL);
#endif
#if USE_SEN0515 == 1
    xTaskCreate(vTaskSEN0515,"TVOC&CO2",256,NULL,1,NULL);
#endif
#if ENABLE_DEBUG // Only if the debug flag is set.
    xTaskCreate(TaskDebugPrint, "DebugUSBPrint", 256, NULL, 1, NULL);
#endif

    //Start FreeRTOS
    vTaskStartScheduler();

    while(1){};
}

/**
	*	@name Debug_LEDBlink 
	*   @Type Example Task
*/
void TaskLEDBlinkvoid(void * pvParameters)
{
    const uint8_t LED_PIN = 3;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    int8_t msg[] = "DebugCycleYES";
    while(true)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_put(LED_PIN,1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_put(LED_PIN,0);
        Print_debug(msg);
    }
}
