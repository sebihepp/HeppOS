#!/bin/bash
qemu-system-x86_64 -cpu Skylake-Client-v1 \
	-m 8192 -cdrom "HeppOS.iso" \
	-serial file:"qemu_serial.log" \
	-usbdevice mouse -usbdevice keyboard \
	-smp cores=4,threads=2,sockets=1
