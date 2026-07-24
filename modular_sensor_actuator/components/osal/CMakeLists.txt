
if($ENV{PLATFORM_ESP32})
    idf_component_register(
        SRCS "freertos/freertos.c"
        INCLUDE_DIRS "include"
    )
elseif($ENV{PLATFORM_STM32})
    add_library(my_os STATIC
        my_os/src/scheduler.c
        # my_os/src/osal_linkedlist.c
        my_os/src/osal_queue.c
        my_os/src/my_os.c
    )

    target_include_directories(my_os PUBLIC
        include
        my_os/include
        my_os/src
    )

    target_link_libraries(my_os PRIVATE
        my_hal
        my_service
    )

endif()