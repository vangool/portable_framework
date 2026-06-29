#include "stm32_uart.h"
#include "stm32_func.h"

hal_uart_init hal_uart_init_func = stm32_uart_init;
hal_uart_receive hal_uart_receive_func = stm32_uart_receive;
hal_uart_transmit hal_uart_transmit_func = stm32_uart_transmit;

hal_status_t stm32_uart_init(hal_uart_config_t* config, uint8_t transmit_pin, uint8_t receive_pin)
{
    hal_status_t ret = stm32fn_is_TX_pin_func(transmit_pin) | stm32fn_is_RX_pin_func(receive_pin);
    if(ret != HAL_STATUS_OK && receive_pin - transmit_pin == 1)
    {
        return HAL_ERROR_INVALID_ARG;
    }

    uint8_t port = (transmit_pin >> 6) & 3;
    volatile GPIO_BLOCK* port_addr = stm32fn_get_gpio_port_block_func(port);
    if(transmit_pin >= 8)
    {
        port_addr->CRH.all &= ~(0xF << ((transmit_pin - 8) * 4));
        port_addr->CRH.all |= (0xB << ((transmit_pin - 8) * 4));
    }
    else
    {
        port_addr->CRL.all &= ~(0xF << (transmit_pin * 4));
        port_addr->CRL.all |= (0xB << (transmit_pin * 4));
    }

    int apb2_bd_divisor = APB2_CLK_SPEED / config->baud_rate;
    int apb1_bd_divisor = APB1_CLK_SPEED / config->baud_rate;
    switch(transmit_pin)
    {
        case STM_PIN_PA09:
            APB2_REG->bits.USART1_EN = 1;
            USART1_REG->BRR = apb2_bd_divisor;
            USART1_REG->CR1.bits.UE = 1;
            USART1_REG->CR1.bits.TE = 1;
            USART1_REG->CR1.bits.RE = 1;
            break;
        case STM_PIN_PA02:
            APB1_REG->bits.USART2_EN = 1;
            USART2_REG->BRR = apb1_bd_divisor;
            USART2_REG->CR1.bits.UE = 1;
            USART2_REG->CR1.bits.TE = 1;
            USART2_REG->CR1.bits.RE = 1;
            break;
        case STM_PIN_PB10:
            APB1_REG->bits.USART3_EN = 1;
            USART3_REG->BRR = apb1_bd_divisor;
            USART3_REG->CR1.bits.UE = 1;
            USART3_REG->CR1.bits.TE = 1;
            USART3_REG->CR1.bits.RE = 1;
            break;

    }
    return HAL_STATUS_OK;
}

void stm32_uart_receive(hal_uart_port_t port)
{
    volatile USART_BLOCK* USART_REG;
    switch (port)
    {
        case HAL_UART_PORT_0:
            USART_REG = USART1_REG;
            break;
        case HAL_UART_PORT_1:
            USART_REG = USART2_REG;
            break;
        case HAL_UART_PORT_2:
            USART_REG = USART3_REG;
            break;
    
        default:
            return;
    }

    while(!(USART_REG->SR.bits.RXNE)){}

    uint8_t rx_byte = 0; 
    rx_byte = (uint8_t)USART_REG->DR & 0xFF;
}

void stm32_uart_transmit(const char* msg, hal_uart_port_t port)
{
    hal_gpio_set_level_func(STM_PIN_PC13, HAL_GPIO_HIGH);

    volatile USART_BLOCK* USART_REG;
    switch (port)
    {
        case HAL_UART_PORT_0:
            USART_REG = USART1_REG;
            break;
        case HAL_UART_PORT_1:
            USART_REG = USART2_REG;
            break;
        case HAL_UART_PORT_2:
            USART_REG = USART3_REG;
            break;
    
        default:
            return;
    }

    for(int i = 0; *(msg + i) != '\0' && i < 20; i++)
    {
        while (!(USART_REG->SR.bits.TXE));
        USART_REG->DR = ( *(msg + i) & 0xFF);
    }

    while (!(USART_REG->SR.bits.TC));

    hal_gpio_set_level_func(STM_PIN_PC13, HAL_GPIO_LOW);

}