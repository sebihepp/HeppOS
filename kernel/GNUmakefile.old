
# These paths need to be adjusted to your machine
CC := x86_64-elf-gcc
CXX := x86_64-elf-g++
AS := x86_64-elf-as
LD := x86_64-elf-ld
LIMINE_PATH := "$${HOME}/opt/limine"
LIMINE := "$(LIMINE_PATH)/bin/limine"


# Get the correct libgcc path
LGCCPATH := $(dir $(shell $(CC) -mno-red-zone -print-libgcc-file-name))

# Definition for flags
CFLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -mcmodel=kernel -mno-red-zone -mno-80387 -mgeneral-regs-only -I src -I "$(LIMINE_PATH)/include"
CXXFLAGS := -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti -fno-use-cxa-atexit -mcmodel=kernel -mno-red-zone -mgeneral-regs-only -I src -I "$(LIMINE_PATH)/include"
ASFLAGS := -I src
LDFLAGS := -T linker.ld -nostdlib -L "$(LGCCPATH)" -lgcc

DEBUG_FLAGS := -g 
RELEASE_FLAGS := -O2


# Get object files for global constructors/destructors
CRTI_OBJ := obj/crti.S.o
CRTBEGIN_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJ := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJ := obj/crtn.S.o

CXX_FILES := $(shell cd src && find -L * -type f -name '*.cpp' | LC_ALL=C sort) 
C_FILES := $(shell cd src && find -L * -type f -name '*.c' | LC_ALL=C sort)
AS_FILES := $(shell cd src && find -L * -type f -name '*.S' | LC_ALL=C sort)
# Make sure crti is first and crtn is last
OBJ_FILES := $(filter-out $(CRTI_OBJ) $(CRTN_OBJ), $(addprefix obj/, $(CXX_FILES:.cpp=.cpp.o) $(C_FILES:.c=.c.o) $(AS_FILES:.S=.S.o)))
OBJ_FILES := $(CRTI_OBJ) $(CRTBEGIN_OBJ) $(OBJ_FILES) $(CRTEND_OBJ) $(CRTN_OBJ)


TARGET := HeppOS.elf

.PHONY: all
all: CFLAGS += $(RELEASE_FLAGS)
all: CXXFLAGS += $(RELEASE_FLAGS)
all: LDFLAGS += $(RELEASE_FLAGS)
all: bin/$(TARGET)

.PHONY: clean
clean: 
	rm -rf bin obj

.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: CXXFLAGS += $(DEBUG_FLAGS)
debug: LDFLAGS += $(DEBUG_FLAGS)
debug: bin/$(TARGET)

bin/$(TARGET): GNUmakefile linker.ld $(OBJ_FILES)
	mkdir -p "$$(dirname $@)"
	$(LD) $(LDFLAGS) -o $@ $(OBJ_FILES)  

obj/%.cpp.o: src/%.cpp GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CXX) -c $< -o $@ $(CXXFLAGS)

obj/%.c.o: src/%.c GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(CC) -c $< -o $@ $(CFLAGS)

obj/%.S.o: src/%.S GNUmakefile
	mkdir -p "$$(dirname $@)"
	$(AS) $< -o $@ $(ASFLAGS)
