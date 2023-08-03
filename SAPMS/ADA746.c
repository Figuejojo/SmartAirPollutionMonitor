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

void parse_gprmc_data(const char *data);
void on_uart_rx();

/**
*	@name vTaskGPS
*   @type Task
*/
void vTaskGPS(void * pvParameters)
{
    const uint8_t cStandByCmd[] = {0x24,0x50,0x4D,0x54,0x4B,0x31,0x36,0x31,0x2C,0x30,0x2A,0x32,0x38,0x0D,0x0A,0xFE};
    //const uint8_t cStandByCmd[] = {'$','P','M','T','K','1','6','1','0','*','2','8','\r','\n'};
    while(1)
    {
        Print_debug("MSG GPS");
        const uint8_t * ptrCMD = cStandByCmd;
        while(*ptrCMD!=0xFE)
        {
            uart_putc(ADA_USART, *ptrCMD);
            ptrCMD++;  
        }

        if (data_received) 
        {
            Print_debug("Data Recv: ");
            // print received data
            uart_puts(UART_ID,(const char*)(receive_buffer));
            data_received = false;
            // parse the received data
            parse_gprmc_data((const char*)receive_buffer);
        }
        vTaskDelay(5000/portTICK_PERIOD_MS);
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
    while (uart_is_readable(UART_ID)) {
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
void parse_gprmc_data(const char *data) {
    char time[7];
    char latitude[10];
    char longitude[11];
    char date[7];
    char altitude[7];
    char msg[30];
    printf("Data is: %s",data);
    // Check if it is GPRMC data
    if (strncmp(data, "$GPRMC", 6) == 0) {
        // Separate the GPRMC data with commas to extract the required information
        char *token;
        int count = 0;
        char *data_copy = strdup(data); 
// Make a copy of the data so as not to destroy the original
        token = strtok(data_copy, ",");
        while (token != NULL) 
        {
            if (count == 1) 
            {
                // Time information (format: hhmmss)
                strncpy(time, token, 6);
                time[6] = '\0';
            } 
            else if (count == 3) 
            {
                // Latitude information (format: ddmm.mmmm)
                strncpy(latitude, token, 9);
                latitude[9] = '\0';
            } 
            else if (count == 5) 
            {
                // Longitude information (format: dddmm.mmmm)
                strncpy(longitude, token, 10);
                longitude[10] = '\0';
            } 
            else if (count == 9) 
            {
                // Date information (format: ddmmyy)
                strncpy(date, token, 6);
                date[6] = '\0';
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

        printf("Time %s, Lat %s, Long %s",time,latitude,longitude);

    } 
    else
    {
        printf("Invalid GPRMC data.\n");
    }
}
