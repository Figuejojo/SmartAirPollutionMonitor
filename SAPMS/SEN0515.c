/** @file sen0515.c
 *  @brief  // Setting up the I2C1 peripheral in the Raspberry Pi Pico to be used for the SEN0515/ENS160
 *
 *  @author //Exam Id
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "SEN0515.h"
#include "hardware/i2c.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/

/*******************************************************************************
* Function Definition
*******************************************************************************/

/**
*	@name vTaskGPS
*   @type Task
*/
void vTaskSEN0515(void * pvParameters)
{
    uint8_t msg[] = "TVOC";
    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
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
#endif
        Print_debug(msg);
    }
}

/**
*	@name  setupSEN0515
*   @Type  TVOC and CO2 sensor setup functon
*/
void setupSEN0515(void)
{
#if USE_SEN0515 == 1
    i2c_init(ENS_I2C, ENS_I2C1_FREQ);
    gpio_set_function(ENS_I2C1_SDA, GPIO_FUNC_I2C);
    gpio_set_function(ENS_I2C1_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(ENS_I2C1_SDA);
    gpio_pull_up(ENS_I2C1_SCL);  
  #endif
}