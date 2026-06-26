/**
 * @file main.c
 * @brief Reference Implementation & Application Entry Point
 * * @details
 * This file serves as a reference application demonstrating how to execute high-level 
 * business logic completely decoupled from specific hardware platforms or operating systems. 
 * By utilizing the underlying Hardware Abstraction Layer (HAL) and Operating System 
 * Abstraction Layer (OSAL), this execution path can compile and run unmodified across 
 * various target architectures (e.g., RTOS-driven ESP32 or Bare-Metal STM32 Super-Loops).
 *
 * @section Architecture_Execution Execution Flow Blueprint
 * +-------------------------------------------------------+
 * |                     System Boot                       |
 * +-------------------------------------------------------+
 * |
 * v
 * +-------------------------------------------------------+
 * |        Low-Level Initialization (hal_init())          |
 * +-------------------------------------------------------+
 * |
 * v
 * +-------------------------------------------------------+
 * |        Kernel/Super-Loop Setup (osal_init())          |
 * +-------------------------------------------------------+
 * |
 * v
 * +-------------------------------------------------------+
 * |           Asynchronous Driver Task Spawning           |
 * |       (e.g., US100 Processing & Logging Loops)        |
 * +-------------------------------------------------------+
 * |
 * v
 * +-------------------------------------------------------+
 * |          Non-Blocking Multi-Task Scheduler            |
 * +-------------------------------------------------------+
 * * @section Core_Design_Tenets Core Application Design Tenets
 * - Zero Direct Hardware/OS Coupling: No vendor headers (such as `esp_idf.h` or `stm32f1xx.h`) 
 * or explicit kernel calls (`vTaskDelay`) are permitted within this application scope.
 * - Thread & ISR Safety: Asynchronous background data captured via hardware interrupts 
 * is extracted safely through non-blocking, memory-bounded OSAL pipeline primitives.
 * - Target Agnosticism: The scheduler pattern loops deterministically whether backed by 
 * a true real-time preemptive kernel or a single-threaded cooperative super-loop.
 * * @note Enforces strict compile-time type safety via the standard <stdint.h> taxonomy.
 * * @copyright Copyright (c) 2026. All Rights Reserved.
 */

#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#include "ultrasonic/ultrasonic.h"

#include "../components/osal/include/osal.h"
#include "../components/my_hal/include/hal_uart.h"

#define LED_PIN 4
#define TRIG_PIN 17
#define ECHO_PIN 18

#define TX_PIN 33
#define RX_PIN 32

osal_queue_t sensor_queue;

void US100(void *pvParameters)
{
    ultrasonic_config_t ultrasonic_config = 
        { .trig_pin = TRIG_PIN, .echo_pin = ECHO_PIN };

    osal_get_current_task_handle_func(&(ultrasonic_config.task_to_notify));

    hal_intr_flag_t intr_flag = get_intr_flag();
    hal_gpio_install_isr_service_func(intr_flag);
    
    sensor_msg_t sensor_msg = {0};
    
    add_new_ultrasonic_sensor(&ultrasonic_config);
    while(1)
    {
        osal_action_upon_notification_func(OSAL_TRUE, 0);
        fire_ultrasonic_sensor(&ultrasonic_config);
        if(osal_action_upon_notification_func(OSAL_TRUE, 100) > 0)
        {
            ultrasonic_get_distance_cm(&ultrasonic_config, &sensor_msg);
            osal_push_queue_func(sensor_queue, &sensor_msg, 0);
        }
        else
        {
            hal_log(HAL_LOG_WARN, "SENSOR", "Ultrasonic timeout (no echo)\n");
        }
        osal_task_delay_ms_func(100); 
    }
}

void boss_task(void *pvParameters)
{
    sensor_msg_t received_msg;
    while (1)
    {
        if(osal_pop_queue_func(sensor_queue, &received_msg, osal_max_delay))
        {
            hal_log(HAL_LOG_INFO, "SENSOR", "Distance of: %d\n", received_msg.distance);
        }
        osal_task_delay_ms_func(100); 
    }
}

void BlinkLED(void *pvParameters)
{
    uint8_t LED_STATE = 0;
    osal_Tick lastExecTime = osal_get_tick_count_func();
    hal_gpio_set_direction_func(LED_PIN, HAL_GPIO_MODE_OUTPUT);
    while(1) 
    {
        LED_STATE = 1 - LED_STATE;
        hal_gpio_set_level_func(LED_PIN, LED_STATE);
        
        osal_task_delay_until_ms_func(&lastExecTime, 1000);
    }
}

void UART_RX(void *pvParameters)
{
    hal_uart_receive_func();
}

void UART_TX(void *pvParameters)
{
    hal_uart_transmit_func("Hello World!");
}

void app_main(void)
{
    sensor_queue = osal_create_queue_func(10, sizeof(sensor_msg_t));

    hal_intr_flag_t intr_flag = get_intr_flag();
    hal_gpio_install_isr_service_func(intr_flag);

    hal_uart_config_t uart_config = 
    {
        .baud_rate = 9600,
        .data_bits = HAL_UART_DATA_BITS_8,
        .parity = HAL_UART_PARITY_DISABLE,
        .stop_bits = HAL_UART_STOP_BITS_1,
        .flow_ctrl = HAL_UART_FLOW_CONTROL_DISABLE,
    };

    hal_uart_init_func(&uart_config, TX_PIN, RX_PIN);


    osal_create_task_func(BlinkLED, "BlinkLED", 1024, NULL, 1, NULL);
    osal_create_task_func(boss_task, "BOSS", 2048, (void*) sensor_queue.queue, 10, NULL);
    osal_create_task_func(US100, "Ultrasonic", 4096, (void*) sensor_queue.queue, 2, NULL);
    osal_create_task_func(UART_RX, "UART", 4096, NULL, 1, NULL); 
    osal_create_task_func(UART_TX, "UART", 4096, NULL, 1, NULL);  


}