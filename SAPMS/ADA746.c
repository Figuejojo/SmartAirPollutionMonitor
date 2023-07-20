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
    const uint8_t cStandByCmd[] = {0x24,0x50,0x4D,0x54,0x4B,0x31,0x36,0x31,0x2C,0x30,0x2A,0x32,0x38,0x0D,0x0A,0xFE};
    //const uint8_t cStandByCmd[] = {'$','P','M','T','K','1','6','1','0','*','2','8','\r','\n'};
    while(1)
    {
        const uint8_t * ptrCMD = cStandByCmd;
        while(*ptrCMD!=0xFE)
        {
            uart_putc(ADA_USART, *ptrCMD);
            ptrCMD++;  
        }
        vTaskDelay(5000/portTICK_PERIOD_MS);
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