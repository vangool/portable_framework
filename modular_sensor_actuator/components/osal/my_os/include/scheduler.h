#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>

#include "../../include/osal.h"
#include "../../my_hal/stm32/stm32f103/stm32f103.h"

typedef void (*task_function_t)(void);

typedef enum
{
    THREAD_READY,
    THREAD_RUNNING,
    THREAD_BLOCKED,
    THREAD_SLEEP
} osal_thread_state_t;

typedef struct 
{
    uint32_t* stack_ptr;
    task_function_t func;
    uint32_t period_ms;
    uint32_t last_run;
    osal_thread_state_t state; 
} osal_task_t;

osal_status_t create_task(void* func, void* arg);
void thread_create(osal_task_t*, void*, uint32_t*, uint32_t);
void scheduler_run(void);

#endif // __SCHEDULER_H__