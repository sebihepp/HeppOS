
CC := "$${HOME}/opt/cross/bin/x86_64-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/x86_64-elf-g++"
AS := "$${HOME}/opt/cross/bin/x86_64-elf-as"
LD := "$${HOME}/opt/cross/bin/x86_64-elf-ld"

# Get the correct libgcc path
LGCCPATH := $(dir $(shell $(CC) -mno-red-zone -print-libgcc-file-name))

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -mcmodel=kernel
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -mcmodel=kernel
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -r -nostdlib -L "$(LGCCPATH)" -lgcc


CXX_OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp)) 
C_OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
AS_OBJECTS := $(patsubst %.s, %.o, $(wildcard *.s))

TARGET := kernel.elf


all: $(TARGET)

.PHONY: clean
clean: 
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(AS_OBJECTS) $(TARGET)

$(TARGET): $(CXX_OBJECTS) $(C_OBJECTS) $(AS_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ 

%.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


