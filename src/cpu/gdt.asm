[bits 32]

section .text
global gdt_flush

gdt_flush:
    mov     eax, DWORD [esp + 4]
    lgdt    [eax]

    mov     ax, 10h
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp    08h:flush
flush:
    ret