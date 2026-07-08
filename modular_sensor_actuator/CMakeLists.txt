cmake_minimum_required(VERSION 3.16)

set(ENV{PLATFORM_ESP32} 0)
set(ENV{PLATFORM_STM32} 0)
set(ENV{PLATFORM_MOCK}  0)

message("CMAKE INFO: The value of TARGET_PLAFORM is: ${TARGET_PLATFORM}")

if(TARGET_PLATFORM STREQUAL "ESP32")
    set(ENV{PLATFORM_ESP32} 1)

    include($ENV{IDF_PATH}/tools/cmake/project.cmake)
    project(embedded_libraries)

elseif(TARGET_PLATFORM STREQUAL "STM32")
    enable_language(ASM)

    set(ENV{PLATFORM_STM32} 1)

    set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
    project(embedded_libraries)

    add_subdirectory(components/my_hal)
    #add_subdirectory(components/osal)
    add_subdirectory(main)

endif()



