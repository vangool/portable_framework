#include "../include/scheduler.h"

#define MAX_TASK_COUNT 12

static task_t tasks [MAX_TASK_COUNT];
static uint8_t task_count = 0;


hal_status_t create_task(void* func, uint32_t delay)
{
    if(task_count >= MAX_TASK_COUNT)
    {
        return HAL_ERROR_GENERAL;
    }
    
    task_t task = { .func = func, .last_run = 0, .period_ms = delay};
    tasks[task_count] = task;
    task_count++;

    return HAL_STATUS_OK;
}

void scheduler_run()
{
    uint32_t current_tick = hal_timer_get_time_func();
    for(int i = 0; i < task_count; i++)
    {
        if((current_tick - tasks[i].last_run) >= tasks[i].period_ms)
        {
            tasks[i].last_run = current_tick;
            tasks[i].func();
        }
    }
}