/******************************************************************************
 * File: freertos.h
 * Description: Wrapper for this application and the associated FreeRTOS
 *          functions. 
 * Author: Michael Van Gool
 * Date: 2026-05-04
 * License: MIT
******************************************************************************/

#include "freertos.h"

osal_task_notify_give_from_isr  osal_task_notify_give_from_isr_func = freertos_tasknotify_give_from_ISR;
osal_get_current_task_handle    osal_get_current_task_handle_func   = freertos_get_current_task_handle;

static osal_result_t translate_result()
{
    return OSAL_OK;
}

static osal_bool_t translate_bool(BaseType_t boolean)
{
    return boolean == pdTRUE ? OSAL_TRUE : OSAL_FALSE;
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