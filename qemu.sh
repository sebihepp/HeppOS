#!/bin/bash
qemu-system-x86_64 -cpu EPYC -m 8192 -cdrom "HeppOS.iso" -display sdl -d int -no-shutdown -no-reboot