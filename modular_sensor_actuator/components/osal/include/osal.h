/******************************************************************************
 * @file: osal.h
 * @brief: Top-level platform-agnostic Operating System Abstraction Layer (OSAL) API.
 *
 * This file defines a universal, RTOS-independent interface for core system 
 * primitives. By isolating operations like task creation, mutex locking, semaphore 
 * signaling, and queue management, this layer ensures that upper-level firmware 
 * remains completely decoupled from a specific kernel (such as FreeRTOS, Zephyr, 
 * or ThreadX) and can easily adapt to bare-metal schedulers or host-side simulators.
 *
 * @author: Michael Van Gool
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
#ifndef __OSAL_H__
#define __OSAL_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if UINTPTR_MAX == 0xFFFFFFFFFFFFFFFFULL
    #define SYS_SIZE    64
    typedef uint64_t    UINT_M;
    typedef int64_t     INT_M;
#elif UINTPTR_MAX == 0xFFFFFFFFUL
    #define SYS_SIZE    32
    typedef uint32_t    UINT_M;
    typedef int32_t     INT_M;
#elif UINTPTR_MAX == 0xFFFFU
    #define SYS_SIZE    16
    typedef uint16_t    UINT_M
    typedef int16_t     INT_M
#else
    #error "Unsupported target architecture pointer size"
#endif

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

typedef struct
{
    void *queue;
} osal_queue_t;

typedef UINT_M osal_Tick;
typedef UINT_M osal_ubase_type;
typedef INT_M  osal_base_type;
typedef UINT_M osal_stack_depth;
extern UINT_M osal_max_delay;

static inline bool osal_queue_is_valid(osal_queue_t q)
{
    return (q.queue != NULL);
}

typedef void (*osal_function_ptr)(void*);
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

/**
 * @brief Function pointer type for a simple relative task delay.
 *
 * Yields processor execution to lower-priority tasks for a minimum specified 
 * duration. This is a relative delay; the actual time the task spends blocked 
 * can be extended by interrupts or higher-priority tasks.
 *
 * @param[in] delay_ms  The duration to block the calling task, in milliseconds.
 */
typedef void (*osal_task_delay_ms)(uint32_t delay_ms);

/**
 * @brief Function pointer type for an absolute/periodic task delay.
 *
 * Blocks the calling task until an absolute wake time is reached. This type 
 * is vital for executing deterministic, constant-frequency loops (e.g., control 
 * loops), as it automatically accounts for the execution time of the loop body.
 *
 * @param[in,out] from_ms   Pointer to a variable holding the last wake time. 
 * The underlying RTOS updates this value automatically 
 * upon unblocking.
 * @param[in]     total_ms  The total cycle period length, in milliseconds.
 */
typedef void (*osal_task_delay_until_ms)(osal_Tick* from_ms, uint32_t total_ms);

/**
 * @brief Function pointer type to retrieve the system uptime clock tick count.
 *
 * Returns the total number of clock ticks elapsed since the microcontroller 
 * booted up. Used for high-level timing calculations and software timeouts.
 *
 * @return The current system uptime measurement in native OS ticks (`osal_Tick`).
 */
typedef osal_Tick (*osal_get_tick_count)();

/**
 * @brief Function pointer type for blocking on a lightweight direct-to-task notification.
 *
 * Forces the calling task into a blocked state until it receives an event notification 
 * or signal from an interrupt service routine (ISR) or another running task.
 *
 * @param[in] clear_on_exit If set to true, clears the internal notification state 
 * upon unblocking.
 * @param[in] ms_to_wait     The maximum duration to wait for a notification before 
 * timing out, in milliseconds.
 *
 * @return An integer status representing the notification state, or a negative value 
 * if a timeout occurred.
 */
typedef int16_t (*osal_action_upon_notification)(osal_bool_t clear_on_exit, uint32_t ms_to_wait);

