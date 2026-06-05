/******************************************************************************
 * File: osal.h
 * @brief Operating System Abstraction Layer (OSAL) interface.
 * 
 * Description: Provides a portable API for interacting with the operating
 *      system through platform-specific implementations. This allows 
 *      application code to remain independent of the underlying operating
 *      system or SDK.
 * 
 * Author: Michael Van Gool
 * Date: 2026-05-04
 * License: MIT
******************************************************************************/
#ifndef __OSAL_H__
#define __OSAL_H__

typedef enum 
{
    OSAL_OK = 0,
    OSAL_ERROR,
    OSAL_TIMEOUT
} osal_status_t;

typedef enum 
{
    OSAL_FAIL = 0,
    OSAL_PASS
} osal_result_t;

typedef enum 
{
    OSAL_FALSE = 0,
    OSAL_TRUE
} osal_bool_t;

typedef struct 
{
    void *taskhandle;
} osal_taskhandle_t;

/**
 * @brief Notify a task from interrupt context.
 *
 * Sends a task notification to the specified task handle from an
 * interrupt service routine (ISR). This can be used to unblock a
 * task waiting on a notification via ulTaskNotifyTake().
 *
 * This function is safe to call from interrupt context.
 * 
 * This function should also include a function similar to FreeRTOS
 * portYIELD_FROM_ISR.
 *
 * @param[in] task Task to notify.
 */
typedef void (*osal_task_notify_give_from_isr)(osal_taskhandle_t task);

/**
 * @brief Retrieve the handle of the currently executing task.
 *
 * Stores the FreeRTOS handle of the calling task in the provided
 * OSAL task handle structure. The handle can later be used for
 * task notifications or other task-related operations.
 *
 * This function must be called from task context.
 *
 * @param taskhandler[in] Pointer to an OSAL task handle structure that
 *                    receives the current task handle.
 */
typedef void (*osal_get_current_task_handle)(osal_taskhandle_t* taskhandler);

/* Active OS implementations */
extern osal_task_notify_give_from_isr osal_task_notify_give_from_isr_func;
extern osal_get_current_task_handle osal_get_current_task_handle_func;

/* Default OS Implementations */
void osal_task_notify_give_from_isr_default(osal_taskhandle_t task);
void osal_get_current_task_handle_default(osal_taskhandle_t* taskhandler);

#endif //__OSAL_H__