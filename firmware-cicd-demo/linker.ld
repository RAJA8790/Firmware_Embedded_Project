/* linker.ld — Minimal linker script for STM32F407 (Cortex-M4)
   Used only for the ARM cross-compile build target.
   Host (test) builds do not use this file. */

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x08000000, LENGTH = 1024K
    RAM   (rwx) : ORIGIN = 0x20000000, LENGTH = 128K
}

SECTIONS
{
    .text :
    {
        *(.text*)
        *(.rodata*)
    } > FLASH

    .data :
    {
        *(.data*)
    } > RAM AT > FLASH

    .bss :
    {
        *(.bss*)
        *(COMMON)
    } > RAM

    /DISCARD/ :
    {
        *(.ARM.exidx*)
    }
}
