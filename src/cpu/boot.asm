[BITS 32]

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

    dd 0                ;find suitable framebuffer text
    dd 0                ;find suitable framebuffer width
    dd 0                ;find suitable framebuffer height
    dd 0                ;find suitable framebuffer bpp

global start
start:
    mov esp, _sys_stack ;move stack to esp
    extern kmain        ;declare kmain is an external function
    push ebx            ;push the multiboot structure to ebx for our kernel to read
    call kmain          ;call the kmain function
    jmp loop            ;if we exit the kmain somehow jump to loop function and halt the system

loop:    
    cli                 ;disable interrupts
    hlt                 ;halt the system
    jmp $               ;loop indefinitely
    

SECTION .bss            ;reserve some space for .bss section in the stack
    resb 8192           ;reserve 8KB

_sys_stack: