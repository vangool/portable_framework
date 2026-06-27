
if($ENV{PLATFORM_ESP32})
    idf_component_register(
        SRCS "freertos/freertos.c"
        INCLUDE_DIRS "include"
    )
endif()