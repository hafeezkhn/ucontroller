PROJ = main
CPU ?= cortex-m3
BOARD ?= stm32vldiscovery

OBJ = boot.o  start.o

.PHONY: all
all: $(PROJ).elf

%.o: %.S
	arm-none-eabi-as -mthumb -mcpu=$(CPU) -g -c $^ -o $@

%.o: %.c
	arm-none-eabi-gcc -mthumb -mcpu=$(CPU) -g -c $^ -o $@


$(PROJ).elf: $(OBJ)
	arm-none-eabi-ld -T map.ld  $^ -o $@
	arm-none-eabi-objdump -D -S $@ > $@.lst
	arm-none-eabi-readelf -a $@ > $@.debug

qemu:
	qemu-system-arm -S -M $(BOARD) -cpu $(CPU) -nographic -kernel $(PROJ).elf -gdb tcp::1234


clean:
	rm -f  *.out *.o *.elf *.lst *.debug $(OBJ) .gdb_history