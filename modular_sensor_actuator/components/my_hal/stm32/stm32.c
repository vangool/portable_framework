#include "addr.h"
#include "stm32.h"
#include "stm32_func.h"

hal_gpio_set_direction          hal_gpio_set_direction_func         = stm32_gpio_set_direction;
hal_gpio_set_level              hal_gpio_set_level_func             = stm32_gpio_set_level;
hal_gpio_get_level              hal_gpio_get_level_func             = stm32_gpio_get_level;
hal_gpio_set_intr_type          hal_gpio_set_intr_type_func         = stm32_gpio_set_intr_type;
hal_gpio_isr_handler_add        hal_gpio_isr_handler_add_func       = stm32_gpio_isr_handler_add;
hal_rom_delay_us                hal_rom_delay_us_func               = stm32_rom_delay_us;
hal_timer_get_time              hal_timer_get_time_func             = stm32_timer_get_time;
hal_gpio_install_isr_service    hal_gpio_install_isr_service_func   = stm32_gpio_install_isr_service;
hal_log_info                    hal_log                             = stm32_log_info;

volatile uint32_t system_ms = 0;

void Default_Handler_Loop() { while (1) {} }

void NMI_Handler(void) {}
void HardFault_Handler(uint32_t *stack) 
{   
    volatile uint32_t r0  = stack[0];
    volatile uint32_t r1  = stack[1];
    volatile uint32_t r2  = stack[2];
    volatile uint32_t r3  = stack[3];
    volatile uint32_t r12 = stack[4];
    volatile uint32_t lr  = stack[5];
    volatile uint32_t pc  = stack[6];
    volatile uint32_t psr = stack[7]; 
    while (1) {}
}
void MemManage_Handler(void) { Default_Handler_Loop(); }
void BusFault_Handler(void) { Default_Handler_Loop(); }
void UsageFault_Handler(void) { Default_Handler_Loop(); }

void SVC_Handler(void) {}
void DebugMon_Handler(void) {}
void PendSV_Handler(void) {}

void SysTick_Handler(void)
{
    system_ms++;
}

hal_status_t stm32_gpio_set_direction(uint8_t pin, hal_gpio_config_t direction)
{
    hal_status_t ret = verify_pin(pin);
    if(ret != HAL_STATUS_OK)
        return ret;

    uint8_t port = (pin >> 6) & 3;
    volatile GPIO_BLOCK* base_addr = get_gpio_port_block_func(port);
    if(base_addr == 0)
    {
        return HAL_ERROR_INVALID_ARG;
    }

    uint8_t gpio_config = translate_gpio_mode_func(direction);
    pin &= 0xF;

    if(pin >= 8)
    {
        base_addr->CRH.all &= ~(0xF << ((pin - 8) * 4));
        base_addr->CRH.all |= (gpio_config << ((pin - 8) * 4));
    }
    else
    {
        base_addr->CRL.all &= ~(0xF << (pin * 4));
        base_addr->CRL.all |= (gpio_config << (pin * 4));
    }

    return HAL_STATUS_OK;
}

hal_status_t stm32_gpio_set_level(uint8_t pin, hal_gpio_level_t level)
{
    if (level > HAL_GPIO_HIGH)
        return HAL_ERROR_INVALID_ARG;

    hal_status_t ret = verify_pin(pin);
    if(ret != HAL_STATUS_OK)
        return ret;

    uint8_t port = (pin >> 6) & 3;
    volatile GPIO_BLOCK* base_addr = get_gpio_port_block_func(port);
    if(base_addr == 0)
        return HAL_ERROR_INVALID_ARG;

    pin &= 63;
    uint8_t shift_amt = level ? pin : pin + 16;

    base_addr->BSRR.all = (1 << shift_amt);

    return HAL_STATUS_OK;
}

int8_t stm32_gpio_get_level(uint8_t pin)
{
    hal_status_t ret = verify_pin(pin);
    if(ret != HAL_STATUS_OK)
        return -1;

    uint8_t port = (pin >> 6) & 3;
    volatile GPIO_BLOCK* base_addr = get_gpio_port_block_func(port);

    if(base_addr == 0)
        return -1;

    pin &= 63;

    return (base_addr->IDR.all >> pin) & 1;
}

void stm32_log_init(void)
{
    APB2_REG->bits.USART1_EN = 1;
    volatile GPIO_BLOCK* gpioa_addr = get_gpio_port_block_func(0);
    gpioa_addr->CRH.all &= ~(0xF << 4);
    gpioa_addr->CRH.all |= (0xB << 4);

    // Assuming a 72 MHz system clock: 72,000,000 / 9600 = 7500
    // 7500 in hex is 0x1D4C
    USART1_REG->BRR = 0x1D4C; 

    // USART_CR1: UE (USART Enable) is Bit 13, TE (Transmitter Enable) is Bit 3
    USART1_REG->CR1.bits.UE = 1;
    USART1_REG->CR1.bits.TE = 1;
    
    
    

}

