# HeppOS 
This is a hobby OS (Operating System) for learning and having fun programming at the lowest level. 
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
- The kernel should be a monolith (all drivers run in kernel space). This may be changed to a microkernel later on, if possible. 
- The kernel itself should only contain resource management, e.g. handling interrupts, physical and virtual memory management, 
managing memory mapped i/o and ports and scheduling. 
- The kernel executable will most likely also contain a loader for parsing driver executables. This is needed to parse and load the 
initial drivers, supplied as modules. 
### Drivers
- The drivers must be in relocatable elf format. 
- Drivers will be loaded into kernel space. 

### PREREQUISITES
- cross compiler with binutils, gcc, g++ and libgcc as x86_64-elf (with mno-red-zone) in $HOME/opt/cross (binaries in 
subdirectory bin)
- BOCHS for testing
- QEMU for testing


### FAQ

Q: I cannot boot the created iso file  

A: If you use the windows subsystem linux (wsl) for creating the cdrom image, you need to install the grub binaries via "sudo apt-get install grub-pc-bin"  
 
