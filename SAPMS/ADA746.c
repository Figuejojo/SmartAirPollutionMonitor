/** @file ADA746.c
 *  @brief  //Write a brief description 
 *
 *  @author //Y3910898
 */
#include "ADA746.h"
#include "stdlib.h"
#include "pico/stdlib.h"
#include "hardware/uart.h"
#include "hardware/irq.h"
#include "WIFI/process.h"

/*******************************************************************************
* Static & Global Variables
*******************************************************************************/
#define UART_ID uart0
#define BAUD_RATE 9600
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY    UART_PARITY_NONE

//UART0 pin definition
#define UART_TX_PIN 0
#define UART_RX_PIN 1
/*******************************************************************************
* Function Definition
*******************************************************************************/
static int chars_rxed = 0;
#define BUFFER_SIZE 256
volatile char receive_buffer[BUFFER_SIZE];
volatile uint8_t receive_index = 0;
volatile bool data_received = false;

static ERR_t parse_gprmc_data(const char *data, ada_t *AdaData);
static void get_lat_lon(ada_t inData,SAPMS_t * outData);
static void on_uart_rx();

/**
*	@name vTaskGPS
*   @type Task
*/
void vTaskGPS(void * pvParameters)
{
    const uint8_t cStandByCmd[] = {0x24,0x50,0x4D,0x54,0x4B,0x31,0x36,0x31,0x2C,0x30,0x2A,0x32,0x38,0x0D,0x0A,0xFE};
    const uint8_t cRMCCmd[] = "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*290\r\n";
    const uint8_t * ptrCMD = cStandByCmd;
    ada_t AdaData = {0};
    SAPMS_t ADAmsg = {0};
    ERR_t err;
    while(*ptrCMD!=0xFE)
    {
        uart_putc(ADA_USART, *ptrCMD);
        ptrCMD++;  
    }

    while(1)
    {
        err = NO_ERROR;
        Print_debug("MSG GPS");
        vTaskDelay(10000/portTICK_PERIOD_MS);
        if (data_received) 
        {
            // parse the received data
            err = parse_gprmc_data((const char*)receive_buffer, &AdaData);
            if(err >= 0)
            {
                get_lat_lon(AdaData,&ADAmsg);

                printf("%f\n",ADAmsg.sADA.fLat);
                printf("%f\n",ADAmsg.sADA.fLong);
            }
            else
            {
                Print_debug("GPS Satelite error");
            }
            data_received = false;
        }
    }
}

/**
*	@name  GPS_setup
*   @Type  Setup functon
*/
void GPS_setup(void)
{
#if USE_ADA746 == 1
    // Uart initialization
    uart_init(ADA_USART, ADA_BAUD);
    // Uart pin setup
    gpio_set_function(ADA_TX, GPIO_FUNC_UART);
    gpio_set_function(ADA_RX, GPIO_FUNC_UART);
    //Interrupt part//
    // close hardware flow
    uart_set_hw_flow(UART_ID, false, false);
    // set data format
    uart_set_format(UART_ID, DATA_BITS, STOP_BITS, PARITY);
    // T off FIFO
    uart_set_fifo_enabled(UART_ID, false);
    // initialization interrupt
    irq_set_exclusive_handler(UART0_IRQ, on_uart_rx);
    irq_set_enabled(UART0_IRQ, true);
    // Enable receive interrupt, disable transmit interrupt
    uart_set_irq_enables(UART_ID, true, false);
#endif
}

// Serial receive interrupt
void on_uart_rx() {
    while (uart_is_readable(UART_ID)) 
    {
        char data = uart_getc(UART_ID);
        
        // Store the received data into the buffer
        receive_buffer[receive_index] = data;
        receive_index++;
        
        // Check if a terminator was received (assumed to be '\n')
        if (data == '\n' || receive_index >= BUFFER_SIZE - 1) {
            receive_buffer[receive_index] = '\0'; // Null-terminate the string
            data_received = true;
            receive_index = 0;
        }
    }
}

// Parse GPRMC data and extract time, date, latitude, longitude and altitude information
ERR_t parse_gprmc_data(const char *data, ada_t *outData)
{
    ERR_t err = NO_ERROR;
    char latitude[10];
    char in_NS[2];
    char longitude[11];
    char in_EW[2];
    char altitude[7];
    printf("Data is: %s",data);
    // Check if it is GPRMC data
    if (strncmp(data, "$GPRMC", 6) == 0) 
    {
        // Separate the GPRMC data with commas to extract the required information
        char *token;
        int count = 0;
        char *data_copy = strdup(data); 
        // Make a copy of the data so as not to destroy the original
        token = strtok(data_copy, ",");
        while (token != NULL) 
        {
            if(count == 2)
            {
                if(token[0] == 'V')
                {
                    return ER_GPS_V;
                }
            }
            else if (count == 3) 
            {
                // Latitude information (format: ddmm.mmmm)
                strncpy(outData->lat, token, 9);
                outData->lat[9] = '\0';
            } 
            else if (count == 4)
            {
                // NS 
                strncpy(outData->isN, token, 1);
                in_NS[2] = '\0';               
            }
            else if (count == 5) 
            {
                // Longitude information (format: dddmm.mmmm)
                strncpy(outData->lon, token, 10);
                outData->lon[10] = '\0';
            }
            else if (count == 6)
            {
                // NS 
                strncpy(outData->isE, token, 1);
                outData->isE[2] = '\0';  
            }
            else if (count == 8) 
            {
                // altitude information
                strncpy(altitude, token, 6);
                altitude[6] = '\0';
            }
            count++;
            token = strtok(NULL, ",");
        }
        free(data_copy);
        //printf("Lat %s %c, Long %s %c\n",outData->lat,outData->isN[0],outData->lon,outData->isE[0]);

    } 
    else
    {
        printf("Invalid GPRMC data.\n");
        err = ER_GPS_I;
    }
    return err;
}

void get_lat_lon(ada_t inData,SAPMS_t * outData)
{    
    char Degrees[4] = {0};
    char Minutes[8] = {0};

    strncpy(Degrees,inData.lat,2);
    strncpy(Minutes,inData.lat+2,7);
    
    float lat_dd = atoi(Degrees) + atof(Minutes) / 60.0f;
    
    strncpy(Degrees,inData.lon,3);
    strncpy(Minutes,inData.lon+3,7);
    
    float lon_dd = atoi(Degrees) + atof(Minutes) / 60.0f;

    outData->sADA.fLat = (inData.isN[0] == 'N')?(lat_dd):(-lat_dd);
    outData->sADA.fLong= (inData.isE[0] == 'E')?(lon_dd):(-lon_dd);
}
