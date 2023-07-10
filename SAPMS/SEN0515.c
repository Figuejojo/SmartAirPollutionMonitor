/** @file ADA746.c
 *  @brief  //Write a brief description 
 *
 *  @author //Exam Id
 */
#include "SEN0515.h"

/*******************************************************************************
* Static & Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Definition
*******************************************************************************/

/**
*	@name vTaskGPS
*   @type Task
*/
void vTaskSEN0515(void * pvParameters)
{
    //uint8_t msg[] = "TVOC";
    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
        //Print_debug(msg);
    }
}

/**
*	@name  setupSEN0515
*   @Type  Setup functon
*/
void setupSEN0515(void)
{
#if USE_SEN0515 == 1
//Implementation here
#endif
}