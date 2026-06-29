#ifndef __STM32_FUNC_H__
#define __STM32_FUNC_H__

#include "../include/hal.h"

// Must clean this up. Header guards for these.
#include "stm32f103/stm32f103.h"

#include <stdint.h>

typedef uint8_t (*stm32fn_translate_gpio_mode)(hal_gpio_config_t level);
typedef volatile GPIO_BLOCK* (*stm32fn_get_gpio_port_block)(uint8_t port);
typedef hal_status_t (*stm32fn_verify_pin)(uint8_t port);
typedef hal_status_t (*stm32fn_is_TX_pin)(uint8_t pin);
typedef hal_status_t (*stm32fn_is_RX_pin)(uint8_t pin);

extern stm32fn_translate_gpio_mode stm32fn_translate_gpio_mode_func;
extern stm32fn_get_gpio_port_block stm32fn_get_gpio_port_block_func;
extern stm32fn_verify_pin stm32fn_verify_pin_func;
extern stm32fn_is_TX_pin stm32fn_is_TX_pin_func;
extern stm32fn_is_RX_pin stm32fn_is_RX_pin_func;

#endif //__STM32_FUNC_H__