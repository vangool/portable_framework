idf_component_register(
	SRCS "main.c" "ultrasonic/US100.c"
	INCLUDE_DIRS "."
)

target_compile_definitions(${COMPONENT_LIB} PRIVATE
    HAL_GPIO_IRAM_ATTR=IRAM_ATTR
)

target_compile_options(${COMPONENT_LIB} PRIVATE
	-Wall
	-Wextra
	-Werror
	"-Werror=unused-function"
)