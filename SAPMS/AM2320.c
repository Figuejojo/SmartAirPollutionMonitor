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
    const uint8_t ReadCmd[] = {0x03,0x00,0x04};
    const uint8_t WakeCmd = 0x00;
    int state = PICO_OK;
    float fHumidity = 0;
    float fTemp = 0;
    while(true)
    {
        uint8_t cbuff[8] = {0};
        state = PICO_OK;
        // Wake-up device
        i2c_write_blocking(AM_I2C,AM_ADR,&WakeCmd,1,false);
        // Read four registers starting from zero.
        state |= i2c_write_blocking(AM_I2C,AM_ADR,ReadCmd,3,false);
        // Get readings. 
        state |= i2c_read_blocking(AM_I2C,AM_ADR,cbuff,8,false);

        if(PICO_OK <= state)
        {
            fHumidity = (cbuff[2]<<8|cbuff[3])/10.0;
            fTemp = (cbuff[4]<<8|cbuff[5])/10.0;
        }
        
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
    //gpio_pull_up(AM_I2C0_SDA);
    //gpio_pull_up(AM_I2C0_SCL);
#endif
}
