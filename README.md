# HeppOS 
HeppOS is an Operating System with the target to learn and have fun coding. 
It targets x86_64 and should be able to run on desktops as well as single board computers and dev boards. 
## Design choices 
### General 
- 64Bit OS
- Using Limine protocol
- Drivers must be in relocatable elf format
- Drivers are loaded as modules
### Programming languages 
- Assembler (only when necessary) 
- C++ (main language) 
- C (maybe some things are written in C) 
### Kernel 
- The kernel itself must be bootable by limine.
- Kernel space is the last 2GB and must be mapped into every process.
- The kernel should be a mix of monolith and microkernel (some things run in kernel space inside the kernel, while other drivers run as user programms).
- The kernel itself should only contain resource management, e.g. handling interrupts, physical and virtual memory management, paging, 
managing memory mapped i/o and ports, scheduling.
- The kernel executable will most likely also contain a loader for parsing drivers as modules. This is needed to parse and load the 
initial drivers, supplied as modules. 
### Drivers 
- The drivers must be in relocatable elf format. 
- Drivers will be loaded into kernel space. 

### PREREQUISITES 
- cross compiler with binutils, gcc, g++ and libgcc as x86_64-elf (with mno-red-zone and mcmodel=kernel)
- limine build with --enable-bios-cd --enable-bios --enable-uefi-ia32 --enable-uefi-x86-64 --enable-uefi-cd 
and installed into $HOME/opt/limine (by using --prefix=$HOME/opt/limine) - *i am searching for a way to also automate this*
- BOCHS for testing
- QEMU for testing


### FAQ 

Q: I cannot boot the created iso file  

A: If you use the windows subsystem linux (wsl) for creating the cdrom image, you need to install the grub binaries via "sudo apt-get install grub-pc-bin"  
---
Q: When compiling the kernel I get errors like "relocation truncated to fit" 

A: You need to use a libgcc build with -mno-red-zone **and** -mcmodel=kernel 
