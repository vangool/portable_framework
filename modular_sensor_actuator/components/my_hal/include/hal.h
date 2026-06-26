/******************************************************************************
 * @file: hal_gpio.h
 * @brief: Platform-agnostic General Purpose Input/Output (GPIO) HAL API.
 *
 * This file establishes a universal, hardware-independent interface for managing
 * digital I/O pins. Application business logic uses these contracts to read inputs,
 * toggle outputs, and attach interrupt routines without needing exposure to the
 * proprietary register mapping or GPIO driver configurations of the target MCU.
 *
 * @author: Michael Van Gool
 * @date: 2026-05-04
 * @license: MIT License
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
 ******************************************************************************/

#ifndef HAL_GPIO_H_
#define HAL_GPIO_H_

// Universal pin definitions, direction enums (INPUT, OUTPUT), 
// resistor pulling options, and generic GPIO API prototypes go here...

#endif /* HAL_GPIO_H_ */

#ifndef __HAL_H__ //Hardware Abstraction Layer
#define __HAL_H__

#include <stdarg.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

#ifndef HAL_GPIO_ISR_ATTR
    #define HAL_GPIO_ISR_ATTR 
#endif

typedef enum {
    HAL_OK = 0,
    HAL_ERROR_INVALID_ARG,
    HAL_ERROR_GENERAL,
    HAL_ERROR_DEFAULT_IMPLEMENTATION
} hal_status_t;

typedef enum {
    HAL_GPIO_INTR_DISABLE = 0,
    HAL_GPIO_INTR_POSEDGE,
    HAL_GPIO_INTR_ANYEDGE,
    HAL_GPIO_INTR_NEGEDGE,
    HAL_GPIO_INTR_LOW_LEVEL,
    HAL_GPIO_INTR_HIGH_LEVEL
} hal_intr_type_t;

typedef enum {
    HAL_GPIO_MODE_DISABLE = 0,
    HAL_GPIO_MODE_INPUT,
    HAL_GPIO_MODE_OUTPUT,
    HAL_GPIO_MODE_OUTPUT_OD,
    HAL_GPIO_MODE_INPUT_OUTPUT
} hal_gpio_config_t;

typedef enum {
    HAL_INTR_FLAG_LOW = 0,
    HAL_INTR_FLAG_MED,
    HAL_INTR_FLAG_HIGH,
    HAL_INTR_FLAG_IRAM,
    HAL_INTR_FLAG_SHARED
} hal_intr_flag_t;

typedef enum {
    HAL_GPIO_LOW = 0,
    HAL_GPIO_HIGH
} hal_gpio_level_t;

typedef enum {
    HAL_LOG_ERR = 1,
    HAL_LOG_WARN,
    HAL_LOG_INFO,
    HAL_LOG_DEBUG
} hal_log_level_t;


/**
 * @brief Configure the direction of a GPIO pin.
 *
 * Sets the specified GPIO pin as an input or output.
 *
 * @param pin GPIO pin number.
 * @param direction GPIO direction (input or output).
 *
 * @return HAL_OK on success, otherwise an error code.
 */
typedef hal_status_t (*hal_gpio_set_direction)(uint8_t pin, uint8_t direction);

/**
 * @brief Configure the level of a GPIO pin.
 *
 * Sets the level of the specified GPIO pin as HIGH or LOW.
 *
 * @param pin GPIO pin number.
 * @param direction GPIO direction (input or output).
 *
 * @return HAL_OK on success, otherwise an error code.
 */
typedef hal_status_t (*hal_gpio_set_level)(uint8_t pin, uint8_t level);

/**
 * @brief Read the level of a GPIO pin.
 *
 * Read the level of the specified GPIO pin.
 *
 * @param pin GPIO pin number.
 * @param direction GPIO direction (input or output).
 *
 * @return 1 for HIGH or 0 for LOW
 */
