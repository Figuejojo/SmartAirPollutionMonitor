/** @file AM2320.h
 *  @brief This is the main file for I2C.
 *          Currently it is also for AM2320 
 *
 *  @author Jose Jorge Figueroa Figueroa
 */

#ifndef _bapi_I2C0_H_
#define _bapi_I2C0_H_

/*******************************************************************************
* Includes
*******************************************************************************/
#include "common.h"
#include "hardware/i2c.h"

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
#define AM_CYCLE_T  (60000)  /*!< Data Collection time[ms] */

#define AM_I2C      (i2c0)  /*!< I2C channel */
#define AM_I2C0_SDA (12)    /*!< Raspberry Pi Pico Pin for SDA */ 
#define AM_I2C0_SCL (13)    /*!< Raspberry Pi Pico Pin for SCL */

#define AM_ADR      (0x5C)  /*!< AM2320 I2C ADDRESS  0xB8  5C*/
#define AM_I2C0_FRQ (100000) /*!< I2C bus frequency in 100K Hz for std mode */

/*******************************************************************************
* Type definitions
*******************************************************************************/

/*******************************************************************************
* Function Prototypes
*******************************************************************************/
/** @name 	vTask_AM2320
  *	@brief 	Task fro the AM2320 module (Temperature and Humidity sensor)
  *
  * @param 	Void
  * @return Void
  */
void vTask_AM2320(void * pvParameters);

/** @name 	vSetupAM2320
*   @brief  Setup the i2C1 for the AM2320
*              - Pico I2C Pin setup.
*              - Pico I2C Frequency setup as define above.
*
*   @param 	Void
*   @return Void
*/
void vSetupAM2320(void);

#endif //_bapi_I2C0_H_
