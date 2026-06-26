/******************************************************************************
 * @file: esp32_uart.h
 * @brief: Hardware Abstraction Layer (HAL) interface for the ESP32 UART peripheral.
 * * This file defines the structures, configuration parameters, and API signatures 
 * required to manage low-level serial communication on the ESP32. It bridges 
 * the platform-agnostic application logic with the native ESP-IDF drivers.
 * * @author: Michael Van Gool
 * @date: 2026-05-04
 * @license: MIT License
 * * Copyright (c) 2026 Michael Van Gool
 * * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/

#ifndef __ESP32_UART_H__
#define __ESP32_UART_H__

#include "../include/hal_uart.h"
#include "driver/uart.h"

/**
 * @brief Initializes the ESP32 UART peripheral with custom configurations and pin mappings.
 *
 * This function configures the internal hardware parameters of the selected UART 
 * instance (such as baud rate, data bits, parity, and stop bits) using the ESP-IDF driver. 
 * It also routes the internal UART signals to the specified physical GPIO pins.
 *
 * @param config       Pointer to the HAL UART configuration structure holding the setup parameters.
 * @param transmit_pin Physical GPIO pin number to be assigned to the UART TX signal.
 * @param receive_pin  Physical GPIO pin number to be assigned to the UART RX signal.
 * * @return hal_status_t Returns HAL_STATUS_OK on successful initialization, or an error code 
 * if the configuration parameters or pin assignments are invalid.
 */
hal_status_t esp32_uart_init(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin);

/**
 * @brief Handles incoming UART data reception.
 *
 * Depending on your low-level implementation, this function either polls the UART 
 * hardware RX FIFO buffer or processes data received via an Interrupt Service Routine (ISR) / FreeRTOS queue. 
 * Received bytes are typically transferred from the hardware layer into a software ring buffer 
 * for application consumption.
 *
 * @note If implemented as a blocking call, ensure it is executed within a dedicated FreeRTOS 
 * task to prevent starving the ESP32 watchdog timers.
 */
void esp32_uart_receive();

/**
 * @brief Transmits data over the configured ESP32 UART peripheral.
 *
 * This function handles moving data from the software application layer into the hardware 
 * UART transmit FIFO or ring buffer. It manages transmission state, waiting for the 
 * physical bus to become clear if the hardware TX FIFO is full.
 *
 * @note Ensure that esp32_uart_init() has been successfully called before invoking this function, 
 * otherwise the transmission will fail or invoke undefined behavior.
 */
void esp32_uart_transmit(const char* msg);

#endif //__ESP32_UART_H__