void stm32_clock_init(void) 
{
    RCC_CR_REG->bits.HSION = 1;
    while (!(RCC_CR_REG->bits.HSIRDY)) {}

    // Pointer to FLASH_ACR (Flash Access Control Register) at 0x40022000
    volatile uint32_t* flash_acr = (volatile uint32_t*)0x40022000;

    RCC_CR_REG->bits.HSEON = 1;
    while (!(RCC_CR_REG->bits.HSERDY));

    FLASH_BLOCK_REG->FLASH_ACR = 0b1010;

    // Configure the prescalers for AHB, APB1, APB2
    RCC_CFGR_REG->bits.HPRE = 0;
    RCC_CFGR_REG->bits.PPRE1 = 4;
    RCC_CFGR_REG->bits.PPRE2 = 0;

    // Configure the Phase Locked Loop (PLL)
    RCC_CFGR_REG->bits.PLLSRC = 1; 
    RCC_CFGR_REG->bits.PLLMUL = 7;
    RCC_CR_REG->bits.PLLON = 1;
    while (!(RCC_CR_REG->bits.PLLRDY));    

    RCC_CFGR_REG->bits.SW = 2;

    while (RCC_CFGR_REG->bits.SWS != 2);
    while (!(RCC_CR_REG->bits.PLLRDY)); 

    APB2_REG->bits.IOPA_EN = 1;
    APB2_REG->bits.IOPB_EN = 1;
    APB2_REG->bits.IOPC_EN = 1;

    STK_REG->STK_LOAD = (CLK_SPEED / 1000) - 1;
    STK_REG->STK_VAL = 0;
    STK_REG->STK_CTRL = (1 << 2) | (1 << 1) | (1 << 0);

    DEMCR_REG->bits.TRC_ARCHICT_EN = 1;
    DWT_BLOCK_REG->CYCCNT = 0;
    DWT_BLOCK_REG->CTRL.bits.CYCCNT_EN = 1;

    stm32_log_init();
}


hal_status_t stm32_gpio_set_intr_type(uint8_t pin, hal_intr_type_t type)
{
    // clock_init();
    stm32_clock_init();
    return HAL_ERROR_DEFAULT_IMPLEMENTATION;
}

hal_status_t stm32_gpio_isr_handler_add(uint8_t gpio_pin, void* isr_handler, void* args)
{
    return HAL_ERROR_DEFAULT_IMPLEMENTATION;
}

void stm32_rom_delay_us(uint32_t delay_us)
{
    if (delay_us == 0) return;

    uint32_t start = DWT_BLOCK_REG->CYCCNT;
    uint32_t cycles = CLK_SPEED / 1000000;

    while((DWT_BLOCK_REG->CYCCNT - start) < cycles);
}

extern uint32_t _sidata, _sdata, _edata;
extern uint32_t _sbss, _ebss;


int64_t stm32_timer_get_time()
{
    return system_ms;
}

hal_status_t stm32_gpio_install_isr_service(hal_intr_flag_t flag)
{
    return HAL_ERROR_DEFAULT_IMPLEMENTATION;
}

void stm32_log_info(hal_log_level_t level, const char *tag, const char *fmt, ...)
{
    
}

void stm32_log_simple(const char* msg)
{

    // 1. Enable Clocks for GPIOA and USART1
    // RCC_APB2ENR: GPIOA is Bit 2, USART1 is Bit 14
    // RCC->APB2ENR |= (1 << 2) | (1 << 14);


    // 2. Configure PA9 (TX) as Alternate Function Push-Pull (Speed 50MHz)
    // GPIOA->CRH controls pins 8-15. PA9 is controlled by bits 4-7.
    // Clear bits 4, 5, 6, 7 first
    // GPIOA->CRH &= ~(0xF << 4);
    // Set MODE9 to 11 (Output 50MHz) and CNF9 to 10 (Alt Function Push-Pull) -> 1011b (0xB)
    // GPIOA->CRH |= (0xB << 4);



    // 3. Configure Baud Rate to 9600
    // Formula: BRR = SystemClock / BaudRate
    // (If using default 8 MHz clock: 8,000,000 / 9600 = 833.33 -> 833 = 0x341)

    // 4. Enable USART and Transmitter
    
    
    for(int i = 0; *(msg + i) != '\0'; i++)
    {
        while (!(USART1_REG->SR & (1 << 7)));
        USART1_REG->DR = ( *(msg + i) & 0xFF);
        // USART1_DR = ( *(msg + i) & 0xFF);
    }

    while (!(USART1_REG->SR & (1 << 7)));
    USART1_REG->DR = ( '\n' & 0xFF);


}