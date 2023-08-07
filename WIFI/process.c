/** @file process.c
 *  @brief  The file contains the definitions for processing the sensors data
 *              and sending the information to the WIFI thread.
 *
 *  @author Jose Jorge Figueroa Figueroa
 */
/*******************************************************************************
* Includes
*******************************************************************************/
#include "process.h"
#include "iot_configs.h"
#include "wifi_mqtt.h"
/*******************************************************************************
* Static and/or Global Variables
*******************************************************************************/
static QueueHandle_t sgqSensorData; /*!< SensorData FreeRTOS Queue */

/*******************************************************************************
* Static Function Declarations
*******************************************************************************/
static void alarm_irq(void);
static void alarm_in_us(uint32_t delay_us);

/*******************************************************************************
* Function Definition
*******************************************************************************/
/**
*	@name vTaskWireless
*   @type Task
*/
void vTaskProcess(void * pvParameters)
{
    SAPMS_t xQdata = {0};
    SAPMS_t cacheData = {0};
    uint8_t msg[200] = {0};
    uint8_t minutes = 0;
    //uint8_t JSON[500];
    alarm_in_us(TIME2WIFI); 
    while(1)
    {
        xQueueReceive(sgqSensorData,&xQdata,portMAX_DELAY);
        switch (xQdata.eSRC)
        {
            case EPMS:
                cacheData.sPM.fPM1 =  (cacheData.sPM.fPM1 + xQdata.sPM.fPM1)/2;
                cacheData.sPM.fPM25 = (cacheData.sPM.fPM25 + xQdata.sPM.fPM25)/2;
                cacheData.sPM.fPM10 = (cacheData.sPM.fPM10 + xQdata.sPM.fPM10)/2;
                sprintf(msg,"PM1 %0.1f ,PM2.5 %0.1f , PM5 %0.1f",cacheData.sPM.fPM1,cacheData.sPM.fPM25,cacheData.sPM.fPM10);
                break;
            
            case EENS:
                cacheData.sENS.fCO2 = (cacheData.sENS.fCO2 + xQdata.sENS.fCO2)/2;
                cacheData.sENS.fTVOC = (cacheData.sENS.fTVOC + xQdata.sENS.fTVOC)/2;
                sprintf(msg,"CO2 %0.1f ,TVOC %0.1f",cacheData.sENS.fCO2, cacheData.sENS.fTVOC);
                break;

            case EAM:
                cacheData.sAM.fHum = (cacheData.sAM.fHum + xQdata.sAM.fHum)/2;
                cacheData.sAM.fTemp = (cacheData.sAM.fTemp + xQdata.sAM.fTemp)/2;
                sprintf(msg,"Hum %0.1f ,Temp %0.1f",cacheData.sAM.fHum, cacheData.sAM.fTemp);
                break;

            case EWIFI:
                minutes++; 
                alarm_in_us(TIME2WIFI);
                if(minutes < 5)
                {
                    break;
                }
                minutes = 0;
#if (MQTT_TS==1)
                if(0 != cacheData.sAM.fHum)
                {
                    sprintf(msg,"field1=%0.2f&field2=%0.2f",cacheData.sAM.fTemp,cacheData.sAM.fHum);
                    cacheData.sAM.fTemp = 0;
                    cacheData.sAM.fHum = 0;
                }
                if(0 != cacheData.sPM.fPM1)
                {
                    if(msg[0] == 'f')
                    {
                        sprintf(msg,"%s&field3=%0.2f&field4=%0.2f&field5=%0.2f",msg,cacheData.sPM.fPM1,cacheData.sPM.fPM25,cacheData.sPM.fPM10);
                    }
                    else
                    {
                        sprintf(msg,"field3=%0.2f&field4=%0.2f&field5=%0.2f",cacheData.sPM.fPM1,cacheData.sPM.fPM25,cacheData.sPM.fPM10);
                    }
                    cacheData.sPM.fPM1 = 0;
                    cacheData.sPM.fPM25 = 0;
                    cacheData.sPM.fPM10 = 0;
                }
                if(0 != cacheData.sENS.fCO2)
                {
                    if('f' == msg[0])
                    {
                        sprintf(msg,"%s&field6=%0.2f&field7=%0.2f",msg, cacheData.sENS.fTVOC,cacheData.sENS.fCO2);
                    }
                    else
                    {
                        sprintf(msg,"field6=%0.2f&field7=%0.2f",cacheData.sENS.fTVOC,cacheData.sENS.fCO2);
                    }
                    cacheData.sENS.fCO2 = 0;
                    cacheData.sENS.fTVOC = 0;
                }
                publish(msg);
#endif
#if 0
                /** @todo:  do this whenever, the timer has finisehd and has info from all sensors.**/
                //JSON formatter:
                sprintf(JSON,"{\"SAPM_ID\": %s, \"sensors\":[ \
                            {\"name\":\"temp\",\"value\":%f},\
                            {\"name\":\"hum\",\"value\":%f},\
                            {\"name\":\"PM1\",\"value\":%f},\
                            {\"name\":\"PM25\",\"value\":%f},\
                            {\"name\":\"PM10\",\"value\":%f},\
                            {\"name\":\"TVOC\",\"value\":%f},\
                            {\"name\":\"CO2\",\"value\":%f}\
                         ]}",SAPM_ID, cacheData.sAM.fTemp,cacheData.sAM.fHum,\
                         cacheData.sPM.fPM1,cacheData.sPM.fPM25,cacheData.sPM.fPM10,\
                         cacheData.sENS.fTVOC,cacheData.sENS.fCO2);
                //Print_debug()
                break;
#endif
                break;
            default:
                sprintf(msg,"No New Data\n");
                break;
        }
        Print_debug(msg);
        memset(msg,0,strlen(msg));
    }
}

/**
*	@name vSetupWifi
*   @type function
*/
void vSetupProcess(void)
{
    sgqSensorData = xQueueCreate(PROC_QUEUE_SZ, sizeof(SAPMS_t));
}

/**
*	@name vCollectData
*   @type function
*/
void vCollectData(SAPMS_t * psSAPMS, SAPMS_e ESAPMS)
{
    psSAPMS->eSRC = ESAPMS;
    xQueueSendToBack(sgqSensorData,psSAPMS,0);
}

/**
*	@name Alarm in us
*   @type static void function
*/
void alarm_in_us(uint32_t delay_us) 
{
    // Enable the interrupt for our alarm (the timer outputs 4 alarm irqs)
    hw_set_bits(&timer_hw->inte, 1u << 0);
    // Set irq handler for alarm irq
    irq_set_exclusive_handler(TIMER_IRQ_0, alarm_irq);
    // Enable the alarm irq
    irq_set_enabled(TIMER_IRQ_0, true);
    // Enable interrupt in block and at processor

    // Alarm is only 32 bits so if trying to delay more
    // than that need to be careful and keep track of the upper
    // bits
    uint64_t target = timer_hw->timerawl + delay_us;

    // Write the lower 32 bits of the target time to the alarm which
    // will arm it
    timer_hw->alarm[0] = (uint32_t) target;
}

/**
*	@name alarm_irq
*   @type static void interrupt handler
*/
void alarm_irq(void) {
    // Clear the alarm irq
    hw_clear_bits(&timer_hw->intr, 1u << 0);
    // Code
    SAPMS_t SAPMS = 
    {
        .eSRC=EWIFI
    };
    xQueueSendToBackFromISR(sgqSensorData,&SAPMS,NULL);
}
