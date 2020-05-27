SOURCES=boot/boot.o kernel/kernel.o cpu/gdt_handler.o drivers/keyboard.o kernel/isr.o kernel/shell.o kernel/string.o cpu/idt_prep.o cpu/io.o cpu/interrupt.o
CFLAGS=-std=gnu11 -ffreestanding -O2 -Wall -Wextra -g
LDFLAGS=-T linker.ld
ASFLAGS=-felf32
OSNAME=DomOS
OSFILE=$(OSNAME).bin
MKRESCUE ?= grub-mkrescue
.SUFFIXES: .o .c .asm

all: $(SOURCES) link
	-rm -r isodir
	-mkdir -p isodir/boot/grub
	cp $(OSFILE) isodir/boot/$(OSFILE)
	cp grub.cfg isodir/boot/grub/grub.cfg
	$(MKRESCUE) -o isodir/$(OSNAME).iso isodir

clean:
	-rm -r isodir
	-rm *.o $(OSFILE)
	-rm boot/*.o
	-rm cpu/*.o
	-rm kernel/*.o
	-rm -r isodir/boot

link:
	i686-elf-gcc -T linker.ld -o $(OSFILE) -ffreestanding -O2 -nostdlib $(SOURCES) -lgcc

.asm.o:
	nasm $(ASFLAGS) $< -o $@


.c.o:
	i686-elf-gcc -c $< -o $@ $(CFLAGS)
