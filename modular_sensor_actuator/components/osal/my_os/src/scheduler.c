#include "../include/scheduler.h"

static void led1_task();
static void led2_task();

static task_t tasks [] =
{
    {
        .func = led1_task,
        .period_ms = 500,
        .last_run = 0
    },

    {
        .func = led2_task,
        .period_ms = 1000,
        .last_run = 0
    }
};

#define TASK_COUNT 2

void scheduler_run()
{
    C_GPIO_BLOCK_REG->BSRR.bits.BR13 = 1;
    while(1){}
    uint32_t current_tick = hal_timer_get_time_func();
    for(int i = 0; i < TASK_COUNT; i++)
    {
        if((current_tick - tasks[i].last_run) >= tasks[i].period_ms)
        {
            tasks[i].last_run = current_tick;
            tasks[i].func();
        }
    }
}