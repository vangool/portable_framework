/******************************************************************************
 * File: osal.c
 * Description: Implementation of the Operating System Abstraction Layer (OSAL) 
 *          default functions. These functions should not be called during 
 *          production as their sole purpose is to be used for unit testing.
 * Author: Michael Van Gool
 * Date: 2026-05-04
 * License: MIT
******************************************************************************/
#include "../include/osal.h"

osal_task_notify_give_from_isr  osal_task_notify_give_from_isr_func = osal_task_notify_give_from_isr_default;
osal_get_current_task_handle    osal_get_current_task_handle_func   = osal_get_current_task_handle_default;

void osal_task_notify_give_from_isr_default(osal_taskhandle_t task)
{
    return;
}

void osal_get_current_task_handle_default(osal_taskhandle_t* taskhandler)
{
    return;
}
