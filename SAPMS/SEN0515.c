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
#define ENS_CMD_ID (0x00)

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
    uint8_t msg[25] = {0};
    uint8_t cBuff[4] = {0};
    while(1)
    {
        uint8_t cmd = ENS_CMD_ID;
        int state = i2c_write_blocking(ENS_I2C, ENS_I2C_ADDR, &cmd, 1, true);
        if(PICO_OK <= state)
        {
            i2c_read_blocking(ENS_I2C, ENS_I2C_ADDR, cBuff, 2, false);
            sprintf(msg,"TVOC - %x",cBuff[0]|cBuff[1]<<8);
        }
        else
        {
            sprintf(msg,"TVOC - ERROR %d",state);
        }
        //sprintf(msg,"%x %x",cBuff[0],cBuff[1]);
        Print_debug(msg);
        vTaskDelay(1000/portTICK_PERIOD_MS);
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