
.PHONY: all clean

all:
	cd kernel && $(MAKE) all
	cp kernel/kernel.elf iso/boot/kernel.elf
	grub-mkrescue -o HeppOS.iso iso

clean:
	rm -f HeppOS.iso
	rm -f iso/boot/kernel.elf
	cd kernel && $(MAKE) clean

