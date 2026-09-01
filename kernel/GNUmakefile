
CXX := x86_64-elf-g++
CC := x86_64-elf-g++
LD := x86_64-elf-g++
AS := x86_64-elf-g++

INCLUDES := \
	-I"./src" \
	-I"./src/kstdlib" \
	-I "/usr/local/include" 

CXXFLAGS := \
	-Wall -Wextra \
	-fpie \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=large -mno-red-zone -mgeneral-regs-only \
	$(INCLUDES) \
	$(FLAGS)

CCFLAGS := \
	-Wall -Wextra \
	-fpie \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=large -mno-red-zone -mgeneral-regs-only \
	$(INCLUDES) \
	$(FLAGS)

ASFLAGS := \
	-Wall -Wextra \
	-fpie \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=large -mno-red-zone -mgeneral-regs-only \
	$(INCLUDES) \
	$(FLAGS)


LDFLAGS := \
	-T "./linker.ld" \
	-Wl,-z,max-page-size=0x1000 \
	-pie \
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=large -mno-red-zone -mgeneral-regs-only \
	-nostdlib -lgcc \
	$(FLAGS)


SOURCES := \
	src/boot.S \
	src/init.cc \
	src/fini.cc \
	src/main.cc \
	src/limine_stub.cc \
	src/return_value.cc \
	src/boot_serial.cc \
	src/boot_log.cc \
	\
	src/kstdlib/kstring.cc \
	src/kstdlib/ktype.cc \
	\
	\
	src/memory/paging.cc \
	src/memory/pmm.cc \
	\
	src/cpu/gdt.cc \
	src/cpu/interrupt.cc \
	src/cpu/isr_stub.S \
	src/cpu/pic.cc \
	\
	src/video/console.cc \
	src/video/stdfont.cc \
	

OBJECTS := $(patsubst %.S, %.o, $(SOURCES))
OBJECTS := $(patsubst %.cc, %.o, $(OBJECTS))
OBJECTS := $(patsubst %.c, %.o, $(OBJECTS))

kernel: $(OBJECTS) $(SOURCES)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

.PHONY: clean
clean:
	rm -f kernel
	rm -f $(OBJECTS)
	
%.o:%.S
	$(AS) $(ASFLAGS) -c -o $@ $<

%.o:%.cc
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.c
	$(CC) $(CCFLAGS) -c -o $@ $<
