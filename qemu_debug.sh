#!/bin/bash
qemu-system-x86_64 -s -S -cpu Skylake-Client-v1 \
	-m 8192 \
	-cdrom "HeppOS.iso" \
	-d int -no-reboot -no-shutdown 
