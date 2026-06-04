/* STM32F446RE Memory Layout
   Flash: 512KB starting at 0x08000000
   RAM:   128KB starting at 0x20000000
*/

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 512K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

ENTRY(Reset_Handler)

SECTIONS
{
    /* Code goes in flash */
    .text :
    {
        KEEP(*(.isr_vector))   /* interrupt vector table must be first */
        *(.text*)
        *(.rodata*)
    } > FLASH

    /* Initialized data — stored in flash, copied to RAM on startup */
    .data :
    {
        _sdata = .;
        *(.data*)
        _edata = .;
    } > RAM AT > FLASH

    _sidata = LOADADDR(.data);

    /* Zero-initialized data in RAM */
    .bss :
    {
        _sbss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;
    } > RAM

    /* Stack at top of RAM */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
