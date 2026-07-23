/**
 * @file stm32f103.h
 * @brief STM32F103 device-specific hardware abstraction definitions.
 *
 * This header provides STM32F103-specific definitions required by the STM32
 * hardware abstraction layer. It contains device-family mappings, peripheral
 * definitions, GPIO configurations, and hardware-specific information required
 * to support the STM32F103 series of microcontrollers.
 *
 * The purpose of this file is to isolate STM32F103 device-specific details from
 * the common STM32 HAL implementation. This allows the higher-level HAL code to
 * support multiple STM32 families while keeping device-specific configuration
 * contained within the appropriate target module.
 *
 * Responsibilities include:
 * - STM32F103 peripheral mappings
 * - GPIO port and pin definitions
 * - UART/USART pin mappings
 * - Device-specific hardware constants
 * - STM32F103 capability definitions
 *
 * This header should only contain STM32F103-specific information. Generic HAL
 * interfaces and platform-independent functionality should be placed in the
 * common STM32 HAL headers instead.
 *
 * @note This file is intended to be included by STM32F103 HAL implementations
 *       and should not normally be included directly by application code.
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

#ifndef __STM32F103C8T6_H__
#define __STM32F103C8T6_H__

#include <stdint.h>

#include "hal.h"
#include "cortex_m3.h"

#define NUM_OF_PORTS 3
#define TOTAL_PINS_MASK 15

/******************************************************************************
 * 
 * Reset and clock Controls
 * 
******************************************************************************/
#define APB2_CLK_SPEED      72000000U   // 72 000 000 Hz
#define APB1_CLK_SPEED      36000000U   // 36 000 000 Hz
#define LSI_CLK_SPEED       40000U      // 40 000 Hz      

typedef union
{
    struct
    {
        REG_TYPE HSION              : 1;  // Bit 0: Internal clock bit
        REG_TYPE HSIRDY             : 1;  // Bit 1: Internal clock ready bit
        REG_TYPE HSITRIM            : 5;  // Bit 2-6: ??
        REG_TYPE HSICAL             : 8;  // Bit 7-14: ??
        REG_TYPE _reserved1         : 1;  // Bit 15
        
        REG_TYPE HSEON              : 1;  // Bit 16: External clock bit
        REG_TYPE HSERDY             : 1;  // Bit 17: External clock ready bit
        REG_TYPE HSEBYP             : 1;  // Bit 18: ??
        REG_TYPE CSSON              : 1;  // Bit 19: ??
        REG_TYPE _reserved2         : 4;  // Bit 20-23
        
        REG_TYPE PLLON              : 1;  // Bit 24: Phase Locked Loop Multiplier Bit
        REG_TYPE PLLRDY             : 1;  // Bit 25: PLL Ready Bit
        REG_TYPE _reserved3         : 6;  // Bit 26-31
    } bits;

    REG_TYPE all;
} RCC_CR_r;
#define RCC_CR_REG              ((volatile RCC_CR_r*) 0x40021000UL)  // Reset and clock control register

typedef union {
    struct {
        REG_TYPE SW                 : 2;  // Bit 0-1: System clock switch
        REG_TYPE SWS                : 2;  // Bit 2-3: System clock switch status (Read Only)
        REG_TYPE HPRE               : 4;  // Bit 4-7: AHB prescaler
        REG_TYPE PPRE1              : 3;  // Bit 8-10: APB low-speed prescaler (APB1)
        REG_TYPE PPRE2              : 3;  // Bit 11-13: APB high-speed prescaler (APB2)
        REG_TYPE ADCPRE             : 2;  // Bit 14-15: ADC prescaler
        REG_TYPE PLLSRC             : 1;  // Bit 16: PLL entry clock source
        REG_TYPE PLLXTPRE           : 1;  // Bit 17: HSE divider for PLL entry
        REG_TYPE PLLMUL             : 4;  // Bit 18-21: PLL multiplication factor
        REG_TYPE OTGFSPRE           : 1;  // Bit 22: USB OTG FS prescaler (called USBPRE on some F1s)
        REG_TYPE _reserved1         : 1;  // Bit 23
        REG_TYPE MCO                : 3;  // Bit 24-26: Microcontroller clock output
        REG_TYPE _reserved2         : 5;  // Bit 27-31
    } bits;

    REG_TYPE all; 
} RCC_CFGR_r;
#define RCC_CFGR_REG            ((volatile RCC_CFGR_r *) 0x40021004UL) // Clock Configuration Register

