CC = gcc
AS = nasm
LD = ld

CFLAGS = -ffreestanding -m32 -c -Wall -Wextra -I include/
LDFLAGS = -nostdlib -T link.ld -m elf_i386

SRC_CPP = \
	src/boot/loader.cpp \
	src/drivers/keyboard_driver.cpp \
	src/drivers/screen_driver.cpp \
	src/drivers/storage_driver.cpp \
	src/kernel/interrupt_handler.cpp \
	src/kernel/kernel.cpp \
	src/kernel/memory_manager.cpp \
	src/kernel/process_manager.cpp \
	src/utils/math_utils.cpp \
	src/utils/string_utils.cpp

SRC_ASM = src/boot/boot.asm

OBJ_CPP = $(SRC_CPP:.cpp=.o)
OBJ_ASM = $(SRC_ASM:.asm=.o)
OBJ = $(OBJ_CPP) $(OBJ_ASM)

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CC) $(CFLAGS) $< -o $@

%.o: %.asm
	$(AS) -f elf $< -o $@

# Budowanie ISO
build/image.iso: kernel.bin
	mkdir -p iso/boot
	cp kernel.bin iso/boot/
	mkdir -p iso/boot/grub
	cp src/boot/grub.cfg iso/boot/grub/
	xorriso -as mkisofs \
    	-b /usr/lib/syslinux/modules/bios/isolinux.bin \
    	-c boot/isolinux/boot.cat \
    	-no-emul-boot \
    	-boot-load-size 4 \
    	-boot-info-table \
    	-J -r -V "PokmonOS" \
    	-o build/os_image.iso \
    	iso/

all: build/image.iso