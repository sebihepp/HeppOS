#!/bin/bash
qemu-system-x86_64 -cpu Skylake-Client-v1 -m 8192  \
	-bios /usr/share/ovmf/OVMF.fd \
	-drive if=none,id=stick,format=raw,file=HeppOS.img \
	-device nec-usb-xhci,id=xhci \
	-device usb-storage,bus=xhci.0,drive=stick 
