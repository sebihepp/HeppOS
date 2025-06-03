#!/bin/bash
qemu-system-x86_64 -s -S -cpu Skylake-Client-v1 \
	-m 8192 \
	-cdrom "HeppOS.iso" \
	-d int -no-reboot -no-shutdown \
	-serial file:"qemu_serial.log" \
	-usbdevice mouse -usbdevice keyboard \
	-smp cores=4,threads=2,sockets=1