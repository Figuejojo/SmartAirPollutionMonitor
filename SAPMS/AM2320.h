/** @file bapiI2C0.h
 *  @brief This is the main file for I2C.
 *          Currently it is also for AM2320 
 *          TODO: Verify if the current structure is good or
 *                  Create an independent folder for AM2320
 *
 *  @author Y3913624
 */

#ifndef _bapi_I2C0_H_
#define _bapi_I2C0_H_

#include "common.h"
#include "hardware/i2c.h"


#define AM2320_SDA (4)  // Raspberry Pi Pico Pin for SDA
#define AM2320_SCL (5)  // Raspberry Pi Pico Pin for SCL
#define AM2320_I2C_ADR () //AM2320 I2C ADDRESS

/** @name 	vTask_AM2320
  *	@brief 	Task fro the AM2320 module (Temperature and Humidity sensor)
  *
  * @param 	Void
  * @return Void
  */
void vTask_AM2320(void * pvParameters);

#endif //_bapi_I2C0_H_
