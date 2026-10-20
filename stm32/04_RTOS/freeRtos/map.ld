MEMORY
{
    FLASH (rx) : ORIGIN = 0x0, LENGTH = 128K
}

SECTIONS
{
    .text : 
    {
        *(.vectors)
        *(.text)
    } > FLASH
}