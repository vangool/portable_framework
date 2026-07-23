/******************************************************************************
 * @file: hal_watchdog.c
 * @brief: STM32F103 Independent Watchdog (IWDG) hardware abstraction.
 *
 * This module provides the hardware-specific implementation of the watchdog
 * interface for the STM32F103 microcontroller. It configures and services the
 * Independent Watchdog (IWDG) peripheral using the internal Low-Speed Internal
 * (LSI) oscillator.
 *
 * The watchdog timeout is configured by selecting an appropriate prescaler and
 * reload value based on the requested timeout period. Once initialized, the
 * watchdog must be periodically refreshed to prevent a system reset.
 *
 * This module is intended to be used by higher-level software watchdog
 * components and should not be accessed directly by application code.
 *
 * @author: Michael Van Gool
 * @date: 2026-07-22
 * @license: MIT License
 ******************************************************************************/


#include "hal_watchdog.h"
#include "stm32f103/stm32f103.h"

#define IWDG_KEY_ENABLE_WRITE   0x5555U
#define IWDG_KEY_RELOAD         0xAAAAU
#define IWDG_KEY_START          0xCCCCU

static const uint16_t prescalers[] =
{
    4,
    8,
    16,
    32,
    64,
    128,
    256
};

void hal_watchdog_init(uint32_t timeout_ms)
{
    RCC_CSR_REG->bits.LSI_EN = 1;
    while(!(RCC_CSR_REG->bits.LSI_RDY));

    uint32_t reload, pr;
    for (pr = 0; pr < 7; pr++)
    {
        reload = (timeout_ms * LSI_CLK_SPEED) / (1000 * prescalers[pr]);

        if (reload <= 4096)
        {
            reload--;
            break;
        }
    }


    IWDG_BLOCK_REG->KEY_R = IWDG_KEY_ENABLE_WRITE;
    IWDG_BLOCK_REG->PRESCALER_R = pr;
    IWDG_BLOCK_REG->RELOAD_R = reload;

    while(IWDG_BLOCK_REG->STATUS_R);

    IWDG_BLOCK_REG->KEY_R = IWDG_KEY_START;
}

void hal_watchdog_kick(void)
{
    IWDG_BLOCK_REG->KEY_R = IWDG_KEY_RELOAD;
}
