/** @file Debug PMS5003.c
 *  @brief The file contains the definition for the PMS5003 sensor including  
 *          UART setup, PMS5003 RTOS Task and PMS specific functions for 
 *          configuratioin and getting data.   
 *
 *  @author Y3913624
 */

/*******************************************************************************
* Includes
*******************************************************************************/
#include "PMS5003.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/

/*******************************************************************************
* Function Definition
*******************************************************************************/
/**
*	@name vTaskPMS5003
*   @type Task
*/
void vTaskPMS5003(void * pvParameters)
{
    while(1)
    {
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

/**
*	@name PMS5003_setup
*   @type function
*/
void PMS5003_setup(void)
{
#if USE_PMS5003 == 1
    //TODO: Use Asserts!! 

    // Uart initialization
    uart_init(PM_USART, PM_BAUDRATE);
    // Uart pin setup
    gpio_set_function(PM_USART_TXD, GPIO_FUNC_UART);
    gpio_set_function(PM_USART_RXD, GPIO_FUNC_UART);

#if PM_USART_INT == 1   // Initialize INT UART RX
    // And set up and enable the interrupt handlers
    int UART_IRQ = PM_USART == uart0 ? UART0_IRQ : UART1_IRQ;
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);  
#endif

#endif
}
