# HeppOS
A small Operating System for learning purposes

It should use multiboot2 standard and main languages are c and c++ with asm where necessary.

PREREQUISITES:
cross compiler with binutils, gcc, g++ and libgcc as i686-elf and x86_64-elf (with mno-red-zone) in $HOME/opt/cross (binaries in subdirectory bin)
sudo apt-get install grub-pc-bin
BOCHS for testing (alternatives can be qemu, virtualbox, etc.)


FAQ

Q: I cannot boot the created iso file

A: If you use the windows subsystem linux (wsl) for example, you need to install the grub binaries via "sudo apt-get install grub-pc-bin"
 
