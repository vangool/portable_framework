/**
 * @file stm32f103_intr.h
 * @brief STM32F103 interrupt and exception definitions.
 *
 * This header provides STM32F103-specific definitions and interfaces for
 * configuring and managing interrupts. It contains the interrupt vector
 * definitions, IRQ identifiers, NVIC helper declarations, and device-specific
 * interrupt mappings required by the STM32F103 hardware abstraction layer.
 *
 * The STM32F103 implements the ARM Cortex-M3 Nested Vectored Interrupt
 * Controller (NVIC), which provides support for peripheral interrupts,
 * programmable interrupt priorities, and exception handling. This header
 * defines the STM32F103-specific interrupt sources that interface with the
 * Cortex-M3 core.
 *
 * Responsibilities include:
 * - Interrupt request (IRQ) definitions
 * - Interrupt vector identifiers
 * - NVIC configuration interfaces
 * - External interrupt (EXTI) definitions
 * - STM32F103 interrupt source mappings
 * - Interrupt priority definitions
 *
 * This file contains only STM32F103 interrupt definitions. Generic Cortex-M3
 * exception handling and processor definitions are provided by
 * @ref cortex_m3.h.
 *
 * @note This header is intended for use by the STM32 HAL and low-level
 *       platform code. Application code should use the platform-independent
 *       HAL interrupt interface whenever possible.
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

#ifndef __STM32F103_INTR_H__
#define __STM32F103_INTR_H__

#include "stm32f103.h"

#define STM32_EXTI_LINE_COUNT 7

/**
 * @typedef stm32_intr_fn
 * @brief Function pointer type for STM32 interrupt service routines (ISRs).
 *
 * Defines the prototype for interrupt callback functions registered by the
 * STM32 HAL. Each callback receives a user-defined argument that was provided
 * during interrupt registration.
 *
 * @param arg
 * Pointer to user-defined data passed to the interrupt handler.
 */
typedef void (*stm32_intr_fn)(void*);

/**
 * @brief Registered GPIO interrupt callback handlers.
 *
 * Array containing the interrupt service routine (ISR) callbacks associated
 * with each supported interrupt source. Each entry corresponds to an interrupt
 * index returned by stm32intr_get_isr_func_idx().
 */
extern stm32_intr_fn pin_isr_handlers[STM32_EXTI_LINE_COUNT];

/**
 * @brief User arguments associated with registered interrupt handlers.
 *
 * Array of user-defined context pointers passed as the argument to the
 * corresponding ISR callback when an interrupt occurs.
 */
extern void* isr_args[STM32_EXTI_LINE_COUNT];

/**
 * @brief Retrieves the interrupt handler index for a GPIO pin.
 *
 * Maps a GPIO pin identifier to the corresponding index within the interrupt
 * handler and argument arrays. The returned index is used to access the
 * callback registered for the specified interrupt source.
 *
 * @param[in] pin GPIO packed pin identifier.
 *
 * @return Index into the interrupt handler tables corresponding to the specified pin.
 */
uint8_t stm32intr_get_isr_func_idx(uint8_t pin);


#endif //__STM32F103_INTR_H__