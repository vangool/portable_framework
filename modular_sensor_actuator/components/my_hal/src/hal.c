/******************************************************************************
 * File: hal.c
 * Description: Implementation of the Harwdware Abstraction Layer (HAL) default
 *          functions. These functions should not be called during production as
 *          their sole purpose is to be used for unit testing.
 * Author: Michael Van Gool
 * Date: 2026-05-04
 * License: MIT
******************************************************************************/
#include "../include/hal.h"

hal_gpio_set_direction          hal_gpio_set_direction_func         = hal_gpio_set_direction_default;
hal_gpio_set_level              hal_gpio_set_level_func             = hal_gpio_set_direction_default;
hal_gpio_get_level              hal_gpio_get_level_func             = hal_gpio_get_level_default;
hal_gpio_set_intr_type          hal_gpio_set_intr_type_func         = hal_gpio_set_intr_type_default;
hal_gpio_isr_handler_add        hal_gpio_isr_handler_add_func       = hal_gpio_isr_handler_add_default;
hal_rom_delay_us                hal_rom_delay_us_func               = hal_rom_delay_us_default;
hal_timer_get_time              hal_timer_get_time_func             = hal_timer_get_time_default;
hal_gpio_install_isr_service    hal_gpio_install_isr_service_func   = hal_gpio_install_isr_service_default;

static hal_status_t is_unit_test()
{
    #ifdef UNIT_TEST
        return HAL_OK;
    #else
        return HAL_ERROR_DEFAULT_IMPLEMENTATION;
    #endif
}

hal_status_t hal_gpio_set_direction_default(uint8_t pin, uint8_t direction)
{
    return is_unit_test();
}

hal_status_t hal_gpio_set_level_default(uint8_t pin, uint8_t level)
{
    return is_unit_test();
}

int8_t hal_gpio_get_level_default(uint8_t pin)
{
    return -1;
}

hal_status_t hal_gpio_set_intr_type_default(uint8_t pin, hal_intr_type_t type)
{
    return is_unit_test();
}

hal_status_t hal_gpio_isr_handler_add_default(uint8_t gpio_pin, void* isr_handler, void* args)
{
    return is_unit_test();
}

void hal_rom_delay_us_default(uint32_t delay_us){}

int64_t hal_timer_get_time_default(void)
{
    return 0;
}

hal_status_t hal_gpio_install_isr_service_default(hal_intr_flag_t intr_flags)
{
    return is_unit_test();
}

void hal_log_info_default(uint8_t level, const char* tag, const char* fmt, ...)
{
    va_list args;

    printf("[%s] ", tag);

    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);

    printf("\n");
}