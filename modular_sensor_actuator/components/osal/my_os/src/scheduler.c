/******************************************************************************
 * @file: scheduler.c
 * @brief: Internal implementation of the custom STM32 preemptive scheduler.
 *
 * This module implements a lightweight preemptive round-robin scheduler for the
 * ARM Cortex-M3. It manages task creation, stack initialization, task state
 * transitions, sleep scheduling, and task selection while relying on the
 * SysTick, SVC, and PendSV system exceptions to perform context switching.
 *
 * Tasks are statically allocated at compile time to eliminate dynamic memory
 * allocation and provide deterministic memory usage suitable for embedded
 * systems.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-14
 * @license: MIT License
 ******************************************************************************/

#include "scheduler.h"

#include "stm32.h"
#include <stdalign.h>

/**
 * Maximum number of application tasks managed by the scheduler.
 * Task index 0 is reserved for the idle task.
 */
#define MAX_TASK_COUNT 12

/**
 * Stack size allocated to each task in 32-bit words.
 * (128 words = 512 bytes)
 */
#define STACK_SIZE 128

/* Pointer to the currently executing task. Updated during every context switch. */
osal_taskctrlb_t* current_tcb = NULL;
static uint8_t scheduler_lock_count = 0;

/* Static Task Control Block (TCB) storage. */
alignas(8) static osal_taskctrlb_t tasks[MAX_TASK_COUNT];

/* Dedicated idle task control block. */
alignas(8) static osal_taskctrlb_t idle;

/* Per-task stack allocation. */
alignas(8) static uint32_t task_stack[MAX_TASK_COUNT][STACK_SIZE];

/* Number of application tasks currently created. */
static uint8_t task_count = 0;


static uint32_t* stack_init(void* func, void* args, uint32_t* top_of_stack);
static osal_status_t create_task_helper(osal_function_ptr func, void* args, uint8_t idx);
static void idle_task(void);


/**
 * @brief Default task executed when no application task is ready.
 *
 * The idle task continuously executes the ARM Cortex-M WFI (Wait For Interrupt)
 * instruction, allowing the processor to remain idle until the next interrupt
 * occurs.
 */
static void idle_task(void)
{
    while (1)
    {
        __asm volatile("wfi");
    }
}

/**
 * @brief Constructs an initial exception stack frame for a new task.
 *
 * Initializes the stack exactly as if the task had previously been interrupted
 * by the processor. This allows the PendSV context switch routine to restore
 * the registers and begin executing the task naturally through exception return.
 *
 * The initial stack frame follows the ARM Cortex-M exception entry convention,
 * consisting of the hardware-stacked registers (R0-R3, R12, LR, PC, xPSR)
 * followed by the software-saved registers (R4-R11).
 *
 * @param func Task entry function.
 * @param args Optional task argument.
 * @param top_of_stack Initial stack pointer.
 *
 * @return Updated stack pointer ready for the first context restore.
 */
static uint32_t* stack_init(void* func, void* args, uint32_t* top_of_stack)
{    
    *(--top_of_stack) = 0x01000000;             // xPSR: Thumb bit (bit 24) MUST be 1
    *(--top_of_stack) = (uint32_t)func;         // PC (Program Counter)
    *(--top_of_stack) = 0xFFFFFFFD;             // LR (Link Register)
    *(--top_of_stack) = 0x12121212;             // R12
    *(--top_of_stack) = 0x03030303;             // R3
    *(--top_of_stack) = 0x02020202;             // R2
    *(--top_of_stack) = 0x01010101;             // R1
    *(--top_of_stack) = (uint32_t)args;         // R0

    *(--top_of_stack) = 0x11111111;             // R11
    *(--top_of_stack) = 0x10101010;             // R10
    *(--top_of_stack) = 0x09090909;             // R9
    *(--top_of_stack) = 0x08080808;             // R8
    *(--top_of_stack) = 0x07070707;             // R7
    *(--top_of_stack) = 0x06060606;             // R6
    *(--top_of_stack) = 0x05050505;             // R5
    *(--top_of_stack) = 0x04040404;             // R4

    return top_of_stack; 
}

/**
 * @brief Initializes a Task Control Block (TCB) and task stack.
 *
 * Allocates the task's execution context, prepares its initial stack frame,
 * and marks the task as ready for scheduling.
 */
static osal_status_t create_task_helper(osal_function_ptr func, void* args, uint8_t idx)
{
    uint32_t* stack_top = &task_stack[idx][STACK_SIZE];
    stack_top = (uint32_t *)((uintptr_t)stack_top & ~0x7);

    stack_top = stack_init(func, args, stack_top);
    
    tasks[idx].stack_ptr = stack_top;
    tasks[idx].func = func;
    tasks[idx].next_wakeup = 0;
    tasks[idx].state = THREAD_READY;

    current_tcb = &tasks[idx];

    task_count++;
    return OSAL_OK;
}

void os_start(void) {
    enable_threads();
    create_task_helper((osal_function_ptr)idle_task, NULL, 0);
    current_tcb->state = THREAD_RUNNING; 
    __asm volatile ("svc 0");
}

osal_status_t create_task(osal_function_ptr func, void* args)
{
    if(task_count >= MAX_TASK_COUNT)
    {
        return OSAL_ERROR_OVERFLOW;
    }

    uint8_t ret = create_task_helper(func, args, task_count + 1);

    return ret;
}

void update_wakeup(uint32_t time, uint32_t delay)
{
    current_tcb->next_wakeup = time + delay;
    current_tcb->state = THREAD_SLEEP;
    SYS_CTRL_BLOCK->ICSR.bits.PEND_SV_SET = 1;
}

void scheduler_run()
{
    if(task_count <= 1 || scheduler_lock_count > 0)
    {
        return;
    }
    
    uint32_t current_tick = hal_timer_get_time_func();
    for(int i = 0; i < task_count; i++)
    {
        if((int32_t)(current_tick - tasks[i].next_wakeup) >= 0 && tasks[i].state == THREAD_SLEEP)
        {
            tasks[i].state = THREAD_READY;
        }
    }

    if(current_tcb->state == THREAD_RUNNING)
    {
        current_tcb->state = THREAD_READY;
    }

    int start = current_tcb - tasks;
    bool found_ready = false;
    for (int i = 1; i < task_count; i++)
    {
        int next = (start + i) % task_count;

        if (tasks[next].state == THREAD_READY)
        {
            found_ready = true;
            current_tcb = &tasks[next];
            current_tcb->state = THREAD_RUNNING;
            break;
        }

    }

    if(!found_ready)
    {
        current_tcb = &tasks[0];
        current_tcb->state = THREAD_RUNNING;
    }
}

void scheduler_enter_critical(void)
{
    scheduler_lock_count++;
}

void scheduler_exit_critical(void)
{
    scheduler_lock_count--;
}