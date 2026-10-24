    .section .vectors
vector_table:
    .word  __stack
    .word  reset_handler
    .word  xPortSysTickHandler
    .zero  400

    .section .text
    .align 1
    .type reset_handler, %function
reset_handler:
    b start
    b .

    .section .text
    .align 1
    .type start, %function