#include "../addr.h"
#include "stm32f103.h"
#include "../stm32_func.h"

translate_gpio_mode translate_gpio_mode_func    = stm32f103_translate_gpio_mode;
get_gpio_port_block get_gpio_port_block_func    = stm32f103_get_port_block;


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
        case HAL_GPIO_MODE_ANALOG:
            return 0x0; 

        case HAL_GPIO_MODE_INPUT:
            return 0x4; 

        case HAL_GPIO_MODE_OUTPUT:
            return 0x3; 

        case HAL_GPIO_MODE_OUTPUT_OD:       
        case HAL_GPIO_MODE_INPUT_OUTPUT:    
        case HAL_GPIO_MODE_INPUT_OUTPUT_OD:
            return 0x7; 

        case HAL_GPIO_MODE_AF_PP:
            return 0xB; 

        default:
            return 0x0;
    }
}


uint8_t get_port(uint8_t packed_pin)
{
    return (packed_pin >> 6) & 3;
}

uint8_t clear_port(uint8_t packed_pin)
{
    return packed_pin & 63;
}

hal_status_t verify_pin(uint8_t packed_pin)
{
    switch (packed_pin)
    {
        case LOGGING_PIN:
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

uint8_t associated_port_clock(uint8_t port)
{
    switch(port)
    {
        case 0:     return APB2_REG->bits.IOPA_EN;
        case 1:     return APB2_REG->bits.IOPB_EN;
        case 2:     return APB2_REG->bits.IOPC_EN;
        default:    return 0;
    }
}