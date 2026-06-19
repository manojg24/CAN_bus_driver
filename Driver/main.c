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

extern uint32_t _sidata; // Start of .data in FLASH
extern uint32_t _sdata;  // Start of .data in RAM
extern uint32_t _edata;  // End of .data in RAM
extern uint32_t _sbss;   // Start of .bss in RAM
extern uint32_t _ebss;   // End of .bss in RAM
extern void _estack(void); // Top of stack from linker script

// Base addresses
#define RCC_BASE    0x40023800UL
#define GPIOA_BASE  0x40020000UL
#define CAN1_BASE   0x40006400UL

// RCC registers
#define RCC_AHB1ENR  (*(volatile uint32_t *)(RCC_BASE + 0x30))
#define RCC_APB1ENR  (*(volatile uint32_t *)(RCC_BASE + 0x40))

// CAN registers
#define CAN_MCR      (*(volatile uint32_t *)(CAN1_BASE + 0x000))
#define CAN_MSR      (*(volatile uint32_t *)(CAN1_BASE + 0x004))
#define CAN_BTR      (*(volatile uint32_t *)(CAN1_BASE + 0x01C))
#define CAN_TSR      (*(volatile uint32_t *)(CAN1_BASE + 0x008))

// TX mailbox 0
#define CAN_TI0R     (*(volatile uint32_t *)(CAN1_BASE + 0x180))
#define CAN_TDT0R    (*(volatile uint32_t *)(CAN1_BASE + 0x184))
#define CAN_TDL0R    (*(volatile uint32_t *)(CAN1_BASE + 0x188))
#define CAN_TDH0R    (*(volatile uint32_t *)(CAN1_BASE + 0x18C))

// RX FIFO 0
#define CAN_RF0R     (*(volatile uint32_t *)(CAN1_BASE + 0x00C))
#define CAN_RI0R     (*(volatile uint32_t *)(CAN1_BASE + 0x1B0))
#define CAN_RDT0R    (*(volatile uint32_t *)(CAN1_BASE + 0x1B4))
#define CAN_RDL0R    (*(volatile uint32_t *)(CAN1_BASE + 0x1B8))
#define CAN_RDH0R    (*(volatile uint32_t *)(CAN1_BASE + 0x1BC))

// Filter registers
#define CAN_FMR      (*(volatile uint32_t *)(CAN1_BASE + 0x200))
#define CAN_FM1R     (*(volatile uint32_t *)(CAN1_BASE + 0x204))
#define CAN_FS1R     (*(volatile uint32_t *)(CAN1_BASE + 0x20C))
#define CAN_FFA1R    (*(volatile uint32_t *)(CAN1_BASE + 0x214))
#define CAN_FA1R     (*(volatile uint32_t *)(CAN1_BASE + 0x21C))
#define CAN_F0R1     (*(volatile uint32_t *)(CAN1_BASE + 0x240))
#define CAN_F0R2     (*(volatile uint32_t *)(CAN1_BASE + 0x244))

//UART
#define USART2_BASE  0x40004400UL
#define USART2_SR    (*(volatile uint32_t *)(USART2_BASE + 0x00))
#define USART2_DR    (*(volatile uint32_t *)(USART2_BASE + 0x04))
#define USART2_BRR   (*(volatile uint32_t *)(USART2_BASE + 0x08))
#define USART2_CR1   (*(volatile uint32_t *)(USART2_BASE + 0x0C))

//GPIO
#define GPIOA_MODER   (*(volatile uint32_t *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRL    (*(volatile uint32_t *)(GPIOA_BASE + 0x20))

void delay(volatile uint32_t count) 
{
    while(count--);
}

void uart_init(void)
{
    RCC_AHB1ENR |= (1<<0);
    RCC_APB1ENR |= (1<<17);

    volatile uint32_t dummy = RCC_APB1ENR;
    (void)dummy;

    GPIOA_MODER &= ~(3 << 4);
    GPIOA_MODER |=  (2 << 4); 
    GPIOA_MODER &= ~(3 << 6);  
    GPIOA_MODER |=  (2 << 6); 

    GPIOA_AFRL |= (7 << 8);   // PA2 = AF7 (UART2 TX)
    GPIOA_AFRL |= (7 << 12);  // PA3 = AF7 (UART2 RX)  

    USART2_BRR = 0x8B;

    USART2_CR1 |= ((1<<13)| (1<<3));
}

void uart_tx_byte(uint8_t byte)
{
    while (!(USART2_SR & (1 << 7)));  // wait until TXE is set
    USART2_DR = byte;
}

void uart_tx_string(const char* str)
{
    while (*str != '\0')
    {
        uart_tx_byte(*str);
        str++;
    }
}

