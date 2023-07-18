/** @file sen0515.c
 *  @brief  // Setting up the I2C1 peripheral in the Raspberry Pi Pico to be used for the SEN0515/ENS160
 *
 *  @author //Exam Id
 */
#include "SEN0515.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C_PORT i2c1
#define I2C_SDA_PIN 18
#define I2C_SCL_PIN 19
#define I2C_FREQUENCY 100000 // I2C bus frequency in Hz

/**
*	@name vTaskGPS
*   @type Task
*/
void vTaskSEN0515(void * pvParameters)
{
    //uint8_t msg[] = "TVOC";
    while(1)
    {
#if 0 
        // Example: Read data from SEN0515/ENS160
        uint8_t reg_address = 0x00; // Change this to the desired register address

        // Start the I2C transaction
        i2c_write_blocking(I2C_PORT, device_address, &reg_address, 1, true);

        // Read data from SEN0515/ENS160
        uint8_t data[2];
        i2c_read_blocking(I2C_PORT, device_address, data, sizeof(data), false);

        // Process the data (e.g., print it)
        printf("Data: 0x%02X%02X\n", data[0], data[1]);

        
        vTaskDelay(1000/portTICK_PERIOD_MS);
        //Print_debug(msg);
#endif
    }
}

/**
*	@name  setupSEN0515
*   @Type  Setup functon
*/
void setupSEN0515(void)
{
#if USE_SEN0515 == 1
    i2c_init(I2C_PORT, I2C_FREQUENCY);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SCL_PIN);
    gpio_pull_up(I2C_SCL_PIN);  

  #endif
}