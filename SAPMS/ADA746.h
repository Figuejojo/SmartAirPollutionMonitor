/** @file ADA746.h
 *  @brief GPS header file
 *          Containts the definitions for the GPS.
 *
 *  @author "Exam Number"
 */

#ifndef _ADA746_H_
#define _ADA746_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Type definitions
*******************************************************************************/
#define ADA_TX (0)  // Raspberry Pi Pico Pin for TXD
#define ADA_RX (1)  // Raspberry Pi Pico Pin for RXD

#define ADA_BAUD  (9600)

#define ADA_USART (uart0)  /*!< UART interface            */

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	GPS_setup
*   @brief  To setup the Pin modes for the GPS
*
*   @param 	Void
*   @return Void
*/
void GPS_setup(void);

/** @name 	Todo
*   @brief //Brief description Todo
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskGPS(void * pvParameters);



#endif //_ADA746_H_
