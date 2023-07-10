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
static void svReadPM5003(PMS_t * pPMS);

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
    PMS_t svPMS = {0};
    //According to the datasheet the sensor will have stable data after 30 sec
    vTaskDelay(28000/portTICK_PERIOD_MS);
    while(1)
    {
        vTaskDelay(2000/portTICK_PERIOD_MS);
        svConfigPMS5003();
        vTaskDelay(3000/portTICK_PERIOD_MS); 
        svReadPM5003(&svPMS);
    }
}

/**
*	@name svReadPM5003
*   @type Task
*/
static void svReadPM5003(PMS_t * pPMS)
{
    memset(pPMS,0,sizeof(PMS_t));   

    const uint8_t MsgSize = 7;
    const uint8_t RequestData[] = {0x42,0x4D,0xE2,0x00,0x00,0x01,0x71};
    
    //* Critical task so message send and request are not interrupted *//
    taskENTER_CRITICAL();
    //Request Passive Mode
    for(uint8_t MsgCont = 0; MsgCont < MsgSize; MsgCont++)
    {
        uart_putc(PM_USART, RequestData[MsgCont]);
    }

    //Read Mesage after request in Passive Mode.
    uart_read_blocking(PM_USART,pPMS->ubRaw,PM_MSG_LEN);
    taskEXIT_CRITICAL();

    //Parse of the message, checksum verification and data aquired. 
    if(0x42 == pPMS->ubRaw[0] && 0x4D == pPMS->ubRaw[1])
    {
        pPMS->ucSize = pPMS->ubRaw[2]<<16 | pPMS->ubRaw[3]; 
        if(pPMS->ucSize == PM_DATA_SIZE)
        {
           //__breakpoint();   //Used to force break points when optimized. 
        }
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
    for(uint8_t MsgCont = 0; MsgCont < MsgSize; MsgCont++)
    {
        uart_putc(PM_USART,PassiveMode[MsgCont]);
    }
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
