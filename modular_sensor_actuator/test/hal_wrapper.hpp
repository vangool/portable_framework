#ifndef __HAL_WRAPPER_HPP__
#define __HAL_WRAPPER_HPP__

#include <stdint.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../components/my_hal/include/hal.h"

class MockHAL {
public:
    
    MOCK_METHOD(hal_status_t, hal_gpio_set_direction, (uint8_t pin, uint8_t direction));
    MOCK_METHOD(hal_status_t, hal_gpio_set_level, (uint8_t pin, uint8_t level));
    MOCK_METHOD(int8_t, hal_gpio_get_level, (uint8_t pin));
    MOCK_METHOD(hal_status_t, hal_gpio_set_intr_type, (uint8_t pin, hal_intr_type_t type));
    MOCK_METHOD(hal_status_t, hal_gpio_isr_handler_add, (uint8_t gpio_pin, void* isr_handler, void* args));
    MOCK_METHOD(void, hal_rom_delay_us, (uint32_t delay_us));
    MOCK_METHOD(int64_t, hal_timer_get_time, ());
    
    static MockHAL* instance;

    static hal_status_t gpio_set_direction_trampoline(uint8_t pin, uint8_t direction) {
        //auto* mock = static_cast<MockHAL*>(pin, direction);
        return instance->hal_gpio_set_direction(pin, direction);
    }

    static hal_status_t gpio_set_level_trampoline(uint8_t pin, uint8_t level) {
        return instance->hal_gpio_set_level(pin, level);
    }
    static int8_t gpio_get_level_trampoline(uint8_t pin) {
        return instance->hal_gpio_get_level(pin);
    }
    static hal_status_t gpio_set_intr_type_trampoline(uint8_t pin, hal_intr_type_t type) {
        return instance->hal_gpio_set_intr_type(pin, type);
    }
    static hal_status_t gpio_isr_handler_add_trampoline(uint8_t pin, void* isr_handler, void* args) {
        return instance->hal_gpio_isr_handler_add(pin, isr_handler, args);
    }
    static void rom_delay_us_trampoline(uint32_t delay_us) {
        instance->hal_rom_delay_us(delay_us);
    }
    static int64_t timer_get_time_trampoline() {
        return instance->hal_timer_get_time();
    }

};

MockHAL* MockHAL::instance = nullptr;
#endif //__HAL_WRAPPER_HPP__