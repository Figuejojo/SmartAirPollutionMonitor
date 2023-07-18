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
#define ENS_DATA_S (0x20)
//#define ENS_TVOC_D (0x22)

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
/** @name 	eCheckEnsID
*   @brief  Statuc function to only be used in this file.
*              - This is the initial state function.
*              - Request the sensor ID and verify that is correct.
*              - Returns the next state.
*               
*   @param 	Void
*   @return E_ENS_STATES 
*/
static E_ENS_STATES eCheckEnsID(void);

/** @name 	eCheckEnsST
*   @brief  statcic function to only be used in this file.
*              - Request the sensor State acording to the datasheet:
*                    (WarmUp, Initial Start-Up, Normal Operation).
*              - Returns the next state.
*               
*   @param 	Void
*   @return E_ENS_STATES 
*/
static E_ENS_STATES eCheckEnsST(void);

/** @name 	eGetEnsVal
*   @brief  statcic function to only be used in this file.
*              - Get values from the ENS sensor including:
*                  (TVOC, eCO2 and AQI)
*              - Returns the next state.
*               
*   @param 	Void @todo: Implement values by ptr 
*   @return E_ENS_STATES 
*/
static E_ENS_STATES eGetEnsVal(void);
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
    uint16_t CycleTimeMs = 1000;
    E_ENS_STATES eEnsState = EENS_CHECK_ID;
    while(1)
    {
        switch (eEnsState)
        {
            case EENS_CHECK_ID:
                eEnsState = eCheckEnsID();
                if(eEnsState == EENS_CHECK_ID)
                {
                    Print_debug("ENS-Err ID");
                    CycleTimeMs = 1000;
                }
                break;
            case EENS_CHECK_ST:
                eEnsState = eCheckEnsST();
                Print_debug("ENS Status");
                CycleTimeMs = (eEnsState == EENS_CHECK_ST)?(1000):(1000);
                break;

            case EENS_NORMAL:
                Print_debug("ENS Normal");
                CycleTimeMs = 1000;
                break;

            default:
                CycleTimeMs = 1000;
                eEnsState = EENS_CHECK_ID;
                break;
        }
        vTaskDelay(CycleTimeMs/portTICK_PERIOD_MS);
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

E_ENS_STATES eCheckEnsID(void)
{
    uint8_t const IDcmd = ENS_CMD_ID;
    uint8_t cBuff[2] = {0};
    int state = i2c_write_blocking(ENS_I2C, ENS_I2C_ADDR, &IDcmd, 1, true);    
    if(PICO_OK <= state)
    {
        i2c_read_blocking(ENS_I2C, ENS_I2C_ADDR, cBuff, 2, false);
        if(0x0160 != (cBuff[0]|cBuff[1]<<8))
        {
            Print_debug("ENS - No Maching ID");
            return EENS_CHECK_ID;//ER_I2C_ID;
        }
    }
    else
    {
        return EENS_CHECK_ID;//ER_I2C_GEN;
    }
    return EENS_CHECK_ST;
}

E_ENS_STATES eCheckEnsST(void)
{
    uint8_t const IDcmd = ENS_DATA_S;
    uint8_t cBuff = 0;
    uint8_t msg[25];
    int state = i2c_write_blocking(ENS_I2C, ENS_I2C_ADDR, &IDcmd, 1, true);    
    if(PICO_OK <= state)
    {
        i2c_read_blocking(ENS_I2C, ENS_I2C_ADDR, &cBuff, 1, false);
        if(0 == (cBuff & (3u<<2)))
        {
            return EENS_NORMAL;
        }
    }
    return EENS_CHECK_ST;
}

E_ENS_STATES eGetEnsVal(void)
{

}

