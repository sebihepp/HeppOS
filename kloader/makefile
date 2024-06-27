
CC := "$${HOME}/opt/cross/bin/i686-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/i686-elf-g++"
AS := "$${HOME}/opt/cross/bin/i686-elf-as"
LD := "$${HOME}/opt/cross/bin/i686-elf-ld"

CFLAGS := -ffreestanding -O2 -ffreestanding -Wall -Wextra -fno-exceptions
CXXFLAGS := -ffreestanding -O2 -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -nostdlib -L "$$HOME/opt/cross/lib/gcc/i686-elf/14.1.0" -lgcc

CXX_OBJECTS :=
C_OBJECTS :=  
ASM_OBJECTS := boot.o

TARGET := kloader.elf


all: $(TARGET)

.PHONY: clean
clean: 
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(ASM_OBJECTS) $(TARGET)

$(TARGET): $(CXX_OBJECTS) $(C_OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^ 

%.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


