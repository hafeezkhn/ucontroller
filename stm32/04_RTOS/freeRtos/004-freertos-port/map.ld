MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

/* Calculate the exact top of RAM for the Cortex-M hardware stack */
__stack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    .text :
    {
        _stext = .;
        *(.vectors*)
        *(.text*)
        *(.rodata)
        *(.rodata.*)
        _etext = .;
    } > FLASH

    /* Define the load address in FLASH for the startup code to copy from */
    _sidata = LOADADDR(.data);

    .data : ALIGN(4)
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss : ALIGN(4)
    {
        _sbss = .;
        *(.bss*)
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}