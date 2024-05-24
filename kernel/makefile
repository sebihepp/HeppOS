


CC := "$${HOME}/opt/cross/bin/i686-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/i686-elf-g++"
AS := "$${HOME}/opt/cross/bin/i686-elf-as"
LD := "$${HOME}/opt/cross/bin/i686-elf-ld"

CFLAGS := -ffreestanding -O2 -ffreestanding -Wall -Wextra -fno-exceptions
CXXFLAGS := -ffreestanding -O2 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
ASFLAGS := 
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib -lgcc

CXX_OBJECTS := main.o
C_OBJECTS :=  
ASM_OBJECTS := boot.o


.PHONY: clean

all: kernel.elf

clean: 
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(ASM_OBJECTS) kernel.elf

kernel.elf: $(CXX_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.S
	$(AS) $< -o $@ $(ASFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


