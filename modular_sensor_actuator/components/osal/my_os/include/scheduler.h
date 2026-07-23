/******************************************************************************
 * @file: scheduler.h
 * @brief: Public interface for the custom preemptive scheduler.
 *
 * This module defines the task control structures and scheduler API used by
 * the STM32 bare-metal backend. The scheduler implements a lightweight,
 * preemptive round-robin scheduling algorithm driven by the Cortex-M SysTick,
 * SVC, and PendSV exceptions.
 *
 * Tasks are statically allocated, each with an independent stack and Task
 * Control Block (TCB). The scheduler performs context switching, task
 * creation, and sleep management without relying on a third-party RTOS.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-14
 * @license: MIT License
 ******************************************************************************/

#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>

#include "osal.h"
#include "stm32f103.h"
#include "watchdog.h"

typedef enum
{
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED, /**< Reserved for future synchronization support. */
    THREAD_SLEEP
} osal_thread_state_t;

typedef struct 
{
    uint32_t* stack_ptr;
    osal_function_ptr func;
    uint32_t next_wakeup;
    osal_thread_state_t state; 
    wd_client_id_t watchdog_id;
} osal_taskctrlb_t;

/**
 * @brief Pointer to the Task Control Block of the currently executing task.
 *
 * The scheduler updates this pointer whenever a context switch occurs.
 */
extern osal_taskctrlb_t* current_tcb;

/**
 * @brief Creates a new scheduler task.
 *
 * Allocates a Task Control Block and stack from the scheduler's static
 * memory pool, initializes the task context, and marks the task as ready
 * for execution.
 *
 * @param func Pointer to the task entry function.
 * @param args Pointer passed to the task during its initial execution.
 *
 * @return OSAL_OK if the task was successfully created.
 * @return OSAL_ERROR_OVERFLOW if the maximum task count has been reached.
 */
osal_status_t create_task(osal_function_ptr func, void* arg);

/**
 * @brief Places the current task into the sleeping state.
 *
 * Calculates the task's wake-up tick, updates its execution state, and
 * requests a context switch by triggering the PendSV exception.
 *
 * @param time Current scheduler tick.
 * @param delay Number of ticks to delay execution.
 */
void update_wakeup(uint32_t time, uint32_t delay);

/**
 * @brief Executes one scheduler iteration.
 *
 * Updates sleeping tasks, selects the next runnable task using a
 * round-robin scheduling policy, and prepares the next context switch.
 *
 * This function is intended to be invoked from the scheduler's exception
 * handlers.
 */
void scheduler_run(void);

/**
 * @brief Starts the scheduler.
 *
 * Creates the idle task, marks it as the initial running task, and
 * transfers execution to the scheduler by issuing an SVC exception.
 *
 * This function should be called once after all application tasks have
 * been created.
 */
void os_start(void);

#endif // __SCHEDULER_H__