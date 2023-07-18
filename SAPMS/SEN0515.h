/** @file SEN0515.h
 *  @brief TVOC and CO2 sensor
 *          Containts the definitions for SEN0515/ENS160
 *
 *  @author "Exam Number"
 */

#ifndef _SEN0515_H_
#define _SEN0515_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define ENS_I2C       (i2c1)
#define ENS_I2C1_SDA  (18)  // Raspberry Pi Pico Pin for TXD
#define ENS_I2C1_SCL  (19)  // Raspberry Pi Pico Pin for RXD

#define ENS_I2C_ADDR  (0x53)

#define ENS_I2C1_FREQ (100000)  // I2C bus frequency in Hz 48K for speed mode.

/*******************************************************************************
* Type definitions
*******************************************************************************/
typedef enum ENS_States
{
    EENS_CHECK_ID = 0,
    EENS_CHECK_ST = 1,
    EENS_NORMAL   = 2,
    EENS_END,
}E_ENS_STATES;

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	setupSEN0515
*   @brief  Setup the i2C1 for the SEN0515/ENS160
*              - Pico I2C Pin setup.
*              - Pico I2C Frequency setup as define above.
*
*   @param 	Void
*   @return Void
*/
void setupSEN0515(void);

/** @name 	vTaskSEN0515
*   @brief  Collect data from the SEN0515/ENS160 sensor
*               - Every 15sec
*               - Send data out by using the process task. 
*
*   @param 	Void * parameters
*   @return Void
*/
void vTaskSEN0515(void * pvParameters);



#endif //_SEN0515_H_
