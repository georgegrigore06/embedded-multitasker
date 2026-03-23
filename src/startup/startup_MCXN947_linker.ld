ENTRY(Reset_Handler)

MEMORY
{
    FLASH(rx): ORIGIN = 0x00000000 , LENGTH = 2M
    RAM(rwx): ORIGIN = 0x20000000, LENGTH = 32K
}

_stackTop = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .text : ALIGN(4)
    {
        KEEP(*(.isr_vector*))
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
    } > FLASH
    
    .data : ALIGN(4)
    {
        _sdata = .;
        *(.data SORT(.data.*))
        . = ALIGN(4);
        _edata = .;
    } > RAM AT> FLASH

    _sidata = LOADADDR(.data);

    .bss : ALIGN(4)
    {
        __bss_start__ = .;
        _sbss = .;
        *(.bss SORT(.bss.*) COMMON)
        . = ALIGN(4);
        __bss_end__ = .;
    } > RAM

    PROVIDE(end = .);

}