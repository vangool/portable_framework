/******************************************************************************
 * @file: watchdog.c
 * @brief: Software watchdog client manager.
 *
 * This module implements a software watchdog layer that monitors the health of
 * registered system tasks or worker threads. Each client is assigned a unique
 * bit within a heartbeat mask and is responsible for periodically reporting
 * its health by calling watchdog_feed().
 *
 * At the end of each watchdog interval, watchdog_check() verifies that every
 * registered client has reported by comparing the collected heartbeat mask
 * against the set of registered clients. If all clients have reported, the
 * software heartbeat is cleared and the underlying hardware watchdog is
 * refreshed through the hardware abstraction layer (HAL). If any client fails
 * to report, the hardware watchdog is intentionally not serviced, allowing it
 * to expire and reset the system.
 *
 * This module is hardware-independent and relies on hal_watchdog.c to interact
 * with the microcontroller's watchdog peripheral.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-22
 * @license: MIT License
 ******************************************************************************/


#include "watchdog.h"

static wd_client_id_t heartbeat_bitmap = 0;
static wd_client_id_t client_bitmap = 0;
static uint8_t next_wdc_id = 0;



void watchdog_feed(wd_client_id_t client_id)
{
    heartbeat_bitmap |= client_id;
}

void watchdog_check()
{
    if((heartbeat_bitmap & client_bitmap) == client_bitmap)
    {
        heartbeat_bitmap = 0;
        hal_watchdog_kick();
    }
}

wd_client_id_t watchdog_client_add()
{
    if(next_wdc_id >= WD_MAX_CLIENTS)
    {
        return WD_INVALID_CLIENT;
    }

    wd_client_id_t id = (wd_client_id_t)1u << next_wdc_id;
    next_wdc_id++;

    client_bitmap |= id;

    return id;
}

