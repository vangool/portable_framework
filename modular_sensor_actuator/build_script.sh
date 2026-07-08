#!/bin/bash

PLATFORM=$1

if [ -z "$PLATFORM" ]; then
	echo "ERROR: Missing platform argument"
	echo "Usage: ./build_script.sh [ESP32|STM32|MOCK]"
	exit 1
fi

echo "Starting build pipeline for target: $PLATFORM..."

if [ "$PLATFORM" == "ESP32" ]; then
	rm -rf build

	idf.py -DTARGET_PLATFORM=ESP32 build
elif [ "$PLATFORM" == "STM32" ]; then
	rm -rf build
	cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=arm-none-eabi.cmake -DTARGET_PLATFORM=STM32
	cmake --build build --verbose
	arm-none-eabi-objcopy -O binary build/main/firmware.elf build/main/firmware.bin
	st-flash write build/main/firmware.bin 0x8000000 && st-flash reset
elif [ "$PLATFORM" == "MOCK" ]; then
	cmake -DTARGET_PLATFORM=MOCK -B build
	cmake --build build
else
	echo "ERROR: Unknown platform: $PLATFORM"
	exit 1
fi
