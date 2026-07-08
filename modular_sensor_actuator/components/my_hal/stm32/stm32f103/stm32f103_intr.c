/**
 * @file stm32f103_intr.c
 * @brief STM32F103 interrupt and exception handling implementation.
 *
 * This source file provides the STM32F103-specific implementation of interrupt
 * and exception handling for the hardware abstraction layer (HAL). It manages
 * interrupt dispatch, callback registration, and device-specific interrupt
 * processing using the ARM Cortex-M3 Nested Vectored Interrupt Controller
 * (NVIC).
 *
 * The module is responsible for translating hardware interrupt events into
 * registered HAL callback invocations while abstracting the underlying STM32F103
 * interrupt architecture from higher-level application code.
 *
 * Responsibilities include:
 * - Interrupt callback dispatch
 * - GPIO external interrupt (EXTI) handling
 * - Interrupt source lookup
 * - User callback invocation
 * - STM32F103-specific interrupt management
 * - Integration with the Cortex-M3 NVIC
 *
 * This module is intended to be used internally by the STM32 HAL. Applications
 * should register interrupt callbacks through the platform-independent HAL
 * interface rather than interacting with this implementation directly.
 *
 * @note This file contains STM32F103-specific interrupt handling logic and
 *       should only be compiled for STM32F103 targets.
 *
 * @ingroup STM32_HAL
 *
 * @author Michael Van Gool
 * @date 2026-07-07
 *
 * MIT License
 *
 * Copyright (c) 2026 Michael Van Gool
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "stm32f103_intr.h"

stm32_intr_fn pin_isr_handlers[STM32_EXTI_LINE_COUNT] = { NULL };
void* isr_args[STM32_EXTI_LINE_COUNT] = { NULL };

uint8_t stm32intr_get_isr_func_idx(uint8_t pin)
{
    pin = pin & TOTAL_PINS_MASK;
    switch (pin)
    {
        case 0:
            return 0;
        
        case 1:
            return 1;
        
        case 2:
            return 2;
        
        case 3:
            return 3;

        case 4:
            return 4;
        
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
            return 5;

        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
            return 6;

    
    default:
        break;
    }

}

void WWDG_IRQHandler()
{
    return;
}

void PVD_IRQHandler()
{
    return;
}

void TAMPER_IRQHandler()
{
    return;
}

void RTC_IRQHandler()
{
    return;
}

void FLASH_IRQHandler()
{
    return;
}

void RCC_IRQHandler()
{
    return;
}

void EXTI0_IRQHandler()
{
    if(!(EXTI_BLOCK_REG->PR & (1 << EXTI_LINE_0)))
    {
        return;
    }

    uint8_t idx = stm32intr_get_isr_func_idx(EXTI_LINE_0);
    pin_isr_handlers[idx](isr_args[idx]);
    EXTI_BLOCK_REG->PR = (1 << EXTI_LINE_0);
}

void EXTI1_IRQHandler()
{
    if(!(EXTI_BLOCK_REG->PR & (1 << EXTI_LINE_1)))
    {
        return;
    }

    uint8_t idx = stm32intr_get_isr_func_idx(EXTI_LINE_1);
    pin_isr_handlers[idx](isr_args[idx]);
    EXTI_BLOCK_REG->PR = (1 << EXTI_LINE_1);
}

void EXTI2_IRQHandler()
{
    if(!(EXTI_BLOCK_REG->PR & (1 << EXTI_LINE_2)))
    {
        return;
    }

    uint8_t idx = stm32intr_get_isr_func_idx(EXTI_LINE_2);
    pin_isr_handlers[idx](isr_args[idx]);
    EXTI_BLOCK_REG->PR = (1 << EXTI_LINE_2);
}

void EXTI3_IRQHandler()
{
    if(!(EXTI_BLOCK_REG->PR & (1 << EXTI_LINE_3)))
    {
        return;
    }

    uint8_t idx = stm32intr_get_isr_func_idx(EXTI_LINE_3);
    pin_isr_handlers[idx](isr_args[idx]);
    EXTI_BLOCK_REG->PR = (1 << EXTI_LINE_3);
}

void EXTI4_IRQHandler()
{
    if(!(EXTI_BLOCK_REG->PR & (1 << EXTI_LINE_4)))
    {
        return;
    }

    uint8_t idx = stm32intr_get_isr_func_idx(EXTI_LINE_4);
    pin_isr_handlers[idx](isr_args[idx]);
    EXTI_BLOCK_REG->PR = (1 << EXTI_LINE_4);
}

void EXTI9_5_IRQHandler()
{
    uint32_t pending_reg = EXTI_BLOCK_REG->PR;
    uint16_t exti_mask = 
        (1 << EXTI_LINE_5) | (1 << EXTI_LINE_6) | 
        (1 << EXTI_LINE_7) | (1 << EXTI_LINE_8) |
        (1 << EXTI_LINE_9);

    if(!(pending_reg & exti_mask))
    {
        return;
    }

    for(int i = EXTI_LINE_5; i <= EXTI_LINE_9; i++)
    {
        if(pending_reg & (1 << i))
        {
            EXTI_BLOCK_REG->PR = (1 << i);

            uint8_t idx = stm32intr_get_isr_func_idx(i);
            if(pin_isr_handlers[idx])
            {
                pin_isr_handlers[idx](isr_args[idx]);
            }
        }
    }
}

void EXTI15_10_IRQHandler()
{
    uint32_t pending_reg = EXTI_BLOCK_REG->PR;
    uint16_t exti_mask =
        (1 << EXTI_LINE_10) | (1 << EXTI_LINE_11) | 
        (1 << EXTI_LINE_12) | (1 << EXTI_LINE_13) |
        (1 << EXTI_LINE_14) | (1 << EXTI_LINE_15);

    if(!(pending_reg & exti_mask))
    {
        return;
    }

    for(int i = EXTI_LINE_10; i <= EXTI_LINE_15; i++)
    {
        if(pending_reg & (1 << i))
        {
            EXTI_BLOCK_REG->PR = (1 << i);

            uint8_t idx = stm32intr_get_isr_func_idx(i);
            if(pin_isr_handlers[idx])
            {
                pin_isr_handlers[idx](isr_args[idx]);
            }
        }
    }
}