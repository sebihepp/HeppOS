

FLAGS_DEBUG := -D_DEBUG -g -Og
FLAGS_RELEASE := -O2

.PHONY: all
all: FLAGS = $(FLAGS_RELEASE)
all: iso img

.PHONY: debug
debug: FLAGS = $(FLAGS_DEBUG)
debug: iso img

.PHONY: kernel
kernel: FLAGS = $(FLAGS_RELEASE)
kernel: kernel/kernel

.PHONY: kernel-debug
kernel-debug: FLAGS = $(FLAGS_DEBUG)
kernel-debug: kernel/kernel


export FLAGS

.PHONY: clean
clean: 
	rm -f HeppOS.iso
	rm -f HeppOS.img
	cd kernel && $(MAKE) clean

.PHONY: iso
iso: HeppOS.iso
	
.PHONY: img
img: HeppOS.img


HeppOS.iso: kernel/kernel
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/limine
	mkdir -p iso/EFI
	mkdir -p iso/EFI/BOOT
	cp -v kernel/kernel iso/
	cp -v kernel/limine.conf iso/
	cp -v "/usr/local/share/limine/limine-bios.sys" iso/boot/limine/
	cp -v "/usr/local/share/limine/limine-bios-cd.bin" iso/boot/limine/
	cp -v "/usr/local/share/limine/limine-uefi-cd.bin" iso/boot/limine/
	cp -v "/usr/local/share/limine/BOOTIA32.EFI" iso/EFI/BOOT
	cp -v "/usr/local/share/limine/BOOTX64.EFI" iso/EFI/BOOT
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label \
		iso -o HeppOS.iso
	limine bios-install HeppOS.iso


HeppOS.img: kernel/kernel
	dd if=/dev/zero bs=1M count=128 of=HeppOS.img
	sgdisk HeppOS.img -n 1:2048:131071 -t 1:ef00 -c 1:EFI
	sgdisk HeppOS.img -n 2:131072 -t 2:0700 -c 2:FAT32
	limine bios-install HeppOS.img
	mformat -i HeppOS.img@@1M
	mmd -i HeppOS.img@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
	mcopy -i HeppOS.img@@1M "/usr/local/share/limine/limine-bios.sys" ::/boot/limine
	mcopy -i HeppOS.img@@1M "/usr/local/share/limine/BOOTIA32.EFI" ::/EFI/BOOT
	mcopy -i HeppOS.img@@1M "/usr/local/share/limine/BOOTX64.EFI" ::/EFI/BOOT
	mformat -i HeppOS.img@@64M
	mmd -i HeppOS.img@@64M ::/limine
	mcopy -i HeppOS.img@@64M kernel/kernel ::/
	mcopy -i HeppOS.img@@64M kernel/limine.conf ::/


.PHONY: kernel/kernel
kernel/kernel:
	cd kernel && $(MAKE)
