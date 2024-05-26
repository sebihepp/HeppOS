# HeppOS 
This is a hobby OS (Operating System) for learning and having fun programming at the lowest level. 
## Design choices
### Programming languages
- Assembler (only when necessary) 
- C++ (main language) 
- C (maybe some things are written in C) 
### Kernel
- The kernel must be in elf format and multiboot2 capable.
- Kernel space is starting at 2GB virtual memory and must be mapped into every process.
- The kernel should be a monolith (all drivers run in kernel space). This may be changed to a microkernel later on, when possible. 
- The kernel itself should only contain resource management, e.g. handling interrupts, physical and virtual memory management, 
managing memory mapped i/o and ports and scheduling. 
- The kernel executable will most likely also contain a loader for parsing driver executables. This is needed to parse and load the 
initial drivers, supplied as modules via multiboot2. 
### Drivers
- The drivers must be in relocatable elf format. 
- Drivers will be loaded into kernel space. 
- Every driver must implement a C function "\<drivername\>\_load" with two parameters: 
	- pointer to function kmalloc 
	- pointer to function kfree 
	- and return value pointer to class implementing the driver interface class 

## Stuff to remember and include in this readme later
It should use multiboot2 standard and main languages are c and c++ with asm where necessary.  

### PREREQUISITES
- cross compiler with binutils, gcc, g++ and libgcc as i686-elf and x86_64-elf (with mno-red-zone) in $HOME/opt/cross (binaries in 
subdirectory bin)
- BOCHS for testing (alternatives can be qemu, virtualbox, etc.) 


### FAQ

Q: I cannot boot the created iso file  

A: If you use the windows subsystem linux (wsl) for creating the cdrom image, you need to install the grub binaries via "sudo apt-get install grub-pc-bin"  
 
