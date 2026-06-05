
#add_library(hal STATIC
#    src/hal.c
#)

#target_include_directories(hal PUBLIC include)

if(PLATFORM_ESP32)
    set(hal_src "esp32/esp32.c")
elseif(PLATFORM_MOCK)
    set(hal_src "src/hal.c")
endif()

idf_component_register(
    SRCS ${hal_src}
    INCLUDE_DIRS "include"
    REQUIRES esp_timer esp_driver_gpio
)