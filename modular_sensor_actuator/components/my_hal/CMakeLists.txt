message("
    
    >>> COMPILING HAL CMAKE <<<
    
    ")

if($ENV{PLATFORM_ESP32})
    idf_component_register(
        SRCS "esp32/esp32.c" "esp32/esp32_uart.c"
        INCLUDE_DIRS "include"
        REQUIRES esp_timer esp_driver_gpio esp_driver_uart
    )

elseif(1 OR $ENV{PLATFORM_STM32})

    add_library(my_hal STATIC
        stm32/stm32.c
        stm32/stm32f103/stm32f103.c
    )

    target_include_directories(my_hal PUBLIC 
        include
        stm32
        stm32/stm32f103
    )


elseif(PLATFORM_MOCK)
    set(hal_src "src/hal.c" "src/hal_uart.c")
endif()

