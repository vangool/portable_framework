# set(CMAKE_SYSTEM_NAME Generic)
# set(CMAKE_SYSTEM_PROCESSOR arm)

# set(TOOLCHAIN_PREFIX arm-none-eabi)

# set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
# set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
# set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
# set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)

# set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# Force GNU assembler mode
# set(CMAKE_ASM_FLAGS "-x assembler-with-cpp")

# CPU flags (Cortex-M3)
# set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")

#set(CMAKE_C_FLAGS "${CPU_FLAGS} -O2 -g -ffreestanding -nostdlib")
# set(CMAKE_C_FLAGS "${CPU_FLAGS} -O2 -g -ffreestanding")
# set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} ${CPU_FLAGS}")
# set(CMAKE_EXE_LINKER_FLAGS "${CPU_FLAGS}")

#===================

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(TOOLCHAIN_PREFIX arm-none-eabi)

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}-size)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# CPU flags
set(CPU_FLAGS "-mcpu=cortex-m3 -mthumb")

# IMPORTANT: use CACHE STRING for flags
set(CMAKE_C_FLAGS_INIT "${CPU_FLAGS} -O2 -g -ffreestanding")
set(CMAKE_ASM_FLAGS_INIT "${CPU_FLAGS} -x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS_INIT "${CPU_FLAGS}")