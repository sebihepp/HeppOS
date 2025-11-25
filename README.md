# HeppOS 
HeppOS is an Operating System with the target to learn and have fun coding. 
It targets x86_64 and should be able to run on desktops as well as single board computers and dev boards. 
## Design choices 

### General 
- x86_64 target (64bit x86 cpus)
- Using Limine protocol

### Programming languages 
- Assembler (only when necessary) 
- C++ (main language) 
- C (maybe some things are written in C) 

### Kernel 
- The kernel itself must be bootable by limine.
- Kernel space is the last 2GB and must be mapped into every process.
- The kernel should be a mix of monolith and microkernel. 
- The kernel itself should only contain resource management, e.g. handling interrupts, physical and virtual memory management, paging, 
managing memory mapped i/o and ports, scheduling.
- The kernel executable will most likely also contain a loader for parsing drivers as modules. This is needed to parse and load the 
initial drivers, supplied as modules. 

### Drivers 
- The drivers must be in relocatable elf format. 
- Drivers will be loaded as processes in ring 0. 
- Drivers will be passed to the kernel as limine modules.

## PREREQUISITES 
- installed binutils 2.45 (configured with --target=x86_64-elf --with-sysroot --disable-nls --disable-werror)
- installed g++ 15.2.0 (configured with --target=x86_64-elf --disable-nls --enable-languages=c++ --without-headers --disable-hosted-libstdcxx --enable-initfini-array)
- installed libgcc 15.2.0 (build with CFLAGS='-g -O2 -mno-red-zone -mcmodel=large')
- installed limine 10.3.2 (build with --enable-bios-cd --enable-bios --enable-uefi-ia32 --enable-uefi-x86-64 --enable-uefi-cd)
- installed MTools
- installed GNUmake
- BOCHS for testing [Optional]
- QEMU for testing [Optional]

## FAQ 
#### I cannot boot the created iso file 
If you use the windows subsystem linux (wsl) for creating the cdrom image, you need to install the grub binaries via "sudo apt-get install grub-pc-bin" 
#### When compiling the kernel I get errors like "relocation truncated to fit" 
You need to use a libgcc build with **-mno-red-zone and -mcmodel=large** 
#### Bochs 2.8 triple faults after "Init CPMM..."
I get the same error, but with Bochs 3.0 it works. (I assume a bug in bochs)