typedef union
{
    struct
    {
        REG_TYPE AFIO_EN            : 1; // Bit 0: [AFIOEN] Alternate Function I/O Clock Enable
        REG_TYPE _reserved1         : 1; // Bit 1
        REG_TYPE IOPA_EN            : 1; // Bit 2: [IOPAEN] GPIO Port A Clock Enable
        REG_TYPE IOPB_EN            : 1; // Bit 3: [IOPBEN] GPIO Port B Clock Enable
        REG_TYPE IOPC_EN            : 1; // Bit 4: [IOPCEN] GPIO Port C Clock Enable
        REG_TYPE _reserved2         : 4; // Bit 5-8
        REG_TYPE ADC1_EN            : 1; // Bit 9: [ADC1EN] Analog to Digital Converter 1 Clock Enable
        REG_TYPE _reserved3         : 3; // Bit 10-12
        REG_TYPE SPI1_EN            : 1; // Bit 13: [SPI1EN] SPI 1  Clock Enable
        REG_TYPE USART1_EN          : 1; // Bit 14: [USART1] USART 1  Clock Enable
        REG_TYPE _reserved4         : 17; // Bit 15-31 
    } bits;

    REG_TYPE all;
    
} APB2_r;

#define APB2_REG                ((volatile APB2_r*) 0x40021018UL) // Advanced Peripheral Bus Enable Register

typedef union
{
    struct
    {
        REG_TYPE TIM2_EN            : 1; // Bit 0: [TIM2EN] Timer 2 Enable
        REG_TYPE TIM3_EN            : 1; // Bit 1: [TIM3EN] Timer 3 Enable
        REG_TYPE TIM4_EN            : 1; // Bit 2: [TIM4EN] Timer 4 Enable
        REG_TYPE _reserved1         : 11; // Bit 3-13
        REG_TYPE SPI2_EN            : 1; // Bit 14: [SPI2EN] Enable USART 2
        REG_TYPE _reserved2         : 2; // Bit 15-16
        REG_TYPE USART2_EN          : 1; // Bit 17: [USART2EN] USART 2 Enable
        REG_TYPE USART3_EN          : 1; // Bit 18: [USART3EN] USART 3 Enable
        REG_TYPE _reserved3         : 2; // Bit 19-20
        REG_TYPE I2C_C1_EN          : 1; // Bit 21: [I2C1EN] Enable I2C Clock 1
        REG_TYPE _reserved4         : 1; // Bit 22
        REG_TYPE USB_EN             : 1; // Bit 23: [USBEN] USB Enable
        REG_TYPE _reserved5         : 8; // Bit 24-31;
    } bits;

    REG_TYPE all;
} APB1_r;
#define APB1_REG                    ((volatile APB1_r*) 0x4002101CUL)

typedef union
{
    struct
    {
        REG_TYPE LSI_EN         : 1; // Bit 0: [LSION] LSI Enable (Low Speed Internal)
        REG_TYPE LSI_RDY        : 1; // Bit 1: [LSIRDY]LSI Ready
        REG_TYPE _reserved1     :22; // Bit 2-23

        REG_TYPE RMVF           : 1; // Bit 24: Remove Reset Flags
        REG_TYPE _reserved2     : 1; // Bit 25
        
        REG_TYPE PIN_RST_F      : 1; // Bit 26: [PINRSTF] Reset pin triggered reset flag
        REG_TYPE POR_RST_F      : 1; // Bit 27: [PORRSTF] Power-on Reset flag
        REG_TYPE SFT_RST_F      : 1; // Bit 28: [SFTRSTF] Software Reset flag
        REG_TYPE I_WDG_RST_F    : 1; // Bit 29: [IWDGRSTF] Independent Watchdog Reset flag
        REG_TYPE W_WDG_RST_F    : 1; // Bit 30: [WWDGRSTF] Window Watchdog Reset flag
        REG_TYPE LPWR_RST_F     : 1; // Bit 31: [LPWRRSTF] Low power reset flag
    } bits;

    REG_TYPE all;
} RCC_CSR_r;
#define RCC_CSR_REG         ((volatile RCC_CSR_r*) 0x40021024UL)

#define TIM2_CR1_BASE           0x40000000
#define TIM2_CEN                (1 << 0)    // Counter Enable
#define TIM2_SR_OFFSET          0x0C
#define TIM2_PSC_OFFSET         0x28
#define TIM2_ARR_OFFSET         0x2C

