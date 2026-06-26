/******************************************************************************
 * @file: esp32.h
 * @brief: Target-specific Hardware Abstraction Configuration for ESP32.
 *
 * This header maps physical silicon layout components, pin multiplexer nodes, 
 * and declares target-specific driver extensions for the Espressif ESP32 
 * platform architectures.
 *
 * @author: Michael van Gool
 * @date: 2026-06-26
 * @license: MIT License
 * Copyright (c) 2026 Michael Van Gool  . All rights reserved.
 ******************************************************************************/

#ifndef __ESP32_H__
#define __ESP32_H__

#include "esp_timer.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "hal.h"

/**
 * Set direction of an individual pin
 * 
 * @param[in] pin Pin number / id
 * @param[in] direction Classify as input or output pin
 * 
 * @return hal_status_t     HAL_OK for successful, otherwise different status return
 */
hal_status_t esp32_gpio_set_direction(uint8_t pin, uint8_t direction);

/**
 * Set level of an individual pin
 * 
 * @param[in] pin Pin number / id
 * @param[in] level Level to set the pin at
 * 
 * @return hal_status_t     HAL_OK for successful, otherwise different status return
 */
hal_status_t esp32_gpio_set_level(uint8_t pin, uint8_t level);

/**
 * Set direction of an individual pin
 * 
 * @param[in] pin Pin number / id
 * 
 * @return int8_t The level that the param pin is at
 */
int8_t esp32_gpio_get_level(uint8_t pin);

/**
 * @brief Configure the interrupt trigger condition for a GPIO pin.
 *
 * Sets the GPIO interrupt type, determining which signal transition
 * or level causes the interrupt service routine to execute.
 *
 * Supported interrupt types include rising edge, falling edge,
 * both edges, and high/low level triggers.
 *
 * @param[in] pin GPIO pin number.
 * @param[in] intr_type GPIO interrupt trigger type.
 *
 * @return HAL status code.
 */
hal_status_t esp32_gpio_set_intr_type(uint8_t pin, hal_intr_type_t type);

/**
 * @brief Register an interrupt service routine for a GPIO pin.
 *
 * Associates the specified ISR callback with a GPIO interrupt source.
 * The callback will be invoked when the configured interrupt condition
 * (rising edge, falling edge, both edges, or level trigger) occurs on
 * the GPIO.
 *
 * The GPIO interrupt service must be installed before calling this
 * function.
 * 
 * @param[in] gpio_pin Pin number / id
 * @param[in] isr_handler ISR callback function to be called
 * @param[in] args arguments for the ISR callback
 */
hal_status_t esp32_gpio_isr_handler_add(uint8_t gpio_pin, void* isr_handler, void* args);

/**
 * @brief Delay execution for the specified number of microseconds.
 *
 * Performs a busy-wait delay and blocks the CPU for the duration of
 * the delay. No other work can be performed by the current core while
 * this function is executing.
 *
 * Intended for short hardware timing requirements such as sensor
 * trigger pulses and peripheral setup delays.
 *
 * @param[in] delay_us Delay duration in microseconds.
 */ 
void esp32_rom_delay_us(uint32_t delay_us);

/**
 * Get the current time in us.
 * 
 * @return current time in us.
 */
int64_t esp32_timer_get_time();


/**
 * @brief Initialize GPIO interrupt handling.
 *
 * Installs the ESP-IDF GPIO ISR service, allowing interrupt
 * handlers to be registered for individual GPIO pins using
 * gpio_isr_handler_add().
 *
 * This function must be called before registering any GPIO
 * interrupt callbacks.
 *
 * @return HAL status code.
 */
hal_status_t esp32_gpio_install_isr_service(hal_intr_flag_t flag);

/**
 * @brief Thread-safe formatted logging at the INFO severity level with contextual tagging.
 *
 * Formats and routes log streams through the ESP32 UART console interface. This function
 * appends structured system metadata (runtime tick counts and runtime component tags) to the 
 * output stream.
 *
 * @param[in] level  The target logging severity framework tier (e.g., ESP_LOG_INFO equivalent).
 * @param[in] tag    Null-terminated string representing the specific software component or module.
 * @param[in] fmt    Null-terminated format string containing standard printf-style specifiers.
 * @param[in] ...    Variadic arguments corresponding to the parameters defined in the fmt string.
 *
 * @note This function is designed to be completely non-blocking and thread-safe when invoked
 * across multiple concurrent RTOS tasks.
 */
void esp32_log_info(uint8_t level, const char *tag, const char *fmt, ...);


#endif //__ESP32_H__