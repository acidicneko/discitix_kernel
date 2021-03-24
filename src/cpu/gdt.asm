[bits 32]

section .text
global gdt_flush                    ;make gdt_flush a global function

gdt_flush:
    mov     eax, DWORD [esp + 4]
    lgdt    [eax]                   ;lgdt the passed gdt structure

    mov     ax, 10h
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    jmp    08h:flush
flush:
    ret