/******************************************************************************
 * 
 * Debug Exception and Monitor Control
 * 
******************************************************************************/
typedef union
{
    struct
    {
        REG_TYPE _reserved1         : 24; // Bit 0-23
        REG_TYPE TRC_ARCHICT_EN     : 1;  // Bit 24: [TRCENA] Trace Archictecture Enable
        REG_TYPE _reserved2         : 7;  // Bit 25-31
    } bits;

    REG_TYPE all;
} DEMCR_r;
#define DEMCR_REG                   ((volatile DEMCR_r*) 0xE000EDFCUL)  // Debug Exception and Monitor Control Register



typedef union
{
    struct
    {
        REG_TYPE CYCCNT_EN          : 1; // Bit 0: Cycle Counter Enable
        REG_TYPE _reserved1         : 31; // Bit 1-31
    } bits;
    
} DWT_CTRL_r;

typedef struct
{
    volatile DWT_CTRL_r CTRL;
    volatile REG_TYPE CYCCNT;
} DWT_BLOCK;
#define DWT_BLOCK_REG               ((volatile DWT_BLOCK*) 0xE0001000UL)  // Data Watchpoint and Trace


/******************************************************************************
 * 
 * Flash
 * 
******************************************************************************/
typedef struct
{
    REG_TYPE FLASH_ACR;
} FLASH_BLOCK;

#define FLASH_BLOCK_REG     ((volatile FLASH_BLOCK*) 0x40022000UL)

/******************************************************************************
 * 
 * USART
 * 
******************************************************************************/
typedef union 
{
    struct
    {
        REG_TYPE PE             : 1; // Bit 0: Parity Error
        REG_TYPE FE             : 1; // Bit 1: Frame Error
        REG_TYPE NE             : 1; // Bit 2: Noise Error
        REG_TYPE ORE            : 1; // Bit 3: Overrun Error
        REG_TYPE IDLE           : 1; // Bit 4: Idle Line
        REG_TYPE RXNE           : 1; // Bit 5: Receiver Not Empty
        REG_TYPE TC             : 1; // Bit 6: Transmission Complete
        REG_TYPE TXE            : 1; // Bit 7: Transmitter Empty
        REG_TYPE LBD            : 1; // Bit 8: LIN Break
        REG_TYPE CTS            : 1; // Bit 9: Clear to send
        REG_TYPE _reserved1     : 22; //Bit 10-31 
    } bits;

    REG_TYPE all;
} USART_SR_t;

typedef union 
{
    struct 
    {
        REG_TYPE SBK            :1; // Bit 0: Send break
        REG_TYPE RWU            :1; // Bit 1: Receiver wakeup
        REG_TYPE RE             :1; // Bit 2: Receiver enable
        REG_TYPE TE             :1; // Bit 3: Transmitter enable
        REG_TYPE _reserved2     :9; // Bit 4-12
        REG_TYPE UE             :1; // Bit 13: USART enable
        REG_TYPE _reserved1     :18; // Bit 14-31
    } bits;
    REG_TYPE all;
} USART_CR1_t;

