/** @file AM2320.c
 *  @brief This is the main file for I2C.
 *          Currently it is also for AM2320
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "AM2320.h"

/*******************************************************************************
* Static Global Variables
*******************************************************************************/

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
/** @name 	bCheckCRC16
*   @brief  statcic function to only be used in this file.
*              - Calculate the 16bit CRC according to the Datasheet.
*               
*   @param 	Void
*   @return 16bit CRC calculation 
*/
static uint16_t bCheckCRC16(uint8_t * ptrBuff, uint8_t lenBuff);

/*******************************************************************************
* Function Definition
*******************************************************************************/
/**
	*	@name vTask_AM23200
	*   @Type Void Task 
*/
void vTask_AM2320(void * pvParameters)
{
    const uint8_t readCmd[] = {0x03,0x00,0x04};
    const uint8_t wakeCmd = 0x00;
    uint16_t CRC = 0;
    int state = PICO_OK;
    SAPMS_t sAMS = {0};
    while(true)
    {
        uint8_t cbuff[8] = {0};
        state = PICO_OK;
        // Wake-up device
        i2c_write_blocking(AM_I2C,AM_ADR,&wakeCmd,1,false);
        // Read four registers starting from zero.
        state |= i2c_write_blocking(AM_I2C,AM_ADR,readCmd,3,false);
        // Get readings. 
        state |= i2c_read_blocking(AM_I2C,AM_ADR,cbuff,8,false);

        if(PICO_OK <= state)
        {
            CRC = cbuff[7]<<8|cbuff[6];
            if(CRC == bCheckCRC16(cbuff,sizeof(cbuff)-2))
            {
                sAMS.sAM.fHum = (cbuff[2]<<8|cbuff[3])/10.0;
                sAMS.sAM.fTemp = (cbuff[4]<<8|cbuff[5])/10.0;   
                vCollectData(&sAMS,EAM);
            }
        }
        
        vTaskDelay(AM_CYCLE_T/portTICK_PERIOD_MS);
    }
}

/**
*	@name  vSetupAM2320
*   @Type  function
*/
void vSetupAM2320(void)
{
#if USE_AM2320 == 1
    // Initialize the I2C peripheral for the AM2320
    i2c_init(AM_I2C, AM_I2C0_FRQ);

    // Initialize the GPIO peripherals in I2C mode.
    gpio_set_function(AM_I2C0_SDA, GPIO_FUNC_I2C);
    gpio_set_function(AM_I2C0_SCL, GPIO_FUNC_I2C);
#endif
}

static uint16_t bCheckCRC16(uint8_t * ptrBuff, uint8_t lenBuff)
{
    uint16_t crc = 0xFFFF;
    uint8_t sizeBuff = lenBuff;
    while (sizeBuff--)
    {
        crc ^= *ptrBuff++;
        for(int idxBuff = 0; idxBuff < 8; idxBuff++)
        {
            if(crc & 0x01)
            {
                crc >>= 1;
                crc ^= 0xA001; //Datasheet value for CRC calculation.
            }
            else
            {
                crc >>= 1;
            }
        }
    }
    return crc;
}
