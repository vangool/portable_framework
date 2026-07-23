/******************************************************************************
 * @file: hal_watchdog_stub.c
 * @brief: Stub implementation of the watchdog HAL for testing.
 *
 * This module provides a no-operation implementation of the hardware
 * watchdog interface. It is intended for unit testing, simulation, and
 * development environments where the target watchdog peripheral is not
 * available or should not be enabled.
 *
 * The functions in this module maintain the same interface as the production
 * watchdog HAL implementation but do not configure hardware or perform any
 * watchdog refresh operations.
 *
 * This allows higher-level modules, such as the software watchdog manager, to
 * be tested independently from the microcontroller watchdog peripheral.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-22
 * @license: MIT License
 ******************************************************************************/

#include "hal_watchdog.h"

/**
 * @brief Initializes the watchdog stub.
 *
 * This function intentionally performs no operation. The timeout parameter is
 * ignored because no hardware watchdog is configured in the test environment.
 *
 * @param timeout_ms Watchdog timeout period in milliseconds.
 */
void hal_watchdog_init(uint32_t timeout_ms)
{
    (void)timeout_ms;
}

/**
 * @brief Services the watchdog stub.
 *
 * This function intentionally performs no operation.
 */
void hal_watchdog_kick(void)
{
}

