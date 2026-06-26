/******************************************************************************
 * @file: osal.c
 * @brief: Concrete binding and initialization layer for the OSAL API.
 *
 * This file handles the registration and mapping of the global OSAL function 
 * pointers to their target-specific RTOS implementations. It provides an 
 * abstract system management layer, allowing the application to initialize 
 * queues, spin up tasks, and control timing primitives through a single, 
 * unified interface while keeping kernel header files out of application scope.
 *
 * @author: Michael Van Gool
 * @date: 2026-06-25
 * @license: MIT License (See header file for full license conditions)
 * Copyright (c) 2026 Michael Van Gool. All rights reserved.
 ******************************************************************************/
#include "../include/osal.h"

osal_task_notify_give_from_isr  osal_task_notify_give_from_isr_func = osal_task_notify_give_from_isr_default;
osal_get_current_task_handle    osal_get_current_task_handle_func   = osal_get_current_task_handle_default;
osal_task_delay_ms              osal_task_delay_ms_func             = osal_task_delay_ms_default;
osal_task_delay_until_ms        osal_task_delay_until_ms_func       = osal_task_delay_until_ms_default;
osal_get_tick_count             osal_get_tick_count_func            = osal_get_tick_count_default;
osal_action_upon_notification   osal_action_upon_notification_func  = osal_action_upon_notification_default;
osal_create_task                osal_create_task_func               = osal_create_task_default;
osal_pop_queue                  osal_pop_queue_func                 = osal_pop_queue_default;
osal_push_queue                 osal_push_queue_func                = osal_push_queue_default;
osal_create_queue               osal_create_queue_func              = osal_create_queue_default;


static osal_Tick mock_system_ticks = 0;

void osal_task_notify_give_from_isr_default(osal_taskhandle_t task)
{
    return;
}

void osal_get_current_task_handle_default(osal_taskhandle_t* taskhandler)
{
    return;
}

void osal_task_delay_ms_default(uint32_t delay_ms)
{
    mock_system_ticks += delay_ms;
}

void osal_task_delay_until_ms_default(osal_Tick* from_ms, uint32_t total_ms)
{
    if (from_ms == NULL)
    {
        return;
    }

    *from_ms += total_ms;
    mock_system_ticks = *from_ms;
}

osal_Tick osal_get_tick_count_default(void)
{
    return mock_system_ticks;
}

int16_t osal_action_upon_notification_default(osal_bool_t clear_on_exit, uint32_t ms_to_wait)
{
    (void)clear_on_exit;
    
    mock_system_ticks += ms_to_wait;

    /* For deterministic host-side testing, return a mock notification value 
     * (e.g., bit 0 set) to signal that a mock event successfully fired. */
    return 0x0001; 
}

osal_result_t osal_create_task_default(   
    osal_function_ptr task_function,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void *pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t* task_handler_var)
{
    (void)task_name; (void)usStackDepth; (void)pvParameters; (void)priority;

    if (task_function == NULL)
    {
        return OSAL_FAIL;
    }
    
    if (task_handler_var != NULL)
    {
        task_handler_var->taskhandle = (void*)0xDEADBEEF;
    }
    
    return OSAL_PASS;
}

osal_base_type osal_pop_queue_default(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    if (osal_queue_is_valid(queue) == NULL || pvBuffer == NULL)
    {
        return 0;
    }
    
    (void)wait_time;

    return 1;
}

osal_base_type osal_push_queue_default(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    if (osal_queue_is_valid(queue) || pvBuffer == NULL)
    {
        return 0;
    }
    
    (void)wait_time;
    
    return 1;
}

osal_queue_t osal_create_queue_default(osal_ubase_type queue_length, osal_ubase_type item_size)
{
    osal_queue_t mock_queue = { .queue = NULL };
    
    if (queue_length == 0 || item_size == 0)
    {
        return mock_queue; 
    }
    
    mock_queue.queue = (void*)0xBAADF00D;
    
    return mock_queue;
}