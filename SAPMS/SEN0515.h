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
* Type definitions
*******************************************************************************/
#define ENS_I2C       (i2c1)
#define ENS_I2C1_SDA  (18)  // Raspberry Pi Pico Pin for TXD
#define ENS_I2C1_SCL  (19)  // Raspberry Pi Pico Pin for RXD

#define ENS_I2C1_FREQ (400*1000)  // I2C bus frequency in Hz 4K for speed mode.

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
