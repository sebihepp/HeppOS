
AUTOMAKE_OPTIONS = subdir-objects

CXX = x86_64-elf-g++
CCAS = x86_64-elf-g++
LD = x86_64-elf-g++

bin_PROGRAMS = kernel

kernel_CXXFLAGS = \
	-g -O2 \
	-Wall -Wextra \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=kernel -mno-red-zone -mgeneral-regs-only \
	-I"/usr/local/include" -I"$(srcdir)/src"
	
kernel_CCASFLAGS = 

kernel_LDFLAGS = \
	-g -O2 \
	-T "$(srcdir)/linker.ld" \
	-nostdlib

kernel_SOURCES = \
	src/boot.S \
	src/init.cpp \
	src/fini.cpp \
	src/main.cpp \
	src/cstub.cpp \
	src/liminestub.cpp \
	src/retval.cpp \
	\
	src/video/stdfont.cpp \
	src/video/console.cpp \
	\
	src/memory/paging.cpp \
	\
	src/cpu/gdt.cpp \
	src/cpu/interrupt.cpp \
	src/cpu/isr_stub.S \
	src/cpu/pic.cpp

kernel_LDADD = -lgcc
