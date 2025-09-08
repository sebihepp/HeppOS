
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
	-ffreestanding -fno-exceptions -fno-rtti -fno-use-cxa-atexit \
	-mcmodel=kernel -mno-red-zone -mgeneral-regs-only \
	$(INCLUDES) \
	$(FLAGS)

CCFLAGS := \
	-Wall -Wextra \
	-ffreestanding -fno-use-cxa-atexit \
	-mcmodel=kernel -mno-red-zone -mgeneral-regs-only \
	$(INCLUDES) \
	$(FLAGS)

ASFLAGS := \
	-Wall -Wextra \
	$(INCLUDES) \
	$(FLAGS)


LDFLAGS := \
	-T "./linker.ld" \
	-nostdlib -lgcc \
	$(FLAGS)


SOURCES := \
	src/boot.S \
	src/init.cpp \
	src/fini.cpp \
	src/main.cpp \
	src/liminestub.cpp \
	src/retval.cpp \
	src/serial.cpp \
	src/log.cpp \
	\
	src/kstdlib/kstring.cpp \
	src/kstdlib/ktype.cpp \
	\
	\
	src/memory/paging.cpp \
	src/memory/pmm.cpp \
	\
	src/cpu/gdt.cpp \
	src/cpu/interrupt.cpp \
	src/cpu/isr_stub.S \
	src/cpu/pic.cpp \
	\
	src/video/console.cpp \
	src/video/stdfont.cpp \
	

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
	$(AS) $(ASFLAGS) -c -o $@ $<

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:%.c
	$(CC) $(CCFLAGS) -c -o $@ $<
