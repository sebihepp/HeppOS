
# These paths need to be adjusted to your machine
CC := "$${HOME}/opt/cross/bin/x86_64-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/x86_64-elf-g++"
AS := "$${HOME}/opt/cross/bin/x86_64-elf-as"
LD := "$${HOME}/opt/cross/bin/x86_64-elf-ld"

# Get the correct libgcc path
LGCCPATH := $(dir $(shell $(CC) -mno-red-zone -print-libgcc-file-name))

# Definition for flags
CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -mcmodel=kernel -mno-red-zone -mno-80387 -mno-sse -mno-sse2 -mno-mmx -I src
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti -mcmodel=kernel -mno-red-zone -mno-80387 -mno-sse -mno-sse2 -mno-mmx -I src
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -r -nostdlib -L "$(LGCCPATH)" -lgcc


CXX_FILES := $(shell cd src && find -L * -type f -name '*.cpp' | LC_ALL=C sort) 
C_FILES := $(shell cd src && find -L * -type f -name '*.c' | LC_ALL=C sort)
AS_FILES := $(shell cd src && find -L * -type f -name '*.S' | LC_ALL=C sort)
# Make sure crti is first and crtn is last
OBJ_FILES := $(addprefix obj/,$(filter-out crti.S.o crtn.S.o , $(CXX_FILES:.cpp=.cpp.o) $(C_FILES:.c=.c.o) $(AS_FILES:.S=.S.o)))
OBJ_FILES := obj/crti.S.o $(OBJ_FILES) obj/crtn.S.o

# Needed for constructors and destructors
CRTBEGIN_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)

TARGET := kernel.elf

.PHONY: all
all: bin/$(TARGET)

.PHONY: clean
clean: 
	rm -rf bin obj


bin/$(TARGET): GNUmakefile linker.ld $(OBJ_FILES)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) -o $@ $(OBJ_FILES)  

obj/%.cpp.o: src/%.cpp GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CXX) -c $< -o $@ $(CXXFLAGS)

obj/%.c.o: src/%.c
	mkdir -p "$$(dirname $@)"
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.S.o: src/%.S
	mkdir -p "$$(dirname $@)"
	$(AS) -c $< -o $@ $(ASFLAGS)
