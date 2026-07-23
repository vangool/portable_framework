/**
 * @file stm32_func.h
 * @brief Hardware Abstraction Layer (HAL) interface for STM32 microcontrollers.
 *
 * This header declares the public interface for the STM32 hardware abstraction
 * layer used by the project. It provides a platform-specific implementation of
 * the project's HAL, allowing application code to interact with STM32 hardware
 * through a consistent, portable API.
 *
 * The STM32 HAL is responsible for interfacing with the underlying hardware,
 * including GPIO, clocks, timers, interrupts, communication peripherals, and
 * other STM32 MCU-specific features required by the application.
 *
 *
 * @author Michael Van Gool
 * @date 2026
 * @copyright Copyright (c) 2026 Michael Van Gool
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

#ifndef __STM32_FUNC_H__
#define __STM32_FUNC_H__

#include "hal.h"

#include "stm32f103/stm32f103.h"
#include "stm32f103/stm32f103_intr.h"

#include <stdint.h>

/**
 * @typedef stm32fn_translate_gpio_mode
 * @brief Converts a generic HAL GPIO configuration mode into an STM32-specific GPIO mode value.
 *
 * This function translates the platform-independent GPIO configuration defined
 * by the HAL into the corresponding STM32 GPIO register configuration value.
 * The returned value can be used when initializing or modifying a GPIO pin.
 *
 * @param[in] config Generic HAL GPIO configuration mode to translate.
 *
 * @return STM32-specific GPIO mode identifier.
 */
typedef uint8_t (*stm32fn_translate_gpio_mode)(hal_gpio_config_t config);

/**
 * @typedef stm32fn_get_port_id
 * @brief Retrieves the STM32 GPIO port identifier associated with a pin.
 *
 * This function converts a HAL pin identifier into the STM32 GPIO port index
 * required for accessing the corresponding GPIO peripheral block.
 *
 * @param[in] pin STM32 packed pin.
 *
 * @return GPIO port identifier.
 */
typedef uint8_t (*stm32fn_get_port_id)(uint8_t pin);

/**
 * @typedef stm32fn_get_gpio_port_block
 * @brief Retrieves the memory-mapped GPIO peripheral block for a port.
 *
 * This function returns a pointer to the STM32 GPIO register block associated
 * with the requested GPIO port. The returned pointer can be used to access
 * GPIO registers directly.
 *
 * @param[in] port GPIO port identifier.
 *
 * @return Pointer to the GPIO peripheral register block.
 */
typedef volatile GPIO_BLOCK* (*stm32fn_get_gpio_port_block)(uint8_t port);

/**
* @typedef stm32fn_verify_pin
* @brief Validates whether a GPIO port or pin configuration is supported.
*
* This function verifies that the supplied GPIO identifier represents a valid
* and available STM32 GPIO resource.
*
* @param[in] port GPIO port identifier to validate.
*
* @return HAL status code indicating whether the validation succeeded.
*/
typedef hal_status_t (*stm32fn_verify_pin)(uint8_t port);

/**
 * @typedef stm32fn_is_TX_pin
 * @brief Determines whether a pin supports UART transmit functionality.
 *
 * This function checks whether the specified pin is configured or capable of
 * being used as a UART TX pin on the target STM32 device.
 *
 * @param[in] pin Packed pin identifier to check.
 *
 * @return HAL status code indicating whether the pin supports TX functionality.
 */
typedef hal_status_t (*stm32fn_is_TX_pin)(uint8_t pin);

/**
 * @typedef stm32fn_is_RX_pin
 * @brief Determines whether a pin supports UART receive functionality.
 *
 * This function checks whether the specified pin is configured or capable of
 * being used as a UART RX pin on the target STM32 device.
 *
 * @param[in] pin Packed pin identifier to check.
 *
 * @return HAL status code indicating whether the pin supports RX functionality.
 */
typedef hal_status_t (*stm32fn_is_RX_pin)(uint8_t pin);


/* Active Implementation */
extern stm32fn_translate_gpio_mode stm32fn_translate_gpio_mode_func;
extern stm32fn_get_port_id stm32fn_get_port_id_func;
extern stm32fn_get_gpio_port_block stm32fn_get_gpio_port_block_func;
extern stm32fn_verify_pin stm32fn_verify_pin_func;
extern stm32fn_is_TX_pin stm32fn_is_TX_pin_func;
extern stm32fn_is_RX_pin stm32fn_is_RX_pin_func;

#endif //__STM32_FUNC_H__