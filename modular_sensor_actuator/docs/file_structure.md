### 📂 Repository File Structure

```text
├── main/                           # Application & Core Driver Layer
│   ├── main.c                      # Portable, hardware-agnostic application entry point
│   └── ultrasonic/                 # Event-driven US100 ultrasonic sensor driver
│       ├── ultrasonic.c
│       └── ultrasonic.h
│
├── components/                     # Platform abstraction subsystems
│   ├── my_hal/                     # Hardware Abstraction Layer (HAL)
│   │   ├── include/                # Platform-independent HAL interfaces
│   │   │   └── hal.h
│   │   ├── src/                    # Shared HAL wrapper implementations
│   │   │   └── hal.c               # Default stub implementations for host-side testing
│   │   │
│   │   ├── esp32/                  # ESP32 HAL backend (ESP-IDF)
│   │   │   ├── esp32.c / .h        # GPIO, interrupt, and platform initialization
│   │   │   └── esp32_uart.c / .h   # ESP-IDF UART implementation
│   │   │
│   │   └── stm32/                  # STM32 HAL backend
│   │       ├── addr.h              # Peripheral base address definitions
│   │       ├── stm32_func.h        # Shared STM32 abstraction interfaces
│   │       ├── stm32.c / .h        # Common STM32 peripheral routing
│   │       ├── stm32_uart.c / .h   # Register-level UART implementation
│   │       └── stm32f103/          # STM32F103 device-specific implementation
│   │           ├── cortexm3.c / .h # Cortex-M3 core support (SysTick, NVIC, core utilities)
│   │           ├── stm32f103_intr.c / .h # EXTI and NVIC interrupt configuration
│   │           ├── stm32f103.c / .h # Device-specific peripheral configuration
│   │           ├── stm32f103.ld    # Linker script
│   │           └── stm32f103.s     # Startup code and exception vector table
│   │
│   └── osal/                       # Operating System Abstraction Layer (OSAL)
│       ├── include/                # Platform-independent OS interfaces
│       │   └── osal.h
│       ├── src/                    # Shared OSAL wrapper implementations
│       │   └── osal.c
│       │
│       ├── freertos/               # ESP32 FreeRTOS backend
│       │   └── freertos.c / .h     # FreeRTOS task, delay, and queue bindings
│       │
│       └── my_os/                  # Custom STM32 scheduler backend
│           ├── include/
│           │   ├── my_os.h         # Public kernel API exposed through the OSAL
│           │   └── scheduler.h     # Internal scheduler interfaces and task management
│           └── src/
│               ├── my_os.c         # OSAL backend implementation for the custom scheduler
│               └── scheduler.c     # Preemptive scheduler and context-switch management
│
├── test/                           # Host-side verification suite (x86_64)
│   ├── hal_wrapper.hpp             # GoogleMock proxy for HAL interfaces
│   ├── test_keys.hpp               # Test constants and mock expectations
│   └── ultrasonic_test.cpp         # Unit tests for the US100 driver
│
└── build_script.sh                 # Unified build, flash, and deployment automation
```