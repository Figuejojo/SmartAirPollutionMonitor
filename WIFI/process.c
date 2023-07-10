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
    
}

