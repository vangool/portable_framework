/******************************************************************************
 * @file: my_os.h
 * @brief: Operating System Abstraction Layer (OSAL) backend for the custom STM32 scheduler.
 *
 * This file declares the backend implementation used by the OSAL when executing
 * on the custom STM32 preemptive scheduler. These functions provide the concrete
 * implementation behind the platform-independent OSAL API, exposing task
 * scheduling, timing, notifications, and queue operations while allowing the
 * application layer to remain independent of the underlying scheduler.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-13
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

#ifndef __MY_OS_H__
#define __MY_OS_H__

#include "osal.h"

UINT_M osal_max_delay = ~0;

/**
 * @brief Sends a task notification from an Interrupt Service Routine (ISR).
 *
 * Signals the specified task that an asynchronous event has occurred while
 * executing in interrupt context. This function provides the scheduler backend
 * implementation used by the OSAL notification API.
 *
 * @param taskhandle Handle of the task to be notified.
 *
 * @note This function is intended for use only within an ISR.
 * 
 * @note This function is not yet implemented for the STM32 custom scheduler
 */
void my_os_task_notify_give_from_isr(osal_taskhandle_t taskhandle);

/**
 * @brief Retrieves the handle of the currently executing task.
 *
 * Stores the scheduler's active task handle into the supplied OSAL wrapper,
 * allowing application code to reference the currently running task without
 * exposing scheduler internals.
 *
 * @param taskhandler Pointer to the OSAL task handle wrapper to receive the
 *                    currently executing task.
 * 
 * @note This function is not yet implemented for the STM32 custom scheduler
 */
void my_os_get_current_task_handle(osal_taskhandle_t* taskhandler);

/**
 * @brief Delays execution of the current task for a fixed duration.
 *
 * Suspends the calling task for the requested number of milliseconds,
 * allowing the scheduler to execute other ready tasks until the delay
 * interval expires.
 *
 * @param delay_ms Delay duration in milliseconds.
 */
void my_os_task_delay_ms(uint32_t delay_ms);

/**
 * @brief Performs a periodic delay using an absolute time reference.
 *
 * Delays the calling task relative to a previous wake-up time rather than
 * the current execution time. This helps maintain a consistent execution
 * period for cyclic tasks.
 *
 * @param from_ms Pointer to the previous wake-up tick.
 * @param total_ms Desired execution period in milliseconds.
 */
void my_os_task_delay_until_ms(osal_Tick* from_ms, uint32_t total_ms);

/**
 * @brief Retrieves the current system tick count.
 *
 * Returns the scheduler tick maintained by the custom operating system
 * backend. This value is typically incremented from the SysTick interrupt.
 *
 * @return Current scheduler tick count.
 */
osal_Tick my_os_get_tick_count();

/**
 * @brief Waits for a task notification.
 *
 * Suspends the calling task until a notification is received or the timeout
 * period expires. Notification state may optionally be cleared upon exit.
 *
 * @param clear_on_exit Specifies whether the notification state should be
 *                      cleared before returning.
 * @param ms_to_wait Maximum time to wait in milliseconds.
 * 
 * @note This function is not yet implemented for the STM32 custom scheduler
 *       and currently returns -1
 *
 * @return Implementation-defined notification result.
 */
int16_t my_os_action_upon_notification(osal_bool_t clear_on_exit,
                                       uint32_t ms_to_wait);

/**
 * @brief Creates a new scheduler task.
 *
 * This function provides a basic implementation of the OSAL task creation
 * interface by creating a task using the custom scheduler backend.
 *
 * The current implementation supports task function execution and parameter
 * passing. Task naming, stack depth configuration, priority assignment, and
 * task handle retrieval are not currently supported by the custom scheduler.
 *
 * @param task_function Pointer to the task entry function.
 * @param task_name Name assigned to the task (currently unused).
 * @param usStackDepth Requested stack depth (currently unused).
 * @param pvParameters Parameter passed to the task function.
 * @param priority Task priority (currently unused).
 * @param task_handler_var Pointer to store task handle (currently unused).
 *
 * @return OSAL_SUCCESS if the task is created successfully.
 */
osal_result_t my_os_create_task(
    osal_function_ptr task_function,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void *pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t* task_handler_var
);

/**
 * @brief Retrieves an item from an OSAL queue.
 *
 * Removes an item from the specified queue and copies it into the supplied
 * application buffer.
 *
 * @param queue Queue to read from.
 * @param pvBuffer Destination buffer.
 * @param wait_time Maximum time to wait for an item.
 *
 * @note This function is not yet implemented for the STM32 custom scheduler
 *       and currently returns -1
 * 
 * @return Backend-specific queue operation result.
 */
osal_base_type my_os_dequeue(osal_queue_t queue,
                               void* pvBuffer,
                               osal_Tick wait_time);

/**
 * @brief Inserts an item into an OSAL queue.
 *
 * Copies application data into the specified queue for later retrieval by
 * another execution context.
 *
 * @param queue Queue to write to.
 * @param pvBuffer Pointer to the item being enqueued.
 * @param wait_time Maximum time to wait if the queue is full.
 *
 * @note This function is not yet implemented for the STM32 custom scheduler
 *       and currently returns -1
 * 
 * @return Backend-specific queue operation result.
 */
osal_base_type my_os_enqueue(osal_queue_t queue,
                                void* pvBuffer,
                                osal_Tick wait_time);

/**
 * @brief Creates an OSAL queue.
 *
 * Allocates and initializes a queue capable of storing a fixed number of
 * equally sized items.
 *
 * @param queue_length Maximum number of elements the queue can contain.
 * @param item_size Size of each queue element in bytes.
 *
 * @note This function is not yet implemented for the STM32 custom scheduler
 *       and currently returns an empty queue
 * 
 * @return Handle to the newly created queue.
 */
osal_queue_t my_os_create_queue(osal_ubase_type queue_length,
                                osal_ubase_type item_size);

#endif // __MY_OS_H__