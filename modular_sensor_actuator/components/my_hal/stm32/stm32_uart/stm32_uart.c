/**
 * @file stm32_uart.c
 * @brief STM32 UART hardware abstraction layer implementation.
 *
 * This source file provides the STM32-specific implementation of the UART
 * hardware abstraction layer. It translates the generic HAL UART interface
 * into STM32 peripheral configuration and register operations.
 *
 * The module is responsible for configuring STM32 UART/USART peripherals,
 * assigning GPIO pins using the appropriate alternate-function mappings, and
 * providing transmit and receive functionality through the HAL interface.
 *
 * Responsibilities include:
 * - UART peripheral initialization
 * - UART clock and peripheral configuration
 * - GPIO configuration for UART TX/RX pins
 * - UART data transmission
 * - UART data reception
 * - STM32-specific UART resource management
 *
 * This module should not be accessed directly by application code. Applications
 * should use the platform-independent HAL UART interface, which internally
 * selects the appropriate platform implementation.
 *
 * @note This file contains STM32-specific implementation details and relies on
 *       STM32 UART/USART peripherals.
 *
 * @ingroup STM32_HAL
 *
 * @author Michael Van Gool
 * @date 2026-07-19
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

#include "stm32_uart.h"

hal_uart_init hal_uart_init_func = stm32_uart_init;
hal_uart_receive hal_uart_receive_func = stm32_uart_receive;
hal_uart_transmit hal_uart_transmit_func = stm32_uart_transmit;

hal_status_t stm32_uart_init(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin)
{
    hal_status_t ret = stm32fn_is_TX_pin_func(transmit_pin) | stm32fn_is_RX_pin_func(receive_pin);
    if(ret != HAL_STATUS_OK && receive_pin - transmit_pin == 1)
    {
        return HAL_ERROR_INVALID_ARG;
    }

    uint8_t port = (transmit_pin >> 6) & 3;
    volatile GPIO_BLOCK* port_addr = stm32fn_get_gpio_port_block_func(port);
    if(transmit_pin >= 8)
    {
        port_addr->CRH.all &= ~(0xF << ((transmit_pin - 8) * 4));
        port_addr->CRH.all |= (0xB << ((transmit_pin - 8) * 4));
    }
    else
    {
        port_addr->CRL.all &= ~(0xF << (transmit_pin * 4));
        port_addr->CRL.all |= (0xB << (transmit_pin * 4));
    }

    int apb2_bd_divisor = APB2_CLK_SPEED / config->baud_rate;
    int apb1_bd_divisor = APB1_CLK_SPEED / config->baud_rate;
    switch(transmit_pin)
    {
        case STM_PIN_PA09:
            APB2_REG->bits.USART1_EN = 1;
            USART1_REG->BRR = apb2_bd_divisor;
            USART1_REG->CR1.bits.UE = 1;
            USART1_REG->CR1.bits.TE = 1;
            USART1_REG->CR1.bits.RE = 1;
            break;
        case STM_PIN_PA02:
            APB1_REG->bits.USART2_EN = 1;
            USART2_REG->BRR = apb1_bd_divisor;
            USART2_REG->CR1.bits.UE = 1;
            USART2_REG->CR1.bits.TE = 1;
            USART2_REG->CR1.bits.RE = 1;
            break;
        case STM_PIN_PB10:
            APB1_REG->bits.USART3_EN = 1;
            USART3_REG->BRR = apb1_bd_divisor;
            USART3_REG->CR1.bits.UE = 1;
            USART3_REG->CR1.bits.TE = 1;
            USART3_REG->CR1.bits.RE = 1;
            break;

    }
    return HAL_STATUS_OK;
}

void stm32_uart_receive(hal_uart_port_t port)
{
    volatile USART_BLOCK* USART_REG;
    switch (port)
    {
        case HAL_UART_PORT_0:
            USART_REG = USART1_REG;
            break;
        case HAL_UART_PORT_1:
            USART_REG = USART2_REG;
            break;
        case HAL_UART_PORT_2:
            USART_REG = USART3_REG;
            break;
    
        default:
            return;
    }

    while(!(USART_REG->SR.bits.RXNE)){}

    uint8_t rx_byte = 0; 
    rx_byte = (uint8_t)USART_REG->DR & 0xFF;
}

void stm32_uart_transmit(const char* msg, hal_uart_port_t port)
{
    volatile USART_BLOCK* USART_REG;
    switch (port)
    {
        case HAL_UART_PORT_0:
            USART_REG = USART1_REG;
            break;
        case HAL_UART_PORT_1:
            USART_REG = USART2_REG;
            break;
        case HAL_UART_PORT_2:
            USART_REG = USART3_REG;
            break;
    
        default:
            return;
    }

    for(int i = 0; *(msg + i) != '\0' && i < 20; i++)
    {
        while (!(USART_REG->SR.bits.TXE));
        USART_REG->DR = ( *(msg + i) & 0xFF);
    }

    while (!(USART_REG->SR.bits.TC));
}