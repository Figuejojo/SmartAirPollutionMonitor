/** @file bapiI2C0.c
 *  @brief This is the main file for I2C.
 *          Currently it is also for AM2320 
 *          TODO: Verify if the current structure is good or
 *                  Create an independent folder for AM2320
 *
 *  @author Y3913624
 */

#include "AM2320.h"

extern QueueHandle_t qDebugPrint;

/**
	*	@name vTask_AM23200
	*   @Type Void Task 
*/
void vTask_AM2320(void * pvParameters)
{
    #if 0
    i2c_inst_t *AM2320_interface = i2c0;
    i2c_init(AM2320_interface, 400 * 1000);

    gpio_set_function(AM2320_SDA, GPIO_FUNC_I2C);
    gpio_set_function(AM2320_SCL, GPIO_FUNC_I2C);
    #endif

    while(true)
    {
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}