/******************************************************************************
 * @file: hal_uart.c
 * @brief: Default stub implementation of the platform-agnostic UART HAL interface.
 *
 * This file provides fallback implementations for the global UART HAL functions.
 * It serves primarily as a hardware-in-the-loop (HIL) testing mechanism, a mockup 
 * for host-side simulation environments, or as a default catch-all error state 
 * for unsupported targets.
 *
 * @author: Michael Van Gool
 * @date: 2026-05-04
 * @license: MIT License (See header file for full license conditions)
 * Copyright (c) 2026 Michael Van Gool. All rights reserved.
 ******************************************************************************/
#include "hal_uart.h"
#include "../../../main/utils.h"

hal_status_t hal_uart_init_default(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin)
{
    return is_unit_test();
}

void hal_uart_receive_default()
{
    return is_unit_test();
}

void hal_uart_transmit_default(const char* msg)
{
    return is_unit_test();
}
