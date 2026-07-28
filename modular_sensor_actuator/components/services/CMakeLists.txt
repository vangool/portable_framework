if($ENV{PLATFORM_ESP32})

elseif($ENV{PLATFORM_STM32})

    add_library(my_service STATIC
        watchdog/watchdog.c
    )

    target_include_directories(my_service PUBLIC 
        watchdog
    )

    target_link_libraries(my_service PRIVATE
        my_hal
    )

elseif(PLATFORM_MOCK)

endif()

