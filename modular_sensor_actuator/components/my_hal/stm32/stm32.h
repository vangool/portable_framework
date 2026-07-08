/**
 * @file stm32.h
 * @brief STM32 hardware abstraction layer core interface.
 *
 * This header defines the public interface for the STM32 hardware abstraction
 * layer (HAL). It provides the declarations, types, and interfaces required
 * for initializing and interacting with STM32-based platforms.
 *
 * The STM32 HAL provides a platform-specific implementation layer that
 * abstracts STM32 hardware details from application-level code. This allows
 * higher-level software to use a consistent hardware interface across multiple
 * microcontroller families.
 *
 * This module provides the core STM32 functionality required by peripheral HAL
 * modules, including GPIO, UART, SPI, I2C, timers, and other hardware drivers.
 *
 * Responsibilities include:
 * - STM32 platform initialization
 * - MCU-specific configuration
 * - Access to STM32 HAL services
 * - Common STM32 hardware definitions
 * - Interfaces shared between STM32 peripheral modules
 *
 * @note This header is intended to be included by STM32-specific HAL modules
 *       and platform initialization code. Application code should preferably
 *       use the generic HAL interface instead.
 *
 * @author Michael Van Gool 
 * @date 2026-07-07
 *
 *
 * MIT License
 *
 * Copyright (c) 2026 Michael Van Gool
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef __ESP32_H__
#define __ESP32_H__

#include "../include/hal.h"
#include "../../osal/include/osal.h"
#include "stm32f103/stm32f103.h"
#include "stm32f103/stm32f103_intr.h"

#include <stdlib.h>



/**
 * Set direction of an individual pin
 * 
 * @param[in] pin Pin number / id
 * @param[in] direction Classify as input or output pin
 * 
 * @return hal_status_t     HAL_STATUS_OK for successful, otherwise different status return
 */
hal_status_t stm32_gpio_set_direction(uint8_t pin, hal_gpio_config_t direction);

/**
 * Set level of an individual pin
 * 
 * @param[in] pin Pin number / id
 * @param[in] level Level to set the pin at
 * 
 * @return hal_status_t     HAL_STATUS_OK for successful, otherwise different status return
 */
hal_status_t stm32_gpio_set_level(uint8_t pin, hal_gpio_level_t level);

/**
 * Set direction of an individual pin
 * 
 * @param[in] pin Pin number / id
 * 
 * @return int8_t The level that the param pin is at
 */
int8_t stm32_gpio_get_level(uint8_t pin);

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
hal_status_t stm32_gpio_set_intr_type(uint8_t pin, hal_intr_type_t type);

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
hal_status_t stm32_gpio_isr_handler_add(uint8_t gpio_pin, void* isr_handler, void* args);

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
void stm32_rom_delay_us(uint32_t delay_us);

/**
 * Get the current time in us.
 * 
 * @return current time in us.
 */
int64_t stm32_timer_get_time();


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
hal_status_t stm32_gpio_install_isr_service(hal_intr_flag_t flag);

/**
 * @brief Outputs a formatted STM32 HAL log message.
 *
 * Provides a platform-specific logging interface for the STM32 HAL. This
 * function formats and outputs diagnostic messages using the configured STM32
 * logging backend.
 *
 * The log level is used to classify the severity of the message, while the tag
 * identifies the source module or component generating the log entry. The
 * message format follows the same rules as printf(), allowing additional
 * arguments to be supplied for formatted output.
 *
 * This function is intended to provide a consistent logging interface across
 * supported platforms. Platform-specific logging implementations may route
 * messages differently depending on the target environment.
 *
 * @param level
 * Log severity level associated with the message.
 *
 * @param tag
 * Null-terminated string identifying the module or component generating the
 * log message.
 *
 * @param fmt
 * printf-style format string describing the message format.
 *
 * @param ...
 * Additional arguments required by the format string.
 *
 * @return
 * None.
 */
void stm32_log(hal_log_level_t level, const char *tag, const char *fmt, ...);


#endif //__STM32_H__