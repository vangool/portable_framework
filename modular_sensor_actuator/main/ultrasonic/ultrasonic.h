/**
 * @file ultrasonic.h
 * @brief Polymorphic Hardware-Agnostic Distance Sensing Interface
 *
 * @details
 * This header defines the universal, platform-agnostic contract for ultrasonic and 
 * distance-ranging sensors within the framework. By abstracting the generic capabilities 
 * of distance measurement (triggering, data acquisition, and state management) away from 
 * specific silicon variants, this interface allows upper application layers to remain 
 * completely blind to the underlying sensor hardware model.
 *
 * @note Reference Implementation: For this framework instantiation, this interface is 
 * backed by the `US100.c` driver module, leveraging asynchronous edge-triggered GPIO 
 * Interrupt Service Routines (ISRs) for high-precision microsecond pulse capture.
 *
 * @section Interface_Abstractions Polymorphic Execution Mapping
 * * [ Application Layer (main.c) ] 
 * |
 * v  (Calls generic interface methods)
 * +------------------------------------+
 * |            ultrasonic.h            |  <-- Generic Contract
 * +------------------------------------+
 * |
 * +--------+-----------+
 * | (Current Bind)     | (Potential Future Scale)
 * v                    v
 * [ US100.c ]       [ HC-SR04.c ]
 *
 * @section Design_Constraints Key Design Specifications
 * - Unified Lifecycle Contract: Mandates a standardized, non-blocking control pipeline 
 * across all implementing driver sub-modules.
 * - Zero Heap Dependency: All polymorphic driver bindings must operate strictly within 
 * static or stack-bounded memory to guarantee deterministic execution bounds.
 * - Width-Bounded Data Scaling: Enforces strict type compliance using the <stdint.h> 
 * taxonomy to guarantee raw time-of-flight translations scale uniformly on all targets.
 *
 * @copyright Copyright (c) 2026. All Rights Reserved.
 */

#ifndef __US_100_H__
#define __US_100_H__

#include <stdbool.h>

#include "../../components/my_hal/include/hal.h"
#include "../../components/osal/include/osal.h"

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/**
 * @brief Dynamic configuration structure for an individual ultrasonic sensor instance.
 * @details This structure isolates instance-specific pin mappings, asynchronous state tracking 
 * timestamps, and RTOS task notification handles to allow multi-sensor scaling without global variable leaks.
 */
typedef struct 
{
    uint8_t trig_pin;
    uint8_t echo_pin;
    volatile uint64_t start_time;
    volatile uint64_t end_time;
    osal_taskhandle_t task_to_notify;
    
} ultrasonic_config_t ;

/**
 * @brief Standardized data packet wrapper for pipeline communication layers.
 * @details Encapsulates finalized distance measurements alongside metadata, allowing telemetry data 
 * to be safely pushed into OSAL queues or transmitted via UART serialize routines.
 */
typedef struct
{
    uint8_t sensor_id;
    uint8_t decimal_places;
    int16_t distance;
} sensor_msg_t;

/**
 * @brief Initializes the low-level hardware modules necessary for the reference US100 sensor.
 * @details Configures global clock gating, UART lines, and peripheral matrices needed to process 
 * the baseline US100 transceiver interface.
 */
void US100_setup();

/**
 * @brief Simple synchronous distance capture utility for the primary reference sensor.
 * @return int32_t Computed distance value, or a negative integer denoting a timeout/hardware fault.
 */
int32_t US100_get_distance();

/**
 * @brief Extracts the current hardware interrupt configuration flag matrix.
 * @return hal_intr_flag_t Target-specific flag state denoting current operational ISR constraints.
 */
hal_intr_flag_t get_intr_flag();

/**
 * @brief Registers a new ultrasonic hardware configuration instance into the runtime ecosystem.
 * @details Binds the specified trigger/echo pins to the underlying HAL matrix and maps the 
 * respective echo edge-transitions directly to the global asynchronous ISR engine.
 * @param[in] config Pointer to the static sensor configuration struct.
 * @return hal_status_t HAL_STATUS_OK on successful peripheral allocation, or failure code.
 */
hal_status_t add_new_ultrasonic_sensor(ultrasonic_config_t*);

/**
 * @brief Processes raw microsecond timestamps into a localized, scaled centimeter measurement.
 * @details Computes the mathematical time-of-flight delta ($\Delta t = \text{end\_time} - \text{start\_time}$) 
 * and applies environmental sonic speed constants to extract a stable, width-bounded output packet.
 * @param[in]  config Pointer to the sensor configuration containing raw tracking timestamps.
 * @param[out] msg    Pointer to the output message pipeline struct to populate with scaled results.
 */
void ultrasonic_get_distance_cm(ultrasonic_config_t*, sensor_msg_t*);

/**
 * @brief Commands the HAL layer to fire a hardware excitation pulse down the designated trigger line.
 * @details Forces the configured trigger pin HIGH for the precise microsecond window required to settle 
 * the transducer array and clear the underlying state-machine timestamps ahead of incoming echo interrupts.
 * @param[in,out] config Pointer to the target sensor configuration instance to stimulate.
 */
void fire_ultrasonic_sensor(ultrasonic_config_t*);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__US_100_H