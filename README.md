# Bare-Metal CAN & UART Driver Architecture

A zero-dependency, bare-metal C device driver stack for the **STM32F446xx (ARM Cortex-M4)** microcontroller. This project implements register-level configurations for the **bxCAN1** and **USART2** peripherals entirely from scratch, utilizing custom linker scripts, localized startup initialization, and an automated **Pytest** Hardware-In-the-Loop (HIL) testing harness.

🌐 **Project Documentation & Deep Dive:** [manojg24.github.io/can-driver.html](https://manojg24.github.io/can-driver.html)

---

## Technical Highlights
* **Zero-HAL Dependency:** Bypasses all standard abstraction layers (HAL/LL/CubeMX) using explicit `-nostartfiles` and `-nodefaultlibs` compilation flags.
* **Custom Custom Memory Bootstrapping:** Implements a localized vector table referencing linker symbols (`_estack`), featuring manual startup routines to copy the `.data` segment from Flash to SRAM and zero out the `.bss` section.
* **FPU Bypass:** Configures the Coprocessor Access Control Register (`CPACR` at `0xE000ED88UL`) to explicitly unmask the hardware Floating Point Unit (FPU) out of reset.
* **Bit-Timing Accuracy:** Hand-calculates precise baud rate dividers (`0x8B` for 115,200 baud USART) and discrete Time Segment segments for bxCAN loopback mode.

---

## File Structure

├── main.c              # Core register definitions, memory setup, and peripheral drivers
├── stm32f446re.ld      # Linker script managing physical Flash (512KB) and RAM (128KB) layouts
├── Makefile            # Direct arm-none-eabi compilation and OpenOCD flash rules
└── test_stability.py   # Pytest/PySerial HIL validation script (Under work)


Compilation and deployment

# Clean previous binary targets
make clean

# Compile source files and optimize link mappings
make

# Flash the target image to physical Flash address 0x08000000
make flash
