[BITS 32]
global start
start:
    mov esp, _sys_stack
    jmp stublet

ALIGN 4
mboot:
    MULTIBOOT_PAGE_ALIGN    equ 1<<0
    MULTIBOOT_MEMORY_INFO   equ 1<<1
    MULTIBOOT_VIDEO_INFO    equ 1<<2
    MULTIBOOT_AOUT_KLUDGE	equ 1<<16
    MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_AOUT_KLUDGE | MULTIBOOT_VIDEO_INFO
    MULTIBOOT_CHECKSUM	    equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
    EXTERN code, bss, end

    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

    dd mboot
    dd code
    dd bss
    dd end
    dd start

    dd 0    ;text
    dd 0    ;width
    dd 0    ;height
    dd 0    ;bpp

stublet:
    extern kmain
    push ebx
    call kmain
    jmp $

SECTION .bss
    resb 8192

_sys_stack: