#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>

#include "../../my_hal/include/hal.h"
#include "../../my_hal/stm32/stm32f103/stm32f103.h"

typedef void (*task_function_t)(void);

typedef struct 
{
    task_function_t func;
    uint32_t period_ms;
    uint32_t last_run;
} task_t;

hal_status_t create_task(void* func, uint32_t delay);
void scheduler_run(void);

#endif // __SCHEDULER_H__