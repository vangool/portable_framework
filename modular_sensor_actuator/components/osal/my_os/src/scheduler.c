#include "../include/scheduler.h"

#define MAX_TASK_COUNT 12

#define STACK_SIZE         128         // 128 words = 512 bytes
#define XPSR_THUMB_BIT     (1 << 24)   // Cortex-M3 requires Thumb bit set
#define STACK_CANARY       0xDEADBEEF  // Magic number for stack painting

static osal_task_t tasks [MAX_TASK_COUNT];
static uint8_t task_count = 0;

osal_status_t create_task(void* func, void* args)
// osal_status_t create_task(void (*func)(void*), void* arg)
{
    if(task_count >= MAX_TASK_COUNT)
    {
        return OSAL_ERROR_OVERFLOW;
    }
    
    osal_task_t task = { .func = func, .last_run = 0, .period_ms = 500, .state = THREAD_READY };
    tasks[task_count] = task;
    task_count++;

    return OSAL_OK;
}

// 2. The Compatible Thread Initializer
void thread_create(osal_task_t* tcb, void (*task_function)(void), uint32_t *stack_start, uint32_t stack_size) {
    
    // Step A: Paint the stack for future safety/budget checks
    for (uint32_t i = 0; i < stack_size; i++) {
        stack_start[i] = STACK_CANARY;
    }

    // Step B: Point to the top of the stack 
    // (Cortex-M stacks are full-descending, so we start at the end of the array)
    uint32_t *stk = stack_start + stack_size;

    // Step C: Build the Hardware Stack Frame (CPU auto-restores these)
    stk--; *stk = XPSR_THUMB_BIT;       // xPSR: Mandatory Thumb state bit
    stk--; *stk = (uint32_t)task_function; // PC: Where the thread starts executing
    stk--; *stk = 0xFFFFFFFD;           // LR: Return code (Return to Thread Mode, use PSP)
    stk--; *stk = 0x12121212;           // R12: Dummy debugging pattern
    stk--; *stk = 0x03030303;           // R3: Dummy
    stk--; *stk = 0x02020202;           // R2: Dummy
    stk--; *stk = 0x01010101;           // R1: Dummy
    stk--; *stk = 0x00000000;           // R0: Dummy (Can be used to pass a thread argument)

    // Step D: Build the Software Stack Frame (Our PendSV assembly routine will pop these)
    stk--; *stk = 0x11111111;           // R11: Dummy
    stk--; *stk = 0x10101010;           // R10: Dummy
    stk--; *stk = 0x09090909;           // R9: Dummy
    stk--; *stk = 0x08080808;           // R8: Dummy
    stk--; *stk = 0x07070707;           // R7: Dummy
    stk--; *stk = 0x06060606;           // R6: Dummy
    stk--; *stk = 0x05050505;           // R5: Dummy
    stk--; *stk = 0x04040404;           // R4: Dummy

    // Step E: Save this exact layout address into the TCB
    tcb->stack_ptr = stk;
    tcb->state = THREAD_READY;
}

void scheduler_run()
{
    uint32_t current_tick = hal_timer_get_time_func();
    for(int i = 0; i < task_count; i++)
    {
        if((current_tick - tasks[i].last_run) >= tasks[i].period_ms)
        {
            tasks[i].state = THREAD_READY;
        }

        if(tasks[i].state == THREAD_READY)
        {
            tasks[i].state = THREAD_RUNNING;
            tasks[i].last_run = current_tick;
            tasks[i].func();
            tasks[i].state = THREAD_SLEEP;
        }
    }
}