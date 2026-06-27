#ifndef __STM32_FUNC_H__
#define __STM32_FUNC_H__

#include "../include/hal.h"

#include <stdint.h>

typedef uint8_t (*translate_gpio_mode)(hal_gpio_config_t level);
typedef volatile GPIO_BLOCK* (*get_gpio_port_block)(uint8_t port);

extern translate_gpio_mode translate_gpio_mode_func;
extern get_gpio_port_block get_gpio_port_block_func;

hal_status_t verify_pin(uint8_t packed_pin);

uint8_t associated_port_clock(uint8_t port);

#endif //__STM32_FUNC_H__