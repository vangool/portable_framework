
if(PLATFORM_ESP32)
    set(hal_src "esp32/esp32.c" "esp32/esp32_uart.c")
elseif(PLATFORM_MOCK)
    set(hal_src "src/hal.c" "src/hal_uart.c")
endif()

idf_component_register(
    SRCS ${hal_src}
    INCLUDE_DIRS "include"
    REQUIRES esp_timer esp_driver_gpio esp_driver_uart
)