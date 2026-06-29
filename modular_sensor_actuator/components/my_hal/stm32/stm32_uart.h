#ifndef __STM32_UART_H__
#define __STM32_UART_H__

#include "hal_uart.h"
#include "stm32_func.h"

hal_status_t stm32_uart_init(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin);
void stm32_uart_receive(hal_uart_port_t port);
void stm32_uart_transmit(const char* msg, hal_uart_port_t port);

#endif //__STM32_UART_H__