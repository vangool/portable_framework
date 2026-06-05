idf_component_register(
	SRCS "main.c" "ultrasonic/US100.c"
	INCLUDE_DIRS "."
	#COMPONENT_COMPILE_DEFINITIONS "HAL_GPIO_IRAM_ATTR=IRAM_ATTR"
)

target_compile_definitions(${COMPONENT_LIB}
    PRIVATE HAL_GPIO_IRAM_ATTR=IRAM_ATTR
)