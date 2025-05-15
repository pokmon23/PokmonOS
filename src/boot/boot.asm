section .multiboot
    MULTIBOOT_PAGE_ALIGN    equ 1 << 0
    MULTIBOOT_MEMORY_INFO   equ 1 << 1
    MULTIBOOT_HEADER_MAGIC  equ 0x18ADB002
    MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
    MULTIBOOT_CHECKSUM      equ - (MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)


    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

section .bss
    align 4
    stack_bottom:
        resb 16384
    stack_top:


section .text
global _start
extern kernel_main


_start:
    mov esp, stack_top

    call kernel_main

    cli

.hang:
    hlt
    jmp .hang