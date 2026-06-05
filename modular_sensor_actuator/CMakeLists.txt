cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(embedded_libraries)

option(PLATFORM_ESP32 "Build for ESP32" ON)
option(PLATFORM_MOCK  "Build for MOCK " OFF)

#add_subdirectory(components/hal)
#add_subdirectory(components/osal)

#target_link_libraries(PRIVATE hal osal)
#target_link_libraries(PRIVATE hal)
