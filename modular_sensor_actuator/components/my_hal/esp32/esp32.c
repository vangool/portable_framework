/******************************************************************************
 * @file: esp32.c
 * @brief: Target-specific system initialization and core management for ESP32.
 *
 * This file implements the concrete system-wide hooks required to bootstrap
 * the ESP32 silicon platform. It manages global hardware initializations,
 * system-level status mappings, memory-heap allocations, and power-management
 * configuration routines that are not handled directly by the operating system.
 *
 * @author: Michael Van Gool
 * @date: 2026-05-04
 * @license: MIT License (See header file for full license conditions)
 * * Copyright (c) 2026 Michael Van Gool. All rights reserved.
 ******************************************************************************/
#include "esp32.h"

hal_gpio_set_direction          hal_gpio_set_direction_func         = esp32_gpio_set_direction;
hal_gpio_set_level              hal_gpio_set_level_func             = esp32_gpio_set_level;
hal_gpio_get_level              hal_gpio_get_level_func             = esp32_gpio_get_level;
hal_gpio_set_intr_type          hal_gpio_set_intr_type_func         = esp32_gpio_set_intr_type;
hal_gpio_isr_handler_add        hal_gpio_isr_handler_add_func       = esp32_gpio_isr_handler_add;
hal_rom_delay_us                hal_rom_delay_us_func               = esp_rom_delay_us;
hal_timer_get_time              hal_timer_get_time_func             = esp_timer_get_time;
hal_gpio_install_isr_service    hal_gpio_install_isr_service_func   = esp32_gpio_install_isr_service;
hal_log_info                    hal_log                             = esp32_log_info;

/**
 * Translate the status from ESP32 API to HAL API
 * 
 * @param[in] status The esp32 error code to be translated
 * 
 * @return hal_status_t Returns the HAL associated status
 */
static hal_status_t hal_translate_status(esp_err_t status)
{
    switch(status) 
    {
        case ESP_OK:                return HAL_OK;
        case ESP_ERR_INVALID_ARG:   return HAL_ERROR_INVALID_ARG;
        default:                    return HAL_ERROR_GENERAL;
    }
}
/**
 * Translate HAL interrupt 
 * 
 * @param[in] flag The HAL flag to convert to ESP32 flag
 * 
 * @return uint32_t esp32flag
 */
static uint32_t hal_to_esp_intr_flag(hal_intr_type_t flag)
{
    switch(flag)
    {
        case HAL_INTR_FLAG_LOW:     return ESP_INTR_FLAG_LOWMED;
        case HAL_INTR_FLAG_MED:     return ESP_INTR_FLAG_LEVEL1;
        case HAL_INTR_FLAG_HIGH:    return ESP_INTR_FLAG_LEVEL2;
        case HAL_INTR_FLAG_IRAM:    return ESP_INTR_FLAG_IRAM;
        case HAL_INTR_FLAG_SHARED:  return ESP_INTR_FLAG_SHARED;
        default:                    return -1;
    }
}

hal_status_t esp32_gpio_set_direction(uint8_t pin, uint8_t direction)
{
    esp_err_t ret = gpio_set_direction(pin, direction);
    return hal_translate_status(ret);
}

hal_status_t esp32_gpio_set_level(uint8_t pin, uint8_t level)
{
    esp_err_t ret = gpio_set_level(pin, level);
    return hal_translate_status(ret);
}

int8_t esp32_gpio_get_level(uint8_t pin)
{
    return (int8_t)gpio_get_level(pin);
}

hal_status_t esp32_gpio_set_intr_type(uint8_t pin, hal_intr_type_t type)
{
    gpio_int_type_t esp_type;

    switch(type)
    {
        case HAL_GPIO_INTR_DISABLE:      esp_type = GPIO_INTR_DISABLE; break;
        case HAL_GPIO_INTR_POSEDGE:      esp_type = GPIO_INTR_POSEDGE; break;
        case HAL_GPIO_INTR_NEGEDGE:      esp_type = GPIO_INTR_NEGEDGE; break;
        case HAL_GPIO_INTR_ANYEDGE:      esp_type = GPIO_INTR_ANYEDGE; break;
        case HAL_GPIO_INTR_LOW_LEVEL:    esp_type = GPIO_INTR_LOW_LEVEL; break;
        case HAL_GPIO_INTR_HIGH_LEVEL:   esp_type = GPIO_INTR_HIGH_LEVEL; break;
        default:
            return HAL_ERROR_INVALID_ARG;
    }

    esp_err_t ret = gpio_set_intr_type((gpio_num_t)pin, esp_type);

    return hal_translate_status(ret);
}

void esp32_rom_delay_us(uint32_t delay_us)
{
    esp_rom_delay_us(delay_us);
}

hal_status_t esp32_gpio_isr_handler_add(uint8_t gpio_pin, void* isr_handler, void* args)
{
    if(!isr_handler) return HAL_ERROR_INVALID_ARG;
    
    esp_err_t ret = gpio_isr_handler_add((gpio_num_t) gpio_pin, (gpio_isr_t) isr_handler, args);
    return hal_translate_status(ret);
}

int64_t esp32_timer_get_time()
{
    return esp_timer_get_time();
}

hal_status_t esp32_gpio_install_isr_service(hal_intr_flag_t flag)
{
    uint32_t esp32_flag = hal_to_esp_intr_flag(flag);
    esp_err_t ret = gpio_install_isr_service(esp32_flag);
    return hal_translate_status(ret);
}

void esp32_log_info(uint8_t level, const char *tag, const char *fmt, ...)
{
    if(level == 0)
        return;

    va_list args;
    va_start(args, fmt);

    esp_log_writev(ESP_LOG_INFO, tag, fmt, args);

    va_end(args);
}