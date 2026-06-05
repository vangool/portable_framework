#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "driver/gpio.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "ultrasonic/ultrasonic.h"
#include "utils.h"

#include "../components/osal/include/osal.h"

#define LED_PIN 4
#define TRIG_PIN 17
#define ECHO_PIN 18

QueueHandle_t sensor_queue;

void US100(void *pvParameters)
{
    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = TRIG_PIN, .echo_pin = ECHO_PIN };

    osal_get_current_task_handle_func(&(ultrasonic_config.task_to_notify));

    hal_intr_flag_t intr_flag = get_intr_flag();
    hal_gpio_install_isr_service_func(intr_flag);
    
    sensor_msg_t sensor_msg = {0};
    
    add_new_ultrasonic_sensor(&ultrasonic_config);
    printf("Ultrasonic sensor added\n");
    printf("task handle = %p\n", ultrasonic_config.task_to_notify.taskhandle); 
    while(1)
    {
        ulTaskNotifyTake(pdTRUE, 0);
        fire_ultrasonic_sensor(&ultrasonic_config);
        if(ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100)) > 0)
        {
            printf("Information has been sent\n");
            ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);
            xQueueSend(sensor_queue, &sensor_msg, 0);
        }
        else
        {
            printf("Ultrasonic timeout (no echo)\n");
        }
        vTaskDelay(pdMS_TO_TICKS(100));
        printf("Ultrasonic sensor fired\n");
    }
}

void boss_task(void *pvParameters)
{
    sensor_msg_t received_msg;
    while (1)
    {
        printf("Boss is reading\n");
        if(xQueueReceive(sensor_queue, &received_msg, portMAX_DELAY))
        {
            printf("Boss received distance of: %d\n", received_msg.distance);
        }
        vTaskDelay(pdMS_TO_TICKS(100)); 
    }
}

void BlinkLED(void *pvParameters)
{
    uint8_t LED_STATE = 0;
    TickType_t lastExecTime = xTaskGetTickCount();
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    while(1) 
    {
        LED_STATE = 1 - LED_STATE;
        gpio_set_level(LED_PIN, LED_STATE);
        vTaskDelayUntil(&lastExecTime, pdMS_TO_TICKS(1000));
    }
}

void app_main(void)
{
    sensor_queue = xQueueCreate(10, sizeof(sensor_msg_t));
    printf("Starting application\n");

    xTaskCreate(BlinkLED, "BlinkLED", 1024, NULL, 1, NULL);
    xTaskCreate(boss_task, "BOSS", 2048, (void*) sensor_queue, 10, NULL);
    xTaskCreate(US100, "Ultrasonic", 4096, (void*) sensor_queue, 2, NULL);   

    printf("Threads created\n");
}
