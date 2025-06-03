#!/bin/bash
qemu-system-x86_64 -s -S -cpu Skylake-Client-v1 \
	-m 8192 \
	-bios /usr/share/ovmf/OVMF.fd \
	-drive if=none,id=stick,format=raw,file=HeppOS.img \
	-device nec-usb-xhci,id=xhci \
	-device usb-storage,bus=xhci.0,drive=stick \
	-d int -no-reboot -no-shutdown \
	-serial file:"qemu_serial.log" \
	-usbdevice mouse -usbdevice keyboard \
	-smp cores=4,threads=2,sockets=1
