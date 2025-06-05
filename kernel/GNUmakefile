
CXX := x86_64-elf-g++
CC := x86_64-elf-g++
LD := x86_64-elf-g++
AS := x86_64-elf-as


CXXFLAGS := \
	-g -O2 \
	-Wall -Wextra \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=kernel -mno-red-zone -mgeneral-regs-only \
	-I"/usr/local/include" -I"./src" \
	$(FLAGS)

CCFLAGS := 

ASFLAGS := 

LDFLAGS := \
	-g -O2 \
	-T "./linker.ld" \
	-nostdlib -lgcc

SOURCES := \
	src/boot.S \
	src/init.cpp \
	src/fini.cpp \
	src/main.cpp \
	src/cstub.cpp \
	src/liminestub.cpp \
	src/retval.cpp \
	src/serial.cpp \
	src/log.cpp \
	\
	\
	src/memory/paging.cpp \
	src/memory/pmm.cpp \
	\
	src/cpu/gdt.cpp \
	src/cpu/interrupt.cpp \
	src/cpu/isr_stub.S \
	src/cpu/pic.cpp

OBJECTS := $(patsubst %.S, %.o, $(SOURCES))
OBJECTS := $(patsubst %.cpp, %.o, $(OBJECTS))
OBJECTS := $(patsubst %.c, %.o, $(OBJECTS))

kernel: $(OBJECTS) $(SOURCES)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

.PHONY: clean
clean:
	rm -f kernel
	rm -f $(OBJECTS)
	
%.o:%.S
	$(AS) $(ASFLAGS) -o $@ $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.c
	$(CC) $(CCFLAGS) -c -o $@ $<
