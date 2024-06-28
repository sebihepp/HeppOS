
.PHONY: all clean

all:
	cd kloader && $(MAKE) all
	cd kernel && $(MAKE) all
	cp kloader/kloader.elf iso/boot/kloader.elf
	cp kernel/kernel.elf iso/kernel.elf
	grub-mkrescue -o HeppOS.iso iso

clean:
	rm -f HeppOS.iso
	rm -f iso/boot/kloader.elf
	rm -f iso/kernel.elf
	cd kernel && $(MAKE) clean
	cd kloader && $(MAKE) clean