typedef struct 
{
    volatile USART_SR_t SR;
    REG_TYPE DR;
    REG_TYPE BRR;
    volatile USART_CR1_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_BLOCK;

#define USART1_REG  ((volatile USART_BLOCK*)0x40013800UL)
#define USART2_REG  ((volatile USART_BLOCK*)0x40004400UL)
#define USART3_REG  ((volatile USART_BLOCK*)0x40004800UL)

/******************************************************************************
 * 
 * PORT A GPIO PINS
 * 
******************************************************************************/
#define A_GPIO_BASE 0x40010800UL


/**
 * Pin capabilities
 * 
 * PA0
 * PWM
 * Analog           (ADC0)
 * USART            (CTS2)
 * Timer & Channel  (T2C1E)
 * Control          (Wake up)
 * 3.3V
 */
#define STM_PIN_PA00 0

/**
 * PA1
 * PWM
 * Analog           (ADC1)
 * USART            (RTS2)
 * Timer & Channel  (T2C2)
 * 3.3V
 */
#define STM_PIN_PA01 1

/**
 * PA2
 * PWM
 * Analog           (ADC2)
 * USART2           (TX2)
 * Timer & Channel  (T2C3)
 * 3.3V
 */
#define STM_PIN_PA02 2

/**
 * PA3
 * PWM
 * Analog           (ADC3)
 * USART2           (RX2)
 * Timer & Channel  (T2C4)
 * 3.3V
 */
#define STM_PIN_PA03 3

/**
 * PA4
 * Analog           (ADC4)
 * SPI              (NSS1)
 * USART            (CK1)
 * 3.3V
 */ 
#define STM_PIN_PA04 4

/**
 * PA5
 * Analog           (ADC5)
 * SPI              (SCK1)
 * 3.3V
 */
#define STM_PIN_PA05 5

/**
 * PA6
 * PWM
 * Analog           (ADC6)
 * SPI              (MISO1)
 * Timer & Channel  (T3C1)
 * Timer & Channel  (T1BKIN)
 * 3.3V
 */
#define STM_PIN_PA06 6

/**
 * PA7
 * PWM
 * Analog           (ADC7)
 * SPI              (MOSI1)
 * Timer & Channel  (T3C2)
 * Timer & Channel  (T1C1N)
 * 3.3V
 */
#define STM_PIN_PA07 7

/**
 * PA8
 * PWM
 * USART            (CK1)
 * Timer & Channel  (T1C1)
 * Misc             (MCO)
 * 5V
 */
#define STM_PIN_PA08 8

/**
 * PA9
 * PWM
 * USART1           (TX1)
 * Timer & Channel  (T1C2)
 * 5V
 */
#define STM_PIN_PA09 9

/**
 * PA10
 * USART1           (RX1)
 * Timer & Channel  (T1C3)
 * PWM
 * 5V
 */
#define STM_PIN_PA10 10

/**
 * PA11
 * USB              (USB-)
 * PWM
 * USART            (CTS1)
 * Timer & Channel  (T1C4)
 * CAN Bus          (CANRX)
 * 5V
 */
#define STM_PIN_PA11 11

/**
 * PA12
 * USB              (USB+)
 * USART            (RTS1)
 * Timer & Channel  (T1ETR)
 * CAN Bus          (CANTX)
 * 5V
 */
#define STM_PIN_PA12 12

/**
 * PA13
 * CONTROL          (SWDIO)
 * CONTROL          (JTMS)
 * 5V
 */
#define STM_PIN_PA13 13

/**
 * PA14
 * CONTROL          (SWCLK)
 * CONTROL          (JTCK)
 * 5V
 */
#define STM_PIN_PA14 14

/**
 * PA15
 * CONTROL          (JTDI)
 * SPI              (NSS1)
 * Timer & Channel  (T2C1E)
 * 5V
 */
#define STM_PIN_PA15 15



/******************************************************************************
 * 
 * PORT B GPIO PINS
 * 
******************************************************************************/
#define B_GPIO_BASE 0x40010C00UL
/**
 * Pin Capabilities
 * 
 * PB0
 * PWM
 * Analog           (ADC8)
 * Timer & Channel  (T3C3)
 * Timer & Channel  (T1C2N)
 * 3.3V
 */
#define STM_PIN_PB00 64

/**
 * PB1
 * PWM
 * Analog           (ADC9)
 * Timer & Channel  (T3C4)
 * Timer & Channel  (T1C3N)
 * 3.3V
 */ 
#define STM_PIN_PB01 65

/**
 * PB2
 * Control          (BOOT1)
 * 5V
 */
#define STM_PIN_PB02 66

/**
 * PB3
 * CONTROL          (JTDO)
 * SPI              (SCLK1)
 * Timer & Channel  (T2C2)
 * Misc             (TRACE SWO)
 * 5V
 */
#define STM_PIN_PB03 67
#define DEBUG_PIN STM_PIN_PB03

/**
 * PB4
 * CONTROL          (JTRST)
 * SPI              (MISO1)
 * Timer & Channel  (T3C1)
 * 5V
 */
#define STM_PIN_PB04 68
#define DEBUG_RST STM_PIN_PB04

/**
 * PB5
 * I2C              (SMBAI1)
 * SPI              (MOSI1)
 * Timer & Channel  (T2C2)
 * 3.3V
 */
#define STM_PIN_PB05 69

/**
 * PB6
 * PWM
 * I2C              (SCL1)
 * Timer & Channel  (T4C1)
 * USART            (TX1)
 * 5V
 */
#define STM_PIN_PB06 70

/**
 * PB7
 * PWM
 * I2C              (SDA1)
 * Timer & Channel  (T4C2)
 * USART            (RX1)
 */
#define STM_PIN_PB07 71

/**
 * PB8
 * PWM
 * Timer & Channel  (T4C3)
 * I2C              (SCL1)
 * CAN Bus          (CANRX)
 * 5V
 */
#define STM_PIN_PB08 72

/**
 * PB9
 * PWM
 * Timer & Channel  (T4C4)
 * I2C              (SDA1)
 * CAN Bus          (CANTX)
 * 5V
 */
#define STM_PIN_PB09 73

/**
 * PB10
 * I2C              (SCL2)
 * USART3           (TX3)
 * Timer & Channel  (T2C3N)
 * 5V
 */
#define STM_PIN_PB10 74

/**
 * PB11
 * I2C              (SDA2)
 * USART3           (RX3)
 * Timer & Channel  (T2C4N)
 * 5V
 */
#define STM_PIN_PB11 75

/**
 * PB12
 * I2C              (SMBAI2)
 * SPI              (NSS2)
 * Timer & Channel  (T1NKIN)
 * USART            (CK3)
 */
#define STM_PIN_PB12 76

/**
 * PB13
 * SPI              (SCK2)
 * Timer & Channel  (T1C1N)
 * USART            (CTS3)
 */
#define STM_PIN_PB13 77

/**
 * PB14
 * SPI              (MISO2)
 * Timer & Channel  (T1C2N)
 * USART            (RTS3)
 */
#define STM_PIN_PB14 78

/**
 * PB15
 * SPI              (MOSI2)
 * Timer & Channel  (T1C3N)
 */
#define STM_PIN_PB15 79


/******************************************************************************
 * 
 * PORT C GPIO PINS
 * 
******************************************************************************/
#define C_GPIO_BASE             0x40011000UL


/**
 * PC13
 ** HEAVILY RESTRICTED **
 * Misc             Tamper RTC
 * Board Hardware   Onboard LED
 */
#define STM_PIN_PC13 141

/******************************************************************************
 * 
 * MISC PINS
 * 
******************************************************************************/
/**
 * CONTROL          (NRST)
 * BOARD HARDWARE   (Reset Button)
 * 3.3V
 */
#define STM_PIN_RESET 7

#define STM_PIN_44 -1 //BOOT0

/******************************************************************************
 *
 * GPIO BLOCKS
 *  
******************************************************************************/

typedef union
{
    struct
    {
        REG_TYPE MODE0              : 2;  // Bit 0-1:   Pin 0 Mode
        REG_TYPE CNF0               : 2;  // Bit 2-3:   Pin 0 Config
        REG_TYPE MODE1              : 2;  // Bit 4-5:   Pin 1 Mode
        REG_TYPE CNF1               : 2;  // Bit 6-7:   Pin 1 Config
        REG_TYPE MODE2              : 2;  // Bit 8-9:   Pin 2 Mode
        REG_TYPE CNF2               : 2;  // Bit 10-11: Pin 2 Config
        REG_TYPE MODE3              : 2;  // Bit 12-13: Pin 3 Mode
        REG_TYPE CNF3               : 2;  // Bit 14-15: Pin 3 Config
        REG_TYPE MODE4              : 2;  // Bit 16-17: Pin 4 Mode
        REG_TYPE CNF4               : 2;  // Bit 18-19: Pin 4 Config
        REG_TYPE MODE5              : 2;  // Bit 20-21: Pin 5 Mode
        REG_TYPE CNF5               : 2;  // Bit 22-23: Pin 5 Config
        REG_TYPE MODE6              : 2;  // Bit 24-25: Pin 6 Mode
        REG_TYPE CNF6               : 2;  // Bit 26-27: Pin 6 Config
        REG_TYPE MODE7              : 2;  // Bit 28-29: Pin 7 Mode
        REG_TYPE CNF7               : 2;  // Bit 30-31: Pin 7 Config
    } bits;

    REG_TYPE all;
} GPIO_CRL_r;


typedef union
{
    struct
    {
        REG_TYPE MODE8              : 2;  // Bit 0-1:   Pin 8 Mode
        REG_TYPE CNF8               : 2;  // Bit 2-3:   Pin 8 Configuration
        REG_TYPE MODE9              : 2;  // Bit 4-5:   Pin 9 Mode
        REG_TYPE CNF9               : 2;  // Bit 6-7:   Pin 9 Configuration
        REG_TYPE MODE10             : 2;  // Bit 8-9:   Pin 10 Mode
        REG_TYPE CNF10              : 2;  // Bit 10-11: Pin 10 Configuration
        REG_TYPE MODE11             : 2;  // Bit 12-13: Pin 11 Mode
        REG_TYPE CNF11              : 2;  // Bit 14-15: Pin 11 Configuration
        REG_TYPE MODE12             : 2;  // Bit 16-17: Pin 12 Mode
        REG_TYPE CNF12              : 2;  // Bit 18-19: Pin 12 Configuration
        REG_TYPE MODE13             : 2;  // Bit 20-21: Pin 13 Mode
        REG_TYPE CNF13              : 2;  // Bit 22-23: Pin 13 Configuration
        REG_TYPE MODE14             : 2;  // Bit 24-25: Pin 14 Mode
        REG_TYPE CNF14              : 2;  // Bit 26-27: Pin 14 Configuration
        REG_TYPE MODE15             : 2;  // Bit 28-29: Pin 15 Mode
        REG_TYPE CNF15              : 2;  // Bit 30-31: Pin 15 Configuration
    } bits;

    REG_TYPE all;
} GPIO_CRH_r;

typedef union
{
    struct
    {
        REG_TYPE IDR0               : 1;  // Bit 0: Pin 0 Input State
        REG_TYPE IDR1               : 1;  // Bit 1: Pin 1 Input State
        REG_TYPE IDR2               : 1;  // Bit 2: Pin 2 Input State
        REG_TYPE IDR3               : 1;  // Bit 3: Pin 3 Input State
        REG_TYPE IDR4               : 1;  // Bit 4: Pin 4 Input State
        REG_TYPE IDR5               : 1;  // Bit 5: Pin 5 Input State
        REG_TYPE IDR6               : 1;  // Bit 6: Pin 6 Input State
        REG_TYPE IDR7               : 1;  // Bit 7: Pin 7 Input State
        REG_TYPE IDR8               : 1;  // Bit 8: Pin 8 Input State
        REG_TYPE IDR9               : 1;  // Bit 9: Pin 9 Input State
        REG_TYPE IDR10              : 1;  // Bit 10: Pin 10 Input State
        REG_TYPE IDR11              : 1;  // Bit 11: Pin 11 Input State
        REG_TYPE IDR12              : 1;  // Bit 12: Pin 12 Input State
        REG_TYPE IDR13              : 1;  // Bit 13: Pin 13 Input State
        REG_TYPE IDR14              : 1;  // Bit 14: Pin 14 Input State
        REG_TYPE IDR15              : 1;  // Bit 15: Pin 15 Input State
        REG_TYPE _reserved          : 16; // Bit 16-31: Reserved
    } bits;

    REG_TYPE all;
} GPIO_IDR_r;

typedef union
{
    struct
    {
        REG_TYPE ODR0               : 1;  // Bit 0: Pin 0 Output Drive State
        REG_TYPE ODR1               : 1;  // Bit 1: Pin 1 Output Drive State
        REG_TYPE ODR2               : 1;  // Bit 2: Pin 2 Output Drive State
        REG_TYPE ODR3               : 1;  // Bit 3: Pin 3 Output Drive State
        REG_TYPE ODR4               : 1;  // Bit 4: Pin 4 Output Drive State
        REG_TYPE ODR5               : 1;  // Bit 5: Pin 5 Output Drive State
        REG_TYPE ODR6               : 1;  // Bit 6:
        REG_TYPE ODR7               : 1;  // Bit 7: Pin 7 Output Drive State
        REG_TYPE ODR8               : 1;  // Bit 8: Pin 8 Output Drive State
        REG_TYPE ODR9               : 1;  // Bit 9: Pin 9 Output Drive State
        REG_TYPE ODR10              : 1;  // Bit 10: Pin 10 Output Drive State
        REG_TYPE ODR11              : 1;  // Bit 11: Pin 11 Output Drive State
        REG_TYPE ODR12              : 1;  // Bit 12: Pin 12 Output Drive State
        REG_TYPE ODR13              : 1;  // Bit 13: Pin 13 Output Drive State
        REG_TYPE ODR14              : 1;  // Bit 14: Pin 14 Output Drive State
        REG_TYPE ODR15              : 1;  // Bit 15: Pin 15 Output Drive State
        REG_TYPE _reserved1         : 16; // Bit 16-31: Reserved
    } bits;

    REG_TYPE all;
} GPIO_ODR_r;

typedef union
{
    struct
    {
        // Lower 16 bits: Bit Set (Pins 0-15)
        REG_TYPE BS0        : 1;  // Bit 0: 
        REG_TYPE BS1        : 1;  // Bit 1: 
        REG_TYPE BS2        : 1;  // Bit 2: 
        REG_TYPE BS3        : 1;  // Bit 3: 
        REG_TYPE BS4        : 1;  // Bit 4: 
        REG_TYPE BS5        : 1;  // Bit 5: 
        REG_TYPE BS6        : 1;  // Bit 6: 
        REG_TYPE BS7        : 1;  // Bit 7: 
        REG_TYPE BS8        : 1;  // Bit 8: 
        REG_TYPE BS9        : 1;  // Bit 9: 
        REG_TYPE BS10       : 1;  // Bit 10:
        REG_TYPE BS11       : 1;  // Bit 11:
        REG_TYPE BS12       : 1;  // Bit 12:
        REG_TYPE BS13       : 1;  // Bit 13:
        REG_TYPE BS14       : 1;  // Bit 14:
        REG_TYPE BS15       : 1;  // Bit 15:

        // Upper 16 bits: Bit Reset (Pins 0-15)
        REG_TYPE BR0        : 1;  // Bit 16: Reset Pin 0 Low
        REG_TYPE BR1        : 1;  // Bit 17: Reset Pin 1 Low
        REG_TYPE BR2        : 1;  // Bit 18: Reset Pin 2 Low
        REG_TYPE BR3        : 1;  // Bit 19: Reset Pin 3 Low
        REG_TYPE BR4        : 1;  // Bit 20: Reset Pin 4 Low
        REG_TYPE BR5        : 1;  // Bit 21: Reset Pin 5 Low
        REG_TYPE BR6        : 1;  // Bit 22: Reset Pin 6 Low
        REG_TYPE BR7        : 1;  // Bit 23: Reset Pin 7 Low
        REG_TYPE BR8        : 1;  // Bit 24: Reset Pin 8 Low
        REG_TYPE BR9        : 1;  // Bit 25: Reset Pin 9 Low
        REG_TYPE BR10       : 1;  // Bit 26: Reset Pin 10 Low
        REG_TYPE BR11       : 1;  // Bit 27: Reset Pin 11 Low
        REG_TYPE BR12       : 1;  // Bit 28: Reset Pin 12 Low
        REG_TYPE BR13       : 1;  // Bit 29: Reset Pin 13 Low
        REG_TYPE BR14       : 1;  // Bit 30: Reset Pin 14 Low
        REG_TYPE BR15       : 1;  // Bit 31: Reset Pin 15 Low
    } bits;

    REG_TYPE all;
} GPIO_BSRR_r;

typedef struct
{
    volatile GPIO_CRL_r CRL;
    volatile GPIO_CRH_r CRH;
    volatile GPIO_IDR_r IDR;
    volatile GPIO_ODR_r ODR;
    volatile GPIO_BSRR_r BSRR;
} GPIO_BLOCK;

#define A_GPIO_BLOCK_REG             ((volatile GPIO_BLOCK*) A_GPIO_BASE)
#define B_GPIO_BLOCK_REG             ((volatile GPIO_BLOCK*) B_GPIO_BASE)
#define C_GPIO_BLOCK_REG             ((volatile GPIO_BLOCK*) C_GPIO_BASE)

/******************************************************************************
 *
 * Interrupts (EXTI)
 *  
******************************************************************************/

typedef enum
{
    EXTI_LINE_0 = 0,
    EXTI_LINE_1,
    EXTI_LINE_2,
    EXTI_LINE_3,
    EXTI_LINE_4,
    EXTI_LINE_5,
    EXTI_LINE_6,
    EXTI_LINE_7,
    EXTI_LINE_8,
    EXTI_LINE_9,
    EXTI_LINE_10,
    EXTI_LINE_11,
    EXTI_LINE_12,
    EXTI_LINE_13,
    EXTI_LINE_14,
    EXTI_LINE_15,
    EXTI_LINE_16,
    EXTI_LINE_17,
    EXTI_LINE_18,
    EXTI_LINE_19,
} EXTI_LINE_e;

typedef struct
{
    REG_TYPE IMR;      // Interrupt Mask Register
    REG_TYPE EMR;      // Event Mask Register
    REG_TYPE RTSR;     // Rising trigger selection register
    REG_TYPE FTSR;     // Falling trigger selection register
    REG_TYPE SWIER;    // Software interrupt event register
    REG_TYPE PR;       // Pending Register
} EXTI_BLOCK;

#define EXTI_BLOCK_REG      ((volatile EXTI_BLOCK*) 0x40010400)

/******************************************************************************
 *
 * Watch Dog
 *  
******************************************************************************/


typedef struct
{
    REG_TYPE KEY_R;         // [IWDG_KR]
    REG_TYPE PRESCALER_R;   // [IWDG_PR]
    REG_TYPE RELOAD_R;      // [IWDG_RLR]
    REG_TYPE STATUS_R;      // [IWDG_SR]

} IWDG_BLOCK;

#define IWDG_BLOCK_REG      ((volatile IWDG_BLOCK*) 0x40003000)

/******************************************************************************
 *
 * Alternate Function Input Output AFIO
 *  
******************************************************************************/
typedef struct
{
    uint32_t val         : 4;
} exti_field_t;

typedef union 
{
    struct 
    {
        volatile exti_field_t exti_x[4];
        REG_TYPE _reserved  :16;
    } bits;

    REG_TYPE all;
} EXTI_r;


typedef struct {
    volatile EXTI_r* exti_cr;
    uint8_t exti_index;
    volatile uint32_t* nvic_reg;
    uint8_t nvic_shift;
} exti_hw_map_t;

typedef struct
{
    REG_TYPE EV_CR;             // [EVCR] Event Control
    REG_TYPE RMAP_R;            // [MAPR] Remap Register
    volatile EXTI_r EXTI_CR1;   // [EXTICR1]
    volatile EXTI_r EXTI_CR2;   // [EXTICR2]
    volatile EXTI_r EXTI_CR3;   // [EXTICR3]
    volatile EXTI_r EXTI_CR4;   // [EXTICR4]
} AFIO_BLOCK;

#define AFIO_BLOCK_REG      ((volatile AFIO_BLOCK*) 0x40010000)

/******************************************************************************
 *
 * STM32F103 Functions
 *  
******************************************************************************/

/**
 * @brief Converts a generic HAL GPIO configuration into an STM32F103 GPIO mode.
 *
 * Translates the platform-independent GPIO configuration provided by the HAL
 * into the corresponding STM32F103 GPIO mode value used by the GPIO peripheral
 * configuration registers.
 *
 * @param[in] hal_pin_cfg Generic HAL GPIO configuration value.
 *
 * @return STM32F103-specific GPIO mode identifier.
 */
uint8_t stm32f103_translate_gpio_mode(hal_gpio_config_t hal_pin_cfg);

/**
 * @brief Retrieves the STM32F103 GPIO port identifier for a pin.
 *
 * Converts a HAL pin identifier into the corresponding STM32F103 GPIO port
 * index. This value is used to access the correct GPIO peripheral block.
 *
 * @param[in] pin STM32 packed pin identifier.
 *
 * @return STM32F103 GPIO port identifier.
 */
uint8_t stm32f103_get_port_id(uint8_t pin);

/**
 * @brief Retrieves the STM32F103 GPIO peripheral register block.
 *
 * Returns the memory-mapped GPIO register block associated with the specified
 * STM32F103 GPIO port.
 *
 * @param[in] port STM32F103 GPIO port identifier.
 *
 * @return Pointer to the GPIO peripheral register block.
 */
volatile GPIO_BLOCK* stm32f103_get_port_block(uint8_t port);

/**
 * @brief Verifies whether an STM32F103 GPIO pin is valid.
 *
 * Checks whether the supplied pin identifier corresponds to a supported GPIO
 * pin available on the target STM32F103 device.
 *
 * @param[in] pin Packed pin identifier to validate.
 *
 * @return HAL status code indicating whether the pin is valid.
 */
hal_status_t stm32f103_verify_pin(uint8_t pin);

/**
 * @brief Checks whether an STM32F103 pin supports UART transmission.
 *
 * Determines whether the specified GPIO pin can be configured as a UART TX
 * pin based on the STM32F103 alternate-function mapping.
 *
 * @param[in] pin Packed pin identifier to check.
 *
 * @return HAL status code indicating whether the pin supports UART TX.
 */
hal_status_t stm32f103_is_TX_pin(uint8_t pin);

/**
 * @brief Checks whether an STM32F103 pin supports UART reception.
 *
 * Determines whether the specified GPIO pin can be configured as a UART RX
 * pin based on the STM32F103 alternate-function mapping.
 *
 * @param[in] pin Packed pin
 *
 * @return HAL status code indicating whether the pin supports UART RX.
 */
hal_status_t stm32f103_is_RX_pin(uint8_t pin);


#endif //__STM32F103C8T6