/** @file PMS5003.h
 *  @brief The file contains the declaration for the PMS5003 sensor including  
 *          UART setup, PMS5003 RTOS Task and PMS specific functions for 
 *          configuratioin and getting data.   
 *
 *  @author Jose Jorge Figueroa Figueroa
 */

#ifndef _PMS5003_H_
#define _PMS5003_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define PM_MSG_LEN   (32)   /*!< PMS msg including start and size bytes */
#define PM_DATA_SIZE (28)   /*!< PMS data size              */ 

#define PM_USART     (uart1)  /*!< UART interface            */
#define PM_USART_TXD (8)      /*!< RP Pico TX pin            */
#define PM_USART_RXD (9)      /*!< RP Pico RX pin            */
#define PM_BAUDRATE  (9600)   /*!< Baudrate                  */

#define PM_USART_INT (0)      /*!< Not in use / Not Avilable */

/*******************************************************************************
* Type definitions
*******************************************************************************/
/**
 * @name PMS_t
 * @type struct
 * @brief This struct has is for handling the uart PMS5003 messages.
 */
typedef struct valuesPMS
{
/*@{*/
    uint8_t ubRaw[PM_MSG_LEN];  /*!< Raw PMS5003 Uart Message */
    uint8_t ucSize;             /*!< Size of Message          */
    uint16_t uhwCheckSum;       /*!< CheckSum;                */
    uint16_t uhwPM1;            /*!< PM 1.0 ug/m3 Atm env.	  */
    uint16_t uhwPM2_5;          /*!< PM 2.5 ug/m3 Atm env.    */
    uint16_t uhwPM10;           /*!< PM 10. ug/m3 Atm env.    */
/*@}*/
}PMS_t;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name  vSetupPMS5003();
*   @brief Setup the PM_USART configuration and pins at PM_USART_TXD and PM_USART_RXD
*
*   @param 	Void
*   @return Void
*/
void vSetupPMS5003(void);

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
