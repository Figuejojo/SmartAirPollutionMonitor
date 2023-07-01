/** @file Debug PMS5003.h
 *  @brief The file contains the declaration for the PMS5003 sensor including  
 *          UART setup, PMS5003 RTOS Task and PMS specific functions for 
 *          configuratioin and getting data.   
 *
 *  @author Y3913624
 */

#ifndef _PMS5003_H_
#define _PMS5003_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define PM_USART_INT (0)

#define PM_USART     (uart1)  // UART interface
#define PM_USART_TXD (8)      // RP Pico TX pin
#define PM_USART_RXD (9)      // RP Pico RX pin
#define PM_BAUDRATE  (9600)   // Baudrate
/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	PMS5003_setup();
*   @brief Setup the UART1 configuration and pins at GP# and GP#
*
*   @param 	Void
*   @return Void
*/
void PMS5003_setup(void);

/** @name   vTaskPMS5003
*   @brief  PMS5003 taks dedicated to aquire the sensor data.
*               Runs Every XXXXs
*               Uses XXX_Queue to send sensor information.    
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskPMS5003(void * pvParameters);

#endif //_PMS5003_H_
