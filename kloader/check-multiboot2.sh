#!/bin/bash

if grub-file --is-x86-multiboot2 kloader.elf
then
	echo "Multiboot2 - OK"
else
	echo "Multiboot2 - ERROR"
fi

