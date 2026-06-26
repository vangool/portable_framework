cmake_minimum_required(VERSION 3.16)

project(unit_tests)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(ultrasonic_tests
    ultrasonic_test.cpp
    ../main/ultrasonic/US100.c
    ../components/my_hal/src/hal.c
    ../components/osal/src/osal.c
)

target_include_directories(ultrasonic_tests PRIVATE
    ../main
)

target_link_libraries(ultrasonic_tests
    gtest
    gtest_main
    gmock
    gmock_main
)

include(GoogleTest)
gtest_discover_tests(ultrasonic_tests)
