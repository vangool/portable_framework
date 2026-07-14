/**
 * @file cortex_m3.h
 * @brief ARM Cortex-M3 core definitions and hardware abstraction interface.
 *
 * This header provides ARM Cortex-M3 core definitions and interfaces used by
 * the project's hardware abstraction layer (HAL). It contains architecture-
 * specific definitions, register mappings, and helper functions required for
 * interacting with Cortex-M3 processor features.
 *
 * The Cortex-M3 core implements the Armv7-M architecture and provides the CPU
 * functionality shared across Cortex-M3-based microcontrollers, including
 * interrupt handling, exception management, system control, and core register
 * access. Device-specific peripherals are defined separately by the target MCU
 * implementation.
 *
 * Responsibilities include:
 * - Cortex-M3 core register definitions
 * - System Control Block (SCB) definitions
 * - Nested Vectored Interrupt Controller (NVIC) definitions
 * - SysTick timer definitions
 * - Core exception and interrupt support
 *
 * This header contains only Cortex-M3 architecture-specific definitions. Device
 * peripherals (GPIO, UART, SPI, timers, etc.) are provided by the corresponding
 * microcontroller-specific headers.
 *
 * @note This file targets ARM Cortex-M3 processors implementing the Armv7-M
 *       architecture and is intended for use by the STM32 HAL and other
 *       Cortex-M3-based platforms.
 *
 * @ingroup STM32_HAL
 *
 * @author Michael Van Gool
 * @date 2026-07-13
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

#ifndef __CORTEX_M3_H__
#define __CORTEX_M3_H__

#include <stdint.h>

#define REG_PTR volatile uint32_t*
#define REG_TYPE volatile uint32_t

/******************************************************************************
 *
 * Cortex M3 Declarations
 *  
******************************************************************************/


typedef struct 
{
    REG_TYPE I_SE_R0;       // [NVIC_ISERx] Interrupt Set Enable Register
    REG_TYPE I_SE_R1;       // [NVIC_ISERx] Interrupt Set Enable Register
    REG_TYPE I_SE_R2;       // [NVIC_ISERx] Interrupt Set Enable Register
    REG_TYPE I_CE_R0;       // [NVIC_ICERx] Interrupt Clear Enable Register
    REG_TYPE I_CE_R1;       // [NVIC_ICERx] Interrupt Clear Enable Register
    REG_TYPE I_CE_R2;       // [NVIC_ICERx] Interrupt Clear Enable Register
    REG_TYPE I_SP_R0;       // [NVIC_ISPRx] Interrupt Set Pending Register
    REG_TYPE I_SP_R1;       // [NVIC_ISPRx] Interrupt Set Pending Register
    REG_TYPE I_SP_R2;       // [NVIC_ISPRx] Interrupt Set Pending Register
    REG_TYPE I_CP_R0;       // [NVIC_ICPRx] Interrupt Clear Pending Register
    REG_TYPE I_CP_R1;       // [NVIC_ICPRx] Interrupt Clear Pending Register
    REG_TYPE I_CP_R2;       // [NVIC_ICPRx] Interrupt Clear Pending Register
    REG_TYPE I_AB_R0;       // [NVIC_IABRx] Interrupt Active Bit Regitser
    REG_TYPE I_AB_R1;       // [NVIC_IABRx] Interrupt Active Bit Regitser
    REG_TYPE I_AB_R2;       // [NVIC_IABRx] Interrupt Active Bit Regitser
    REG_TYPE I_P_R0;        // [NVIC_IPRx]  Interrupt Priority Register
    REG_TYPE I_P_R1;        // [NVIC_IPRx]  Interrupt Priority Register
    REG_TYPE I_P_R2;        // [NVIC_IPRx]  Interrupt Priority Register
    REG_TYPE ST_I_R0;       // [NVIC_STIRx] Software Trigger Interrupt Register
    REG_TYPE ST_I_R1;       // [NVIC_STIRx] Software Trigger Interrupt Register
    REG_TYPE ST_I_R2;       // [NVIC_STIRx] Software Trigger Interrupt Register

} NVIC_BLOCK;

#define NVIC_BLOCK_REG      ((volatile NVIC_BLOCK*) 0XE000E100UL)

/******************************************************************************
 *
 *  EXTI Line Enum
 *  
******************************************************************************/

typedef enum
{
    EXTI0 = 6,
    EXTI1 = 7,
    EXTI2 = 8,
    EXTI3 = 9,
    EXTI4 = 10,
    EXTI9_5 = 23,
    EXT15_10 = 40
} NVIC_ISER_ENUM;

/******************************************************************************
 * 
 * System Tick Handler
 * 
******************************************************************************/

typedef struct 
{
    REG_TYPE STK_CTRL; // SysTick Control & Status Register
    REG_TYPE STK_LOAD; // SysTick Reload Value Register
    REG_TYPE STK_VAL;  // SysTick Current Value Register
} SYSTK_BLOCK;

#define STK_REG             ((volatile SYSTK_BLOCK*)  0xE000E010UL)

/******************************************************************************
 * 
 * System Control
 * 
******************************************************************************/
typedef union
{
    struct 
    {
        REG_TYPE VECT_ACTIVE    : 9; // Bit 0-8: [VECTACTIVE]
        REG_TYPE _reserved1     : 2; // Bit 9-10
        REG_TYPE RETOBASE       : 1; // Bit 11
        REG_TYPE VECT_PENDING   :10; // Big 12-21: [VECT_PENDING]
        REG_TYPE ISR_PENDING    : 1; // Bit 22: [ISRPENDING]
        REG_TYPE _reserved2     : 2; // Bit 23-24
        REG_TYPE PEND_ST_CLR    : 1; // Bit 25: [PENDSTCLR] System Tick
        REG_TYPE PEND_ST_SET    : 1; // Bit 26: [PENDSTSET]
        REG_TYPE PEND_SV_CLR    : 1; // Bit 27: [PENDSVCLR] Pendable Service Call exception.
        REG_TYPE PEND_SV_SET    : 1; // Bit 28: [PEND_SV_SET]
        REG_TYPE _reserved3     : 2; // Bit 29-30
        REG_TYPE NMI_PEND_SET   : 1; // Bit 31 [NMIPENDSET]
    } bits;

    REG_TYPE all; 
} ICSR_r;



typedef struct 
{
    REG_TYPE CPUID;         // [SCB_CPUID]
    volatile ICSR_r ICSR;   // [SCB_ICSR]
    REG_TYPE VTOR;          // [SCB_VTOR]
    REG_TYPE AIRCR;         // [SCB_AIRCR]
    REG_TYPE SCR;           // [SCB_SCR]
} SYS_CTRL;

#define SYS_CTRL_BLOCK      ((volatile SYS_CTRL*) 0xE000ED00)

#endif //__CORTEX_M3_H__