typedef int8_t (*hal_gpio_get_level)(uint8_t pin);

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
typedef hal_status_t (*hal_gpio_set_intr_type)(uint8_t pin, hal_intr_type_t type);

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
typedef hal_status_t (*hal_gpio_isr_handler_add)(uint8_t gpio_pin, void* isr_handler, void* args);

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
typedef void (*hal_rom_delay_us)(uint32_t delay_us);

/**
 * Get the current time in us.
 * 
 * @return current time in us.
 */
typedef int64_t (*hal_timer_get_time)(void);

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
typedef hal_status_t (*hal_gpio_install_isr_service)(hal_intr_flag_t intr_flags);

/**
 * @brief Function pointer type definition for system-wide logging outputs.
 *
 * This typedef establishes a standardized contract for all logging backends. 
 * By using a function pointer, the upper-level application can register different 
 * logger implementations (e.g., UART terminal console, network socket syslog, 
 * or flash memory flash logs) seamlessly without modifying the core HAL.
 *
 * @param[in] level  The logging severity level (e.g., DEBUG, INFO, WARN, ERROR).
 * @param[in] tag    A null-terminated string representing the software component 
 * emitting the log (used for targeted filtering).
 * @param[in] fmt    A standard C format control string (identical to printf).
 * @param[in] ...    A variable-length argument list matching the format specifiers 
 * defined in the `fmt` parameter.
 *
 * @note Implementations of this function pointer must handle variadic arguments 
 * internally using the standard C `<stdarg.h>` macros (`va_list`, `va_start`, 
 * `vprintf`/`vsnprintf`, and `va_end`).
 */
typedef void (*hal_log_info)(uint8_t level, const char* tag, const char* fmt, ...);

/* Active GPIO implementation. */
extern hal_gpio_set_direction hal_gpio_set_direction_func;
extern hal_gpio_set_level hal_gpio_set_level_func;
extern hal_gpio_get_level hal_gpio_get_level_func;
extern hal_gpio_set_intr_type hal_gpio_set_intr_type_func;
extern hal_gpio_isr_handler_add hal_gpio_isr_handler_add_func;
extern hal_rom_delay_us hal_rom_delay_us_func;
extern hal_timer_get_time hal_timer_get_time_func;
extern hal_gpio_install_isr_service hal_gpio_install_isr_service_func;
extern hal_log_info hal_log;

/* Default GPIO implementation. */
hal_status_t hal_gpio_set_direction_default(uint8_t pin, uint8_t direction);
hal_status_t hal_gpio_set_level_default(uint8_t pin, uint8_t level);
int8_t hal_gpio_get_level_default(uint8_t pin);
hal_status_t hal_gpio_set_intr_type_default(uint8_t pin, hal_intr_type_t type);
hal_status_t hal_gpio_isr_handler_add_default(uint8_t gpio_pin, void* isr_handler, void* args);
void hal_rom_delay_us_default(uint32_t delay_us);
int64_t hal_timer_get_time_default(void);
hal_status_t hal_gpio_install_isr_service_default(hal_intr_flag_t intr_flags);
void hal_log_info_default(uint8_t level, const char* tag, const char* fmt, ...);

/* HAL functions */
/**
 * @brief Converts a HAL status code enum into a human-readable string.
 *
 * This utility function translates numeric HAL error codes into their literal 
 * text representations. It is primarily used to enhance readability in debug 
 * logging outputs, console telemetry streams, or error reporting systems.
 *
 * @param[in] status  The HAL status enumeration value to be converted.
 *
 * @return A constant pointer (`const char*`) to a null-terminated string literal 
 * representing the status name. If the status code is unrecognized, a default 
 * catch-all error string is returned.
 *
 * @warning The returned string pointer references a read-only literal permanently 
 * stored in Flash/ROM memory (.rodata). The calling application **must not** * attempt to modify the characters or free this pointer via `free()`, as doing 
 * so will trigger a catastrophic hardware memory fault.
 */
const char* hal_status_to_string(hal_status_t status);

hal_status_t is_unit_test();

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__HAL_H__