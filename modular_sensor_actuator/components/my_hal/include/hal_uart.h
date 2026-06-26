/******************************************************************************
 * @file: hal_uart.h
 * @brief: Platform-agnostic Universal Asynchronous Receiver-Transmitter (UART) API.
 *
 * This file establishes a standardized interface for serial communications. 
 * High-level application tasks interact exclusively with the function signatures 
 * and structures defined here, while target-specific files (e.g., `esp32_uart.c` or 
 * `stm32_uart.c`) map these contracts directly to their respective silicon drivers.
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
#ifndef __HAL_UART_H__
#define __HAL_UART_H__

#include "hal.h"

typedef enum {
    HAL_UART_DATA_BITS_5,
    HAL_UART_DATA_BITS_6,
    HAL_UART_DATA_BITS_7,
    HAL_UART_DATA_BITS_8,
    HAL_UART_DATA_BITS_MAX
} hal_uart_data_bits_t;

typedef enum {
    HAL_UART_PARITY_DISABLE,
    HAL_UART_PARITY_EVEN,
    HAL_UART_PARITY_ODD
} hal_uart_parity_t;

typedef enum {
    HAL_UART_STOP_BITS_1,
    HAL_UART_STOP_BITS_1_5,
    HAL_UART_STOP_BITS_2
} hal_uart_stop_bits_t;

typedef enum {
    HAL_UART_FLOW_CONTROL_DISABLE,
    HAL_UART_FLOW_CONTROL_RTS,
    HAL_UART_FLOW_CONTROL_CTS,
    HAL_UART_FLOW_CONTROL_RTS_CTS,
    HAL_UART_FLOW_CONTROL_MAX
} hal_uart_flow_control_t;

typedef struct 
{
    int baud_rate;
    hal_uart_data_bits_t data_bits;
    hal_uart_parity_t parity;
    hal_uart_stop_bits_t stop_bits;
    hal_uart_flow_control_t flow_ctrl;
} hal_uart_config_t;


typedef hal_status_t (*hal_uart_init)(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin);
typedef void (*hal_uart_receive)();
typedef void (*hal_uart_transmit)(const char* msg);

extern hal_uart_init hal_uart_init_func;
extern hal_uart_receive hal_uart_receive_func;
extern hal_uart_transmit hal_uart_transmit_func;

hal_status_t hal_uart_init_default(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin);
void hal_uart_receive_default();
void hal_uart_transmit_default(const char* msg);




#endif //__HAL_UART_H__