/**
 * @brief Function pointer type for dynamic thread/task creation.
 *
 * Allocates runtime memory structures and spins up a new independent thread of execution 
 * inside the underlying RTOS kernel scheduler.
 *
 * @param[in]  task_function      Pointer to the core function code executing the task loop.
 * @param[in]  task_name          A descriptive name string for debugging and identification.
 * @param[in]  usStackDepth       The unique memory footprint size allocated to this task's stack.
 * @param[in]  pvParameters       Pointer to context data passed directly into the function loop.
 * @param[in]  priority           The scheduler execution priority layer assigned to the task.
 * @param[out] task_handler_var   Pointer to an opaque storage handle used to modify or terminate 
 * the task later.
 *
 * @return An `osal_result_t` indicating either success (`OSAL_PASS`) or structural 
 * initialization failure.
 */
typedef osal_result_t (*osal_create_task)( 
    osal_function_ptr task_function,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void *pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t* task_handler_var);

/**
 * @brief Function pointer type for pulling (receiving) data out of a thread-safe message queue.
 *
 * Copies an item out from the front of the queue into a local memory buffer. If the 
 * queue is empty, the calling task blocks until a message arrives or the timeout expires.
 *
 * @param[in]  queue      The target opaque queue identification handle.
 * @param[out] pvBuffer   Pointer to the destination memory block where data will be copied.
 * @param[in]  wait_time  Maximum time to block waiting for an item, in ticks.
 *
 * @return A base status code mapping to native success (`true`) or timeout failure (`false`).
 */
typedef osal_base_type (*osal_pop_queue)(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);

/**
 * @brief Function pointer type for pushing (sending) data into a thread-safe message queue.
 *
 * Copies an item to the back of the queue. If the queue is full, the calling task blocks 
 * until space becomes available or the timeout expires.
 *
 * @param[in] queue      The target opaque queue identification handle.
 * @param[in] pvBuffer   Pointer to the source memory block containing the data item to copy.
 * @param[in] wait_time  Maximum time to block waiting for space, in ticks.
 *
 * @return A base status code mapping to native success (`true`) or timeout failure (`false`).
 */
typedef osal_base_type (*osal_push_queue)(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);

/**
 * @brief Function pointer type for instantiating a thread-safe message queue peripheral.
 *
 * Dynamically allocates a block of RAM inside the RTOS heap to serve as an inter-task 
 * FIFO (First-In, First-Out) pipeline.
 *
 * @param[in] queue_length The total maximum storage capacity allocation count.
 * @param[in] item_size    The exact memory byte-size requirement of a single data element.
 *
 * @return A unique initialization `osal_queue_t` pointer handle, or NULL if allocation fails.
 */
typedef osal_queue_t (*osal_create_queue)(osal_ubase_type queue_length, osal_ubase_type item_size);

/* Active OS implementations */
extern osal_task_notify_give_from_isr osal_task_notify_give_from_isr_func;
extern osal_get_current_task_handle osal_get_current_task_handle_func;
extern osal_task_delay_ms osal_task_delay_ms_func;
extern osal_task_delay_until_ms osal_task_delay_until_ms_func;
extern osal_get_tick_count osal_get_tick_count_func;
extern osal_action_upon_notification osal_action_upon_notification_func;
extern osal_create_task osal_create_task_func;
extern osal_pop_queue osal_pop_queue_func;
extern osal_push_queue osal_push_queue_func;
extern osal_create_queue osal_create_queue_func;

/* Default OS Implementations */
void osal_task_notify_give_from_isr_default(osal_taskhandle_t task);
void osal_get_current_task_handle_default(osal_taskhandle_t* taskhandler);
void osal_task_delay_ms_default(uint32_t delay_ms);
void osal_task_delay_until_ms_default(osal_Tick* from_ms, uint32_t total_ms);
osal_Tick osal_get_tick_count_default();
int16_t osal_action_upon_notification_default(osal_bool_t, uint32_t);
osal_result_t osal_create_task_default(   
    osal_function_ptr task_function,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void *pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t* task_handler_var);

osal_base_type osal_pop_queue_default(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);
osal_base_type osal_push_queue_default(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time);
osal_queue_t osal_create_queue_default(osal_ubase_type queue_length, osal_ubase_type item_size);


#endif //__OSAL_H__