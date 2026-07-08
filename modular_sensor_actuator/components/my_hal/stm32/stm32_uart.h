/**
 * @file stm32_uart.c
 * @brief STM32 UART hardware abstraction layer implementation.
 *
 * This source file provides the STM32-specific implementation of the UART
 * functionality defined by the project hardware abstraction layer (HAL).
 *
 * The implementation translates platform-independent HAL UART operations into
 * STM32-specific peripheral configuration, GPIO alternate-function setup, and
 * UART register operations.
 *
 * This module provides functionality for:
 * - UART peripheral initialization
 * - UART transmit and receive operations
 * - UART GPIO configuration
 * - STM32-specific UART peripheral handling
 *
 * Application code should use the platform-independent HAL interface rather
 * than calling functions from this module directly.
 *
 * @note This file contains STM32-specific implementation details.
 *
 * @author Michael Van Gool
 * @date 2026-07-07
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

#ifndef __STM32_UART_H__
#define __STM32_UART_H__

#include "hal_uart.h"
#include "stm32_func.h"

/**
 * @brief Initializes an STM32 UART peripheral.
 *
 * Configures the selected UART peripheral using the provided HAL UART
 * configuration structure and assigns the GPIO pins used for UART transmission
 * and reception.
 *
 * This function performs STM32-specific UART initialization, including
 * peripheral configuration, clock setup, and GPIO alternate-function mapping
 * required for UART communication.
 *
 * @param[in] config Pointer to the UART configuration structure containing 
 * parameters such as baud rate, data bits, stop bits, parity, and UART 
 * peripheral selection.
 *
 * @param[in] transmit_pin GPIO pin identifier used for UART transmission (TX).
 *
 * @param[in] receive_pin GPIO pin identifier used for UART reception (RX).
 *
 * @return HAL status code indicating whether initialization was successful.
 */
hal_status_t stm32_uart_init(hal_uart_config_t* config,
                             uint8_t transmit_pin,
                             uint8_t receive_pin);

/**
 * @brief Receives data from an STM32 UART peripheral.
 *
 * Waits for incoming UART data on the specified UART port and processes the
 * received data according to the HAL UART implementation.
 *
 * The behavior of this function depends on the configured UART mode. It may
 * operate using polling, interrupt-driven reception, or another STM32-specific
 * receive mechanism.
 *
 * @param[in] portUART peripheral identifier used to select the UART instance.
 *
 * @return None.
 */
void stm32_uart_receive(hal_uart_port_t port);

/**
 * @brief Transmits a message through an STM32 UART peripheral.
 *
 * Sends a null-terminated character string through the selected UART port.
 * The function handles STM32-specific UART transmission details while exposing
 * a platform-independent interface to higher-level application code.
 *
 * @param[in] msg Pointer to the null-terminated string to transmit.
 *
 * @param[in] port UART peripheral identifier used to select the UART instance.
 *
 * @return
 * None.
 */
void stm32_uart_transmit(const char* msg, hal_uart_port_t port);

#endif //__STM32_UART_H__