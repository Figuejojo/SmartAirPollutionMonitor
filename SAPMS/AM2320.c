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


    while(true)
    {
        vTaskDelay(2000/portTICK_PERIOD_MS);
    }
}

void vSetupAM2320(void)
{
#if USE_AM2320 == 1
    // Initialize the I2C peripheral for the AM2320
    i2c_init(AM_I2C, AM_I2C0_FRQ);

    // Initialize the GPIO peripherals in I2C mode.
    gpio_set_function(AM_I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(AM_I2C0_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(AM_I2C0_SDA);
    gpio_pull_up(AM_I2C0_SCL);
#endif
}
