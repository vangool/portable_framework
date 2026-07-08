.syntax unified
.cpu cortex-m3
.thumb

.global Reset_Handler
.global SysTick_Handler
.global _estack

/* Vector table */
.section .isr_vector,"a",%progbits
.align 2
__isr_vector:
    .word _estack          /* Top of Stack (0x20005000) */
    .word Reset_Handler    /* Reset Handler */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0

    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word SysTick_Handler

    .word WWDG_IRQHandler          /* 16: Window Watchdog Interrupt */
    .word PVD_IRQHandler           /* 17: PVD through EXTI Line detection */
    .word TAMPER_IRQHandler        /* 18: Tamper Interrupt */
    .word RTC_IRQHandler           /* 19: RTC Global Interrupt */
    .word FLASH_IRQHandler         /* 20: Flash Global Interrupt */
    .word RCC_IRQHandler           /* 21: RCC Global Interrupt */
    .word EXTI0_IRQHandler         /* 22: EXTI Line 0 Interrupt (Offset 0x58) */
    .word EXTI1_IRQHandler         /* 23: EXTI Line 1 Interrupt */

    .word EXTI2_IRQHandler         /* 24: EXTI Line 2 Interrupt */
    .word EXTI3_IRQHandler         /* 25: EXTI Line 3 Interrupt */
    .word EXTI4_IRQHandler         /* 26: EXTI Line 4 Interrupt */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0                        /* 31 */

    .word 0                        /* 32 */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word EXTI9_5_IRQHandler        /* 39: EXTI Lines 5 - 9 Interrupt */

    .word 0                         /* 40 */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0                         /* 47 */

    .word 0                         /* 48 */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0                         /* 55 */

    .word EXTI15_10_IRQHandler      /* 56: EXTI Lines 10 - 15 Interrupt */
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0
    .word 0                         /* 63 */

/* Reset handler */
.section .text.Reset_Handler
.type Reset_Handler, %function
.type SysTick_Handler, %function

Reset_Handler:
    /* 1. Initialize the Main Stack Pointer (MSP) */
    ldr r0, =_estack
    msr msp, r0
    isb

    /* 2. Copy the .data section from FLASH to RAM */
    ldr r0, =_etext        /* Source address in FLASH */
    ldr r1, =_sdata        /* Destination start address in RAM */
    ldr r2, =_edata        /* Destination end address in RAM */

copy_data_loop:
    cmp r1, r2
    bhs zero_bss_init      /* If destination source >= end, proceed to BSS */
    ldr r3, [r0], #4       /* Load word from Flash, increment Flash pointer */
    str r3, [r1], #4       /* Store word to RAM, increment RAM pointer */
    b copy_data_loop

zero_bss_init:
    /* 3. Initialize the .bss section to 0 in RAM */
    ldr r1, =_sbss        /* BSS start address in RAM */
    ldr r2, =_ebss        /* BSS end address in RAM */
    movs r3, #0           /* Value to write (0) */

zero_bss_loop:
    cmp r1, r2
    bhs launch_main        /* If BSS pointer >= end, proceed to main */
    str r3, [r1], #4       /* Store 0 to RAM, increment pointer */
    b zero_bss_loop

launch_main:
    /* 4. Branch to your main function */
    bl system_init
    bl main

_hang:
    b _hang


/* Define a default fallback for SysTick and other exceptions */
.section .text.Default_Handler, "ax", %progbits
Default_Handler:
    b Default_Handler

/* Map SysTick_Handler weakly to the Default_Handler loop */
/* If you define void SysTick_Handler(void) in C, it will override this! */
.weak SysTick_Handler
.thumb_set SysTick_Handler, Default_Handler