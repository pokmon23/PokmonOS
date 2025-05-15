CXX = g++
NASM = nasm
LD = ld

CXXFLAGS = -m32 -ffreestanding -fno-exceptions -fno-rtti -Wall -Wextra
NASMFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld
SRC_DIR = src
BUILD_DIR = build
KERNEL_SOURCES = $(wildcard$(SRC_DIR)/kernel/.cpp) $(wildcard$(SRC_DIR)/drivers/.cpp)
BOOT_SOURCE = $(SRC_DIR)/boot/boot.asm

KERNEL_OBJECTS = $(KERNEL_SOURCES:.cpp=.o)
BOOT_OBJECT = $(SRC_DIR)/boot/boot.o
KERNEL_BIN = $(BUILD_DIR)/pokmonos.bin
ISO_IMAGE = $(BUILD_DIR)/pokmonos.iso

.PHONY: all clean iso run
all: $(KERNEL_BIN)


%.o:%.cpp$(CXX)$(CXXFLAGS) -c $< -o $@

%.o:%.asm $(NASM) $(NASMFLAGS) $< -o $@

$(KERNEL_BIN):$(BOOT_OBJECT) $(KERNEL_OBJECTS)
	mkdir -p $(BUILD_DIR) $(LD) $(LDFLAGS) -o $@ $^

iso:$(KERNEL_BIN)
	mkdir -p $(BUILD_DIR)/iso/boot/grub
	cp $(KERNEL_BIN) $(BUILD_DIR)/iso/boot/
	echo "menuentry 'PokmonOS' {" > $(BUILD_DIR)/iso/boot/grub/grub.cfg
	echo "multiboot /boot/pokmonos.bin" >> $(BUILD_DIR)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO_IMAGE) $(BUILD_DIR)/iso


run: iso
	qemu-system-i386 -cdrom $(ISO_IMAGE)

clean: rm -rf $(SRC_DIR)/**/.o $(BUILD_DIR)/boot/*.o