if($ENV{PLATFORM_ESP32})
    idf_component_register(
        SRC_DIRS "esp32"
        INCLUDE_DIRS "include"
        REQUIRES esp_timer esp_driver_gpio esp_driver_uart
    )

elseif($ENV{PLATFORM_STM32})

    add_library(my_hal STATIC
        src/hal_func.c
        stm32/stm32.c
        stm32/stm32_uart/stm32_uart.c
        stm32/stm32f103/stm32f103.c
        stm32/stm32f103/stm32f103_intr.c
        stm32/stm32f103_watchdog/stm32f103_watchdog.c
        utils/printf/printf.c
    )

    target_include_directories(my_hal PUBLIC 
        include
        src
        stm32
        stm32/stm32_uart
        stm32/stm32f103
        stm32/stm32f103_watchdog
        utils/printf
    )

    target_link_libraries(my_hal PRIVATE
        my_service
    )


elseif(PLATFORM_MOCK)
    set(hal_src "src/hal.c" "src/hal_uart.c")
endif()

