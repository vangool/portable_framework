/******************************************************************************
 * File: freertos.h
 * Description: Wrapper for this application and the associated FreeRTOS
 *          functions. 
 * Author: Michael Van Gool
 * Date: 2026-05-04
 * License: MIT
******************************************************************************/

#ifndef __FREERTOS_H__
#define __FREERTOS_H__

#include "../include/osal.h"


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

#endif //__FREERTOS_H__