


CC := "$${HOME}/opt/cross/bin/x86_64-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/x86_64-elf-g++"
AS := "$${HOME}/opt/cross/bin/x86_64-elf-as"
LD := "$${HOME}/opt/cross/bin/x86_64-elf-ld"

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -r -nostdlib -L "$$HOME/opt/cross/lib/gcc/x86_64-elf/14.1.0" -lgcc

CXX_OBJECTS := main.o pmm.o
C_OBJECTS :=  
ASM_OBJECTS := 

TARGET := kernel.elf


all: $(TARGET)

.PHONY: clean
clean: 
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(ASM_OBJECTS) $(TARGET)

$(TARGET): $(CXX_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


