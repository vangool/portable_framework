cmake_minimum_required(VERSION 3.16)

set(ENV{PLATFORM_ESP32} 0)
set(ENV{PLATFORM_STM32} 0)
set(ENV{PLATFORM_MOCK}  0)


if(TARGET_PLATFORM STREQUAL "ESP32")
    message("
    --- Auto-Detected ESP32 platform ---
    ")
    set(ENV{PLATFORM_ESP32} 1)

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(embedded_libraries)

elseif(TARGET_PLATFORM STREQUAL "STM32")
    message("
    --- Auto-Detected STM32 platform ---
    ")

    enable_language(ASM)

    set(ENV{PLATFORM_STM32} 1)

    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    project(embedded_libraries)

    add_subdirectory(components/my_hal)
    #add_subdirectory(components/osal)
    add_subdirectory(main)

endif()



