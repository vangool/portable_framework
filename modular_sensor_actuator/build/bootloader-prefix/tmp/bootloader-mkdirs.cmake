# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/mike/CodeProjects/ESP32/esp-idf/components/bootloader/subproject"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/tmp"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/src/bootloader-stamp"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/src"
  "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/mike/CodeProjects/Portfolio/embedded/modular_sensor_actuator/build/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
