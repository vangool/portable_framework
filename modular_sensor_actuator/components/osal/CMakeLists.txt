#add_library(hal STATIC
#    src/hal.c
#)

#target_include_directories(hal PUBLIC include)

#if(PLATFORM_ESP32)
#    target_sources(hal PRIVATE esp32/esp32.c)
#elseif(PLATFORM_MOCK)
#    target_sources(hal PRIVATE mock/mock.c)
#endif()

idf_component_register(
    SRCS "freertos/freertos.c"
    INCLUDE_DIRS "include"
)