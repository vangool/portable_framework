/******************************************************************************
 * @file: my_os.c
 * @brief: STM32 custom scheduler implementation of the Operating System
 * Abstraction Layer (OSAL).
 *
 * This file binds the platform-independent OSAL interface to the custom
 * preemptive scheduler used by the STM32 backend. It assigns the OSAL function
 * pointers to scheduler-specific implementations, allowing application code to
 * execute without knowledge of the underlying operating system.
 *
 * Features currently implemented include task creation, scheduler delays, and
 * system tick retrieval. Queue operations and task notification primitives are
 * currently provided as placeholders for future development.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-14
 * @license: MIT License
 ******************************************************************************/
#include "../include/my_os.h"
#include "../include/scheduler.h"

osal_task_notify_give_from_isr  osal_task_notify_give_from_isr_func = my_os_task_notify_give_from_isr;
osal_get_current_task_handle    osal_get_current_task_handle_func   = my_os_get_current_task_handle;
osal_task_delay_ms              osal_task_delay_ms_func             = my_os_task_delay_ms;
osal_task_delay_until_ms        osal_task_delay_until_ms_func       = my_os_task_delay_until_ms;
osal_get_tick_count             osal_get_tick_count_func            = my_os_get_tick_count;
osal_action_upon_notification   osal_action_upon_notification_func  = my_os_action_upon_notification;
osal_create_task                osal_create_task_func               = my_os_create_task;
osal_pop_queue                  osal_pop_queue_func                 = my_os_pop_queue;
osal_push_queue                 osal_push_queue_func                = my_os_push_queue;
osal_create_queue               osal_create_queue_func              = my_os_create_queue;

void my_os_task_notify_give_from_isr(osal_taskhandle_t taskhandle){}
void my_os_get_current_task_handle(osal_taskhandle_t* taskhandler){}

void my_os_task_delay_ms(uint32_t delay_ms)
{
    update_wakeup(hal_timer_get_time_func(), delay_ms);
}

void my_os_task_delay_until_ms(osal_Tick* from_ms, uint32_t total_ms)
{
    update_wakeup(*from_ms, total_ms);
    *from_ms = *from_ms + total_ms;
}

osal_Tick my_os_get_tick_count()
{
    return (osal_Tick) hal_timer_get_time_func();
}

int16_t my_os_action_upon_notification(osal_bool_t clear_on_exit, uint32_t ms_to_wait)
{
    return -1;
}

osal_result_t my_os_create_task( 
    osal_function_ptr task_function,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void *pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t* task_handler_var)
{
    create_task(task_function, pvParameters);
}

osal_base_type my_os_pop_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    return -1;
}

osal_base_type my_os_push_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    return -1;
}

osal_queue_t my_os_create_queue(osal_ubase_type queue_length, osal_ubase_type item_size)
{
    osal_queue_t queue;
    return queue;
}