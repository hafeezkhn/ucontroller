MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 128K
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 8K
}

SECTIONS
{
    .text :
    {
        *(.vectors)
        *(.text)
    } > FLASH

    __StackTop = ORIGIN(RAM) + LENGTH(RAM);
    PROVIDE(__stack = __StackTop - 4);
}