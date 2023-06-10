#include "common.h"

QueueHandle_t xQueue1;

void led_task()
{   

    const uint LED_PIN = 3;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    while (true) {
        gpio_put(LED_PIN, 1);
        vTaskDelay(500/portTICK_PERIOD_MS); // Ticks to delay
        gpio_put(LED_PIN, 0);
        vTaskDelay(500/portTICK_PERIOD_MS);
        printf("Blinking!\r\n");
    }
}

int main()
{
    stdio_init_all();

    xQueue1 = xQueueCreate(10, sizeof(int));

    xTaskCreate(led_task, "LED_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1){};
}