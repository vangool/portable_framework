/**
 * @file US100.c
 * @brief Asynchronous Concrete Driver Implementation for the US100 Ultrasonic Sensor
 *
 * @details
 * This module provides the concrete execution logic for the generic distance sensing 
 * interface (`ultrasonic.h`). It specializes in driving the US100 transceiver module 
 * using an event-driven, non-blocking interrupt paradigm. 
 *
 * Rather than stalling the CPU during the sensor's echo transmission flight window, 
 * this implementation configures a physical GPIO line as an edge-triggered interrupt. 
 * High-precision hardware timestamps are captured instantly on both rising and falling 
 * edges within a low-overhead Interrupt Service Routine (ISR), freeing the main processor 
 * to handle concurrent application tasks.
 *
 * @section Interrupt_Lifecycle Asynchronous ISR State Machine
 * * GPIO Echo Line          State Action / Side Effects
 * __________________
 * |                  |
 * _____|                  |_____
 * ^                  ^
 * | [1] Rising Edge  | [2] Falling Edge
 * |                  |
 * +-- Captures       +-- Captures end_time snapshot.
 * start_time.    +-- Issues non-blocking OSAL signaling
 * primitive to notify waiting task.
 *
 * @section Implementation_Specs Peripheral Dependencies & Math
 * - Time-of-Flight Derivation: Distance computation utilizes the microsecond delta 
 * ($\Delta t = \text{end\_time} - \text{start\_time}$) scaled against the environmental 
 * speed of sound constant at standard room temperature (~343 m/s).
 * - Encapsulated State Management: Leverages explicit pass-by-reference pointer tracking 
 * (`ultrasonic_config_t*`) to update instance parameters safely across asynchronous execution 
 * boundaries without global scope leaks.
 * - Thread-Safe Notification: Uses target-safe OSAL mechanisms within the ISR context 
 * to wake blocked application loops immediately upon lifecycle completion.
 *
 * @note This file is compiled selectively by the build system to bind the abstract 
 * interface layout specifically to the US100 hardware target layout.
 *
 * @copyright Copyright (c) 2026. All Rights Reserved.
 */

#include "ultrasonic.h"

static void HAL_GPIO_ISR_ATTR echo_isr_handler(void*);

static uint8_t gotten_intr_flag = false;

hal_intr_flag_t get_intr_flag()
{
    gotten_intr_flag = true;
    return HAL_INTR_FLAG_IRAM;
}

static int8_t is_intr_flag_retrieved()
{
    return gotten_intr_flag;
}

static void HAL_GPIO_ISR_ATTR echo_isr_handler(void* arg)
{
    ultrasonic_config_t* sensor = (ultrasonic_config_t*) arg;
    if(hal_gpio_get_level_func(sensor->echo_pin) == HAL_GPIO_HIGH)
    {
        sensor->start_time = hal_timer_get_time_func();
    }
    else
    {
        sensor->end_time = hal_timer_get_time_func();
        osal_task_notify_give_from_isr_func(sensor->task_to_notify);
    }
}

hal_status_t add_new_ultrasonic_sensor(ultrasonic_config_t* config)
{
    if(is_intr_flag_retrieved() == false)
        return HAL_ERROR_GENERAL;

    hal_gpio_set_direction_func(config->trig_pin, HAL_GPIO_MODE_OUTPUT);
    hal_gpio_set_direction_func(config->echo_pin, HAL_GPIO_MODE_INPUT);
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_LOW);

    hal_status_t ret = hal_gpio_set_intr_type_func(config->echo_pin, HAL_GPIO_INTR_ANYEDGE);
    ret |= hal_gpio_isr_handler_add_func(config->echo_pin, echo_isr_handler, (void*) config);
    return ret;
}

void ultrasonic_get_distance_cm(ultrasonic_config_t* config, sensor_msg_t* msg)
{
    if(config->end_time < config->start_time)
        return;

    static int64_t avg_distance = 0;
    const int64_t sound_speed = 34300;
    int64_t distance = (sound_speed/* cm/s */ * ((config->end_time/* us */ - config->start_time/* us */)));
    distance/* cm/ms */ /= (int64_t)(1000000/* s/us */ * 2);

    if(avg_distance == 0)
    {
        avg_distance = distance;
    }
    else
    {
        avg_distance += (int64_t)((distance - avg_distance) / 2);
    }
    msg->distance = avg_distance;
}

void fire_ultrasonic_sensor(ultrasonic_config_t* config)
{
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_HIGH);
    hal_rom_delay_us_func(10);
    hal_gpio_set_level_func(config->trig_pin, HAL_GPIO_LOW);
}

