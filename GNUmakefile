

# These paths need to be adjusted to your machine
LIMINE_PATH := "$${HOME}/opt/limine"
LIMINE := "$(LIMINE_PATH)/bin/limine"


.PHONY: all
all: kernel iso img

.PHONY: clean
clean: clean-img clean-iso clean-kernel

.PHONY: kernel
kernel:
	cd kernel && $(MAKE) all
	
.PHONY: iso
iso:
	mkdir -p iso
	mkdir -p iso/boot
	mkdir -p iso/boot/limine
	mkdir -p iso/EFI
	mkdir -p iso/EFI/BOOT
	cp -v kernel/bin/HeppOS.elf iso/
	cp -v kernel/limine.conf iso/
	cp -v "$(LIMINE_PATH)/share/limine/limine-bios.sys" iso/boot/limine/
	cp -v "$(LIMINE_PATH)/share/limine/limine-bios-cd.bin" iso/boot/limine/
	cp -v "$(LIMINE_PATH)/share/limine/limine-uefi-cd.bin" iso/boot/limine/
	cp -v "$(LIMINE_PATH)/share/limine/BOOTIA32.EFI" iso/EFI/BOOT
	cp -v "$(LIMINE_PATH)/share/limine/BOOTX64.EFI" iso/EFI/BOOT
	xorriso -as mkisofs -b boot/limine/limine-bios-cd.bin -no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot boot/limine/limine-uefi-cd.bin -efi-boot-part --efi-boot-image --protective-msdos-label \
		iso -o HeppOS.iso
	$(LIMINE) bios-install HeppOS.iso
	
.PHONY: img
img:
	dd if=/dev/zero bs=1M count=128 of=HeppOS.img
	sgdisk HeppOS.img -n 1:2048:131071 -t 1:ef00 -c 1:EFI
	sgdisk HeppOS.img -n 2:131072 -t 2:0700 -c 2:FAT32
	$(LIMINE) bios-install HeppOS.img
	mformat -i HeppOS.img@@1M
	mmd -i HeppOS.img@@1M ::/EFI ::/EFI/BOOT ::/boot ::/boot/limine
	mcopy -i HeppOS.img@@1M "$(LIMINE_PATH)/share/limine/limine-bios.sys" ::/boot/limine
	mcopy -i HeppOS.img@@1M "$(LIMINE_PATH)/share/limine/BOOTIA32.EFI" ::/EFI/BOOT
	mcopy -i HeppOS.img@@1M "$(LIMINE_PATH)/share/limine/BOOTX64.EFI" ::/EFI/BOOT
	mformat -i HeppOS.img@@64M
	mmd -i HeppOS.img@@64M ::/limine
	mcopy -i HeppOS.img@@64M kernel/bin/HeppOS.elf ::/
	mcopy -i HeppOS.img@@64M kernel/limine.conf ::/

.PHONY: clean-kernel
clean-kernel:
	cd kernel && $(MAKE) clean

.PHONY: clean-iso
clean-iso:
	rm -f HeppOS.iso
	rm -rf iso
	
.PHONY: clean-img
clean-img:
	rm -f HeppOS.img
