/******************************************************************************
 * @file: freertos.c
 * @brief: Concrete OSAL implementation binding to the FreeRTOS Kernel.
 *
 * This source file implements the underlying operating system abstractions 
 * using the native FreeRTOS SDK framework. It provides thread-safe queue 
 * handling, deterministic task scheduling wrappers, and timing primitives.
 *
 * @note This file is compiled exclusively when targeting real hardware 
 * (e.g., ESP32, STM32) and is omitted during host-side PC unit testing.
 *
 * @author: [NAME]
 * @date: 2026-06-26
 * @license: MIT License
 * Copyright (c) 2026 [NAME]. All rights reserved.
 ******************************************************************************/

#include "freertos.h"

osal_task_notify_give_from_isr  osal_task_notify_give_from_isr_func = freertos_tasknotify_give_from_ISR;
osal_get_current_task_handle    osal_get_current_task_handle_func   = freertos_get_current_task_handle;
osal_task_delay_ms              osal_task_delay_ms_func             = freertos_task_delay_ms;
osal_task_delay_until_ms        osal_task_delay_until_ms_func       = freertos_task_delay_until_ms;
osal_get_tick_count             osal_get_tick_count_func            = freertos_get_tick_count;
osal_action_upon_notification   osal_action_upon_notification_func  = freertos_action_upon_notification;
osal_create_task                osal_create_task_func               = freertos_create_task;
osal_create_queue               osal_create_queue_func              = freertos_create_queue;
osal_pop_queue                  osal_pop_queue_func                 = freertos_pop_queue;
osal_push_queue                 osal_push_queue_func                = freertos_push_queue;


static osal_result_t translate_result(BaseType_t result)
{
    return result == pdPASS ? OSAL_PASS : OSAL_FAIL;
}

static BaseType_t translate_bool(osal_bool_t boolean)
{
    return boolean == OSAL_TRUE ? pdTRUE : pdFALSE;
}

void freertos_tasknotify_give_from_ISR(osal_taskhandle_t task)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    TaskHandle_t freertos_task = (TaskHandle_t)task.taskhandle;
    vTaskNotifyGiveFromISR(freertos_task, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}


void freertos_get_current_task_handle(osal_taskhandle_t* task_handle)
{
    task_handle->taskhandle = (void*)xTaskGetCurrentTaskHandle();
}

void freertos_task_delay_ms(uint32_t delay_ms)
{
    vTaskDelay(pdMS_TO_TICKS(delay_ms));
}

void freertos_task_delay_until_ms(osal_Tick* from_ms, uint32_t total_ms)
{
    TickType_t start_ms = (TickType_t)(*from_ms);
    vTaskDelayUntil(&start_ms, pdMS_TO_TICKS(total_ms));
    *from_ms = start_ms;
}

osal_Tick freertos_get_tick_count()
{
    return (osal_Tick)xTaskGetTickCount();
}

int16_t freertos_action_upon_notification(osal_bool_t clear_on_exit, uint32_t ms_to_wait)
{
    TickType_t ticks_to_wait = pdMS_TO_TICKS(ms_to_wait);
    return ulTaskNotifyTake(translate_bool(clear_on_exit), ticks_to_wait);
}

osal_result_t freertos_create_task(
    osal_function_ptr function_ptr,
    const char* const task_name,
    const osal_stack_depth usStackDepth,
    void* pvParameters,
    osal_ubase_type priority,
    osal_taskhandle_t *task_handler_var)
{
    void* task_function = (void*)function_ptr;
    const configSTACK_DEPTH_TYPE uStackDepth = (configSTACK_DEPTH_TYPE) usStackDepth; 
    TaskHandle_t* task_handler = (TaskHandle_t*) task_handler_var;
    return translate_result(
        xTaskCreate(task_function, task_name, uStackDepth, pvParameters, priority, task_handler)
    );
}

osal_base_type freertos_pop_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    osal_base_type ret = 0;
    return (osal_base_type)xQueueReceive((QueueHandle_t) queue.queue, pvBuffer, (TickType_t) wait_time);
}

osal_base_type freertos_push_queue(osal_queue_t queue, void* pvBuffer, osal_Tick wait_time)
{
    return (osal_base_type)xQueueSend((QueueHandle_t) queue.queue, pvBuffer, (TickType_t) wait_time);
}

osal_queue_t freertos_create_queue(osal_ubase_type queue_length, osal_ubase_type item_size)
{
    osal_queue_t handler;
    handler.queue = xQueueCreate((UBaseType_t) queue_length, (UBaseType_t) item_size);
    return handler;
}

