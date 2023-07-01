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
* Static Function Declarations
*******************************************************************************/
/** @name  svReadPM5003();
*   @brief Request and Read PMS5003 data.
*   @param Void
*   @return Void
*/
static void svReadPM5003(void);

/** @name  svConfigPMS5003();
*   @brief Passive Mode Configuration to PMS5003.
*   @param Void
*   @return Void
*/
static void svConfigPMS5003(void);

/*******************************************************************************
* Function Definition
*******************************************************************************/
/**
*	@name vTaskPMS5003
*   @type Task
*/
void vTaskPMS5003(void * pvParameters)
{
    uint8_t buff[28] = {0};
    svConfigPMS5003();
    while(1)
    {
        svReadPM5003();
        vTaskDelay(5000/portTICK_PERIOD_MS);
    }
}

/**
*	@name svReadPM5003
*   @type Task
*/
void svReadPM5003(void)
{
    const uint8_t MsgSize = 7;
    const uint8_t RequestData[] = {0x42,0x4D,0xE2,0x00,0x00,0x01,0x71};
    for(uint8_t MsgCont = 0; MsgCont < MsgSize; MsgCont++)
    {
        taskENTER_CRITICAL();
        uart_putc(PM_USART, RequestData[MsgCont]);
        taskEXIT_CRITICAL();
    }
}

/**
*	@name PMS5003_config
*   @type Task
*/
void svConfigPMS5003(void)
{
    const uint8_t MsgSize = 7;
    const uint8_t PassiveMode[] = {0x42,0x4D,0xE1,0x00,0x00,0x01,0x70};
    taskENTER_CRITICAL();
    for(uint8_t MsgCont = 0; MsgCont < MsgSize; MsgCont++)
    {
        uart_putc(PM_USART,PassiveMode[MsgCont]);
    }
    taskEXIT_CRITICAL();
}

/**
*	@name vSetupPMS5003
*   @type function
*/
void vSetupPMS5003(void)
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
