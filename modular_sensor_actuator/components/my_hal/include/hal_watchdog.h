/******************************************************************************
 * @file: hal_watchdog.h
 * @brief: Hardware abstraction interface for the watchdog peripheral.
 *
 * This module defines the platform-independent interface used to initialize
 * and service the hardware watchdog timer. The implementation is responsible
 * for configuring the target microcontroller's watchdog peripheral and
 * handling any hardware-specific details.
 *
 * Higher-level modules should use this interface rather than accessing
 * watchdog registers directly. This allows the watchdog functionality to be
 * reused across different hardware platforms by replacing only the HAL
 * implementation.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-22
 * @license: MIT License
 ******************************************************************************/

#ifndef __HAL_WATCHDOG_H__
#define __HAL_WATCHDOG_H__

#include <stdint.h>

/**
 * @brief Initializes the hardware watchdog timer.
 *
 * Configures and enables the watchdog peripheral with the requested timeout
 * period.
 *
 * @param timeout_ms Watchdog timeout period in milliseconds.
 */
void hal_watchdog_init(uint32_t timeout_ms);

/**
 * @brief Refreshes the hardware watchdog timer.
 *
 * Resets the watchdog countdown timer. This function must be called
 * periodically before the timeout expires to prevent a watchdog reset.
 */
void hal_watchdog_kick(void);

#endif // __HAL_WATCHDOG_H__
