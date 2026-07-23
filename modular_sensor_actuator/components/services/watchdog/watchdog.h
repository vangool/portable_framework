/******************************************************************************
 * @file: watchdog.h
 * @brief: Public interface for the software watchdog manager.
 *
 * This module provides a lightweight software watchdog for monitoring the
 * health of registered system tasks or worker threads. Each client registers
 * with the watchdog and receives a unique client identifier, which must be
 * periodically fed to indicate continued execution.
 *
 * The watchdog manager tracks all registered clients over a monitoring
 * interval. During each watchdog check, it verifies that every registered
 * client has reported. If one or more clients fail to feed within the
 * configured period, the watchdog may invoke the underlying hardware watchdog
 * or execute another platform-specific recovery action.
 *
 * The hardware-specific watchdog implementation is provided through the
 * hardware abstraction layer (HAL), allowing this module to remain portable
 * across supported platforms.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-22
 * @license: MIT License
 ******************************************************************************/

#ifndef __WATCHDOG_H__
#define __WATCHDOG_H__

#include <stdint.h>
#include <stdbool.h>

#include "hal_watchdog.h"

/**
 * @brief Bitmask representing a registered watchdog client.
 *
 * Each client is assigned a unique bit within a 32-bit mask.
 * The returned value should be stored by the caller and passed
 * to watchdog_feed() whenever the client has completed a healthy
 * execution cycle.
 */
typedef uint32_t wd_client_id_t;

/** Maximum number of watchdog clients supported. */
#define WD_MAX_CLIENTS (sizeof(wd_client_id_t) * 8)

/** Returned when client registration fails. */
#define WD_INVALID_CLIENT 0

/**
 * @brief Registers a new watchdog client.
 *
 * Allocates a unique client ID that can be used to report health
 * to the watchdog.
 *
 * @return A unique client ID, or WD_INVALID_CLIENT if no client
 *         slots are available.
 */
wd_client_id_t watchdog_client_add(void);

/**
 * @brief Reports that a watchdog client is alive.
 *
 * This function should be called periodically by each registered
 * client (typically from a worker thread or task). Calling this
 * function marks the client as having successfully executed during
 * the current watchdog period.
 *
 * @param thread_id Client ID returned by watchdog_client_add().
 */
void watchdog_feed(wd_client_id_t thread_id);

/**
 * @brief Evaluates watchdog status for all registered clients.
 *
 * Checks whether every registered client has fed the watchdog
 * during the current monitoring interval. If one or more clients
 * have not reported, the watchdog implementation may trigger the
 * configured recovery action (such as resetting the hardware
 * watchdog or logging a fault).
 *
 * This function is typically called periodically by a timer or
 * supervisor task.
 */
void watchdog_check(void);

#endif // __WATCHDOG_H__
