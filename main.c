/** @file Main.c
 *  @brief This is the main file of the Smart Air Pollution Monitor.
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "AM2320.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
QueueHandle_t qDebugPrint;

/*******************************************************************************
* Function Declaration
*******************************************************************************/
void TaskDebugPrint(void * pvParameters);
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

    //*** Queues Creation
    qDebugPrint = xQueueCreate(10, sizeof(int8_t)*DEBUGMSG_SIZE);

    //*** FreeRTOS tASKS ***/
    xTaskCreate(vTask_AM2320,"TempAndHumd",256,NULL,1,NULL);
    xTaskCreate(TaskLEDBlinkvoid,"Ledblink",256,NULL,1,NULL);
#if ENABLE_DEBUG // Only if the debug flag is set.
    xTaskCreate(TaskDebugPrint, "DebugUSBPrint", 256, NULL, 1, NULL);
#endif

    //Start FreeRTOS
    vTaskStartScheduler();

    while(1){};
}

/**
	*	@name Debug_Print
	*   @Type Task
*/
void TaskDebugPrint(void * pvParameters)
{   
    stdio_init_all();
    int8_t msg[DEBUGMSG_SIZE];
    while (true) {
        xQueueReceive(qDebugPrint,msg,portMAX_DELAY);
        printf("%s\r\n",msg);
    }
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
    int8_t msg[] = "DebugCycle";
    while(true)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_put(LED_PIN,1);
        vTaskDelay(500/portTICK_PERIOD_MS);
        gpio_put(LED_PIN,0);
        xQueueSendToBack(qDebugPrint,&msg,0);
    }
}