void can_init(void) {
    // 1. Enable CAN1 clock — bit 25 in RCC_APB1ENR
    // your code
    RCC_APB1ENR |= (1<<25);

    volatile uint32_t delay_clk = 100;
    while(delay_clk--);

    // 2. Request init mode — set INRQ bit (bit 0) in CAN_MCR
    // your code
    CAN_MCR |= (1 << 0);

    // 3. Wait until INAK bit (bit 0) is set in CAN_MSR
    // your code
    while (!(CAN_MSR & (1 << 0)));

    // 4. Clear SLEEP bit (bit 1) in CAN_MCR
    // your code
    CAN_MCR &= ~(1<<1);

    // 5. Configure bit timing in CAN_BTR
    //    Enable loopback mode — set LBKM bit (bit 30)
    //    Set baud rate prescaler, time segments
    // your code
    CAN_BTR = 0;
    CAN_BTR |= (1<< 30); //LBKM
    CAN_BTR |= (5<<20);
    CAN_BTR |= (10<<16);
    CAN_BTR |= (4<<0);

    // 6. Leave init mode — clear INRQ bit in CAN_MCR
    // your code
    CAN_MCR &= ~(1<< 0);

    // 7. Wait until INAK is cleared in CAN_MSR
    // your code
    while (CAN_MSR & (1 << 0));

    // 8. Configure filter — accept all messages
    // your code
    CAN_FMR |= (1<<0);  // enter filter init mode FIRST
    CAN_FA1R &= ~(1<<0);

    CAN_FM1R &= ~(1<<0); //mask mode
    CAN_FS1R |= (1<<0); // 32 bit scale

    CAN_F0R1 = 0x00000000;
    CAN_F0R2 = 0x00000000;

    //exit
    CAN_FA1R |=  (1<<0);  // activate filter FIRST
    CAN_FMR  &= ~(1<<0);  // then leave filter init mode
}

void can_tx(uint32_t id, uint8_t* data, uint8_t len) {
    // 9. Wait for TX mailbox 0 to be empty — check TME0 bit in CAN_TSR
    // your code
    while (!(CAN_TSR & (1<<26)));

    // 10. Set CAN ID in CAN_TI0R — shift id by 21 for standard frame
    // your code
    CAN_TI0R = 0;
    CAN_TI0R |= (id<<21);

    // 11. Set data length in CAN_TDT0R
    // your code
    CAN_TDT0R = len & 0x0F;

    // 12. Write data bytes to CAN_TDL0R and CAN_TDH0R
    // your code
    CAN_TDL0R = (data[0])   |
            (data[1] << 8)  |
            (data[2] << 16) |
            (data[3] << 24);
    
    CAN_TDH0R = (data[4])   |
            (data[5] << 8)  |
            (data[6] << 16) |
            (data[7] << 24);

    // 13. Request transmission — set TXRQ bit (bit 0) in CAN_TI0R
    // your code
    CAN_TI0R |= (1<<0);
}

uint8_t can_rx(uint32_t* id, uint8_t* data) {
    // 14. Check if message waiting — FMP bits in CAN_RF0R
    // your code
    if (!(CAN_RF0R & 0x03))
        return 0;

    // 15. Read ID from CAN_RI0R
    // your code
    *id = (CAN_RI0R >> 21) & 0x7FF;

    // 16. Read data length from CAN_RDT0R
    // your code
    uint16_t len = CAN_RDT0R & 0x0F;

    // 17. Read data bytes from CAN_RDL0R and CAN_RDH0R
    // your code
    data[0] = (CAN_RDL0R >> 0) & 0xFF;
    data[1] = (CAN_RDL0R >> 8) & 0xFF;
    data[2] = (CAN_RDL0R >> 16) & 0xFF;
    data[3] = (CAN_RDL0R >> 24) & 0xFF;
    data[4] = (CAN_RDH0R >> 0) & 0xFF;
    data[5] = (CAN_RDH0R >> 8) & 0xFF;
    data[6] = (CAN_RDH0R >> 16) & 0xFF;
    data[7] = (CAN_RDH0R >> 24) & 0xFF;
    

    // 18. Release FIFO — set RFOM0 bit (bit 5) in CAN_RF0R
    // your code
    CAN_RF0R |= (1<<5);

    return len;
}

void Reset_Handler(void) {
    // Enable FPU
    *(volatile uint32_t *)(0xE000ED88UL) |= ((3UL << 20) | (3UL << 22));
    __asm volatile ("isb");

    // Copy .data to RAM
    uint32_t *pSource = &_sidata;
    uint32_t *pDest = &_sdata;
    while (pDest < &_edata) {
        *pDest++ = *pSource++;
    }

    // Zero out .bss
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

    // Initialize UART first
    uart_init();
    
    // Test point 1: Print immediately to verify clock-to-UART path
    uart_tx_string("--- System Boot Complete ---\r\n");

    // Initialize CAN
    can_init();
    
    // Test point 2: Verify code successfully exited the can_init loops
    uart_tx_string("CAN Peripheral Initialized Safely\r\n");

    uint8_t tx_data[8] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};
    uint32_t rx_id;
    uint8_t rx_data[8];

    while (1) {
        // Continuous loop heartbeat
        uart_tx_string("Transmitting CAN Frame...\r\n");
        
        can_tx(0x123, tx_data, 8);
        delay(100000);

        uint8_t len = can_rx(&rx_id, rx_data);
        if(len > 0) {
            uart_tx_string("Success: CAN Loopback RX OK!\r\n");
        } else {
            uart_tx_string("Error: CAN RX Empty/Filtered\r\n");
        }

        delay(2000000);
    }
}

void (*const vector_table[])(void) __attribute__((section(".isr_vector"))) = {
    (void (*)(void))&_estack, 
    Reset_Handler,
};