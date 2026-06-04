//#include <stdint.h> with these:
typedef unsigned char          uint8_t;
typedef unsigned short         uint16_t;
typedef unsigned int           uint32_t;
typedef unsigned long long     uint64_t;

typedef signed char            int8_t;
typedef signed short           int16_t;
typedef signed int             int32_t;
typedef signed long long       int64_t;

typedef unsigned int           uintptr_t;

// Base addresses
#define RCC_BASE   0x40023800UL
#define GPIOA_BASE 0x40020000UL

// RCC registers
#define RCC_AHB1ENR  (*(volatile uint32_t *)(RCC_BASE + 0x30))

// GPIOA registers
#define GPIOA_MODER  (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_ODR    (*(volatile uint32_t *)(GPIOA_BASE + 0x14))

// Simple busy-wait delay
void delay(volatile uint32_t count) {
    while (count--);
}

// Interrupt vector table + reset handler
void Reset_Handler(void);

__attribute__((section(".isr_vector")))
void (*const vector_table[])(void) = {
    (void (*)(void))0x20020000,  // initial stack pointer — top of RAM
    Reset_Handler,               // reset handler
};

void Reset_Handler(void) {
    // 1. Enable GPIOA clock — set bit 0 in RCC_AHB1ENR
    // your code
    RCC_AHB1ENR |= (1 << 0);

    // 2. Set PA5 as output — MODER bits 11:10 = 01
    // your code
    GPIOA_MODER &= ~(3<<10);
    GPIOA_MODER |= (1<<10);



    while (1) {
        // 3. Set PA5 HIGH — use GPIOA_ODR
        // your code
        GPIOA_ODR |= (1<<5);

        delay(1000000);

        // 4. Set PA5 LOW
        // your code
        GPIOA_ODR &= ~(1 << 5);
        
        delay(1000000);
    }
}