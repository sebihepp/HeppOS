
# These paths need to be adjusted to your cross compiler
CC := "$${HOME}/opt/cross/bin/i686-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/i686-elf-g++"
AS := "$${HOME}/opt/cross/bin/i686-elf-as"
LD := "$${HOME}/opt/cross/bin/i686-elf-ld"

# Get the correct libgcc path
LGCCPATH := $(dir $(shell $(CC) -print-libgcc-file-name))

# Definition for flags
CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -fno-use-cxa-atexit
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -nostdlib -L "$(LGCCPATH)" -lgcc

CXX_OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp)) 
C_OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
# All .s files should have .o files, except crti.s and crtn.s - they have to be treated specially
AS_OBJECTS := $(filter-out crti.o crtn.o ,$(patsubst %.s, %.o, $(wildcard *.s)))

# Needed for constructors and destructors
CRTI_OBJECT := crti.o
CRTBEGIN_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJECT := crtn.o

TARGET := kloader.elf

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean: 
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(AS_OBJECTS) $(TARGET) $(CRTI_OBJECT) $(CRTN_OBJECT)

$(TARGET): $(CRTI_OBJECT) $(CRTBEGIN_OBJECT) $(AS_OBJECTS) $(CXX_OBJECTS) $(C_OBJECTS) $(CRTEND_OBJECT) $(CRTN_OBJECT)
	$(LD) $(LDFLAGS) -o $@ $^ 

%.o: %.s
	$(AS) $< -o $@ $(ASFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS)

%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)


