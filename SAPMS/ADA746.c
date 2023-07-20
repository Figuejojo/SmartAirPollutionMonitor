/** @file ADA746.c
 *  @brief  //Write a brief description 
 *
 *  @author //Y3910898
 */
#include "ADA746.h"

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
void vTaskGPS(void * pvParameters)
{
    
    while(1)
    {
        
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

/**
*	@name  GPS_setup
*   @Type  Setup functon
*/
void GPS_setup(void)
{
#if USE_ADA746 == 1
    // Uart initialization
    uart_init(ADA_USART, ADA_BAUD);
    // Uart pin setup
    gpio_set_function(ADA_TX, GPIO_FUNC_UART);
    gpio_set_function(ADA_RX, GPIO_FUNC_UART);
#endif
}