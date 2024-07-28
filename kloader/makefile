
CC := "$${HOME}/opt/cross/bin/i686-elf-gcc"
CXX := "$${HOME}/opt/cross/bin/i686-elf-g++"
AS := "$${HOME}/opt/cross/bin/i686-elf-as"
LD := "$${HOME}/opt/cross/bin/i686-elf-ld"

CFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions
CXXFLAGS := -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti
ASFLAGS := 
LDFLAGS := -T linker.ld -O2 -nostdlib -L "$$HOME/opt/cross/lib/gcc/i686-elf/14.1.0" -lgcc

CXX_OBJECTS := $(patsubst %.cpp, %.o, $(wildcard *.cpp)) 
C_OBJECTS := $(patsubst %.c, %.o, $(wildcard *.c))
AS_OBJECTS := $(patsubst %.s, %.o, $(wildcard *.s))
AS_OBJECTS_TMP := $(AS_OBJECTS)
AS_OBJECTS := $(filter-out crti.o, $(AS_OBJECTS_TMP))
AS_OBJECTS_TMP := $(AS_OBJECTS)
AS_OBJECTS := $(filter-out crtn.o, $(AS_OBJECTS_TMP))


CRTI_OBJECT := crti.o
CRTBEGIN_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_OBJECT := $(shell $(CC) $(CFLAGS) -print-file-name=crtend.o)
CRTN_OBJECT := crtn.o

TARGET := kloader.elf


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


