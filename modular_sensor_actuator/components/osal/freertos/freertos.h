/******************************************************************************
 * @file: freertos.h
 * @brief: Target-specific implementation wrapper for the FreeRTOS kernel.
 *
 * This file implements the Operating System Abstraction Layer (OSAL) contracts
 * specifically for the FreeRTOS environment. It maps abstract OS primitives 
 * (such as tasks, mutexes, and queues) directly to native FreeRTOS SDK functions, 
 * serving as the translation layer between the platform-agnostic application 
 * and the underlying kernel scheduler running on the target hardware.
 *
 * @author: Michael Van GOol
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

#ifndef FREERTOS_H_
#define FREERTOS_H_

// Core FreeRTOS include files (FreeRTOS.h, task.h, queue.h, semphr.h) 
// and the concrete mapping implementations for your OSAL definitions go here...

#endif /* FREERTOS_H_ */

#ifndef __FREERTOS_H__
#define __FREERTOS_H__

#include "../include/osal.h"
#include "freertos/FreeRTOS.h"

UINT_M osal_max_delay = portMAX_DELAY;

/**
 * @brief Notify a task from interrupt context.
 *
 * Sends a task notification to the specified task handle from an
 * interrupt service routine (ISR). This can be used to unblock a
 * task waiting on a notification via ulTaskNotifyTake().
 *
 * This function is safe to call from interrupt context.
 *
 * @note Wraps FreeRTOS vTaskNotifyGiveFromISR() and calls FreeRTOS
 * portYIELD_FROM_ISR.
 *
 * @param[in] task Task to notify.
 */
void freertos_tasknotify_give_from_ISR(osal_taskhandle_t task);

/**
 * @brief Retrieve the handle of the currently executing task.
 *
 * Stores the FreeRTOS handle of the calling task in the provided
 * OSAL task handle structure. The handle can later be used for
 * task notifications or other task-related operations.
 *
 * This function must be called from task context.
 *
 * @note Wraps FreeRTOS xTaskGetCurrentTaskHandle
 * @param task_handle[in] Pointer to an OSAL task handle structure that
 *                    receives the current task handle.
 */
void freertos_get_current_task_handle(osal_taskhandle_t* task);

void freertos_task_delay_ms(uint32_t delay_ms);
void freertos_task_delay_until_ms(osal_Tick* from_ms, uint32_t total_ms);
osal_Tick freertos_get_tick_count();
int16_t freertos_action_upon_notification(osal_bool_t clear_on_exit, uint32_t ms_to_wait);
osal_result_t freertos_create_task(
    osal_function_ptr function_ptr,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void* pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t *task_handler_var);

osal_base_type freertos_pop_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);
osal_base_type freertos_push_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);
osal_queue_t freertos_create_queue(osal_ubase_type queue_length, osal_ubase_type item_size);



#endif //__FREERTOS_H__