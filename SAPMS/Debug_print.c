/** @file Debug_print.c
 *  @brief  The file contains the print function definitions for debugging thruogh 
 *           the USB in Raspberry pi pico, without bloquing the program. 
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "Debug_print.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/
QueueHandle_t qDebugPrint;

/*******************************************************************************
* Function Definition
*******************************************************************************/

/**
*	@name TaskDebugPrint
*   @type Task
*/
void TaskDebugPrint(void * pvParameters)
{   
#if ENABLE_DEBUG == 1 
    int8_t msg[DEBUGMSG_SIZE];
    while (true) 
    {
        xQueueReceive(qDebugPrint,msg,portMAX_DELAY);
        printf("%s\r\n",msg);
    }
#endif
}

/**
*	@name  Debug_Print
*   @Type  function
*/
void Print_debug(int8_t msg[])
{   
#if ENABLE_DEBUG == 1 
    int8_t buff[DEBUGMSG_SIZE] = {0};
    sprintf(buff,"%s",msg);
    xQueueSendToBack(qDebugPrint,&buff,0);
#endif
}

/**
*	@name  debug_queue_setup
*   @Type  Setupt functon
*/
void debug_queue_setup(void)
{
#if ENABLE_DEBUG == 1
    stdio_init_all();
    //*** Queues Creation
    qDebugPrint = xQueueCreate(10, sizeof(int8_t)*DEBUGMSG_SIZE);
#endif
}
