#include "../addr.h"
#include "stm32f103.h"
#include "../stm32_func.h"

stm32fn_translate_gpio_mode stm32fn_translate_gpio_mode_func    = stm32f103_translate_gpio_mode;
stm32fn_get_port_id stm32fn_get_port_id_func                    = stm32f103_get_port_id;
stm32fn_get_gpio_port_block stm32fn_get_gpio_port_block_func    = stm32f103_get_port_block;
stm32fn_verify_pin stm32fn_verify_pin_func                      = stm32f103_verify_pin;
stm32fn_is_TX_pin stm32fn_is_TX_pin_func                        = stm32f103_is_TX_pin;
stm32fn_is_RX_pin stm32fn_is_RX_pin_func                        = stm32f103_is_RX_pin;

/**
 * @brief Translates generic HAL GPIO modes into the raw 4-bit CNF[1:0] + MODE[1:0] nibble 
 * for the STM32F103 CRL/CRH registers.
 * @param gpio_config The application-level abstract mode.
 * @return uint8_t A 4-bit configuration value (0x0 to 0xF).
 */
uint8_t stm32f103_translate_gpio_mode(hal_gpio_config_t gpio_config)
{
    switch (gpio_config)
    {
        case HAL_GPIO_MODE_DISABLE:
        case HAL_GPIO_MODE_ANALOG:          return 0x0;

        case HAL_GPIO_MODE_INPUT:           return 0x4;

        case HAL_GPIO_MODE_INPUT_PULLUP:
        case HAL_GPIO_MODE_INPUT_PULLDN:    return 0x8;
        case HAL_GPIO_MODE_OUTPUT:          return 0x2;

        case HAL_GPIO_MODE_OUTPUT_PUSHPULL: return 0x3;

        case HAL_GPIO_MODE_OUTPUT_OD:
        case HAL_GPIO_MODE_INPUT_OUTPUT:
        case HAL_GPIO_MODE_INPUT_OUTPUT_OD: return 0x7;

        case HAL_GPIO_MODE_AF_PUSHPULL:     return 0xB;

        case HAL_GPIO_MODE_AF_OD:           return 0xF;
    
        default:                            return 0x0;
    }
}


uint8_t stm32f103_get_port_id(uint8_t packed_pin)
{
    return (packed_pin >> 6) & 3;
}

uint8_t clear_port(uint8_t packed_pin)
{
    return packed_pin & 63;
}

hal_status_t stm32f103_verify_pin(uint8_t packed_pin)
{
    switch (packed_pin)
    {
        case STM_PIN_PA13:
        case STM_PIN_PA14:
        case STM_PIN_PA15:
        case STM_PIN_PB02: // Boot pin
        case STM_PIN_PB03:
        case STM_PIN_PB04:
            return HAL_ERROR_RESTRICTED;        
    }

    if(packed_pin >= STM_PIN_PA00 && packed_pin <= STM_PIN_PA12)
    {
        return HAL_STATUS_OK;
    }

    if(packed_pin >= STM_PIN_PB00 && packed_pin <= STM_PIN_PB01 ||
        packed_pin >= STM_PIN_PB05 && packed_pin <= STM_PIN_PB15)
    {
        return HAL_STATUS_OK;
    }

    if(packed_pin == STM_PIN_PC13)
        return HAL_STATUS_OK;

    return HAL_ERROR_GENERAL;
}

volatile GPIO_BLOCK* stm32f103_get_port_block(uint8_t port)
{
    switch(port)
    {
        case 0:     return A_GPIO_BLOCK_REG;
        case 1:     return B_GPIO_BLOCK_REG;
        case 2:     return C_GPIO_BLOCK_REG;
        default:    return 0;
    }
}

hal_status_t stm32f103_is_TX_pin(uint8_t pin)
{
    hal_status_t ret = stm32f103_verify_pin(pin);
    if(ret != HAL_STATUS_OK)
    {
        return ret;
    }

    switch (pin)
    {
        case STM_PIN_PA02:
        case STM_PIN_PA09:
        case STM_PIN_PB06:
        case STM_PIN_PB10:
            return HAL_STATUS_OK;
    
    default:
        return HAL_ERROR_GENERAL;
    }
}

hal_status_t stm32f103_is_RX_pin(uint8_t pin)
{
    hal_status_t ret = stm32f103_verify_pin(pin);
    if(ret != HAL_STATUS_OK)
    {
        return ret;
    }

    switch (pin)
    {
        case STM_PIN_PA03:
        case STM_PIN_PA10:
        case STM_PIN_PB07:
        case STM_PIN_PB11:
            return HAL_STATUS_OK;
    
    default:
        return HAL_ERROR_GENERAL;
    }
}
