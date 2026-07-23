message("

	>>> COMPILING MAIN CMAKE <<<

")
if($ENV{PLATFORM_ESP32})
	idf_component_register(
		SRCS "main.c" "ultrasonic/US100.c"
		INCLUDE_DIRS "."
		REQUIRES my_hal
	)

	target_compile_definitions(${COMPONENT_LIB} PRIVATE
		HAL_GPIO_IRAM_ATTR=IRAM_ATTR
		PRINTF_INCLUDE_CONFIG_H
	)

	target_compile_options(${COMPONENT_LIB} PRIVATE
		-Wall
		-Wextra
		-Werror
		"-Werror=unused-function"
	)
elseif($ENV{PLATFORM_STM32})

	add_executable(firmware.elf
	    main.c
    	../components/my_hal/stm32/stm32f103/stm32f103.s
	    $<TARGET_OBJECTS:my_hal>
    	$<TARGET_OBJECTS:my_os>
		$<TARGET_OBJECTS:my_service>
	)

	target_link_libraries(firmware.elf PRIVATE
    	my_hal
		my_os
		my_service
	)

	target_compile_definitions(my_hal PRIVATE
		PRINTF_INCLUDE_CONFIG_H
	)

	target_compile_options(firmware.elf PRIVATE 
		-mcpu=cortex-m3 
		-mthumb
		-O0
		-Og
		-g3 
		-ffreestanding 
		-nostartfiles
		)

	target_link_options(firmware.elf PRIVATE 
    	-mcpu=cortex-m3
	    -mthumb
    	-T${CMAKE_SOURCE_DIR}/components/my_hal/stm32/stm32f103/stm32f103.ld
	    -nostartfiles
    	-Wl,--gc-sections     # Safely remove actual dead code
	    -Wl,-Map=firmware.map # Generates a map file so you can check addresses!
    	)
endif()